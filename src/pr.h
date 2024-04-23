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
 * SPDX-License-Identifier: LGLPV2.1
 */

/**
 * @date 2024
 * @author Régis Ruelland <regis.ruelland@laas.fr>
 * @author Ayoub Farah Hassan <ayoub.farah-hassan@laas.fr>
 *
 */

#include "controller.h"
#include "fir.h"

/**
 * @class PrParams
 * @brief all parameters to define the proportional resonant controller.
 *
 * @param Ts sample time
 *
 * @param Kp proportional gain
 *
 * @param Kr resonant gain
 *
 * @param w0 pulsation [rad/s]
 *
 * @param phi_prime angle in rad to compensate delays
 *
 * @param lower_bound min value of the output
 *
 * @param upper_bound max value of the output
 *
 */
struct PrParams {
    float32_t Ts;
    float32_t Kp;
    float32_t Kr;
    float32_t w0;
    float32_t phi_prime;
    float32_t lower_bound;
    float32_t upper_bound;
};

class Pr: public Controller <float32_t, float32_t, float32_t, PrParams> {

public:
    Pr() {};

    int8_t init(PrParams p);

    /**
     * @brief calculate a new command value according to a reference fixed using
     * `set_reference` method and a measuremnt fixed using `set_measurement`.
     *
     * The new command value can be captured using the `get_output` method.
     */
    void calculate(void);

    /**
     * @brief calculate a new command value according the argument values
     *
     * @param yrefs reference
     * @param y measure
     * @return new command value.
     */

    void reset(void);

private:
    float32_t _Ts;
    float32_t _Kp;
    float32_t _Kr;
    float32_t _inverse_Kr;
    Fir _B; // numerator of the resonator
    Fir _A; // denominator of the resonator
    float32_t _resonant; // resonator output
};
