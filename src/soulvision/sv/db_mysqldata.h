#ifndef DB_MYSQLDATA_H
#define DB_MYSQLDATA_H

#include <stdarg.h>

#include "db_mysql.h"
#include "data.h"
#include "db_mysql_columns.h"

class CMySQLData : public IData, public CMySQL
{
public:
					CMySQLData();
					~CMySQLData();

	bool			Valid();

	inline bool		AreVotesSummed();
	inline int		GetTotalPlayers();
	inline int		GetTotalCategories();

	SVID			GetPlayerID(const char* pszGameID, int iNetwork);

	void			GetCategories(sv__CategoryArray* pCategory);

	float			GetPlayerRating(SVID rateID, int iCategory);
	void			GetPlayerRatings(SVID rateID, sv__PlayerRatingArray* pVotes);

	void			RatePlayer(SVID iFromPlayer, SVID iToPlayer, int iCategory, int iRating);
	void			SumVotes();

	SVID			CreateNewPlayer(const char* pszGameID, int iNetwork);

private:
	void			UpdateStatus();

	DB_TABLE_DECLARE(Status);
	DB_TABLE_DECLARE(Users);
	DB_TABLE_DECLARE(UserIDs);
	DB_TABLE_DECLARE(Categories);
	DB_TABLE_DECLARE(Votes);
	DB_TABLE_DECLARE(VoteSums);
};

bool CMySQLData::AreVotesSummed()
{
	const CDBValue* pValue = m_Status.GetValue(COL_STATUS_SUMMED);

	if (pValue->IsDirty())
		UpdateStatus();

	if (pValue->IsNull())
		SumVotes();

	return *pValue;
}

int CMySQLData::GetTotalPlayers()
{
	const CDBValue* pValue = m_Status.GetValue(COL_STATUS_PLAYERS);

	if (pValue->IsDirty())
		UpdateStatus();

	if (pValue->IsNull())
		SumVotes();

	return *pValue;
}

int CMySQLData::GetTotalCategories()
{
	const CDBValue* pValue = m_Status.GetValue(COL_STATUS_CATEGORIES);

	if (pValue->IsDirty())
		UpdateStatus();

	if (pValue->IsNull())
		SumVotes();

	return *pValue;
}

#endif
