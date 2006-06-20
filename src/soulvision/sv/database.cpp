#include "database.h"

#include "mmgr.h"	// Must be LAST include

std::vector<IDBTable*> g_DataTables;

IDBTable::IDBTable()
{
	g_DataTables.push_back(this);
}

void IDBTable::BuildDataMaps()
{
	for (unsigned int i = 0; i < g_DataTables.size(); i++)
		g_DataTables[i]->BuildDataMap();
}

void IDBTable::Map(const char* pszColumn, const char* pszValue)
{
#ifdef _DEBUG
	if ((*m_pDataMap).find(pszColumn) == (*m_pDataMap).end())
	{
		assert(!"You forgot a DB_COLUMN");
		return;
	}
#endif

	CDBValue* pValue = &(*m_pDataMap)[pszColumn];

	pValue->SetDirty(false);

	if (!pszValue)
	{
		pValue->SetNull(true);
		return;
	}
	else
		pValue->SetNull(false);

	switch (pValue->GetType())
	{
	case CDBValue::DB_INT:
		(*pValue) = (int)atol(pszValue);
		break;
	case CDBValue::DB_BOOL:
		(*pValue) = (bool)(atoi(pszValue) != 0);
		break;
	case CDBValue::DB_FLOAT:
		(*pValue) = (float)atof(pszValue);
		break;
	case CDBValue::DB_STRING:
		(*pValue) = pszValue;
		break;
	default:
		break;
	}
}

void IDBTable::ClearDataMap()
{
	for (DataMap::iterator i = (*m_pDataMap).begin(); i != (*m_pDataMap).end(); i++)
		(*i).second.SetDirty(true);
}

IDBTable::DataMap* IDBTable::ReplaceDataMap()
{
	DataMap* pMap = m_pDataMap;
	m_pDataMap = new DataMap();
	BuildDataMap();
	return pMap;
}
