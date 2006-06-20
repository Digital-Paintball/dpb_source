#include "sv_util.h"
#include "scheduler.h"

void CScheduler::AddEvent(float flSecondsFromNow, event_t eEvent)
{
	apr_time_t iNow = apr_time_now();

	if (flSecondsFromNow <= 0)
		m_Events.push_front(new CEvent(0, eEvent));
	else
		m_Events.push_back(new CEvent(iNow + (apr_time_t)(flSecondsFromNow * APR_USEC_PER_SEC), eEvent));
}

event_t CScheduler::GetNextEvent()
{
	if (m_Events.size() <= 0)
		return EVENT_NOP;

	CEvent* pEvent = m_Events.front();

	if (pEvent->m_iTime > apr_time_now())
		return EVENT_NOP;

	m_Events.pop_front();
	event_t eEvent = pEvent->m_eEvent;
	delete pEvent;
	return eEvent;
}

long CScheduler::TimeToNextEvent()
{
	if (m_Events.size() <= 0)
		return -1;

	CEvent* pEvent = m_Events.front();

	apr_time_t iTime = pEvent->m_iTime - apr_time_now();

	if (iTime <= 0)
		return 1;	// 1usec, because 0 will cause accept() to block.

	return (long)iTime;
}

CScheduler::~CScheduler()
{
	// Remove all our jobs from the queue.
	while (true)
	{
		if (m_Events.size() <= 0)
			break;

		CEvent* pEvent = m_Events.front();

		m_Events.pop_front();
		delete pEvent;
	}
}

CEvent::CEvent(apr_time_t iTime, event_t eEvent)
{
	m_iTime = iTime;
	m_eEvent = eEvent;
}
