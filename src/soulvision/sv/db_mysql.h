#ifndef DB_MYSQL_H
#define DB_MYSQL_H

#include "database.h"

// MySQL Windows header, which it doesn't include in mysql.h for some reason.
#if !defined(__linux__)
#include <config-win.h>
#endif

#include <mysql.h>

// config-win.h sets this, and I don't like it.
#if defined(bool_defined)
#undef bool
#endif

#define MYSQL_SUCCESS 0

class CMySQL : public MYSQL
{
	friend class CMySQLTransaction;
	friend class CMySQLQuery;

public:
					CMySQL();
					~CMySQL();

	bool			Valid();

	// Any non-NULL result from Query must be freed with mysql_free_result()
	// Query is designed for data-retrieving queries (SELECT, SHOW, DESCRIBE, EXPLAIN, CHECK TABLE, and so forth)
	MYSQL_RES*		Query(const char* pszQuery, IDBTable* pTable = NULL, bool bStore = true, ...);
	MYSQL_RES*		Query(const char* pszQuery, IDBTable* pTable, bool bStore, va_list argptr);

	// Feed the result of Query. This loads the next result row into the provided map.
	IDBTable*		MapRow(MYSQL_RES* pRes, IDBTable* pMap);

	// Feed the result of Query. This loads all result rows into a vector.
	// If pData is NULL, returned vector will be allocated with new, so the caller is responsible for deleting.
	CDataResults*	MapResults(MYSQL_RES* pRes, IDBTable* pMap, CDataResults* pData = NULL);

	// Escape a string so it does not contain special characters. Use this on all strings from untrusted sources.
	unsigned long	Escape(char *pszTo, const char *pszFrom, unsigned long iLength);

	// Uses mysql_insert_id() to retrieve the last ID of an auto-incremented INSERT.
	my_ulonglong	LastInsertID();

	// Profiling
	inline bool		ProfConfigValue(const char* pszKey, bool bDefault);
	inline bool		AnyProfiling();

	inline void		SetExplain(bool bOn);
	inline bool		GetExplain();

protected:
	bool			m_bValid;

	// Profiling. Add anything here to AnyProfiling()
	bool			m_bAllProfiling;
	bool			m_bExplain;

private:
	DB_TABLE_DECLARE(Explain);
};

class CMySQLTransaction
{
public:
	inline CMySQLTransaction(CMySQL* pMySQL);
	inline ~CMySQLTransaction();

	inline void		Commit();

	inline static bool IsInTransaction();

protected:
	CMySQL*						m_pMySQL;
	static CMySQLTransaction*	s_pTrans;
};

class CMySQLQuery
{
public:
	inline CMySQLQuery(CMySQL* pMySQL, const char* pszQuery, IDBTable* pTable = NULL, bool bStore = true, ...);
	inline ~CMySQLQuery();

	inline MYSQL_RES* GetResults();
	inline void		FreeResults();

	inline static bool IsInResults();
	inline static bool IsQuerying();

protected:
	CMySQL*				m_pMySQL;
	MYSQL_RES*			m_pRes;
	static CMySQLQuery*	s_pRes;
	static bool			s_bQuerying;
};

void CMySQL::SetExplain(bool bOn)
{
	m_bExplain = bOn;
}

bool CMySQL::GetExplain()
{
	return m_bExplain;
}

bool CMySQL::ProfConfigValue(const char* pszKey, bool bDefault)
{
	return m_bAllProfiling || ConfigValue(pszKey, bDefault);
}

bool CMySQL::AnyProfiling()
{
	return m_bAllProfiling || m_bExplain;
}

CMySQLTransaction::CMySQLTransaction(CMySQL* pMySQL)
{
	assert(!s_pTrans);
	s_pTrans = this;

	m_pMySQL = pMySQL;

	CMySQLQuery Query(m_pMySQL, "START TRANSACTION");
}

CMySQLTransaction::~CMySQLTransaction()
{
	assert(!s_pTrans || s_pTrans == this);

	if (s_pTrans)
		Commit();
}

void CMySQLTransaction::Commit()
{
	CMySQLQuery Query(m_pMySQL, "COMMIT");

	s_pTrans = NULL;
}

bool CMySQLTransaction::IsInTransaction()
{
	return s_pTrans != NULL;
}

CMySQLQuery::CMySQLQuery(CMySQL* pMySQL, const char* pszQuery, IDBTable* pTable, bool bStore, ...)
{
	if (s_pRes)
		s_pRes->FreeResults();

	m_pMySQL = pMySQL;

	// Do VA list
	va_list argptr;
	va_start(argptr, bStore);

	s_bQuerying = true;
	m_pRes = m_pMySQL->Query(pszQuery, pTable, bStore, argptr);
	s_bQuerying = false;

	va_end(argptr);

	if (m_pRes)
		s_pRes = this;
}

CMySQLQuery::~CMySQLQuery()
{
	assert(!s_pRes || s_pRes == this);

	if (s_pRes)
		FreeResults();
}

MYSQL_RES* CMySQLQuery::GetResults()
{
	assert(s_pRes == this);
	return m_pRes;
}

void CMySQLQuery::FreeResults()
{
	mysql_free_result(m_pRes);
	s_pRes = NULL;
}

bool CMySQLQuery::IsInResults()
{
	return s_pRes != NULL;
}

bool CMySQLQuery::IsQuerying()
{
	return s_bQuerying;
}

#endif
