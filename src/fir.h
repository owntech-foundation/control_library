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

#ifndef FIR_H_
#define FIR_H_
#include <arm_math.h>

/**
 * @class Fir
 * @brief a class to implement the Finite Impulse Response filter behaviour
 *
 * @param nc number of coefficients
 *
 * @param *coeffs pointer to array of coefficients
 */
class Fir {
public:
    Fir();
    Fir(const uint8_t nc, const float32_t *coeffs);
    /**
     * @brief method to initialize the Fir with its coefficients
     *
     * @param nc  number of coefficients
     * @param coeffs pointer to array of coefficients
     * @return 
     */
    uint8_t init(uint8_t nc, const float32_t *coeffs);
    float32_t update(float32_t new_data);
    void reset();
    void setCoeff(uint8_t n, float32_t value);
    ~Fir();
private:
    uint8_t nc;
    float32_t *coeffs;
    float32_t *datas;
};
#endif
