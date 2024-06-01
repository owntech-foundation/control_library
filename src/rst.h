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
#ifndef RST_H_
#define RST_H_
#include "controller.h"
#include "fir.h" 

/**
 * @class RstParams structure of Rst parameters
 * @brief 
 *
 * @param Ts sample time
 *
 * @param nr number of R coefficients
 *
 * @param r[] array of R coefficients
 *
 * @param ns number of S coefficients
 *
 * @param s[] array of S coefficients
 *
 * @param nt number of t coefficients
 *
 * @param t[] array of T coefficients
 *
 * @param lower_bound minimal value of output
 *
 * @param upper_bound maximal value of output
 *
 */
struct RstParams {
    float32_t Ts;
    uint8_t nr;
    const float32_t *r;
    uint8_t ns;
    const float32_t *s;
    uint8_t nt;
    const float32_t *t;
    float32_t lower_bound;
    float32_t upper_bound;
};


/**
 * @class RST
 * @brief discrete polynomial regulator taking into account saturations.
 *
 * It uses 3 Fir :
 *      * one on the measurements called R(),
 *      * one on the previous command, called S(), 
 *      * and the last on the reference called T()
 *
 * It mainly allows to add some filtering action on reference or measurements.
 * and sometimes to add some filtering poles which help stabilisation.
 *
 * some classical regulators can be implemented by its way like pid and pr.
 *
 */
class RST: public Controller<float32_t, float32_t, float32_t, RstParams> {
public:
    RST() {};

    /**
     * @brief initialize the rst controller 
     *
     * @param p RstParams structure
     * @return 0 if ok -EINVAL if not
     */
    int8_t init(RstParams p) override;

    void calculate(void) override;

    using Controller<float32_t, float32_t, float32_t, RstParams>::calculate;

    void reset(void) override;

private:
    Fir _R;
    Fir _Sp;
    Fir _T;
    float32_t _inv_s0;
};

#endif
