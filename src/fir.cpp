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
 */
#include "fir.h"
#include <errno.h>
#include <zephyr/logging/log.h> 
LOG_MODULE_REGISTER(ot_control, LOG_LEVEL_DBG);
//LOG_MODULE_DECLARE(ot_control, LOG_LEVEL_ERR);

Fir::Fir() {
}

Fir::Fir(const uint8_t nc, const float *coefficients) {
    init(nc, coefficients);
}

uint8_t Fir::init(uint8_t nc, const float *coefficients) {
    if (nc == 0) {
        LOG_ERR("erreur nc = 0");
        return -EINVAL;
    }
    if (coefficients == nullptr) {
        LOG_ERR("coefficients = nullptr");
        return -EINVAL;
    }

    this->nc = nc;
    this->coeffs = new float32_t [nc];
    this->datas = new float32_t [nc]();

    for (uint8_t k=0; k < nc; k++) {
        this->coeffs[k] = coefficients[k];
        LOG_DBG("coeffs[%d] = %f\n", k, (double) this->coeffs[k]);
    }
    return 0;
}

float32_t Fir::update(float32_t new_data) {
    float32_t new_value = 0;
    datas[0] = new_data;
    new_value = coeffs[0] * datas[0];
    for (uint8_t k = nc-1; k > 0; k--)
    {
        new_value += coeffs[k] * datas[k];
        datas[k] = datas[k-1];
    }
    return new_value;
}

void Fir::reset(){
    for (uint8_t k=0; k < nc; k++) {
        datas[k] = 0.0;
    }
}

Fir::~Fir() {
    if (coeffs != nullptr)
        delete[] coeffs;
    if (datas != nullptr)
        delete[] datas;
}

void Fir::setCoeff(uint8_t n, float32_t value) {
    if (n < nc && n >= 0) {
        this->coeffs[n] = value;
    }
}


