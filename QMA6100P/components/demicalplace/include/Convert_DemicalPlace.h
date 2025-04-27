/**
 * @file     Convert_DemicalPlace.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-21
 * @brief    API for converting experimental data of type uint32
 *           into a string with three decimal places.
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _CONVERT_DEMICLA_PLACE_H_
#define _CONVERT_DEMICLA_PLACE_H_

#include <stdio.h>
#include <string.h>
#include <math.h>

typedef __UINT32_TYPE__ uint32_t;

char *Calculate(uint32_t data);

#endif
