/*
 * time.h
 *
 *  Created on: 05.03.2018
 *      Author: Laurent
 */

#ifndef TIME_H_
#define TIME_H_

/**
 * @brief Set the MCU in wait state, until time sync
 */
void time_sync(void);

/**
 * @brief set the RTC values
 * @param   timestring
 *          String from Client, representing localtime
 *          "day mon dd hh:mm:ss yyyy"
 */
void set_time(char *timestring);

/**
 * @brief get current time as String
 * @param   timestring
 *          String used to store current time
 *          "dd-mm-yy hh:mm:ss"
 */
void get_time(char *timestring);

#endif /* TIME_H_ */
