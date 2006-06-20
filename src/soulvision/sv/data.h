#ifndef DATA_H
#define DATA_H

#include "database.h"
#include "sv_shared.h"

class sv__PlayerRatingArray;
class sv__CategoryArray;

class IData : public IDatabase
{
public:
	// IData derivatives' destructors will not be called without this. (But constructors will? WTF?)
	virtual				~IData() {};

	virtual bool		AreVotesSummed() = 0;
	virtual int			GetTotalPlayers() = 0;
	virtual int			GetTotalCategories() = 0;

	virtual SVID		GetPlayerID(const char* pszGameID, int iNetwork) = 0;

	virtual void		GetCategories(sv__CategoryArray* pCategory) = 0;

	virtual float		GetPlayerRating(SVID SVID, int iCategory) = 0;
	virtual void		GetPlayerRatings(SVID SVID, sv__PlayerRatingArray* pVotes) = 0;

	virtual void		RatePlayer(SVID iFromPlayer, SVID iToPlayer, int iCategory, int iRating) = 0;

	virtual void		SumVotes() = 0;
};

// This is so that only db_mysql.cpp needs a <mysql.h> dependency.
IData* CreateMySQL();
// IDataBase* CreateOracle();
// IDataBase* CreateBDB();
// etc

#endif
