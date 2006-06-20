#include <string>

#include "db_mysqldata.h"
#include "soap.h"

#include "mmgr.h"	// Must be LAST include

IData* CreateMySQL()
{
	IData *pDB = new CMySQLData();

	if (pDB->Valid())
	{
		LOGINFO("Using MySQL database");
		return pDB;
	}
	else
	{
		delete pDB;
        return NULL;
	}
}

CMySQLData::CMySQLData()
{
	IDBTable::BuildDataMaps();
}

CMySQLData::~CMySQLData()
{
}

void CMySQLData::UpdateStatus()
{
	CMySQLQuery Query(this, "SELECT * FROM " TABLE_STR " LIMIT 1", &m_Status);

	if (Query.GetResults()->row_count < 1)
	{
		// Insert an empty record, so we have something to work with.
		CMySQLQuery Insert(this, "INSERT " TABLE_STR " () VALUES ()", &m_Status);
		UpdateStatus();
		return;
	}

	MapRow(Query.GetResults(), &m_Status);
}

SVID CMySQLData::GetPlayerID(const char* pszGameID, int iNetwork)
{
	int iStrLen = strlen(pszGameID)*2+11;	//strlen(pszGameID)*2+1 for mysql, +10 for REVERSED()

	char* pszEscaped = new char[iStrLen];
	Escape(pszEscaped, pszGameID, strlen(pszGameID));

	char* pszQuoted = new char[iStrLen];

	// STEAM ID's are stored backwards, so that the relevant characters are in
	// the front and can be indexed easily.
	apr_snprintf(pszQuoted, iStrLen, "%s\"%s\"%s", (iNetwork == 1)?"REVERSE(":"", pszEscaped, (iNetwork == 1)?")":"");

	std::string sQuoted = pszQuoted;

	delete [] pszQuoted;
	delete [] pszEscaped;

	CMySQLQuery Query(this, "SELECT " COL_GAMEIDS_ID " FROM " TABLE_STR " WHERE "
		COL_GAMEIDS_VALUE "=" STRING_STR " AND "
		COL_GAMEIDS_NETWORK "=" INT_STR, &m_UserIDs, true, sQuoted.c_str(), iNetwork);

	// Need to create this ID.
	if (Query.GetResults()->row_count < 1)
		return CreateNewPlayer(sQuoted.c_str(), iNetwork);

	MapRow(Query.GetResults(), &m_UserIDs);

	return m_UserIDs.GetValueInt(COL_GAMEIDS_ID);
}

SVID CMySQLData::CreateNewPlayer(const char* pszGameID, int iNetwork)
{
#ifdef _DEBUG
	CMySQLQuery DebugQuery(this, "SELECT " COL_GAMEIDS_ID " FROM " TABLE_STR " WHERE "
		COL_GAMEIDS_VALUE "=" STRING_STR " AND "
		COL_GAMEIDS_NETWORK "=" INT_STR, &m_UserIDs, true, pszGameID, iNetwork);

	assert(DebugQuery.GetResults()->row_count <= 0);
#endif

	CMySQLQuery UserInsert(this, "INSERT " TABLE_STR " (" COL_USERS_USER ") VALUES ('newuser')", &m_Users);

	SVID iID = (SVID)LastInsertID();

	CMySQLQuery UserIDInsert(this, "INSERT " TABLE_STR
		" (" COL_GAMEIDS_ID "," COL_GAMEIDS_NETWORK "," COL_GAMEIDS_VALUE ") VALUES "
		"(" INT_STR "," INT_STR "," STRING_STR ")",
		&m_UserIDs, true, iID, iNetwork, pszGameID);

	return iID;
}

void CMySQLData::GetCategories(sv__CategoryArray* pCategory)
{
	CDataResults vCategories;
	CMySQLQuery Query(this, "SELECT " COL_CATS_ID "," COL_CATS_NAME " FROM " TABLE_STR,
		&m_Categories, true);

	MapResults(Query.GetResults(), &m_Categories, &vCategories);

	pCategory->__sizecategories = vCategories.size();

	for (int i = 0; i < pCategory->__sizecategories; i++)
	{
		pCategory->categories[i]->id = vCategories.GetInt(i, COL_CATS_ID);

		// New string allocated here, deallocated in CSOAP::DestroyArrays(). Funky, but it works.
		const char* pszCatName = vCategories.GetString(i, COL_CATS_NAME);
		pCategory->categories[i]->name = new char[strlen(pszCatName)+1];
		strcpy(pCategory->categories[i]->name, pszCatName);
	}
}

float CMySQLData::GetPlayerRating(SVID SVID, int iCategory)
{
	CMySQLQuery Query(this, "SELECT " COL_SUMS_VOTE " FROM " TABLE_STR " WHERE "
		COL_SUMS_PLAYER "=" INT_STR " AND "
		COL_SUMS_CATEGORY "=" INT_STR, &m_VoteSums, true, SVID, iCategory);

	if (Query.GetResults()->row_count < 1)
		return -1;

	MapRow(Query.GetResults(), &m_VoteSums);

	return m_VoteSums.GetValueFloat(COL_SUMS_VOTE);
}

void CMySQLData::GetPlayerRatings(SVID SVID, sv__PlayerRatingArray* pVotes)
{
	CDataResults vVotes;
	CMySQLQuery Query(this, "SELECT " COL_SUMS_VOTE "," COL_SUMS_CATEGORY " FROM " TABLE_STR " WHERE "
		COL_SUMS_PLAYER "=" INT_STR, &m_VoteSums, true, SVID);

	MapResults(Query.GetResults(), &m_VoteSums, &vVotes);

	pVotes->__sizeratings = vVotes.size();

	for (int i = 0; i < pVotes->__sizeratings; i++)
	{
		pVotes->ratings[i]->category = vVotes.GetInt(i, COL_SUMS_CATEGORY);
		pVotes->ratings[i]->rating = vVotes.GetFloat(i, COL_SUMS_VOTE);
	}
}

void CMySQLData::RatePlayer(SVID iFromPlayer, SVID iToPlayer, int iCategory, int iRating)
{
	CMySQLQuery Query(this, "REPLACE " TABLE_STR
		" (" COL_VOTES_FROMPLAYER "," COL_VOTES_TOPLAYER "," COL_VOTES_CATEGORY "," COL_VOTES_VOTE ") "
		"VALUES (" INT_STR "," INT_STR "," INT_STR "," INT_STR ")",
		&m_Votes, true, iFromPlayer, iToPlayer, iCategory, iRating);
}

void CMySQLData::SumVotes()
{
	LOGINFO("Summing votes...");

	unsigned int i, iTotalUsers, iTotalCategories;
	CDataResults vUserIDs, vCatIDs;

	CMySQLQuery UsersQuery(this, "SELECT " COL_USERS_ID " FROM " TABLE_STR, &m_Users);
	MapResults(UsersQuery.GetResults(), &m_Users, &vUserIDs);
	iTotalUsers = (unsigned int)UsersQuery.GetResults()->row_count;

	CMySQLQuery CatsQuery(this, "SELECT " COL_CATS_ID " FROM " TABLE_STR, &m_Categories);
	MapResults(CatsQuery.GetResults(), &m_Categories, &vCatIDs);
	iTotalCategories = (unsigned int)CatsQuery.GetResults()->row_count;

	char szValues[100];
	std::string sInsertQuery = "INSERT " TABLE_STR
		"(" COL_SUMS_CATEGORY "," COL_SUMS_PLAYER "," COL_SUMS_VOTE "," COL_SUMS_TOTAL ") VALUES ";
	bool bFirst = true;

	for (i = 0; i < iTotalUsers; i++)
	{
		for (unsigned int j = 0; j < iTotalCategories; j++)
		{
			CMySQLQuery Query(this, "SELECT " COL_VOTES_VOTE " FROM " TABLE_STR " WHERE "
				COL_VOTES_TOPLAYER "=" INT_STR " AND "
				COL_VOTES_CATEGORY "=" INT_STR, &m_Votes, true,
				vUserIDs.GetInt(i, COL_USERS_ID),
				vCatIDs.GetInt(j, COL_CATS_ID));

			unsigned int iTotalVotes = (unsigned int)Query.GetResults()->row_count;

			CDataResults vVotes;
			MapResults(Query.GetResults(), &m_Votes, &vVotes);

			float flVotes = 0;

			for (unsigned int k = 0; k < vVotes.size(); k++)
			{
				flVotes += vVotes.GetInt(k, COL_VOTES_VOTE);
			}

			if (iTotalVotes)
			{
				if (iTotalVotes > 1)
					flVotes /= iTotalVotes;

				if (!bFirst)
					sInsertQuery += ',';
				else
					bFirst = false;

				int iReturn = apr_snprintf(szValues, 100, "(%d,%d,%f,%d)",
					vCatIDs.GetInt(j, COL_USERS_ID),
					vUserIDs.GetInt(i, COL_CATS_ID),
					flVotes, iTotalVotes);

				assert(iReturn < 100);

				sInsertQuery += szValues;
			}
		}
	}

	CMySQLTransaction Transaction(this);

	Query("UPDATE " TABLE_STR " SET "
		COL_STATUS_SUMMED "=1, "
		COL_STATUS_PLAYERS "=" INT_STR ", "
		COL_STATUS_CATEGORIES "=" INT_STR,
		&m_Status, true, iTotalUsers, iTotalCategories);

	// Delete them all!
	Query("DELETE FROM " TABLE_STR, &m_VoteSums);

	if (iTotalUsers)
		Query(sInsertQuery.c_str(), &m_VoteSums);

	Transaction.Commit();

	UpdateStatus();

	LOGINFO("Done.");
}

bool CMySQLData::Valid()
{
	return CMySQL::Valid();
}

DB_TABLE_BEGIN(CMySQLData, Status, status)
	DB_COLUMN(COL_STATUS_SUMMED,		DB_BOOL);
	DB_COLUMN(COL_STATUS_PLAYERS,		DB_INT);
	DB_COLUMN(COL_STATUS_CATEGORIES,	DB_INT);
DB_TABLE_END()

DB_TABLE_BEGIN(CMySQLData, Users, users)
	DB_COLUMN(COL_USERS_ID,				DB_INT);
	DB_COLUMN(COL_USERS_USER,			DB_STRING);
	DB_COLUMN(COL_USERS_PASSWD,			DB_STRING);
DB_TABLE_END()

DB_TABLE_BEGIN(CMySQLData, UserIDs, game_ids)
	DB_COLUMN(COL_GAMEIDS_ID,			DB_INT);
	DB_COLUMN(COL_GAMEIDS_NETWORK,		DB_INT);
	DB_COLUMN(COL_GAMEIDS_VALUE,		DB_STRING);
DB_TABLE_END()

DB_TABLE_BEGIN(CMySQLData, Categories, categories)
	DB_COLUMN(COL_CATS_ID,				DB_INT);
	DB_COLUMN(COL_CATS_NAME,			DB_STRING);
DB_TABLE_END()

DB_TABLE_BEGIN(CMySQLData, Votes, votes)
	DB_COLUMN(COL_VOTES_CATEGORY,		DB_INT);
	DB_COLUMN(COL_VOTES_VOTE,			DB_INT);
	DB_COLUMN(COL_VOTES_FROMPLAYER,		DB_INT);
	DB_COLUMN(COL_VOTES_TOPLAYER,		DB_INT);
DB_TABLE_END()

DB_TABLE_BEGIN(CMySQLData, VoteSums, vote_sums)
	DB_COLUMN(COL_SUMS_CATEGORY,		DB_INT);
	DB_COLUMN(COL_SUMS_PLAYER,			DB_INT);
	DB_COLUMN(COL_SUMS_VOTE,			DB_FLOAT);
	DB_COLUMN(COL_SUMS_TOTAL,			DB_INT);
DB_TABLE_END()
