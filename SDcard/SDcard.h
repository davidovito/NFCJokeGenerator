/*
 * SDcard.h
 *
 *  Created on: Dec 13, 2024
 *      Author: 42194
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include "fatfs.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h> //for va_list var arg functions
#include <time.h>
#include <stdlib.h>




void myprintf(const char *fmt, ...);

uint8_t readRandomJokes(uint8_t mode, char *fillBuffer);
uint8_t sdCardInit();

#endif /* INC_SDCARD_H_ */
