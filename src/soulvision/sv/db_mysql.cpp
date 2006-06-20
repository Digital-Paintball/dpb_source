#include <string>

#include "db_mysql.h"

#include "mmgr.h"	// Must be LAST include

CMySQLTransaction* CMySQLTransaction::s_pTrans = NULL;
CMySQLQuery* CMySQLQuery::s_pRes = NULL;
bool CMySQLQuery::s_bQuerying = false;
static Logger SQLLog;

#define LOGSQLPROF(string) SQLLog.logTex(__FILE__, __LINE__, string, Logger::LOG_INFO)

CMySQL::CMySQL()
{
	m_bValid = false;

	mysql_init(this);

	if (!mysql_real_connect(this,
		ConfigValue("mysql_host", "localhost"),
		ConfigValue("mysql_user", "anonymous"),
		ConfigValue("mysql_pass", ""),
		ConfigValue("mysql_db", "sv"), MYSQL_PORT, NULL, 0))
		return;

	m_bValid = true;

	m_bAllProfiling = ConfigValue("mysql_prof_all", false);

	SetExplain(ProfConfigValue("mysql_prof_explain", false));

	if (AnyProfiling())
		SQLLog.start("statsrv-sql.log");
}

CMySQL::~CMySQL()
{
	mysql_close(this);
}

MYSQL_RES* CMySQL::Query(const char* pszQuery, IDBTable* pTable, bool bStore, ...)
{
	// Do VA list
	va_list argptr;
	va_start(argptr, bStore);

	MYSQL_RES* pRes = Query(pszQuery, pTable, bStore, argptr);

	va_end(argptr);

	return pRes;
}

MYSQL_RES* CMySQL::Query(const char* pszQuery, IDBTable* pTable, bool bStore, va_list argptr)
{
	// Have the last query's results been freed?
	assert(!CMySQLQuery::IsInResults());

	if (!pszQuery)
		return NULL;

	std::string sQuery = pszQuery;
	int iFind;

	if (pTable)
	{
		iFind = sQuery.find(TABLE_STR);
		if (iFind >= 0)
			sQuery.replace(iFind, TABLE_STRLEN, pTable->GetTableName());
	}

	// Waste the first three args
	int iFindLowest;
	CDBValue::DBType LowestType;
	char szVA[20];

	do
	{
		iFindLowest = sQuery.length();
		LowestType = CDBValue::DB_INVALID;

		iFind = sQuery.find(INT_STR);
		if (iFind >= 0 && iFind < iFindLowest)
		{
			iFindLowest = iFind;
			LowestType = CDBValue::DB_INT;
		}
		iFind = sQuery.find(BOOL_STR);
		if (iFind >= 0 && iFind < iFindLowest)
		{
			iFindLowest = iFind;
			LowestType = CDBValue::DB_BOOL;
		}
		iFind = sQuery.find(FLOAT_STR);
		if (iFind >= 0 && iFind < iFindLowest)
		{
			iFindLowest = iFind;
			LowestType = CDBValue::DB_FLOAT;
		}
		iFind = sQuery.find(STRING_STR);
		if (iFind >= 0 && iFind < iFindLowest)
		{
			iFindLowest = iFind;
			LowestType = CDBValue::DB_STRING;
		}

		switch (LowestType)
		{
		case CDBValue::DB_INT:
			apr_snprintf(szVA, 20, "%d", va_arg(argptr, int));
			sQuery.replace(iFindLowest, INT_STRLEN, szVA);
			break;

		case CDBValue::DB_BOOL:
			sQuery.replace(iFindLowest, BOOL_STRLEN, ((va_arg(argptr, int))?"1":"0"));
			break;

		case CDBValue::DB_FLOAT:
			apr_snprintf(szVA, 20, "%f", va_arg(argptr, double));
			sQuery.replace(iFindLowest, FLOAT_STRLEN, szVA);
			break;

		case CDBValue::DB_STRING:
			sQuery.replace(iFindLowest, STRING_STRLEN, va_arg(argptr, char*));
			break;

		default:
			break;
		}
	}
	while (LowestType != CDBValue::DB_INVALID);

	if (GetExplain() && strncasecmp(sQuery.c_str(), "SELECT ", 7) == 0)
	{
		LOGSQLPROF(VarArgs("EXPLAIN data for query: %s", sQuery.c_str()));
		CDataResults vExplain;
		std::string sExplainQuery = sQuery;
		sExplainQuery.insert(0, "EXPLAIN ");
		MYSQL_RES *pRes = Query(sExplainQuery.c_str(), &m_Explain);
		MapResults(pRes, &m_Explain, &vExplain);
		for (unsigned int i = 0; i < pRes->row_count; i++)
		{
			LOGSQLPROF(VarArgs("id:%d select_type:%s table:%s type:%s possible_keys:%s key:%s key_len:%d ref:%s rows:%d Extra:%s",
				vExplain.GetInt			(i, "id"),
				vExplain.GetString		(i, "select_type"),
				vExplain.GetString		(i, "table"),
				vExplain.GetString		(i, "type"),
				vExplain.GetStringNS	(i, "possible_keys"),
				vExplain.GetStringNS	(i, "key"),
				vExplain.GetIntNS		(i, "key_len"),
				vExplain.GetStringNS	(i, "ref"),
				vExplain.GetInt			(i, "rows"),
				vExplain.GetStringNS	(i, "Extra")
			));
		}
	}

	if (mysql_query(this, sQuery.c_str()) != MYSQL_SUCCESS)
	{
		assert(!"mysql_query failed!");
		return NULL;
	}

	MYSQL_RES* pRes;

	if (bStore)
		pRes = mysql_store_result(this);
	else
		pRes = mysql_use_result(this);

	if (!pRes)
		return NULL;

	// If we have a result then we must have a table as well.
	assert(pTable);

	// Must be using a CMySQLQuery object when calling something that returns a result.
	assert(CMySQLQuery::IsQuerying());

	// Build columns list.
	pTable->ClearColumns();

	unsigned int num_fields = mysql_num_fields(pRes);
	for(unsigned int i = 0; i < num_fields; i++)
		pTable->AddColumn(mysql_fetch_field(pRes)->name);

	return pRes;
}

CDataResults* CMySQL::MapResults(MYSQL_RES* pRes, IDBTable* pTable, CDataResults* pData)
{
	if (!pRes)
		return NULL;

	if (!pTable)
		return NULL;

	if (!pData)
		pData = new CDataResults();

	while (MapRow(pRes, pTable))
		(*pData).push_back(pTable->ReplaceDataMap());

	return pData;
}

IDBTable* CMySQL::MapRow(MYSQL_RES* pRes, IDBTable* pTable)
{
	if (!pRes)
		return NULL;

	if (!pTable)
		return NULL;

	MYSQL_ROW pRow = mysql_fetch_row(pRes);
	//unsigned long *lengths = mysql_fetch_lengths(pRes);

	if (!pRow)
	{
		pTable->ClearDataMap();
		return NULL;
	}

	for(unsigned int i = 0; i < pTable->GetTotalColumns(); i++)
		pTable->Map(pTable->GetColumn(i), pRow[i]);

	return pTable;
}

unsigned long CMySQL::Escape(char *pszTo, const char *pszFrom, unsigned long iLength)
{
	return mysql_real_escape_string(this, pszTo, pszFrom, iLength);
}

my_ulonglong CMySQL::LastInsertID()
{
	return mysql_insert_id(this);
}

bool CMySQL::Valid()
{
	return m_bValid;
}

DB_TABLE_BEGIN(CMySQL, Explain, notused)
	DB_COLUMN("id",				DB_INT);
	DB_COLUMN("select_type",	DB_STRING);
	DB_COLUMN("table",			DB_STRING);
	DB_COLUMN("type",			DB_STRING);
	DB_COLUMN("possible_keys",	DB_STRING);
	DB_COLUMN("key",			DB_STRING);
	DB_COLUMN("key_len",		DB_INT);
	DB_COLUMN("ref",			DB_STRING);
	DB_COLUMN("rows",			DB_INT);
	DB_COLUMN("Extra",			DB_STRING);
DB_TABLE_END()
