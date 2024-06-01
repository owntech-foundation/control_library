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
 *
 * @author Régis Ruelland <regis.ruelland@laas.fr>
 * @author Luiz Villa <luiz.villa@laas.fr>
 * @author Ayoub Farah Hassan <ayoub.farah-hassan@laas.fr>
 *
 */

#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include <arm_math.h>
#include "trigo.h"

const float32_t SQRT3_INVERSE  = 0.57735026F;
const float32_t SQRT3_DIV_2    = 0.8660254F;


/**
 * @brief to keep together a,b and c phase values.
 *
 */

struct three_phase_t {
    float32_t a;
    float32_t b;
    float32_t c;

};

/**
 * @brief to keep together α, β and o values.
 *
 */
struct clarke_t {
    float32_t alpha;
    float32_t beta;
    float32_t o;
};

/**
 * @brief to keep together d, q and o values.
 *
 */
struct dqo_t {
    float32_t d;
    float32_t q;
    float32_t o;
};

/** 
 * @brief static class to group methods helping translation between reference frames
 *
 * mainly 3 reference frames:
 * 1. abc :three phase
 * 2. \f$\alpha, \beta, o\f$ : clarke.
 * 3. d, q, o : direct-quadrature.
 */
class Transform
{
public:
    /**
     * @brief make a -\f$\theta\f$ rotation which transform a clarke_t vector to a dqo_t vector.
     */
    static dqo_t rotation_to_dqo(clarke_t Xabo, float32_t theta);
    /**
     * @brief make a \f$\theta\f$ rotation which transform a dqo_t vector to a clarke_t vector. 
     */
    static clarke_t rotation_to_clarke(dqo_t Xdqo, float32_t theta);
    /**
     * @brief transform a three_phase_t vector to a clarke_t vector.
     */
    static clarke_t clarke(three_phase_t Xabc);
    /**
     * @brief transform a clarke_t vector to a three_phase_t vector. 
     */
    static three_phase_t clarke_inverse(clarke_t Xabo);
    /**
     * @brief transform a three_phase_t vector to a dqo_t vector. 
     */
    static dqo_t to_dqo(three_phase_t Xabc, float32_t theta);
    /**
     * @brief transform a dqo_t vector to a three_phase_t vector. 
     */
    static three_phase_t to_threephase(dqo_t Xdqo, float32_t theta);
};
#endif
