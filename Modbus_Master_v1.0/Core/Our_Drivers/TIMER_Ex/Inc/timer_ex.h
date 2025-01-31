/*
 * timer_ex.h
 *
 *  Created on: Jan 24, 2025
 *      Author: Mustafa
 */

#ifndef OUR_DRIVERS_TIMER_EX_INC_TIMER_EX_H_
#define OUR_DRIVERS_TIMER_EX_INC_TIMER_EX_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct
{
	uint32_t start_tick;
	uint32_t interval;
	bool     is_timer_active;
}sTickTime_t;


/**
 * Initializes the timer with a given interval.
 * @param time Pointer to the timer structure.
 * @param interval_ms The interval in milliseconds.
 * @return true if initialization is successful, false otherwise (e.g., invalid input).
 */
bool TickTimer_Init(sTickTime_t *time, uint32_t interval_ms);

/**
 * Sets a new interval for the timer.
 * @param time Pointer to the timer structure.
 * @param interval_ms The interval in milliseconds.
 * @return true if the interval was successfully set, false if the input is invalid.
 */
bool TickTimer_Set(sTickTime_t *time, uint32_t interval_ms);

/**
 * Resets the timer, deactivating it.
 * @param time Pointer to the timer structure.
 * @return true if the timer was successfully reset, false if the input is invalid.
 */
bool TickTimer_Reset(sTickTime_t *time);

/**
 * Checks whether the timer has elapsed the set interval.
 * @param time Pointer to the timer structure.
 * @return true if the timer has elapsed the interval, false otherwise.
 */
bool TickTimer_CheckElapsedTime(sTickTime_t *time);
#endif /* OUR_DRIVERS_TIMER_EX_INC_TIMER_EX_H_ */
