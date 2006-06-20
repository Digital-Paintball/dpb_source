#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <map>

#include <assert.h>

#include "sv_util.h"

#define TABLE_STR "$TABLE$"
#define TABLE_STRLEN 7

#define INT_STR "$INT$"
#define INT_STRLEN 5
#define BOOL_STR "$BOOL$"
#define BOOL_STRLEN 6
#define FLOAT_STR "$FLOAT$"
#define FLOAT_STRLEN 7
#define STRING_STR "$STRING$"
#define STRING_STRLEN 8

class CDBValue
{
public:
	typedef enum
	{
		DB_INVALID = 0,
		DB_INT,
		DB_BOOL,
		DB_FLOAT,
		DB_STRING,
	} DBType;

	inline				CDBValue();
	inline				CDBValue(DBType Type, const char* pszColumn);

	// If it's dirty, don't use it, because you don't know where it's been.
	inline bool			IsDirty() const;
	inline void			SetDirty(bool bDirty);

	// Didn't your mama ever teach you not to reference null CDBValues? They might not be initialized.
	inline bool			IsNull() const;
	inline void			SetNull(bool bNull);
	
	inline DBType		GetType() const;
	inline void			SetType(DBType Type);
	
	inline const int&	operator=( const int &val );
	inline const bool&	operator=( const bool &val );
	inline const float&	operator=( const float &val );
	inline const char*&	operator=( const char* &val );
	
	inline operator const int() const;
	inline operator const bool() const;
	inline operator const float() const;
	inline operator const char*() const;

private:
	union
	{
		int				m_iValue;
		bool			m_bValue;
		float			m_flValue;
		const char*		m_pszValue;
	};

	const char*			m_pszColumn;
	bool				m_bDirty;
	bool				m_bNull;
	DBType				m_Type;
};

class IDBTable
{
public:
						IDBTable();

	typedef std::map<const char*, CDBValue, StrLT> DataMap;

	static void			BuildDataMaps();

	inline const CDBValue* GetValue(const char* pszColumn);
	inline int			GetValueInt(const char* pszColumn);
	inline bool			GetValueBool(const char* pszColumn);
	inline float		GetValueFloat(const char* pszColumn);
	inline const char*	GetValueString(const char* pszColumn);

	inline const char*	GetTableName();

	inline void			ClearColumns();
	inline unsigned int	GetTotalColumns();
	inline void			AddColumn(const char* pszColumn);
	inline const char*	GetColumn(int i);

	void				ClearDataMap();

	void				Map(const char* pszColumn, const char* pszValue);

	DataMap*			ReplaceDataMap();
	virtual void		BuildDataMap() = 0;

protected:
	DataMap*			m_pDataMap;
	std::vector<const char*> m_Columns;
	char				m_szTableName[100];
};

class CDataResults : public std::vector<const IDBTable::DataMap*>
{
public:
	// Automatically delete all data maps on destruction.
	inline ~CDataResults();

	inline const CDBValue* GetValue(int i, const char* pszColumn);

	inline int GetInt(int i, const char* pszColumn);
	inline bool GetBool(int i, const char* pszColumn);
	inline float GetFloat(int i, const char* pszColumn);
	inline const char* GetString(int i, const char* pszColumn);

	// NULL-Safe versions of these functions. If the true value is NULL
	// or dirty then the default value will be returned instead. This is a
	// bit slower, so try to avoid it for fields which have NOT NULL or
	// that you know cannot possibly be NULL.
	inline int GetIntNS(int i, const char* pszColumn, int iDefault = 0);
	inline bool GetBoolNS(int i, const char* pszColumn, bool bDefault = false);
	inline float GetFloatNS(int i, const char* pszColumn, float iDefault = 0.0f);
	inline const char* GetStringNS(int i, const char* pszColumn, const char* pszDefault = "");
};

class IDatabase
{
public:
	// The only thing you can do with an invalid database is delete it.
	// Otherwise it will break and you can keep both pieces.
	virtual bool		Valid() = 0;
};

CDBValue::CDBValue()
{
	m_Type = DB_INVALID;
	m_bDirty = true;
	m_bNull = true;
}

CDBValue::CDBValue(DBType Type, const char* pszColumn)
{
	m_Type = Type;
	m_pszColumn = pszColumn;
	m_bDirty = true;
	m_bNull = true;
}

bool CDBValue::IsDirty() const
{
	return m_bDirty;
}

void CDBValue::SetDirty(bool bDirty)
{
	m_bDirty = bDirty;
}

bool CDBValue::IsNull() const
{
	return m_bNull;
}

void CDBValue::SetNull(bool bNull)
{
	m_bNull = bNull;
}

CDBValue::DBType CDBValue::GetType() const
{
	return m_Type;
}

void CDBValue::SetType(DBType Type)
{
	m_Type = Type;
}

const int& CDBValue::operator=( const int &val )
{
	return m_iValue = val;
}

const bool& CDBValue::operator=( const bool &val )
{
	return m_bValue = val;
}

const float& CDBValue::operator=( const float &val )
{
	return m_flValue = val;
}

const char*& CDBValue::operator=( const char* &val )
{
	return m_pszValue = val;
}

CDBValue::operator const int() const
{
	assert(!m_bNull);
	assert(m_Type == DB_INT);
	return m_iValue;
}

CDBValue::operator const bool() const
{
	assert(!m_bNull);
	assert(m_Type == DB_BOOL);
	return m_bValue;
}

CDBValue::operator const float() const
{
	assert(!m_bNull);
	assert(m_Type == DB_FLOAT);
	return m_flValue;
}

CDBValue::operator const char*() const
{
	assert(!m_bNull);
	assert(m_Type == DB_STRING);
	return m_pszValue;
}

// This lookup could be faster.
const CDBValue* IDBTable::GetValue(const char* pszColumn)
{
	return &(*m_pDataMap)[pszColumn];
}

int IDBTable::GetValueInt(const char* pszColumn)
{
	const CDBValue *pValue = GetValue(pszColumn);
	assert(pValue->GetType() == CDBValue::DB_INT);
	return (*pValue);
}

bool IDBTable::GetValueBool(const char* pszColumn)
{
	const CDBValue *pValue = GetValue(pszColumn);
	assert(pValue->GetType() == CDBValue::DB_BOOL);
	return (*pValue);
}

float IDBTable::GetValueFloat(const char* pszColumn)
{
	const CDBValue *pValue = GetValue(pszColumn);
	assert(pValue->GetType() == CDBValue::DB_FLOAT);
	return (*pValue);
}

const char* IDBTable::GetValueString(const char* pszColumn)
{
	const CDBValue *pValue = GetValue(pszColumn);
	assert(pValue->GetType() == CDBValue::DB_STRING);
	return (*pValue);
}

const char* IDBTable::GetTableName()
{
	return &m_szTableName[0];
}

void IDBTable::ClearColumns()
{
	m_Columns.clear();
}

unsigned int IDBTable::GetTotalColumns()
{
	return m_Columns.size();
}

void IDBTable::AddColumn(const char* pszColumn)
{
	m_Columns.push_back(pszColumn);
}

const char* IDBTable::GetColumn(int i)
{
	return m_Columns[i];
}

CDataResults::~CDataResults()
{
	for (iterator i = begin(); i != end(); i++)
		delete (*i);
}

const CDBValue* CDataResults::GetValue(int i, const char* pszColumn)
{
	return &((*at(i)).find(pszColumn)->second);
}

int CDataResults::GetInt(int i, const char* pszColumn)
{
	return *GetValue(i, pszColumn);
}

bool CDataResults::GetBool(int i, const char* pszColumn)
{
	return *GetValue(i, pszColumn);
}

float CDataResults::GetFloat(int i, const char* pszColumn)
{
	return *GetValue(i, pszColumn);
}

const char* CDataResults::GetString(int i, const char* pszColumn)
{
	return *GetValue(i, pszColumn);
}

int CDataResults::GetIntNS(int i, const char* pszColumn, int iDefault)
{
	const CDBValue* pValue = GetValue(i, pszColumn);
	if (pValue->IsDirty() || pValue->IsNull())
		return iDefault;
	else
		return *pValue;
}

bool CDataResults::GetBoolNS(int i, const char* pszColumn, bool bDefault)
{
	const CDBValue* pValue = GetValue(i, pszColumn);
	if (pValue->IsDirty() || pValue->IsNull())
		return bDefault;
	else
		return *pValue;
}

float CDataResults::GetFloatNS(int i, const char* pszColumn, float flDefault)
{
	const CDBValue* pValue = GetValue(i, pszColumn);
	if (pValue->IsDirty() || pValue->IsNull())
		return flDefault;
	else
		return *pValue;
}

const char* CDataResults::GetStringNS(int i, const char* pszColumn, const char* pszDefault)
{
	const CDBValue* pValue = GetValue(i, pszColumn);
	if (pValue->IsDirty() || pValue->IsNull())
		return pszDefault;
	else
		return *pValue;
}

#define DB_TABLE_DECLARE(name) \
class CDB##name : public IDBTable \
{ \
public: \
	CDB##name(); \
	~CDB##name(); \
	virtual void	BuildDataMap(); \
} m_##name;

#define DB_TABLE_BEGIN(parent,name,tablename) \
parent::CDB##name::CDB##name() \
{ \
	apr_snprintf(m_szTableName, 100, "%s_" #tablename, ConfigValue("mysql_prefix", "sv")); \
	m_pDataMap = new DataMap(); \
} \
parent::CDB##name::~CDB##name() \
{ \
	delete m_pDataMap; \
} \
void parent::CDB##name::BuildDataMap() \
{

#define DB_COLUMN(name,type) \
	(*m_pDataMap)[name] = CDBValue(CDBValue::type, name)

#define DB_TABLE_END() \
}

#endif
