#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	uint Read() const;
	float ReadSec() const;

private:
	uint startedAt;
};

#endif //__j1TIMER_H__