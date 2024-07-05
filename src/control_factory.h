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

#include "pid.h"
#include "pr.h"
#include "rst.h"
#include "filters.h"


class ControlFactory {
    public:
    ControlFactory() {};
    /**
     * @brief return a Pid controller in a standard form.
     * 
     * @param Ts sample time[s]
     * @param Kp proportional gain
     * @param Ti integral time [s]
     * @param Td derivative time [s] 
     * @param N to define derivative constant time filter 
     * @param lower_bound 
     * @param upper_bound 
     * @return Pid 
     */
    Pid pid(float32_t Ts, float32_t Kp, float32_t Ti, float32_t Td, float32_t N, float32_t lower_bound, float32_t upper_bound);
    /**
     * @brief return a Proportional Resonant controller for a fixed pulsation `w0`[rad/s] and possible advanced phase `phi_prime`[rad]
     * 
     * @param Ts sampling time [s] 
     * @param Kp proportional gain 
     * @param Kr resonant gain
     * @param w0 pulsation [rad/s]
     * @param phi_prime advanced phase [rad]
     * @param lower_bound 
     * @param upper_bound 
     * @return * Pr 
     */
    Pr pr(float32_t Ts, float32_t Kp, float32_t Kr, float32_t w0, float32_t phi_prime, float32_t lower_bound, float32_t upper_bound);
    /**
     * @brief return a polynomial RST controller
     * 
     * @param Ts sampling time [s]
     * @param nr number of R coefs 
     * @param *r  pointer to array R
     * @param ns number of S coefs
     * @param *s pointer to array S
     * @param nt number of T coefs
     * @param *t pointer to array T
     * @param lower_bound 
     * @param upper_bound 
     * @return RST 
     */
    RST rst(float32_t Ts, uint8_t nr, const float32_t *r, uint8_t ns, const float32_t *s, uint8_t nt, const float32_t *t, float32_t lower_bound, float32_t upper_bound);
    /**
     * @brief return a phase locked loop filter adapated to sinus tracking at a fixed `f0`[Hz] frequency with a fixed `amplitude` and with a `rise_time` [s] dynamic
     * 
     * @param Ts sampling time [s]
     * @param amplitude 
     * @param f0  frequency [Hz]
     * @param rise_time [s]
     * @return PllSinus 
     */
    PllSinus pllSinus(float32_t Ts, float32_t amplitude, float32_t f0, float32_t rise_time);
    /**
     * @brief return a phase locked loop filter adapated to a saw-tooth between [0, 2Π], tracking at a fixed `f0`[Hz] frequency with a `rise_time` [s] dynamic
     * 
     * @param Ts sampling time [s]
     * @param f0  frequency [Hz]
     * @param rise_time [s]
     * @return PllAngle
     */
    PllAngle pllAngle(float32_t Ts, float32_t f0, float32_t rise_time);

    /**
     * @brief return a notch filter around the frequency `f0`[Hz] with a `bandwidth` [Hz]
     * 
     * @param Ts sampling time [s]
     * @param f0 frequency [Hz]
     * @param bandwidth bandwidth around f0 where gain < -3dB [Hz]
     * @return NotchFilter 
     */
    NotchFilter notchfilter(float32_t Ts, float32_t f0, float32_t bandwidth);
    /**
     * @brief low pass filter
     * 
     * @param Ts sampling time [s]
     * @param tau constant time [s]
     * @return LowPassFirstOrderFilter 
     */
    LowPassFirstOrderFilter lowpassfilter(float32_t Ts, float32_t tau);
};

extern ControlFactory controlLibFactory;
