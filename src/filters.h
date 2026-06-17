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

#ifndef FILTERS_H_
#define FILTERS_H_
#include "arm_math_types.h"
#include "trigo.h" 
#include "fir.h"
#include "pid.h"

class LowPassFirstOrderFilter {
public:
    LowPassFirstOrderFilter() = default;
    LowPassFirstOrderFilter(float32_t Ts, float32_t tau);
    int8_t init(float32_t Ts, float32_t tau);
    float32_t calculateWithReturn(float32_t signal);
    void reset();
    void reset(float32_t value);
private:
    float32_t _Ts;
    float32_t _tau;
    float32_t _a1;
    float32_t _b1;

    float32_t _previous_value;
};

class NotchFilter {
public:
    NotchFilter() {};
    /**
     * @brief its a band stop filter
     *
     * @param Ts sample time [s]
     * @param f0 central frequency to stop [Hz]
     * @param bandwidth  frequency band [Hz] around f0 where gain < -3dB 
     */
    NotchFilter(float32_t Ts, float32_t f0, float32_t bandwidth);

    /**
     * @brief initialize the band stop filter parameters
     *
     * @param Ts sample time [s]
     * @param f0 central frequency to stop in [Hz]
     * @param bandwidth  frequency band [Hz] around f0 where gain < -3dB 
     */
    int8_t init(float32_t Ts, float32_t f0, float32_t bandwidth);
    float32_t calculateWithReturn(float32_t signal);
    void reset();
private:
    float32_t _Ts;
    float32_t _f0;
    float32_t _bandwidth;

    Fir _B; // numerator of the filter
    Fir _A; // denominator of the filter
    float32_t _output;
};

/**
 * @class PllDatas
 * @brief datas returned by pll calculations
 * 
 * @param w pulsation estimated of the tracked signal [rad/s]
 *
 * @param angle of the tracked signal [rad]
 *
 */
struct PllDatas {
    float32_t w;
    float32_t angle;
    float32_t error;
};

class Pll {
public:
    Pll() {};
    PllDatas calculateWithReturn(float32_t signal);
    virtual void reset(float32_t f0);
protected:
    virtual float32_t _error(float32_t ref, float32_t mes) = 0;
    virtual float32_t _filt_error(float32_t error) = 0;  
    virtual float32_t _vco(float32_t error) = 0;
    virtual void _init_pi(float32_t rise_time) = 0;
    int8_t _check_and_get_args(float32_t Ts, float32_t f0, float32_t rise_time);
    float32_t _Ts;
    float32_t _f0;
    float32_t _rt;
    Pid _pi;
    float32_t _w;
    float32_t _angle;
};

class PllSinus: public Pll {
public:
    /**
     * @brief a software phase lock loop on a sinusoidal signal 
     *
     * @param Ts sample time in [s]
     * @param amplitude amplitude of the signal to track.
     * @param f0 mean frequency of the signal to track
     * @param rt rise time of the loop in [s].
     */
    PllSinus() {};
    PllSinus(float32_t Ts, float32_t amplitude, float32_t f0, float32_t rt);
    int8_t init(float32_t Ts, float32_t amplitude, float32_t f0, float32_t rt);
    virtual void reset(float32_t f0) override;
protected:
    virtual float32_t _error(float32_t ref, float32_t mes) override;
    virtual float32_t _filt_error(float32_t error) override;
    virtual float32_t _vco(float32_t error) override;
    virtual void _init_pi(float32_t rise_time) override;
private:
    float32_t _amplitude;
    NotchFilter _notch;
};

class PllAngle: public Pll {
public:
    /**
     * @brief a software phase lock loop on a sawtooth signal 
     *
     * @param Ts sample time in [s]
     * @param f0 mean frequency of the signal to track
     * @param rt rise time of the loop in [s].
     */
    PllAngle() {};
    PllAngle(float32_t Ts, float32_t f0, float32_t rt);
    int8_t init(float32_t Ts, float32_t f0, float32_t rt);
protected:
    virtual float32_t _error(float32_t ref, float32_t mes) override;
    virtual float32_t _filt_error(float32_t error) override;
    virtual float32_t _vco(float32_t error) override;
    virtual void _init_pi(float32_t rise_time) override;
};


/**
 * @class HarmonicDetector
 * @brief Real-time amplitude estimator for a single harmonic using synchronous
 *        demodulation. Per-sample cost: 4 multiplies + 2 LPF updates, no trig
 *        calls after init.
 *
 * @param Ts          sample time [s]
 * @param w_harmonic  pulsation of the harmonic to detect [rad/s]
 * @param tau         LPF time constant [s]; rule of thumb: 1/(pi*f_fundamental)
 */
class HarmonicDetector {
public:
    HarmonicDetector() = default;
    HarmonicDetector(float32_t Ts, float32_t w_harmonic, float32_t tau);
    int8_t init(float32_t Ts, float32_t w_harmonic, float32_t tau);
    /**
     * @brief Feed one sample and return the estimated harmonic peak amplitude.
     * @param signal  raw time-domain sample
     * @return estimated peak amplitude [same unit as signal]
     */
    float32_t calculateWithReturn(float32_t signal);
    void reset();
private:
    float32_t _cos_wh;      // cos(w_harmonic*Ts) — stored for reset
    float32_t _sin_wh;      // sin(w_harmonic*Ts) — stored for reset
    float32_t _two_cos_wh;  // 2*cos(w_harmonic*Ts) — recurrence coefficient
    float32_t _cos_n;       // cos(n * w_harmonic * Ts), current step
    float32_t _cos_prev;    // cos((n-1) * w_harmonic * Ts)
    float32_t _sin_n;       // sin(n * w_harmonic * Ts), current step
    float32_t _sin_prev;    // sin((n-1) * w_harmonic * Ts)
    LowPassFirstOrderFilter _lpf_d;  // low-passes signal*cos (in-phase)
    LowPassFirstOrderFilter _lpf_q;  // low-passes signal*sin (quadrature)
};

#endif
