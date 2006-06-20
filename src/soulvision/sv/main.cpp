#include <sstream>

#include "sv_util.h"
#include "main.h"

#include "mmgr.h"	// Must be LAST include

CStatSrv* CStatSrv::s_pSrv = NULL;

apr_getopt_option_t g_Options[] = {
	{"config-file",	'c', 1, "Configuration file location"},
	{"host",		'h', 1, "Host name to bind networking to"},
	{"port",		'p', 1, "Port to bind networking to"},
	{NULL,			0,   0, NULL},
};

CStatSrv::CStatSrv(int iArgC, char const* const* pszArgV)
{
	apr_app_initialize( &iArgC, &pszArgV, NULL );

	logger.start("statsrv.log");

	LOGINFO("STATSrv version " DPB_VERSION " " DPB_REVISION2 " starting up...");

	s_pSrv = this;

	CreatePrimaryPool();
	CmdParameterInit(iArgC, pszArgV, g_Options);
	ConfigInit(CmdParameterValue('c', VarArgs("%s/server.cfg", CfgDir())));

	const char* pszDBType = ConfigValue("db_type", "mysql");
	assert(apr_strnatcmp(pszDBType, "mysql") == 0);

	m_pDB = CreateMySQL();

	if (!m_pDB)
		return;

	m_SOAP.Init(m_pDB);
}

CStatSrv::~CStatSrv()
{
	delete m_pDB;

	s_pSrv = NULL;

	apr_terminate();
}

bool CStatSrv::StartupOK()
{
	return (m_pDB != NULL);
}

int CStatSrv::Start()
{
	if (!m_pDB->AreVotesSummed())
		m_pDB->SumVotes();

	// Sum votes every half hour or so.
	m_Scheduler.AddEvent(ConfigValue("sum_votes_time", 30.0f)*60, EVENT_SUMVOTES);

	int m, s;

	LOGINFO(VarArgs("Binding SOAP on %s:%d", CmdParameterValue('h', "localhost"), CmdParameterValue('p', 41285)));

	m = m_SOAP.Bind(CmdParameterValue('h', "localhost"), CmdParameterValue('p', 41285), 100);

	if (m < 0)
		exit(-1);

	bool bNoBlock = ConfigValue("debug_noblock", false);

	// I couldn't resist.
	LOGINFO("Ready to serve.");

	while (IsRunning())
	{
		long iTimeToNextEvent = m_Scheduler.TimeToNextEvent();

		if (bNoBlock)
			iTimeToNextEvent = 0;

		if (iTimeToNextEvent > 0)
			s = m_SOAP.Accept(-iTimeToNextEvent);
		else
			s = m_SOAP.Accept(-100000);		// 1/10 of a second.

		if (s >= 0)
			m_SOAP.Serve();

		// This loop executes all events before moving on to the next network
		// message. If an event is too expensive and will monopolize the
		// process, it will need to be moved to another thread, or better yet,
		// another server.
		while (true)
		{
			switch (m_Scheduler.GetNextEvent())
			{
			default:
			case EVENT_NOP:
				break;

			case EVENT_SUMVOTES:
				m_pDB->SumVotes();
				m_Scheduler.AddEvent(ConfigValue("sum_votes_time", 30.0f)*60, EVENT_SUMVOTES);
				continue;
			}

			// If we're here, we didn't hit a continue, and thus we should continue processing.
			break;
		}
	}

	LOGINFO(VarArgs("%d served.", m_SOAP.Served()));

	return 0;
}

CSOAP* CStatSrv::GetSoap()
{
	return &s_pSrv->m_SOAP;
}
