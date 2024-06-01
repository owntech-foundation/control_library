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

#include <errno.h>
#include <zephyr/logging/log.h>
#include "trigo.h"
#include "pr.h"

LOG_MODULE_DECLARE(ot_control);

int8_t Pr::init(PrParams p) {

    _Ts = p.Ts;
    _Kp = p.Kp;
    if (p.Kr == 0.0) {
        LOG_ERR("Kr = 0 is not possible");
        return -EINVAL; 
    }
    _Kr = p.Kr;
    _inverse_Kr = 1.0 / p.Kr;

    _w0 = p.w0;
    _phi_prime = p.phi_prime;

    float b[2];
    b[0] = p.Ts * ot_cos(p.phi_prime);
    b[1] = -p.Ts * ot_cos(p.phi_prime - p.w0 * p.Ts);

    _B.init(2, b);

    float a[2];
    a[0] = - 2 * ot_cos(p.Ts * p.w0);
    a[1] = +1.0;
    _A.init(2, a);

    if (p.upper_bound < p.lower_bound) {
        LOG_ERR("bounds are not correct\n");
        return -EINVAL;
    }
    _lower_bound = p.lower_bound;
    _upper_bound = p.upper_bound;
    
    _output = 0.0;
    _resonant = 0.0;
    return 0;
}

void Pr::calculate(void) {
    float32_t error = _reference - _measure;
    _resonant = _B.update(error) - _A.update(_resonant);
    float32_t tmp_output = _Kp * error + _Kr * _resonant;
    // saturation management ?
    _output = saturate(tmp_output);
    if (tmp_output != _output)
        _resonant = _inverse_Kr * (_output - _Kp * error); 
}

void Pr::reset(void) {
    _A.reset();
    _B.reset();
    _resonant = 0.0;
    _output = 0.0;
}

void Pr::setW0(float32_t value) {
    _w0 = value;
   _B.setCoeff(1, -_Ts * ot_cos(_phi_prime - _w0 * _Ts ));
   _A.setCoeff(0, -2 * ot_cos(_Ts * _w0));
}
