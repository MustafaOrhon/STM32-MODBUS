/*
 * timer_ex.c
 *
 *  Created on: Jan 24, 2025
 *      Author: Mustafa
 */

#include "timer_ex.h"

extern uint32_t ms_tick;

bool TickTimer_Init(sTickTime_t *time,uint32_t interval_ms)
{
    if ((time == NULL) || (interval_ms == 0))
    {
        return false;
    }

	if(TickTimer_Set(time, interval_ms) == false)
	{
		return false;
	}

	return true;
}

bool TickTimer_Set(sTickTime_t *time,uint32_t interval_ms)
{
    if ((time == NULL) || (interval_ms == 0))
    {
        return false;
    }

	time->is_timer_active = true;
	time->interval = interval_ms;
	time->start_tick = ms_tick;

	return true;
}

bool TickTimer_Reset(sTickTime_t *time)
{
	if(time == NULL)
	{
		return false;
	}
	time->is_timer_active = false;
	return true;
}

bool TickTimer_CheckElapsedTime(sTickTime_t *time)
{
    if ((time == NULL) || (time->is_timer_active == false))
    {
        return false;
    }

    uint32_t current_tick = ms_tick;

    if (current_tick >= time->start_tick)
    {
        if ((current_tick - time->start_tick) >= time->interval)
        {
            time->is_timer_active = false;
            return true;
        }
    }
    else
    {
        if ((0xFFFFFFFF - time->start_tick + current_tick) >= time->interval)
        {
            time->is_timer_active = false;
            return true;
        }
    }

    return false;
}
