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

#include <zephyr/logging/log.h>
#include <errno.h>
#include "pid.h"

LOG_MODULE_DECLARE(ot_control);

int8_t Pid::init(PidParams p) {

    if (p.Ts <= 0.0) {
        LOG_ERR("Ts should be > 0");
        return -EINVAL;
    }
    _Ts = p.Ts;
    _inverse_Ts = 1.0 / p.Ts;

    if (p.Kp == 0.0)
    {
        LOG_ERR("Kp equal To 0");
        return -EINVAL;
    }
    _Kp = p.Kp;
    _inverse_Kp = 1.0 / p.Kp;

    if ( p.Ti == 0.0 ) {
        LOG_ERR("Ti can not be equal to 0.0\n");
        return -EINVAL;
    } 
    _Ti = p.Ti;
    _inverse_Ti = 1.0 / p.Ti;

    _Td = p.Td;

    float32_t tau;
    if (p.N == 0.0)
        tau = 0.0;
    else
        tau = _Td / p.N;
    if (tau < 0.0) {
        LOG_ERR("Td/N should be > 0");
        return -EINVAL;
    }
    _N = p.N;
    _b1_filter = _Ts / (_Ts + tau );
    _a1_filter = - tau / (_Ts + tau); 

    if (p.lower_bound > p.upper_bound) {
        LOG_ERR("lower bound > upper_bound");
        return -EINVAL;
    }
    _lower_bound = p.lower_bound;
    _upper_bound = p.upper_bound;


    _integral = 0.0;
    _previous_error = 0.0;
    _previous_f_deriv = 0.0;
    _output = 0.0;

    LOG_DBG("_Ts = %f\n", _Ts);
    LOG_DBG("_Kp = %f\n", _Kp);
    LOG_DBG("_Td = %f\n", _Td);
    LOG_DBG("_Ti = %f\n", _Ti);
    LOG_DBG("_N = %f\n", _N);

    return 0;
}

void Pid::calculate(void) {
    float32_t error;
    float32_t deriv, filtered_deriv;
    float32_t tmp_output;
    error = _reference - _measure;

    _integral = _integral + _Ts * error;

    deriv = _inverse_Ts * (error - _previous_error);

    filtered_deriv = _b1_filter * deriv - _a1_filter * _previous_f_deriv; 

    tmp_output = _Kp * ( error + _inverse_Ti * _integral + _Td * filtered_deriv ) ; 

    _output = saturate(tmp_output);
    // re-compute integral to no have integral divergence during saturation
    if (_output != tmp_output)
        _integral = _Ti * (_inverse_Kp * _output - error - _Td * filtered_deriv);

    _previous_error = error;
    
    _previous_f_deriv = filtered_deriv;
}


void Pid::reset() {
    Pid::reset(0.0);
}

void Pid::reset(float32_t output=0.0) {
    _integral = _Ti * _inverse_Kp * output;
    _output = 0.0;
    _previous_f_deriv = 0.0;
    _previous_error = 0.0;
}
