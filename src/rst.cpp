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
#include "rst.h"
//TODO: make desctructor atleast for test maybe ?
LOG_MODULE_DECLARE(ot_control);


int8_t RST::init(RstParams p) {

    if(p.lower_bound > p.upper_bound) {
        LOG_ERR("lower_bound > upper_bound");
        return -EINVAL;
    }

    this->_lower_bound = p.lower_bound;
    this->_upper_bound = p.upper_bound;

    if (p.s[0] < 1e-6) { // TODO s0 can be negative ?
        LOG_ERR("s0 too low");
        return -EINVAL;
    }
    _inv_s0 = 1.0 / p.s[0];

    if (p.nr == 0 || p.nt == 0 || p.ns <= 1) {
        LOG_ERR("nr or nt == 0 or ns < 2");
        return -EINVAL;
    }

    if (p.r == nullptr || p.s == nullptr || p.t == nullptr) {
        LOG_ERR("nullptr on r, s or t");
        return -EINVAL;
    }
    
    _R.init(p.nr, p.r);
    _T.init(p.nt, p.t); 
    _Sp.init(p.ns-1, &p.s[1]);  // remove first coeff
    
    return 0;
}

void RST::calculate(void) {
    float32_t new_u = 0.0;
    // TODO: integrate inv_s0 in all coeffs ?
    new_u = _inv_s0 * (_T.update(_reference) - _R.update(_measure) - _Sp.update(_output));
    new_u = saturate(new_u);
    _output = new_u;
}

void RST::reset(void) {
    _R.reset();
    _Sp.reset();
    _T.reset();
    this->_output = 0;
}


