//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include "filesystem_tools.h"
#include "cmdlib.h"
#include "scriplib.h"
#include "mathlib.h"
#define EXTERN
#include "studio.h"
#include "motionmapper.h"
#include "vstdlib/strtools.h"
#include "vstdlib/icommandline.h"
#include "utldict.h"
#include <windows.h>
#include "UtlBuffer.h"
#include "UtlSymbol.h"

bool g_quiet = false;
bool g_verbose = false;
char g_outfile[1024];
bool uselogfile = false;

char	g_szFilename[1024];
FILE	*g_fpInput;
char	g_szLine[4096];
int		g_iLinecount;

bool g_bZBrush = false;

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : depth - 
//			*fmt - 
//			... - 
//-----------------------------------------------------------------------------
void vprint( int depth, const char *fmt, ... )
{
	char string[ 8192 ];
	va_list va;
	va_start( va, fmt );
	_vsnprintf( string, sizeof( string ) - 1, fmt, va );
	va_end( va );

	FILE *fp = NULL;

	if ( uselogfile )
	{
		fp = fopen( "log.txt", "ab" );
	}

	while ( depth-- > 0 )
	{
		vprint( 0,  "  " );
		OutputDebugString( "  " );
		if ( fp )
		{
			fprintf( fp, "  " );
		}
	}

	::printf( string );
	OutputDebugString( string );

	if ( fp )
	{
		char *p = string;
		while ( *p )
		{
			if ( *p == '\n' )
			{
				fputc( '\r', fp );
			}
			fputc( *p, fp );
			p++;
		}
		fclose( fp );
	}
}


int k_memtotal;
void *kalloc( int num, int size )
{
	// vprint( 0,  "calloc( %d, %d )\n", num, size );
	// vprint( 0,  "%d ", num * size );
	k_memtotal += num * size;
	return calloc( num, size );
}

void kmemset( void *ptr, int value, int size )
{
	// vprint( 0,  "kmemset( %x, %d, %d )\n", ptr, value, size );
	memset( ptr, value, size );
	return;
}

static bool g_bFirstWarning = true;

void MdlWarning( const char *fmt, ... )
{
	va_list args;
	static char output[1024];

	if (g_quiet)
	{
		if (g_bFirstWarning)
		{
			vprint( 0, "%s :\n", fullpath );
			g_bFirstWarning = false;
		}
		vprint( 0, "\t");
	}

	vprint( 0, "WARNING: ");
	va_start( args, fmt );
	vprint( 0, fmt, args );
}


void MdlError( char const *fmt, ... )
{
	va_list		args;

	if (g_quiet)
	{
		if (g_bFirstWarning)
		{
			vprint( 0, "%s :\n", fullpath );
			g_bFirstWarning = false;
		}
		vprint( 0, "\t");
	}

	vprint( 0, "ERROR: ");
	va_start( args, fmt );
	vprint( 0, fmt, args );

	exit( -1 );
}

int OpenGlobalFile( char *src )
{
	int		time1;
	char	filename[1024];

	strcpy( filename, ExpandPath( src ) );

	int pathLength;
	int numBasePaths = CmdLib_GetNumBasePaths();
	// This is kinda gross. . . doing the same work in cmdlib on SafeOpenRead.
	if( CmdLib_HasBasePath( filename, pathLength ) )
	{
		char tmp[1024];
		int i;
		for( i = 0; i < numBasePaths; i++ )
		{
			strcpy( tmp, CmdLib_GetBasePath( i ) );
			strcat( tmp, filename + pathLength );
			
			time1 = FileTime( tmp );
			if( time1 != -1 )
			{
				if ((g_fpInput = fopen(tmp, "r")) == 0) 
				{
					MdlWarning( "reader: could not open file '%s'\n", src );
					return 0;
				}
				else
				{
					return 1;
				}
			}
		}
		return 0;
	}
	else
	{
		time1 = FileTime (filename);
		if (time1 == -1)
			return 0;

		if ((g_fpInput = fopen(filename, "r")) == 0) 
		{
			MdlWarning( "reader: could not open file '%s'\n", src );
			return 0;
		}

		return 1;
	}
}

bool IsEnd( char const* pLine )
{
	if (strncmp( "end", pLine, 3 ) != 0) 
		return false;
	return (pLine[3] == '\0') || (pLine[3] == '\n');
}


//Wrong name for the use of it.
void scale_vertex( Vector &org )
{
	org[0] = org[0] * g_currentscale;
	org[1] = org[1] * g_currentscale;
	org[2] = org[2] * g_currentscale;
}


void clip_rotations( RadianEuler& rot )
{
	int j;
	// clip everything to : -M_PI <= x < M_PI

	for (j = 0; j < 3; j++) {
		while (rot[j] >= M_PI) 
			rot[j] -= M_PI*2;
		while (rot[j] < -M_PI) 
			rot[j] += M_PI*2;
	}
}


void clip_rotations( Vector& rot )
{
	int j;
	// clip everything to : -180 <= x < 180

	for (j = 0; j < 3; j++) {
		while (rot[j] >= 180) 
			rot[j] -= 180*2;
		while (rot[j] < -180) 
			rot[j] += 180*2;
	}
}


void Build_Reference( s_source_t *psource)
{
	int		i, parent;
	Vector	angle;

	for (i = 0; i < psource->numbones; i++)
	{
		matrix3x4_t m;
		AngleMatrix( psource->rawanim[0][i].rot, m );
		m[0][3] = psource->rawanim[0][i].pos[0];
		m[1][3] = psource->rawanim[0][i].pos[1];
		m[2][3] = psource->rawanim[0][i].pos[2];

		parent = psource->localBone[i].parent;
		if (parent == -1) 
		{
			// scale the done pos.
			// calc rotational matrices
			MatrixCopy( m, psource->boneToPose[i] );
		}
		else 
		{
			// calc compound rotational matrices
			// FIXME : Hey, it's orthogical so inv(A) == transpose(A)
			ConcatTransforms( psource->boneToPose[parent], m, psource->boneToPose[i] );
		}
		// vprint( 0, "%3d %f %f %f\n", i, psource->bonefixup[i].worldorg[0], psource->bonefixup[i].worldorg[1], psource->bonefixup[i].worldorg[2] );
		/*
		AngleMatrix( angle, m );
		vprint( 0, "%8.4f %8.4f %8.4f\n", m[0][0], m[1][0], m[2][0] );
		vprint( 0, "%8.4f %8.4f %8.4f\n", m[0][1], m[1][1], m[2][1] );
		vprint( 0, "%8.4f %8.4f %8.4f\n", m[0][2], m[1][2], m[2][2] );
		*/
	}
}

int Grab_Nodes( s_node_t *pnodes )
{
	int index;
	char name[1024];
	int parent;
	int numbones = 0;

	for (index = 0; index < MAXSTUDIOSRCBONES; index++)
	{
		pnodes[index].parent = -1;
	}

	while (fgets( g_szLine, sizeof( g_szLine ), g_fpInput ) != NULL) 
	{
		g_iLinecount++;
		if (sscanf( g_szLine, "%d \"%[^\"]\" %d", &index, name, &parent ) == 3)
		{
			// check for duplicated bones
			/*
			if (strlen(pnodes[index].name) != 0)
			{
				MdlError( "bone \"%s\" exists more than once\n", name );
			}
			*/
			
			strcpyn( pnodes[index].name, name );
			pnodes[index].parent = parent;
			if (index > numbones)
			{
				numbones = index;
			}
		}
		else 
		{
			return numbones + 1;
		}
	}
	MdlError( "Unexpected EOF at line %d\n", g_iLinecount );
	return 0;
}

void Grab_Vertexanimation( s_source_t *psource )
{
	char	cmd[1024];
	int		index;
	Vector	pos;
	Vector	normal;
	int		t = -1;
	int		count = 0;
	static s_vertanim_t	tmpvanim[MAXSTUDIOVERTS*4];

	while (fgets( g_szLine, sizeof( g_szLine ), g_fpInput ) != NULL) 
	{
		g_iLinecount++;
		if (sscanf( g_szLine, "%d %f %f %f %f %f %f", &index, &pos[0], &pos[1], &pos[2], &normal[0], &normal[1], &normal[2] ) == 7)
		{
			if (psource->startframe < 0)
			{
				MdlError( "Missing frame start(%d) : %s", g_iLinecount, g_szLine );
			}

			if (t < 0)
			{
				MdlError( "VTA Frame Sync (%d) : %s", g_iLinecount, g_szLine );
			}

			tmpvanim[count].vertex = index;
			VectorCopy( pos, tmpvanim[count].pos );
			VectorCopy( normal, tmpvanim[count].normal );
			count++;

			if (index >= psource->numvertices)
				psource->numvertices = index + 1;
		}
		else
		{
			// flush data

			if (count)
			{
				psource->numvanims[t] = count;

				psource->vanim[t] = (s_vertanim_t *)kalloc( count, sizeof( s_vertanim_t ) );

				memcpy( psource->vanim[t], tmpvanim, count * sizeof( s_vertanim_t ) );
			}
			else if (t > 0)
			{
				psource->numvanims[t] = 0;
			}

			// next command
			if (sscanf( g_szLine, "%1023s %d", cmd, &index ))
			{
				if (strcmp( cmd, "time" ) == 0) 
				{
					t = index;
					count = 0;

					if (t < psource->startframe)
					{
						MdlError( "Frame MdlError(%d) : %s", g_iLinecount, g_szLine );
					}
					if (t > psource->endframe)
					{
						MdlError( "Frame MdlError(%d) : %s", g_iLinecount, g_szLine );
					}

					t -= psource->startframe;
				}
				else if (strcmp( cmd, "end") == 0) 
				{
					psource->numframes = psource->endframe - psource->startframe + 1;
					return;
				}
				else
				{
					MdlError( "MdlError(%d) : %s", g_iLinecount, g_szLine );
				}

			}
			else
			{
				MdlError( "MdlError(%d) : %s", g_iLinecount, g_szLine );
			}
		}
	}
	MdlError( "unexpected EOF: %s\n", psource->filename );
}

void Grab_Animation( s_source_t *psource )
{
	Vector pos;
	RadianEuler rot;
	char cmd[1024];
	int index;
	int	t = -99999999;
	int size;

	psource->startframe = -1;

	size = psource->numbones * sizeof( s_bone_t );

	while (fgets( g_szLine, sizeof( g_szLine ), g_fpInput ) != NULL) 
	{
		g_iLinecount++;
		if (sscanf( g_szLine, "%d %f %f %f %f %f %f", &index, &pos[0], &pos[1], &pos[2], &rot[0], &rot[1], &rot[2] ) == 7)
		{
			if (psource->startframe < 0)
			{
				MdlError( "Missing frame start(%d) : %s", g_iLinecount, g_szLine );
			}

			scale_vertex( pos );
			VectorCopy( pos, psource->rawanim[t][index].pos );
			VectorCopy( rot, psource->rawanim[t][index].rot );

			clip_rotations( rot ); // !!!
		}
		else if (sscanf( g_szLine, "%1023s %d", cmd, &index ))
		{
			if (strcmp( cmd, "time" ) == 0) 
			{
				t = index;
				if (psource->startframe == -1)
				{
					psource->startframe = t;
				}
				if (t < psource->startframe)
				{
					MdlError( "Frame MdlError(%d) : %s", g_iLinecount, g_szLine );
				}
				if (t > psource->endframe)
				{
					psource->endframe = t;
				}
				t -= psource->startframe;

				if (psource->rawanim[t] == NULL)
				{
					psource->rawanim[t] = (s_bone_t *)kalloc( 1, size );

					// duplicate previous frames keys
					if (t > 0 && psource->rawanim[t-1])
					{
						for (int j = 0; j < psource->numbones; j++)
						{
							VectorCopy( psource->rawanim[t-1][j].pos, psource->rawanim[t][j].pos );
							VectorCopy( psource->rawanim[t-1][j].rot, psource->rawanim[t][j].rot );
						}
					}
				}
				else
				{
					// MdlError( "%s has duplicated frame %d\n", psource->filename, t );
				}
			}
			else if (strcmp( cmd, "end") == 0) 
			{
				psource->numframes = psource->endframe - psource->startframe + 1;

				for (t = 0; t < psource->numframes; t++)
				{
					if (psource->rawanim[t] == NULL)
					{
						MdlError( "%s is missing frame %d\n", psource->filename, t + psource->startframe );
					}
				}

				Build_Reference( psource );
				return;
			}
			else
			{
				MdlError( "MdlError(%d) : %s", g_iLinecount, g_szLine );
			}
		}
		else
		{
			MdlError( "MdlError(%d) : %s", g_iLinecount, g_szLine );
		}
	}

	MdlError( "unexpected EOF: %s\n", psource->filename );
}

int lookup_index( s_source_t *psource, int material, Vector& vertex, Vector& normal, Vector2D texcoord )
{
	int i;

	for (i = 0; i < numvlist; i++) 
	{
		if (v_listdata[i].m == material
			&& DotProduct( g_normal[i], normal ) > normal_blend
			&& VectorCompare( g_vertex[i], vertex )
			&& g_texcoord[i][0] == texcoord[0]
			&& g_texcoord[i][1] == texcoord[1])
		{
			v_listdata[i].lastref = numvlist;
			return i;
		}
	}
	if (i >= MAXSTUDIOVERTS) {
		MdlError( "too many indices in source: \"%s\"\n", psource->filename);
	}

	VectorCopy( vertex, g_vertex[i] );
	VectorCopy( normal, g_normal[i] );
	Vector2Copy( texcoord, g_texcoord[i] );

	v_listdata[i].v = i;
	v_listdata[i].m = material;
	v_listdata[i].n = i;
	v_listdata[i].t = i;

	v_listdata[i].firstref = numvlist;
	v_listdata[i].lastref = numvlist;

	numvlist = i + 1;
	return i;
}


void ParseFaceData( s_source_t *psource, int material, s_face_t *pFace )
{
	int index[3];
	int i, j;
	Vector p;
	Vector normal;
	Vector2D t;
	int		iCount, bones[MAXSTUDIOSRCBONES];
	float   weights[MAXSTUDIOSRCBONES];
	int bone;

	for (j = 0; j < 3; j++) 
	{
		memset( g_szLine, 0, sizeof( g_szLine ) );

		if (fgets( g_szLine, sizeof( g_szLine ), g_fpInput ) == NULL) 
		{
			MdlError("%s: error on g_szLine %d: %s", g_szFilename, g_iLinecount, g_szLine );
		}

		iCount = 0;

		g_iLinecount++;
		i = sscanf( g_szLine, "%d %f %f %f %f %f %f %f %f %d %d %f %d %f %d %f %d %f",
			&bone, 
			&p[0], &p[1], &p[2], 
			&normal[0], &normal[1], &normal[2], 
			&t[0], &t[1],
			&iCount,
			&bones[0], &weights[0], &bones[1], &weights[1], &bones[2], &weights[2], &bones[3], &weights[3] );
			
		if (i < 9) 
			continue;

		if (bone < 0 || bone >= psource->numbones) 
		{
			MdlError("bogus bone index\n%d %s :\n%s", g_iLinecount, g_szFilename, g_szLine );
		}

		//Scale face pos
		scale_vertex( p );
		
		// continue parsing more bones.
		// FIXME: don't we have a built in parser that'll do this?
		if (iCount > 4)
		{
			int k;
			int ctr = 0;
			char *token;
			for (k = 0; k < 18; k++)
			{
				while (g_szLine[ctr] == ' ')
				{
					ctr++;
				}
				token = strtok( &g_szLine[ctr], " " );
				ctr += strlen( token ) + 1;
			}
			for (k = 4; k < iCount && k < MAXSTUDIOSRCBONES; k++)
			{
				while (g_szLine[ctr] == ' ')
				{
					ctr++;
				}
				token = strtok( &g_szLine[ctr], " " );
				ctr += strlen( token ) + 1;

				bones[k] = atoi(token);

				token = strtok( &g_szLine[ctr], " " );
				ctr += strlen( token ) + 1;
			
				weights[k] = atof(token);
			}
			// vprint( 0, "%d ", iCount );

			//vprint( 0, "\n");
			//exit(1);
		}

		// adjust_vertex( p );
		// scale_vertex( p );

		// move vertex position to object space.
		// VectorSubtract( p, psource->bonefixup[bone].worldorg, tmp );
		// VectorTransform(tmp, psource->bonefixup[bone].im, p );

		// move normal to object space.
		// VectorCopy( normal, tmp );
		// VectorTransform(tmp, psource->bonefixup[bone].im, normal );
		// VectorNormalize( normal );

		// invert v
		t[1] = 1.0 - t[1];

		index[j] = lookup_index( psource, material, p, normal, t );

		if (i == 9 || iCount == 0)
		{
			g_bone[index[j]].numbones = 1;
			g_bone[index[j]].bone[0] = bone;
			g_bone[index[j]].weight[0] = 1.0;
		}
		else
		{
			iCount = SortAndBalanceBones( iCount, MAXSTUDIOBONEWEIGHTS, bones, weights );

			g_bone[index[j]].numbones = iCount;
			for (i = 0; i < iCount; i++)
			{
				g_bone[index[j]].bone[i] = bones[i];
				g_bone[index[j]].weight[i] = weights[i];
			}
		}
	}

	// pFace->material = material; // BUG
	pFace->a		= index[0];
	pFace->b		= index[1];
	pFace->c		= index[2];
	Assert( ((pFace->a & 0xF0000000) == 0) && ((pFace->b & 0xF0000000) == 0) && 
		((pFace->c & 0xF0000000) == 0) );

	if (flip_triangles)
	{
		j = pFace->b;  pFace->b  = pFace->c;  pFace->c  = j;
	}
}

int use_texture_as_material( int textureindex )
{
	if (g_texture[textureindex].material == -1)
	{
		// vprint( 0, "%d %d %s\n", textureindex, g_nummaterials, g_texture[textureindex].name );
		g_material[g_nummaterials] = textureindex;
		g_texture[textureindex].material = g_nummaterials++;
	}

	return g_texture[textureindex].material;
}

int material_to_texture( int material )
{
	int i;
	for (i = 0; i < g_numtextures; i++)
	{
		if (g_texture[i].material == material)
		{
			return i;
		}
	}
	return -1;
}

int lookup_texture( char *texturename, int maxlen )
{
	int i;

	Q_StripExtension( texturename, texturename, maxlen );

	for (i = 0; i < g_numtextures; i++) 
	{
		if (stricmp( g_texture[i].name, texturename ) == 0) 
		{
			return i;
		}
	}

	if (i >= MAXSTUDIOSKINS)
		MdlError("Too many materials used, max %d\n", ( int )MAXSTUDIOSKINS );

//	vprint( 0,  "texture %d = %s\n", i, texturename );
	strcpyn( g_texture[i].name, texturename );

	g_texture[i].material = -1;
	/*
	if (stristr( texturename, "chrome" ) != NULL) {
		texture[i].flags = STUDIO_NF_FLATSHADE | STUDIO_NF_CHROME;
	}
	else {
		texture[i].flags = 0;
	}
	*/
	g_numtextures++;
	return i;
}

int SortAndBalanceBones( int iCount, int iMaxCount, int bones[], float weights[] )
{
	int i;

	// collapse duplicate bone weights
	for (i = 0; i < iCount-1; i++)
	{
		int j;
		for (j = i + 1; j < iCount; j++)
		{
			if (bones[i] == bones[j])
			{
				weights[i] += weights[j];
				weights[j] = 0.0;
			}
		}
	}

	// do sleazy bubble sort
	int bShouldSort;
	do {
		bShouldSort = false;
		for (i = 0; i < iCount-1; i++)
		{
			if (weights[i+1] > weights[i])
			{
				int j = bones[i+1]; bones[i+1] = bones[i]; bones[i] = j;
				float w = weights[i+1]; weights[i+1] = weights[i]; weights[i] = w;
				bShouldSort = true;
			}
		}
	} while (bShouldSort);

	// throw away all weights less than 1/20th
	while (iCount > 1 && weights[iCount-1] < 0.05)
	{
		iCount--;
	}

	// clip to the top iMaxCount bones
	if (iCount > iMaxCount)
	{
		iCount = iMaxCount;
	}

	float t = 0;
	for (i = 0; i < iCount; i++)
	{
		t += weights[i];
	}

	if (t <= 0.0)
	{
		// missing weights?, go ahead and evenly share?
		// FIXME: shouldn't this error out?
		t = 1.0 / iCount;

		for (i = 0; i < iCount; i++)
		{
			weights[i] = t;
		}
	}
	else
	{
		// scale to sum to 1.0
		t = 1.0 / t;

		for (i = 0; i < iCount; i++)
		{
			weights[i] = weights[i] * t;
		}
	}

	return iCount;
}

int vlistCompare( const void *elem1, const void *elem2 )
{
	v_unify_t *u1 = &v_listdata[*(int *)elem1];
	v_unify_t *u2 = &v_listdata[*(int *)elem2];

	// sort by material
	if (u1->m < u2->m)
		return -1;
	if (u1->m > u2->m)
		return 1;

	// sort by last used
	if (u1->lastref < u2->lastref)
		return -1;
	if (u1->lastref > u2->lastref)
		return 1;

	return 0;
}

int faceCompare( const void *elem1, const void *elem2 )
{
	int i1 = *(int *)elem1;
	int i2 = *(int *)elem2;

	// sort by material
	if (g_face[i1].material < g_face[i2].material)
		return -1;
	if (g_face[i1].material > g_face[i2].material)
		return 1;

	// sort by original usage
	if (i1 < i2)
		return -1;
	if (i1 > i2)
		return 1;

	return 0;
}

#define SMALL_FLOAT 1e-12

// NOTE: This routine was taken (and modified) from NVidia's BlinnReflection demo
// Creates basis vectors, based on a vertex and index list.
// See the NVidia white paper 'GDC2K PerPixel Lighting' for a description
// of how this computation works
static void CalcTriangleTangentSpace( s_source_t *pSrc, int v1, int v2, int v3, 
									  Vector &sVect, Vector &tVect )
{
/*
	static bool firstTime = true;
	static FILE *fp = NULL;
	if( firstTime )
	{
		firstTime = false;
		fp = fopen( "crap.out", "w" );
	}
*/
    
	/* Compute the partial derivatives of X, Y, and Z with respect to S and T. */
	Vector2D t0( pSrc->texcoord[v1][0], pSrc->texcoord[v1][1] );
	Vector2D t1( pSrc->texcoord[v2][0], pSrc->texcoord[v2][1] );
	Vector2D t2( pSrc->texcoord[v3][0], pSrc->texcoord[v3][1] );
	Vector p0( pSrc->vertex[v1][0], pSrc->vertex[v1][1], pSrc->vertex[v1][2] );
	Vector p1( pSrc->vertex[v2][0], pSrc->vertex[v2][1], pSrc->vertex[v2][2] );
	Vector p2( pSrc->vertex[v3][0], pSrc->vertex[v3][1], pSrc->vertex[v3][2] );

	sVect.Init( 0.0f, 0.0f, 0.0f );
	tVect.Init( 0.0f, 0.0f, 0.0f );

	// x, s, t
	Vector edge01 = Vector( p1.x - p0.x, t1.x - t0.x, t1.y - t0.y );
	Vector edge02 = Vector( p2.x - p0.x, t2.x - t0.x, t2.y - t0.y );

	Vector cross;
	CrossProduct( edge01, edge02, cross );
	if( fabs( cross.x ) > SMALL_FLOAT )
	{
		sVect.x += -cross.y / cross.x;
		tVect.x += -cross.z / cross.x;
	}

	// y, s, t
	edge01 = Vector( p1.y - p0.y, t1.x - t0.x, t1.y - t0.y );
	edge02 = Vector( p2.y - p0.y, t2.x - t0.x, t2.y - t0.y );

	CrossProduct( edge01, edge02, cross );
	if( fabs( cross.x ) > SMALL_FLOAT )
	{
		sVect.y += -cross.y / cross.x;
		tVect.y += -cross.z / cross.x;
	}
	
	// z, s, t
	edge01 = Vector( p1.z - p0.z, t1.x - t0.x, t1.y - t0.y );
	edge02 = Vector( p2.z - p0.z, t2.x - t0.x, t2.y - t0.y );

	CrossProduct( edge01, edge02, cross );
	if( fabs( cross.x ) > SMALL_FLOAT )
	{
		sVect.z += -cross.y / cross.x;
		tVect.z += -cross.z / cross.x;
	}

	// Normalize sVect and tVect
	VectorNormalize( sVect );
	VectorNormalize( tVect );

/*
	// Calculate flat normal
	Vector flatNormal;
	edge01 = p1 - p0;
	edge02 = p2 - p0;
	CrossProduct( edge02, edge01, flatNormal );
	VectorNormalize( flatNormal );
	
	// Get the average position
	Vector avgPos = ( p0 + p1 + p2 ) / 3.0f;

	// Draw the svect
	Vector endS = avgPos + sVect * .2f;
	fvprint( 0,  fp, "2\n" );
	fvprint( 0,  fp, "%f %f %f 1.0 0.0 0.0\n", endS[0], endS[1], endS[2] );
	fvprint( 0,  fp, "%f %f %f 1.0 0.0 0.0\n", avgPos[0], avgPos[1], avgPos[2] );
	
	// Draw the tvect
	Vector endT = avgPos + tVect * .2f;
	fvprint( 0,  fp, "2\n" );
	fvprint( 0,  fp, "%f %f %f 0.0 1.0 0.0\n", endT[0], endT[1], endT[2] );
	fvprint( 0,  fp, "%f %f %f 0.0 1.0 0.0\n", avgPos[0], avgPos[1], avgPos[2] );
	
	// Draw the normal
	Vector endN = avgPos + flatNormal * .2f;
	fvprint( 0,  fp, "2\n" );
	fvprint( 0,  fp, "%f %f %f 0.0 0.0 1.0\n", endN[0], endN[1], endN[2] );
	fvprint( 0,  fp, "%f %f %f 0.0 0.0 1.0\n", avgPos[0], avgPos[1], avgPos[2] );
	
	// Draw the wireframe of the triangle in white.
	fvprint( 0,  fp, "2\n" );
	fvprint( 0,  fp, "%f %f %f 1.0 1.0 1.0\n", p0[0], p0[1], p0[2] );
	fvprint( 0,  fp, "%f %f %f 1.0 1.0 1.0\n", p1[0], p1[1], p1[2] );
	fvprint( 0,  fp, "2\n" );
	fvprint( 0,  fp, "%f %f %f 1.0 1.0 1.0\n", p1[0], p1[1], p1[2] );
	fvprint( 0,  fp, "%f %f %f 1.0 1.0 1.0\n", p2[0], p2[1], p2[2] );
	fvprint( 0,  fp, "2\n" );
	fvprint( 0,  fp, "%f %f %f 1.0 1.0 1.0\n", p2[0], p2[1], p2[2] );
	fvprint( 0,  fp, "%f %f %f 1.0 1.0 1.0\n", p0[0], p0[1], p0[2] );

	// Draw a slightly shrunken version of the geometry to hide surfaces
	Vector tmp0 = p0 - flatNormal * .1f;
	Vector tmp1 = p1 - flatNormal * .1f;
	Vector tmp2 = p2 - flatNormal * .1f;
	fvprint( 0,  fp, "3\n" );
	fvprint( 0,  fp, "%f %f %f 0.1 0.1 0.1\n", tmp0[0], tmp0[1], tmp0[2] );
	fvprint( 0,  fp, "%f %f %f 0.1 0.1 0.1\n", tmp1[0], tmp1[1], tmp1[2] );
	fvprint( 0,  fp, "%f %f %f 0.1 0.1 0.1\n", tmp2[0], tmp2[1], tmp2[2] );
		
	fflush( fp );
*/
}

typedef CUtlVector<int> CIntVector;

void CalcModelTangentSpaces( s_source_t *pSrc )
{
	// Build a map from vertex to a list of triangles that share the vert.
	int meshID;
	for( meshID = 0; meshID < pSrc->nummeshes; meshID++ )
	{
		s_mesh_t *pMesh = &pSrc->mesh[pSrc->meshindex[meshID]];
		CUtlVector<CIntVector> vertToTriMap;
		vertToTriMap.AddMultipleToTail( pMesh->numvertices );
		int triID;
		for( triID = 0; triID < pMesh->numfaces; triID++ )
		{
			s_face_t *pFace = &pSrc->face[triID + pMesh->faceoffset];
			vertToTriMap[pFace->a].AddToTail( triID );
			vertToTriMap[pFace->b].AddToTail( triID );
			vertToTriMap[pFace->c].AddToTail( triID );
		}

		// Calculate the tangent space for each triangle.
		CUtlVector<Vector> triSVect;
		CUtlVector<Vector> triTVect;
		triSVect.AddMultipleToTail( pMesh->numfaces );
		triTVect.AddMultipleToTail( pMesh->numfaces );
		for( triID = 0; triID < pMesh->numfaces; triID++ )
		{
			s_face_t *pFace = &pSrc->face[triID + pMesh->faceoffset];
			CalcTriangleTangentSpace( pSrc, 
				pMesh->vertexoffset + pFace->a, 
				pMesh->vertexoffset + pFace->b, 
				pMesh->vertexoffset + pFace->c, 
				triSVect[triID], triTVect[triID] );
		}	

		// calculate an average tangent space for each vertex.
		int vertID;
		for( vertID = 0; vertID < pMesh->numvertices; vertID++ )
		{
			const Vector &normal = pSrc->normal[vertID+pMesh->vertexoffset];
			Vector4D &finalSVect = pSrc->tangentS[vertID+pMesh->vertexoffset];
			Vector sVect, tVect;

			sVect.Init( 0.0f, 0.0f, 0.0f );
			tVect.Init( 0.0f, 0.0f, 0.0f );
			for( triID = 0; triID < vertToTriMap[vertID].Size(); triID++ )
			{
				sVect += triSVect[vertToTriMap[vertID][triID]];
				tVect += triTVect[vertToTriMap[vertID][triID]];
			}

			// In the case of zbrush, everything needs to be treated as smooth.
			if( g_bZBrush )
			{
				int vertID2;
				Vector vertPos1( pSrc->vertex[vertID][0], pSrc->vertex[vertID][1], pSrc->vertex[vertID][2] );
				for( vertID2 = 0; vertID2 < pMesh->numvertices; vertID2++ )
				{
					if( vertID2 == vertID )
					{
						continue;
					}
					Vector vertPos2( pSrc->vertex[vertID2][0], pSrc->vertex[vertID2][1], pSrc->vertex[vertID2][2] );
					if( vertPos1 == vertPos2 )
					{
						int triID2;
						for( triID2 = 0; triID2 < vertToTriMap[vertID2].Size(); triID2++ )
						{
							sVect += triSVect[vertToTriMap[vertID2][triID2]];
							tVect += triTVect[vertToTriMap[vertID2][triID2]];
						}
					}
				}
			}

			// make an orthonormal system.
			// need to check if we are left or right handed.
			Vector tmpVect;
			CrossProduct( sVect, tVect, tmpVect );
			bool leftHanded = DotProduct( tmpVect, normal ) < 0.0f;
			if( !leftHanded )
			{
				CrossProduct( normal, sVect, tVect );
				CrossProduct( tVect, normal, sVect );
				VectorNormalize( sVect );
				VectorNormalize( tVect );
				finalSVect[0] = sVect[0];
				finalSVect[1] = sVect[1];
				finalSVect[2] = sVect[2];
				finalSVect[3] = 1.0f;
			}
			else
			{
				CrossProduct( sVect, normal, tVect );
				CrossProduct( normal, tVect, sVect );
				VectorNormalize( sVect );
				VectorNormalize( tVect );
				finalSVect[0] = sVect[0];
				finalSVect[1] = sVect[1];
				finalSVect[2] = sVect[2];
				finalSVect[3] = -1.0f;
			}
		}
	}
}

void BuildIndividualMeshes( s_source_t *psource )
{
	int i, j, k;
	
	// sort new vertices by materials, last used
	static int v_listsort[MAXSTUDIOVERTS];	// map desired order to vlist entry
	static int v_ilistsort[MAXSTUDIOVERTS]; // map vlist entry to desired order

	for (i = 0; i < numvlist; i++)
	{
		v_listsort[i] = i;
	}
	qsort( v_listsort, numvlist, sizeof( int ), vlistCompare );
	for (i = 0; i < numvlist; i++)
	{
		v_ilistsort[v_listsort[i]] = i;
	}


	// allocate memory
	psource->numvertices = numvlist;
	psource->localBoneweight = (s_boneweight_t *)kalloc( psource->numvertices, sizeof( s_boneweight_t ) );
	psource->globalBoneweight = NULL;
	psource->vertexInfo = (s_vertexinfo_t *)kalloc( psource->numvertices, sizeof( s_vertexinfo_t ) );
	psource->vertex = new Vector[psource->numvertices];
	psource->normal = new Vector[psource->numvertices];
	psource->tangentS = new Vector4D[psource->numvertices];
	psource->texcoord = (Vector2D *)kalloc( psource->numvertices, sizeof( Vector2D ) );

	// create arrays of unique vertexes, normals, texcoords.
	for (i = 0; i < psource->numvertices; i++)
	{
		j = v_listsort[i];

		VectorCopy( g_vertex[v_listdata[j].v], psource->vertex[i] );
		VectorCopy( g_normal[v_listdata[j].n], psource->normal[i] );		
		Vector2Copy( g_texcoord[v_listdata[j].t], psource->texcoord[i] );

		psource->localBoneweight[i].numbones		= g_bone[v_listdata[j].v].numbones;
		int k;
		for( k = 0; k < MAXSTUDIOBONEWEIGHTS; k++ )
		{
			psource->localBoneweight[i].bone[k]		= g_bone[v_listdata[j].v].bone[k];
			psource->localBoneweight[i].weight[k]	= g_bone[v_listdata[j].v].weight[k];
		}

		// store a bunch of other info
		psource->vertexInfo[i].material		= v_listdata[j].m;

		psource->vertexInfo[i].firstref		= v_listdata[j].firstref;
		psource->vertexInfo[i].lastref		= v_listdata[j].lastref;
		// vprint( 0, "%4d : %2d :  %6.2f %6.2f %6.2f\n", i, psource->boneweight[i].bone[0], psource->vertex[i][0], psource->vertex[i][1], psource->vertex[i][2] );
	}

	// sort faces by materials, last used.
	static int facesort[MAXSTUDIOTRIANGLES];	// map desired order to src_face entry
	static int ifacesort[MAXSTUDIOTRIANGLES];	// map src_face entry to desired order
	
	for (i = 0; i < g_numfaces; i++)
	{
		facesort[i] = i;
	}
	qsort( facesort, g_numfaces, sizeof( int ), faceCompare );
	for (i = 0; i < g_numfaces; i++)
	{
		ifacesort[facesort[i]] = i;
	}

	psource->numfaces = g_numfaces;
	// find first occurance for each material
	for (k = 0; k < MAXSTUDIOSKINS; k++)
	{
		psource->mesh[k].numvertices = 0;
		psource->mesh[k].vertexoffset = psource->numvertices;

		psource->mesh[k].numfaces = 0;
		psource->mesh[k].faceoffset = g_numfaces;
	}

	// find first and count of indices per material
	for (i = 0; i < psource->numvertices; i++)
	{
		k = psource->vertexInfo[i].material;
		psource->mesh[k].numvertices++;
		if (psource->mesh[k].vertexoffset > i)
			psource->mesh[k].vertexoffset = i;
	}

	// find first and count of faces per material
	for (i = 0; i < psource->numfaces; i++)
	{
		k = g_face[facesort[i]].material;

		psource->mesh[k].numfaces++;
		if (psource->mesh[k].faceoffset > i)
			psource->mesh[k].faceoffset = i;
	}

	/*
	for (k = 0; k < MAXSTUDIOSKINS; k++)
	{
		vprint( 0, "%d : %d:%d %d:%d\n", k, psource->mesh[k].numvertices, psource->mesh[k].vertexoffset, psource->mesh[k].numfaces, psource->mesh[k].faceoffset );
	}
	*/

	// create remapped faces
	psource->face = (s_face_t *)kalloc( psource->numfaces, sizeof( s_face_t ));
	for (k = 0; k < MAXSTUDIOSKINS; k++)
	{
		if (psource->mesh[k].numfaces)
		{
			psource->meshindex[psource->nummeshes] = k;

			for (i = psource->mesh[k].faceoffset; i < psource->mesh[k].numfaces + psource->mesh[k].faceoffset; i++)
			{
				j = facesort[i];

				psource->face[i].a = v_ilistsort[g_src_uface[j].a] - psource->mesh[k].vertexoffset;
				psource->face[i].b = v_ilistsort[g_src_uface[j].b] - psource->mesh[k].vertexoffset;
				psource->face[i].c = v_ilistsort[g_src_uface[j].c] - psource->mesh[k].vertexoffset;
				Assert( ((psource->face[i].a & 0xF0000000) == 0) && ((psource->face[i].b & 0xF0000000) == 0) && 
					((psource->face[i].c & 0xF0000000) == 0) );
				// vprint( 0, "%3d : %4d %4d %4d\n", i, psource->face[i].a, psource->face[i].b, psource->face[i].c );
			}

			psource->nummeshes++;
		}
	}

	CalcModelTangentSpaces( psource );
}

void Grab_Triangles( s_source_t *psource )
{
	int		i;
	int		tcount = 0;	
	Vector	vmin, vmax;

	vmin[0] = vmin[1] = vmin[2] = 99999;
	vmax[0] = vmax[1] = vmax[2] = -99999;

	g_numfaces = 0;
	numvlist = 0;
 
	//
	// load the base triangles
	//
	int texture;
	int material;
	char texturename[64];

	while (1) 
	{
		if (fgets( g_szLine, sizeof( g_szLine ), g_fpInput ) == NULL) 
			break;

		g_iLinecount++;

		// check for end
		if (IsEnd( g_szLine )) 
			break;

		// Look for extra junk that we may want to avoid...
		int nLineLength = strlen( g_szLine );
		if (nLineLength >= 64)
		{
			MdlWarning("Unexpected data at line %d, (need a texture name) ignoring...\n", g_iLinecount );
			continue;
		}

		// strip off trailing smag
		strncpy( texturename, g_szLine, 63 );
		for (i = strlen( texturename ) - 1; i >= 0 && ! isgraph( texturename[i] ); i--)
		{
		}
		texturename[i + 1] = '\0';

		// funky texture overrides
		for (i = 0; i < numrep; i++)  
		{
			if (sourcetexture[i][0] == '\0') 
			{
				strcpy( texturename, defaulttexture[i] );
				break;
			}
			if (stricmp( texturename, sourcetexture[i]) == 0) 
			{
				strcpy( texturename, defaulttexture[i] );
				break;
			}
		}

		if (texturename[0] == '\0')
		{
			// weird source problem, skip them
			fgets( g_szLine, sizeof( g_szLine ), g_fpInput );
			fgets( g_szLine, sizeof( g_szLine ), g_fpInput );
			fgets( g_szLine, sizeof( g_szLine ), g_fpInput );
			g_iLinecount += 3;
			continue;
		}

		if (stricmp( texturename, "null.bmp") == 0 || stricmp( texturename, "null.tga") == 0)
		{
			// skip all faces with the null texture on them.
			fgets( g_szLine, sizeof( g_szLine ), g_fpInput );
			fgets( g_szLine, sizeof( g_szLine ), g_fpInput );
			fgets( g_szLine, sizeof( g_szLine ), g_fpInput );
			g_iLinecount += 3;
			continue;
		}

		texture = lookup_texture( texturename, sizeof( texturename ) );
		psource->texmap[texture] = texture;	// hack, make it 1:1
		material = use_texture_as_material( texture );

		s_face_t f;
		ParseFaceData( psource, material, &f );
	
		g_src_uface[g_numfaces] = f;
		g_face[g_numfaces].material = material;
		g_numfaces++;
	}

	BuildIndividualMeshes( psource );
}


int Load_SMD ( s_source_t *psource )
{
	char	cmd[1024];
	int		option;

	if (!OpenGlobalFile( psource->filename ))
		return 0;

	if( !g_quiet )
	{
		printf ("SMD MODEL %s\n", psource->filename);
	}

	g_iLinecount = 0;

	while (fgets( g_szLine, sizeof( g_szLine ), g_fpInput ) != NULL) 
	{
		g_iLinecount++;
		int numRead = sscanf( g_szLine, "%s %d", cmd, &option );

		// Blank line
		if ((numRead == EOF) || (numRead == 0))
			continue;

		if (strcmp( cmd, "version" ) == 0) 
		{
			if (option != 1) 
			{
				MdlError("bad version\n");
			}
		}
		else if (strcmp( cmd, "nodes" ) == 0) 
		{
			psource->numbones = Grab_Nodes( psource->localBone );
		}
		else if (strcmp( cmd, "skeleton" ) == 0) 
		{
			Grab_Animation( psource );
		}
		else if (strcmp( cmd, "triangles" ) == 0) 
		{
			Grab_Triangles( psource );
		}
		else if (strcmp( cmd, "vertexanimation" ) == 0) 
		{
			Grab_Vertexanimation( psource );
		}
		else 
		{
			MdlWarning("unknown studio command\n" );
		}
	}
	fclose( g_fpInput );

	is_v1support = true;

	return 1;
}

//-----------------------------------------------------------------------------
// Checks to see if the model source was already loaded
//-----------------------------------------------------------------------------
static s_source_t *FindCachedSource( char const* name, char const* xext )
{
	int i;

	if( xext[0] )
	{
		// we know what extension is necessary. . look for it.
		sprintf (g_szFilename, "%s%s.%s", cddir[numdirs], name, xext );
		for (i = 0; i < g_numsources; i++)
		{
			if (stricmp( g_szFilename, g_source[i]->filename ) == 0)
				return g_source[i];
		}
	}
	else
	{
		// we don't know what extension to use, so look for all of 'em.
		sprintf (g_szFilename, "%s%s.vrm", cddir[numdirs], name );
		for (i = 0; i < g_numsources; i++)
		{
			if (stricmp( g_szFilename, g_source[i]->filename ) == 0)
				return g_source[i];
		}
		sprintf (g_szFilename, "%s%s.smd", cddir[numdirs], name );
		for (i = 0; i < g_numsources; i++)
		{
			if (stricmp( g_szFilename, g_source[i]->filename ) == 0)
				return g_source[i];
		}
		/*
		sprintf (g_szFilename, "%s%s.vta", cddir[numdirs], name );
		for (i = 0; i < g_numsources; i++)
		{
			if (stricmp( g_szFilename, g_source[i]->filename ) == 0)
				return g_source[i];
		}
		*/
	}

	// Not found
	return 0;
}

static void FlipFacing( s_source_t *pSrc )
{
	unsigned short tmp;

	int i, j;
	for( i = 0; i < pSrc->nummeshes; i++ )
	{
		s_mesh_t *pMesh = &pSrc->mesh[i];
		for( j = 0; j < pMesh->numfaces; j++ )
		{
			s_face_t &f = pSrc->face[pMesh->faceoffset + j];
			tmp = f.b;  f.b  = f.c;  f.c  = tmp;
		}
	}
}

//-----------------------------------------------------------------------------
// Loads an animation source
//-----------------------------------------------------------------------------

s_source_t *Load_Source( char const *name, const char *ext, bool reverse, bool isActiveModel )
{
	if ( g_numsources >= MAXSTUDIOSEQUENCES )
		MdlError( "Load_Source( %s ) - overflowed g_numsources.", name );

	Assert(name);
	int namelen = strlen(name) + 1;
	char* pTempName = (char*)_alloca( namelen );
	char xext[32];
	int result = false;

	strcpy( pTempName, name );
	Q_ExtractFileExtension( pTempName, xext, sizeof( xext ) );

	if (xext[0] == '\0')
	{
		strcpyn( xext, ext );
	}
	else
	{
		Q_StripExtension( pTempName, pTempName, namelen );
	}

	s_source_t* pSource = FindCachedSource( pTempName, xext );
	if (pSource)
	{
		if (isActiveModel)
			pSource->isActiveModel = true;
		return pSource;
	}

	g_source[g_numsources] = (s_source_t *)kalloc( 1, sizeof( s_source_t ) );
	strcpyn( g_source[g_numsources]->filename, g_szFilename );


	if (isActiveModel)
	{
		g_source[g_numsources]->isActiveModel = true;
	}

	if ( ( !result && xext[0] == '\0' ) || stricmp( xext, "smd" ) == 0)
	{
		sprintf (g_szFilename, "%s%s.smd", cddir[numdirs], pTempName );
		strcpyn( g_source[g_numsources]->filename, g_szFilename );
		result = Load_SMD( g_source[g_numsources] );
	}

	if ( !result)
	{
		MdlError( "could not load file '%s'\n", g_source[g_numsources]->filename );
	}

	g_numsources++;
	if( reverse )
	{
		FlipFacing( g_source[g_numsources-1] );
	}
	return g_source[g_numsources-1];
}

void SaveNodes( s_source_t *source, CUtlBuffer& buf )
{
	if ( source->numbones <= 0 )
		return;

	buf.Printf( "nodes\n" );

	for ( int i = 0; i < source->numbones; ++i )
	{
		s_node_t *bone = &source->localBone[ i ];

		buf.Printf( "%d \"%s\" %d\n", i, bone->name, bone->parent );
	}

	buf.Printf( "end\n" );
}

// FIXME:  since we don't us a .qc, we could have problems with scaling, etc.???
void descale_vertex( Vector &org )
{
	float invscale = 1.0f / g_currentscale;

	org[0] = org[0] * invscale;
	org[1] = org[1] * invscale;
	org[2] = org[2] * invscale;
}

void SaveAnimation( s_source_t *source, CUtlBuffer& buf )
{
	if ( source->numbones <= 0 )
		return;

	buf.Printf( "skeleton\n" );

	for ( int frame = 0; frame < source->numframes; ++frame )
	{
		buf.Printf( "time %i\n", frame + source->startframe );

		for ( int i = 0; i < source->numbones; ++i )
		{
			s_bone_t *bone = &source->rawanim[ frame ][ i ];
			s_bone_t *prev = NULL;
			if ( frame > 0 )
			{
				if ( source->rawanim[ frame - 1 ] )
				{
					prev = &source->rawanim[ frame - 1 ][ i ];
				}
			}

			Vector pos = source->rawanim[ frame ][ i ].pos;
			descale_vertex( pos );
			RadianEuler rot = source->rawanim[ frame ][ i ].rot;

// If this is enabled, then we delta this pos vs the prev frame and don't write out a sample if it's the same value...
#if 0
			if ( prev )
			{
				Vector ppos = source->rawanim[ frame -1 ][ i ].pos; 
				descale_vertex( pos );
				RadianEuler prot = source->rawanim[ frame -1 ][ i ].rot;

				// Only output it if there's a delta
				if ( ( ppos != pos ) || 
					Q_memcmp( &prot, &rot, sizeof( prot ) ) )
				{
					buf.Printf
						( "%d %f %f %f %f %f %f\n", 
						i,				// bone index
						pos[ 0 ],
						pos[ 1 ],
						pos[ 2 ],
						rot[ 0 ],
						rot[ 1 ],
						rot[ 2 ]
						);
				}
			}
			else
#endif
			{
				buf.Printf
					( "%d %f %f %f %f %f %f\n", 
					i,				// bone index
					pos[ 0 ],
					pos[ 1 ],
					pos[ 2 ],
					rot[ 0 ],
					rot[ 1 ],
					rot[ 2 ]
					);
			}
		}
	}

	buf.Printf( "end\n" );
}

void Save_SMD( char const *filename, s_source_t *source )
{
	// Text buffer
	CUtlBuffer buf( 0, 0, true );

	buf.Printf( "version 1\n" );

	SaveNodes( source, buf );
	SaveAnimation( source, buf );

	FileHandle_t fh = g_pFileSystem->Open( filename, "wb" );
	if ( FILESYSTEM_INVALID_HANDLE != fh )
	{
		g_pFileSystem->Write( buf.Base(), buf.TellPut(), fh );
		g_pFileSystem->Close( fh );
	}
}

s_source_t *MotionMap( s_source_t *pSource, s_source_t *pTarget )
{
#if 0
	// Process motion mapping into out and return that
	s_source_t *out = new s_source_t;

	return out;
#else
	// Just returns the start animation, to test the Save_SMD API.
	return pSource;
#endif
}


void UsageAndExit()
{
	MdlError( "usage: motionmapper [-quiet] [-verbose] sourceanim.smd targetskeleton.smd output.smd\n\
		\tsourceanim:  should contain ref pose and animation data\n\
		\ttargetsekeleton:  should contain new ref pose, animation data ignored/can be absent\n\
		\toutput:  animation from source mapped onto target skeleton (contains new ref pose)\n");
}

void PrintHeader()
{
	vprint( 0, "Valve Software - motionmapper.exe ((c) Valve Coroporation %s)\n", __DATE__ );
	vprint( 0, "--- Maps motion from one animation/skeleton onto another skeleton ---\n" );
}

/*
==============
main
==============
*/
int main (int argc, char **argv)
{
	int		i;

	PrintHeader();

	CommandLine()->CreateCmdLine( argc, argv );

	InstallSpewFunction();
	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f, false, false, false, false );

	g_currentscale = g_defaultscale = 1.0;
	g_defaultrotation = RadianEuler( 0, 0, M_PI / 2 );

	if (argc == 1)
	{
		UsageAndExit();
	}
	
	g_quiet = false;	
	
	CUtlVector< CUtlSymbol >	filenames;

	for (i = 1; i < argc; i++) 
	{
		if (argv[i][0] == '-') 
		{
			if (!stricmp(argv[i], "-allowdebug"))
			{
				// Ignore, used by interface system to catch debug builds checked into release tree
				continue;
			}

			if (!stricmp(argv[i], "-quiet"))
			{
				g_quiet = true;
				g_verbose = false;
				continue;
			}

			if (!stricmp(argv[i], "-verbose"))
			{
				g_quiet = false;
				g_verbose = true;
				continue;
			}
		}
		else
		{
			CUtlSymbol sym = argv[ i ];
			filenames.AddToTail( sym );
		}
	}	

	if ( filenames.Count() != 3 )
	{
		// misformed arguments
		// otherwise generating unintended results
		UsageAndExit();
	}

	int sourceanim = 0;
	int targetskel = 1;
	int outputanim = 2;

	strcpy( g_outfile, filenames[ outputanim ].String() );

	CmdLib_InitFileSystem( g_outfile );

	Q_FileBase( g_outfile, g_outfile, sizeof( g_outfile ) );

	if (!g_quiet)
	{
		vprint( 0, "%s, %s, %s, path %s\n", qdir, gamedir, g_outfile );
	}

	Q_DefaultExtension(g_outfile, ".smd", sizeof( g_outfile ) );
	if (!g_quiet)
	{
		vprint( 0, "Source animation:  %s\n", filenames[ sourceanim ].String() );
		vprint( 0, "Target skeleton:  %s\n", filenames[ targetskel ].String() );

		vprint( 0, "Creating on \"%s\"\n", g_outfile);
	}

	strcpy( fullpath, g_outfile );
	strcpy( fullpath, ExpandPath( fullpath ) );
	strcpy( fullpath, ExpandArg( fullpath ) );
	
	// Do work
	s_source_t *pSource = Load_Source( filenames[sourceanim].String(), "smd", false, false );
	s_source_t *pTarget = Load_Source( filenames[targetskel].String(), "smd", false, false );

	// Process skeleton
	s_source_t *pMappedAnimation = MotionMap( pSource, pTarget );

	// Save output (ref skeleton & animation data);
	Save_SMD( fullpath, pMappedAnimation );

	Q_StripExtension( filenames[outputanim].String(), outname, sizeof( outname ) );

	if (!g_quiet)
	{
		vprint( 0, "\nCompleted \"%s\"\n", g_outfile);
	}

	return 0;
}




