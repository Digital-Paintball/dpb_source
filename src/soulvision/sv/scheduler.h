#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>

typedef enum
{
	EVENT_NOP = 0,
	EVENT_SUMVOTES,
} event_t;

class CEvent
{
public:
	CEvent(apr_time_t iTime, event_t m_eEvent);

	apr_time_t	m_iTime;
	event_t		m_eEvent;
};

class CScheduler
{
public:
	~CScheduler();

	void	AddEvent(float flSecondsFromNow, event_t eEvent);
	event_t	GetNextEvent();
	long	TimeToNextEvent();

private:
	std::list<CEvent*>	m_Events;
};

#endif
