#ifndef MAIN_H
#define MAIN_H

#include "soap.h"
#include "data.h"
#include "config.h"
#include "scheduler.h"

class CStatSrv
{
public:
					CStatSrv(int iArgC, char const* const* pszArgV);
					~CStatSrv();

	virtual bool	StartupOK();
	virtual int		Start();

	virtual bool	IsRunning() = 0;
	virtual const char*	CfgDir() = 0;

	static CSOAP*	GetSoap();

	CSOAP			m_SOAP;
	CScheduler		m_Scheduler;
	IData*			m_pDB;

protected:
	static CStatSrv*	s_pSrv;
};

#endif
