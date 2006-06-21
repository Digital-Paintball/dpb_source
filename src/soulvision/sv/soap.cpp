#include "soap.h"
#include "main.h"

#include "SVBinding.nsmap"

#include "mmgr.h"	// Must be LAST include

CSOAP::CSOAP()
{
	m_rgRatings = NULL;
	m_rgCategories = NULL;
	m_bHaveCategories = false;
}

CSOAP::~CSOAP()
{
	DestroyArrays();
}

void CSOAP::Init(IData* pDB)
{
	m_pDB = pDB;

	CreateArrays();
}

void CSOAP::CreateArrays()
{
	DestroyArrays();

	m_iCategories = m_pDB->GetTotalCategories();

	m_rgRatings = new sv__PlayerRating*[m_iCategories];
	m_RatingArray.ratings = m_rgRatings;
	for (int i = 0; i < m_iCategories; i++)
	{
		m_rgRatings[i] = new sv__PlayerRating;
	}

	m_rgCategories = new sv__Category*[m_iCategories];
	m_CategoryArray.categories = m_rgCategories;
	for (int i = 0; i < m_iCategories; i++)
	{
		m_rgCategories[i] = new sv__Category;
		m_rgCategories[i]->name = NULL;
	}
	m_bHaveCategories = false;
}

void CSOAP::DestroyArrays()
{
	if (m_rgRatings)
	{
		for (int i = 0; i < m_iCategories; i++)
		{
			delete m_rgRatings[i];
		}
		delete[] m_rgRatings;
		m_RatingArray.ratings = NULL;
	}

	if (m_rgCategories)
	{
		for (int i = 0; i < m_iCategories; i++)
		{
			if (m_rgCategories[i]->name)
				delete[] m_rgCategories[i]->name;
			delete m_rgCategories[i];
		}
		delete[] m_rgCategories;
		m_CategoryArray.categories = NULL;
	}
}

int CSOAP::Bind(const char* pszHost, const int iPort, const int iBacklog)
{
	return soap_bind(this, pszHost, iPort, iBacklog);
}

int CSOAP::Accept(long iTimeout)
{
	accept_timeout = iTimeout;
	return soap_accept(this);
}

void CSOAP::Serve()
{
	// Hopefully I can outdo McDonalds.
	m_iServed++;

	soap_serve(this);
	soap_destroy(this);
	soap_end(this);
}

long CSOAP::Served()
{
	return m_iServed;
}

int CSOAP::GetSVVersion(int iMajor, int iMinor, struct sv__doGetSVVersionResponse &Result)
{
	Result.major = DPB_MAJOR_VERSION;
	Result.minor = DPB_MINOR_VERSION;

	return SOAP_OK;
}

int CSOAP::GetCategories(struct sv__doGetCategoriesResponse &Result)
{
	// Only need to grab the categories once.
	if (!m_bHaveCategories)
	{
		m_pDB->GetCategories(&m_CategoryArray);
		m_bHaveCategories = true;
	}

	Result.result = &m_CategoryArray;

	return SOAP_OK;
}

int CSOAP::GetPlayerID(const char* pszGameID, int iNetwork, int &iResult)
{
	if (iNetwork != 1)
		return soap_sender_fault(this, "Invalid parameter", "Invalid network.");

	iResult = m_pDB->GetPlayerID(pszGameID, iNetwork);

	return SOAP_OK;
}

int CSOAP::GetPlayerRating(SVID SVID, int iCategory, float &flResult)
{
	flResult = m_pDB->GetPlayerRating(SVID, iCategory);

	return SOAP_OK;
}

int CSOAP::GetPlayerRatings(SVID SVID, struct sv__doGetPlayerRatingsResponse &Result)
{
	m_pDB->GetPlayerRatings(SVID, &m_RatingArray);

	Result.result = &m_RatingArray;

	return SOAP_OK;
}

int CSOAP::RatePlayer(SVID iFromPlayer, SVID iToPlayer, int iCategory, int iRating, struct sv__doRatePlayerResponse &Result)
{
	if (iFromPlayer <= 0)
		return SOAP_OK;

	if (iToPlayer <= 0)
		return SOAP_OK;

	if (iCategory <= 0)
		return SOAP_OK;

	if (iRating < 0)
		iRating = 0;

	if (iRating > 5)
		iRating = 5;

	m_pDB->RatePlayer(iFromPlayer, iToPlayer, iCategory, iRating);

	return SOAP_OK;
}

SOAP_STUB_STRUCT_INTINT(GetSVVersion, iMajor, iMinor);
SOAP_STUB_STRUCT(GetCategories);
SOAP_STUB_INT_STRINGINT(GetPlayerID, pszGameID, iNetwork);
SOAP_STUB_FLOAT_INTINT(GetPlayerRating, SVID, iCategory);
SOAP_STUB_STRUCT_INT(GetPlayerRatings, SVID);
SOAP_STUB_STRUCT_INTINTINTINT(RatePlayer, iFromPlayer, iToPlayer, iCategory, iRating);
