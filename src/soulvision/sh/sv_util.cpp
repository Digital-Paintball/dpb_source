#include <map>
#include "sv_util.h"
#include "configfile.h"
#include "logger.h"

#include "mmgr.h"	// Must be LAST include

apr_pool_t *g_pPool = NULL;

void CreatePrimaryPool()
{
	if (apr_pool_create(&g_pPool, NULL) != APR_SUCCESS)
	{
		LOGERROR("Couldn't create primary memory pool.");
		exit(-1);
	}
}

apr_pool_t* GetPrimaryPool()
{
	return g_pPool;
}

std::map<char, const char*> g_Parameters;

void CmdParameterInit(int iArgC, char const* const* pszArgV, const apr_getopt_option_t* pOptions)
{
	apr_getopt_t* pOpt;

	const char *pszParameter;
	int cOption;

	apr_getopt_init(&pOpt, GetPrimaryPool(), iArgC, pszArgV);

	while (apr_getopt_long(pOpt, pOptions, &cOption, &pszParameter) == APR_SUCCESS)
	{
		g_Parameters[cOption] = pszParameter;
	}
}

bool CmdParameterValue(char cOption)
{
	if (g_Parameters.find(cOption) != g_Parameters.end())
		return true;
	else
		return false;
}

const char* CmdParameterValue(char cOption, const char* pszDefault)
{
	if (g_Parameters.find(cOption) != g_Parameters.end())
		return g_Parameters[cOption];
	else
		return pszDefault;
}

long CmdParameterValue(char cOption, long iDefault)
{
	if (g_Parameters.find(cOption) != g_Parameters.end())
		return atol(g_Parameters[cOption]);
	else
		return iDefault;
}

ConfigFile g_ConfigFile;

void ConfigInit(const char* pszConfigFile)
{
	if (!pszConfigFile || strlen(pszConfigFile) == 0)
		return;

	g_ConfigFile = ConfigFile(pszConfigFile);
}

bool ConfigValue(const char* pszKey, bool bDefault)
{
	return g_ConfigFile.read( pszKey, bDefault );
}

const char* ConfigValue(const char* pszKey, const char* pszDefault)
{
	return g_ConfigFile.read( pszKey, pszDefault );
}

long ConfigValue(const char* pszKey, long iDefault)
{
	return g_ConfigFile.read( pszKey, iDefault );
}

float ConfigValue(const char* pszKey, float flDefault)
{
	return g_ConfigFile.read( pszKey, flDefault );
}

char *VarArgs(char *pszFormat, ...)
{
	va_list argptr;
	static char szString[1024];
	
	va_start (argptr, pszFormat);
	apr_vsnprintf(szString, sizeof(szString), pszFormat, argptr);
	va_end (argptr);

	return szString;	
}
