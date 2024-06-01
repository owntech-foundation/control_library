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

#include "control_factory.h"

Pid ControlFactory::pid(float32_t Ts, float32_t Kp, float32_t Ti, float32_t Td, float32_t N, float32_t lower_bound, float32_t upper_bound) {
    PidParams p(Ts, Kp, Ti, Td, N, lower_bound, upper_bound);
    Pid controller = Pid();
    controller.init(p);
    return controller;
}

Pr ControlFactory::pr(float32_t Ts, float32_t Kp, float32_t Kr, float32_t w0, float32_t phi_prime, float32_t lower_bound, float32_t upper_bound){
    PrParams p(Ts, Kp, Kr, w0, phi_prime, lower_bound, upper_bound);
    Pr controller = Pr();
    controller.init(p);
    return controller;
}

RST ControlFactory::rst(float32_t Ts, uint8_t nr, const float32_t *r, uint8_t ns, const float32_t *s, uint8_t nt, const float32_t *t, float32_t lower_bound, float32_t upper_bound){
    RstParams p(Ts, nr, r, ns, s, nt, t, lower_bound, upper_bound);
    RST controller = RST();
    controller.init(p);
    return controller;
}

PllSinus ControlFactory::pllSinus(float32_t Ts, float32_t amplitude, float32_t f0, float32_t rise_time){
    PllSinus pll = PllSinus(Ts, amplitude, f0, rise_time);
    return pll;
}

PllAngle ControlFactory::pllAngle(float32_t Ts, float32_t f0, float32_t rise_time) {
    PllAngle pll = PllAngle(Ts, f0, rise_time);
    return pll;
}

NotchFilter ControlFactory::notchfilter(float32_t Ts, float32_t f0, float32_t bandwidth){
    NotchFilter filter = NotchFilter(Ts, f0, bandwidth);
    filter.reset();
    return filter;
}

LowPassFirstOrderFilter  ControlFactory::lowpassfilter(float32_t Ts, float32_t tau){
    LowPassFirstOrderFilter filter = LowPassFirstOrderFilter(Ts, tau);
    filter.reset();
    return filter;
}

ControlFactory controlLibFactory = ControlFactory();

