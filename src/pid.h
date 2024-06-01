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
#ifndef PID_H_
#define PID_H_
#include "controller.h"

/**
 * @class PidParams
 * @brief all parameters of a standard pid 
 *
 * @param Ts sample time
 *
 * @param Kp proportional gain
 *
 * @param Ti integral time constant
 *
 * @param Td derivative time constant
 *
 * @param N derivative filter coefficient
 *
 * @param lower_bound min value of the output
 *
 * @param upper_bound max value of the output
 *
 */
struct PidParams {
    float32_t Ts;
    float32_t Kp;
    float32_t Ti;
    float32_t Td;
    float32_t N;
    float32_t lower_bound;
    float32_t upper_bound;
};


/**
 * @class Pid
 * @brief Pid in a standard form taking into account saturation
 * 
 *  out = Kp * (error + 1 / Ti * error / s + 1 / (1 + Td / N * s) * Td * s * error )
 * 
 *  It uses backward euler integration method.
 *
 *  @details
 *  Example of use:
 *
 *  Pid mypid;
 *  PidParams params(Ts, Kp, Ti, Td, N, lower_bound, upper_bound);
 *  mypid.init(params);
 *  mypid.setMeasurement(y);
 *  mypid.setReference(yref);
 *  mypid.calculate();
 *  mypid.getOutput();
 *  
 */
class Pid: public Controller <float32_t, float32_t, float32_t, PidParams> {

public:
    Pid(){};

    /**
     * @brief initialize the standard pid
     *
     * @param params is a PidParams structure with all the parameters of the Pid.
     * @return 0 if ok else -EINVAL
     */
    int8_t init(PidParams params) override; 

    void calculate(void) override;

    void reset() override;

    void reset(float32_t output);

private:
    float32_t _integral;
    float32_t _Kp;
    float32_t _Ti;
    float32_t _Td;
    float32_t _N;
    float32_t _previous_f_deriv; // previous filtered derivative value
    float32_t _previous_error;  // previous error

    float32_t _inverse_Ts;
    float32_t _inverse_Ti;
    float32_t _inverse_Kp;
    float32_t _b1_filter;
    float32_t _a1_filter;
};
#endif
