#include <cbase.h>
#include <assert.h>

#include "sv_soap.h"
#include "sv.h"
#include "SVBinding.nsmap"

C_SOAP C_SOAP::s_SOAP;

C_SOAP::C_SOAP()
{
	m_bValid = false;
}

bool C_SOAP::Valid()
{
	return m_bValid;
}

#define INIT_STRUCT(str,name) \
	struct str name; \
	memset(&name, 0, sizeof(name));

void C_SOAP::Init(const char *pszEndpoint)
{
	endpoint = pszEndpoint;

	INIT_STRUCT(sv__doGetSVVersionResponse, Result);

	int iSoapResult = sv__doGetSVVersion(GetSTAT()->MajorVersion(), GetSTAT()->MinorVersion(), Result);

	// If you add a reason that this interface is invalid, DOCUMENT it in RATE.h!
	m_bValid = true;

	// Can't connect? Invalidate.
	VALIDATE(iSoapResult);

	// Major versions are incompatible with each other.
	if (Result.major != GetSTAT()->MajorVersion())
		m_bValid = false;

	// Minor version of the client is only compatible with equal or greater
	// minor version from the server.
	if (Result.minor < GetSTAT()->MinorVersion())
		m_bValid = false;

	// Patch versions are always compatible.
}

void C_SOAP::GetCategories(Categories* pCategories)
{
	if (!Valid())
		return;

	INIT_STRUCT(sv__doGetCategoriesResponse, Result);

	int iSoapResult = sv__doGetCategories(Result);

	VALIDATE(iSoapResult);

	if (!Valid())
		return;

	int iTotalCategories = Result.result->__sizecategories;
	(*pCategories).resize(iTotalCategories);

	for (int i = 0; i < iTotalCategories; i++)
	{
		(*pCategories)[i].m_iID = Result.result->categories[i]->id;
		(*pCategories)[i].m_pszName = Result.result->categories[i]->name;
	}
}

int C_SOAP::GetPlayerID(const char* pszGameID)
{
	if (!Valid())
		return -1;

	if (!pszGameID)
		return -1;

	int iRateID = 0;

	// Network is default 1 (Steam) for now.
	int iSoapResult = sv__doGetPlayerID(const_cast<char*>(pszGameID), 1, iRateID);

	VALIDATE(iSoapResult);

	if (!Valid())
		return -1;

	return iRateID;
}

float C_SOAP::GetPlayerRating(SVID iPlayer, int iCategory)
{
	if (!Valid())
		return -1;

	if (iPlayer <= 0)
		return -1;

	float flRating = 0;

	int iSoapResult = sv__doGetPlayerRating(iPlayer, iCategory, flRating);

	VALIDATE(iSoapResult);

	if (!Valid())
		return -1;

	return flRating;
}

void C_SOAP::GetPlayerRatings(SVID iPlayer, PlayerRatings* pRatings)
{
	if (!Valid())
		return;

	if (iPlayer <= 0)
		return;

	if (!pRatings)
		return;

	INIT_STRUCT(sv__doGetPlayerRatingsResponse, Result);

	int iSoapResult = sv__doGetPlayerRatings(iPlayer, Result);

	VALIDATE(iSoapResult);

	if (!Valid())
		return;

	int iTotalRatings = Result.result->__sizeratings;
	(*pRatings).resize(iTotalRatings);

	for (int i = 0; i < iTotalRatings; i++)
	{
		(*pRatings)[i].m_iCategory = Result.result->ratings[i]->category;
		(*pRatings)[i].m_flRating = Result.result->ratings[i]->rating;
	}
}

void C_SOAP::RatePlayer(SVID iFromPlayer, SVID iToPlayer, int iCategory, int iRating)
{
	if (!Valid())
		return;

	if (iFromPlayer <= 0 || iToPlayer <= 0)
		return;

	if (iFromPlayer == iToPlayer)
		return;

	INIT_STRUCT(sv__doRatePlayerResponse, Result);

	int iSoapResult = sv__doRatePlayer(iFromPlayer, iToPlayer, iCategory, iRating, Result);

	VALIDATE(iSoapResult);
}
