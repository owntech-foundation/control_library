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
 * @brief to give interface for some trigonometric functions
 *
 * - sinus
 * - cosinus
 * 
 * and modulo 2.Π
 */

#include "trigo.h"

const uint32_t MODULO_SIZE     = 32767;  // 2**15-1
const float32_t INV_MODULO_RES = 32767.0 / (2.0 * PI);
const float32_t MODULO_RES     = (2.0 * PI) / 32767.0;


float32_t ot_sin(float32_t x) {
        return arm_sin_f32(x);
};

float32_t ot_cos(float32_t x) {
        return arm_cos_f32(x);
};

#ifdef CORDIC
float32_t ot_atan2(float32_t y, float32_t x) {

    // configuration for cordic
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CORDIC);
    
    LL_CORDIC_Config(CORDIC,
                     LL_CORDIC_FUNCTION_PHASE, /* phase function */
                     LL_CORDIC_PRECISION_6CYCLES, /* max precision for q1.31 cosine */
                     LL_CORDIC_SCALE_0, /* no scale */
                     LL_CORDIC_NBWRITE_2, /* x first and y after */
                     LL_CORDIC_NBREAD_1, /* angle to read */
                     LL_CORDIC_INSIZE_32BITS, /* q1.31 format for input data */
                     LL_CORDIC_OUTSIZE_32BITS); 



    cos_theta = (int32_t) (x * 2147483648.0);
    sin_theta = (int32_t) (y * 2147483648.0);
    LL_CORDIC_WriteData(CORDIC, cos_theta); // give X data
    LL_CORDIC_WriteData(CORDIC, sin_theta); // give y data
    angle_cordic = M_PI * q31_to_f32((int32_t)LL_CORDIC_ReadData(CORDIC)); // retrieve phase of x+iy

}
#endif


float32_t ot_modulo_2pi(float32_t x)
{
    float32_t division;
    int32_t quotient;
    const float32_t inverse_2pi = 1.5915493667125701904296875E-1;

    division = x * inverse_2pi;

    quotient = (int32_t) division;

    if (x < 0.0F)
    {
        quotient--;
    }

    return x - ((float32_t) quotient * 2.0*PI);
}

