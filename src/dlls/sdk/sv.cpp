#include "cbase.h"
#include "config.h"
#include "sv.h"
#include "sv_soap.h"

C_STAT C_STAT::s_STAT;

const char* C_STAT::Version()
{
	return DPB_VERSION " " DPB_REVISION2;
}

int C_STAT::MajorVersion()
{
	return DPB_MAJOR_VERSION;
}

int C_STAT::MinorVersion()
{
	return DPB_MINOR_VERSION;
}

int C_STAT::PatchVersion()
{
	return DPB_PATCH_VERSION;
}

int C_STAT::LookupCategory(Categories *pCategories, const char* pszName)
{
	for (unsigned int i = 0; i < (*pCategories).size(); i++)
		if (Q_stricmp(pszName, (*pCategories)[i].m_pszName) == 0)
			return (*pCategories)[i].m_iID;
	return -1;
}

void GetInterfaces(INetworkInterface* &pNetwork, ISTATInterface* &pSTAT)
{
	pNetwork = GetSOAP();
	pSTAT = GetSTAT();
}
