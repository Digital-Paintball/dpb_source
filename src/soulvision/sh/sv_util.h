#ifndef SV_UTIL_H
#define SV_UTIL_H

#include "runtime.h"
#include "logger.h"

void CreatePrimaryPool();
apr_pool_t* GetPrimaryPool();

void CmdParameterInit(int iArgc, char const* const* pszArgV, const apr_getopt_option_t* pOptions);

bool CmdParameterValue(char cOption);
const char* CmdParameterValue(char cOption, const char* pszDefault);
long CmdParameterValue(char cOption, long iDefault);

void ConfigInit(const char* pszConfigFile);
bool ConfigValue(const char* pszKey, bool bDefault);
const char* ConfigValue(const char* pszKey, const char* pszDefault);
long ConfigValue(const char* pszKey, long iDefault);
float ConfigValue(const char* pszKey, float flDefault);

char *VarArgs( char *format, ... );

struct StrLT
{
	bool operator()(const char* s1, const char* s2) const
	{
		return apr_strnatcmp(s1, s2) < 0;
	}
};

#endif
