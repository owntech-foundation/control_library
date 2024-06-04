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
 * @author Guillaume Arthaud.
 *
 */
#include <arm_math.h>
#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include <zephyr/logging/log.h>

/**
 * @brief Controller interface for various inherited class like pid, rst, pr,...
 *
 * @tparam refs_T type of the reference
 * @tparam meas_T type of the measure
 * @tparam outputs_T type of the output
 * @tparam params_T type of the parameter 
 * @param parameters structure including all parameters needs to make calculations. 
 *
 * we assume that outputs_T has already an order relation implemented.
 *
 */
template<typename refs_T, typename meas_T, typename outputs_T, typename params_T>
class Controller
{
    public:
    /**
     * @brief initialize the controller.
     *
     * @param parameters 
     * @return 0 if ok -EINVAL else.
     */
    virtual int8_t init(params_T parameters) = 0; //ref + mesure

    /**
     * @brief reset internal states and the last command of the controller.
     */ 
    virtual void reset(void) = 0;

    /**
     * @brief calculate a new command value according to a reference fixed using
     * `setReference` method and a measuremnt fixed using `setMeasurement`.
     *
     * The new command value can be captured using the `get_output` method.
     */
    virtual void calculate(void) = 0; 
    /**
     * @brief calculate a new command value according the argument values
     *
     * @param yrefs reference
     * @param y measure
     * @return new command value.
     */
    virtual outputs_T calculateWithReturn(refs_T yref, meas_T y) {
        this->setReference(yref);
        this->setMeasurement(y);
        this->calculate();
        return this->getOutput();
    }

    /**
     * @brief capture a new reference.
     *
     * @param reference 
     */
    virtual void setReference(refs_T reference) {
        _reference = reference;
    }; 

    /**
     * @brief capture a new measurement.
     *
     * @param measure 
     */
    virtual void setMeasurement(meas_T measure) {
        _measure = measure;
    }; 

    /**
     * @brief retrieve the last command value calculated.
     *
     * @return 
     */
    virtual outputs_T getOutput() {
        return _output;
    }; 

    /**
     * @brief limit the argument `u` between `upper_bound` and `lower_bound` 
     * it is called by the `calculate` method.
     *
     * @param u should be a command value.
     * @return 
     */
    virtual outputs_T saturate(outputs_T u) {
        if ( u > _upper_bound) {
            u = _upper_bound;
        }
        if (u < _lower_bound) {
            u = _lower_bound;
        }
        return u;
    };

    /**
     * @brief change lower an upper bounds of output saturation.
     *
     * @param lower  
     * @param upper
     * @return 
     */
    virtual void setBounds(float32_t lower, float32_t upper) {
        if (lower < upper) {
            _lower_bound = lower;
            _upper_bound = upper;
        }
    }

protected:
    float32_t _Ts; // sample time
    outputs_T _lower_bound;
    outputs_T _upper_bound;
    // template 
    refs_T _reference;
    outputs_T _output;
    meas_T _measure;

};

#endif /* !CONTROLLER_H_ */
