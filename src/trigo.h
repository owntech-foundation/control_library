/*
 * Copyright (c) 2024 LAAS-CNRS
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: LGPL-2.1
 */

/**
 * @date 2024
 * @author Régis Ruelland <regis.ruelland@laas.fr>
 */

/**
 * @file trigo.cpp
 * @brief some trigonometrics functions 
 */

#include <arm_math.h>
extern const uint32_t MODULO_SIZE;     
extern const float32_t INV_MODULO_RES;
extern const float32_t MODULO_RES; 

// TODO: use CORDIC ?
#ifdef CORDIC 
#include "stm32g4xx_ll_cordic.h"
#endif


float32_t ot_sin(float32_t x);
float32_t ot_cos(float32_t x);
float32_t ot_modulo_2pi(float32_t theta);

