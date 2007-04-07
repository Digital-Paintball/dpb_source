#ifndef SV_H
#define SV_H

#include <vector>

#include "sv_shared.h"

class CPlayerRating
{
public:
	int		m_iCategory;
	float	m_flRating;
};

class CCategory
{
public:
	const char*	m_pszName;
	int			m_iID;
};

typedef std::vector<CPlayerRating> PlayerRatings;
typedef std::vector<CCategory> Categories;

/** INetworkInterface
 * Every function in this interface except Valid() makes a network query,
 * and blocks until it receives a response.
 *
 * If the network is invalidated (for example, if the RATE server goes
 * down) it will be detected the next time one of these functions is
 * called, the function will return an invalid result (for example -1) and
 * Valid() will return false. Always be sure to check the return value for
 * an invalid result before using it for another purpose. However, calling
 * into the interface if it is invalid will not result in network queries,
 * and will not cause any crashing or other problems of that nature.
 *
 * It would be nice if there were a Validate() or something that could be
 * called before any batch of network commands, to tell ahead of time if
 * the interface has been invalidated, but the overhead would be too much,
 * so it is best to check as the functions are called instead as it
 * involves no extra overhead.
 *
 * The prescribed method of attempting to revalidate an invalidated network
 * interface is to call its Init() function again. This is a good idea to
 * do between map changes, or every half hour or something.
 */
class INetworkInterface
{
public:
	// An interface can be invalid because:
	//   - The server is unavailable.
	//   - The major version of the server differs.
	//   - The minor version is lesser on the server.
	// Don't use the interface if it's invalid. It won't work.
	virtual bool	Valid() = 0;

	virtual void	Init(const char* pszEndpoint) = 0;

	virtual void	GetCategories(Categories* pCategories) = 0;

	// Invalidated will return -1.
	virtual SVID	GetPlayerID(const char* pszGameID) = 0;

	// Invalidated will return -1.
	virtual float	GetPlayerRating(SVID iPlayer, int iCategory) = 0;
	virtual void	GetPlayerRatings(SVID iPlayer, PlayerRatings* pRatings) = 0;

	virtual void	RatePlayer(SVID iFromPlayer, SVID iToPlayer, int iCategory, int iRating) = 0;
};

class ISTATInterface
{
public:
	virtual const char*	Version() = 0;
	virtual int			MajorVersion() = 0;
	virtual int			MinorVersion() = 0;
	virtual int			PatchVersion() = 0;

	virtual	int			LookupCategory(Categories *pCategories, const char* pszName) = 0;
};

void GetInterfaces(INetworkInterface* &pNetwork, ISTATInterface* &pSTAT);

class C_STAT : public ISTATInterface
{
public:
	// IRATEInterface
	virtual const char*	Version();
	virtual int			MajorVersion();
	virtual int			MinorVersion();
	virtual int			PatchVersion();

	virtual	int			LookupCategory(Categories *pCategories, const char* pszName);

	inline static C_STAT*	GetSTAT() { return &s_STAT; };

private:
	static C_STAT	s_STAT;
};

#define GetSTAT() C_STAT::GetSTAT()

#endif
