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

#include <errno.h>
#include <zephyr/logging/log.h>
#include "filters.h"
#include "pid.h"
#include "trigo.h"

LOG_MODULE_DECLARE(ot_control, LOG_LEVEL_DBG);


LowPassFirstOrderFilter::LowPassFirstOrderFilter(float32_t Ts, float32_t tau) {
    this->init(Ts, tau);
}

uint8_t LowPassFirstOrderFilter::init(float32_t Ts, float32_t tau) {
    _Ts = Ts;
    _tau = tau;
    if (_tau <= 0.0) {
        LOG_ERR("tau must be > 0");
        // we do not filter
        _a1 = 0.0;
        _b1 = 1.0;
        return -EINVAL;
    }
    float32_t inverse_tau = 1.0 / _tau;
    // series expansion of -exp(-Ts/τ)
    _a1 = -(1.0 + (-Ts * inverse_tau) + (-Ts * inverse_tau) * (-Ts * inverse_tau) * 0.5);
    _b1 = 1 + _a1; 
    _previous_value = 0.0;
    return 0;
}

float32_t LowPassFirstOrderFilter::calculateWithReturn(float32_t signal) {
    float32_t value;
    value = _b1 * signal - _a1 * _previous_value;
    _previous_value = value;
    return value;
};

void LowPassFirstOrderFilter::reset() {
    _previous_value = 0.0F;
}

void LowPassFirstOrderFilter::reset(float32_t value) {
    _previous_value = value;
}


NotchFilter::NotchFilter(float32_t Ts, float32_t f0, float32_t bandwidth) {
    this->init(Ts, f0, bandwidth);
}


uint8_t NotchFilter::init(float32_t Ts, float32_t f0, float32_t bandwidth) {
    _Ts = Ts;
    _f0 = f0;
    _bandwidth = bandwidth;

    float32_t w0 = 2.0 * PI * _f0 * _Ts;
    float32_t deltaW = 2.0 * PI * _bandwidth * _Ts;
    float32_t bgain = 1.0 / (1.0 + deltaW * 0.5);

    float32_t b[3];
    b[0] = bgain;
    b[1] = -2.0 * bgain * ot_cos(w0);
    b[2] = bgain;
    _B.init(3, b);

    float32_t a[2];
    a[0] = -2.0 * bgain * ot_cos(w0);
    a[1] = 2 * bgain - 1.0;
    _A.init(2, a);

    _output = 0.0;
    return 0;
}

float32_t NotchFilter::calculateWithReturn(float32_t signal) {
    _output =  _B.update(signal) - _A.update(_output);
    return _output;
}

void NotchFilter::reset() {
    _output = 0.0;
    _B.reset();
    _A.reset();
}

PllSinus::PllSinus(float32_t Ts, float32_t amplitude, float32_t f0, float32_t rt) {
    this->init(Ts, amplitude, f0, rt);
}

uint8_t PllSinus::init(float32_t Ts, float32_t amplitude, float32_t f0, float32_t rt) {

    float32_t xi = 0.7;
    float32_t wn;
    float32_t Kp;
    float32_t Ti;

    if (Ts < 0) {
        LOG_ERR("Ts must be > 0");
        return -EINVAL;
    }
    if (rt <= 1.e-6) {
        LOG_ERR("rise time must be > 0");
        return -EINVAL;
    }
    _rt = rt;

    if (amplitude <= 1.e-6) {
        LOG_ERR("amplitude must be > 0");
        return -EINVAL;
    }
    _amplitude = amplitude;

    if (f0 < 0.0) {
        LOG_ERR("f0 must be > 0");
        return -EINVAL;
    }
    _Ts = Ts;
    _f0 = f0;

    wn  = 3.0/ _rt;
    Kp = 2.0 * wn * xi / _amplitude;
    Ti = 2.0 * xi / wn;
    PidParams pi_params(_Ts, Kp, Ti, 0.0, 0.0, -100.0 * _f0, 100.0 * _f0);
    _pi.init(pi_params);
    _notch.init(_Ts, 2 * _f0, 0.2*_f0);
    _angle = 0.0;
    _w = 0.0;

    return 0;
}

PllDatas PllSinus::calculateWithReturn(float32_t signal) {
    float32_t error; 
    float32_t error_filtered;
    error = ot_cos(_angle) * signal;

    error_filtered = _notch.calculateWithReturn(error);

    _w  = _pi.calculateWithReturn(error_filtered, 0.0);
    if (_w < 0.0) _w = -_w;
    
    _angle = ot_modulo_2pi(_angle + _w * _Ts);

    return PllDatas(_w, _angle, error_filtered);
    }


void PllSinus::reset(float32_t f0=0.0) {
    _angle = 0.0;
    _w = f0*2.0*PI;
    _notch.reset();
    _pi.reset(_w);
}
