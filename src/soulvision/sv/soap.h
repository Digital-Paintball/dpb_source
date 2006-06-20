#ifndef SOAP_H
#define SOAP_H

#include "soapSVBindingObject.h"
#include "data.h"

class CSOAP : public SVBinding
{
public:
			CSOAP();
			~CSOAP();

	void	Init(IData* pDB);
	int		Bind(const char* pszHost, const int iPort, const int iBacklog);
	int		Accept(long iTimeout = 0);
	void	Serve();

	int		GetSVVersion(int iMajor, int iMinor, struct sv__doGetSVVersionResponse &Result);

	int		GetCategories(struct sv__doGetCategoriesResponse &Result);

	int		GetPlayerID(const char* pszGameID, int iNetwork, int &iResult);

	int		GetPlayerRating(int SVID, int iCategory, float &flResult);
	int		GetPlayerRatings(int SVID, struct sv__doGetPlayerRatingsResponse &Result);

	int		RatePlayer(SVID iFromPlayer, SVID iToPlayer, int iCategory, int iRating, struct sv__doRatePlayerResponse &Result);

	void	DestroyArrays();
	void	CreateArrays();

	char*	StrDup(const char* pszSource);

	long	Served();

private:
	IData*	m_pDB;
	int		m_iCategories;

	long	m_iServed;

	sv__PlayerRating**		m_rgRatings;
	sv__PlayerRatingArray	m_RatingArray;

	bool					m_bHaveCategories;
	sv__Category**			m_rgCategories;
	sv__CategoryArray		m_CategoryArray;
};

#define SOAP_IPV4(soap,i) ((int)(soap->ip>>8*(4-i))&0xFF)

#define SOAP_STUB(callname) \
{	CSOAP* pSoap = CStatSrv::GetSoap(); \
	LOGINFO(VarArgs("%d.%d.%d.%d - " #callname, SOAP_IPV4(pSoap,1), SOAP_IPV4(pSoap,2), SOAP_IPV4(pSoap,3), SOAP_IPV4(pSoap,4))); \
	if (!pSoap) return soap_receiver_fault(soap, "Internal error", "Unrecognized soap object.");

#define SOAP_STUB_INT(callname) \
	int sv__do##callname(struct soap* soap, int &iResult) \
	SOAP_STUB(callname) \
	return pSoap->callname(iResult); }

#define SOAP_STUB_FLOAT_INT(callname,var1) \
	int sv__do##callname(struct soap* soap, int var1, float &flResult) \
	SOAP_STUB(callname) \
	return pSoap->callname(var1, flResult); }

#define SOAP_STUB_FLOAT_INTINT(callname,var1,var2) \
	int sv__do##callname(struct soap* soap, int var1, int var2, float &flResult) \
	SOAP_STUB(callname) \
	return pSoap->callname(var1, var2, flResult); }

#define SOAP_STUB_INT_STRINGINT(callname,var1,var2) \
	int sv__do##callname(struct soap* soap, char* var1, int var2, int &iResult) \
	SOAP_STUB(callname) \
	return pSoap->callname(var1, var2, iResult); }

#define SOAP_STUB_STRUCT(callname) \
	int sv__do##callname(struct soap* soap, struct sv__do##callname##Response &Result) \
	SOAP_STUB(callname) \
	return pSoap->callname(Result); }

#define SOAP_STUB_STRUCT_INT(callname,var1) \
	int sv__do##callname(struct soap* soap, int var1, struct sv__do##callname##Response &Result) \
	SOAP_STUB(callname) \
	return pSoap->callname(var1, Result); }

#define SOAP_STUB_STRUCT_INTINT(callname,var1,var2) \
	int sv__do##callname(struct soap* soap, int var1, int var2, struct sv__do##callname##Response &Result) \
	SOAP_STUB(callname) \
	return pSoap->callname(var1, var2, Result); }

#define SOAP_STUB_STRUCT_INTINTINTINT(callname,var1,var2,var3,var4) \
	int sv__do##callname(struct soap* soap, int var1, int var2, int var3, int var4, struct sv__do##callname##Response &Result) \
	SOAP_STUB(callname) \
	return pSoap->callname(var1, var2, var3, var4, Result); }

#endif
