#ifndef SV_SOAP_H
#define SV_SOAP_H

#include "sv.h"
#include "soapSVBindingProxy.h"

class C_SOAP : public SVBinding, public INetworkInterface
{
public:
	C_SOAP();

	// INetworkInterface
	bool	Valid();

	void	Init(const char *pszEndpoint);

	void	GetCategories(Categories* pCategories);

	SVID	GetPlayerID(const char* pszGameID);

	float	GetPlayerRating(SVID iPlayer, int iCategory);
	void	GetPlayerRatings(SVID iPlayer, PlayerRatings* pRatings);

	void	RatePlayer(SVID iFromPlayer, SVID iToPlayer, int iCategory, int iRating);

	inline static C_SOAP*	GetSOAP() { return &s_SOAP; };

private:
	static C_SOAP	s_SOAP;

	bool			m_bValid;
};

#define VALIDATE(result) \
	if (result != SOAP_OK) \
		m_bValid = false;

#define GetSOAP() C_SOAP::GetSOAP()

#endif
