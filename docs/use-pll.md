# Use the software Phase Locked Loop.

## Introduction

The `PllSinus`:material-information-outline:{title="M. Bhardwaj, « Software Phase Locked Loop Design Using C2000™ Microcontrollers for Single Phase Grid Connected Inverter », 2013."} is used to track sinusoid and extract angle and pulsation.
It is included in the `filters.cpp` module.


<fig>
![pll block diagram](./images/pll_block_diagram.svg){width=700px}
<figcaption>block diagram of software phase locked loop</figcaption>
</fig>

### Parameters:

* We set $f_0$ the frequency of the signal to track linked with $\omega_0=2.\pi.f_0$. 
* $\Delta \omega$ has been arbitrarily fixed to be 20% of $\omega_0$.
* $K_p, T_i$ parameters are computed using the `rise_time` of the closed loop, which
  fix the dynamic of the PLL and we have also to take into account of the `amplitude` of
  the signal.

## Use of the `PllSinus` filter.

The use of the `PllSinus is based on **3 steps**.

1. PllSinus object instanciation (declaration).
2. PllSinus initialisation.
3. PllSinus execution.

### 1. `PllSinus` object and parameters declaration.

We define constants used to initialize the parameter structure.
```c++
#include "filters.h"

static float32_t amplitude = 16.0F; // amplitude of the voltage sinus to track.
static float32_t f0 = 50.0;               // frequency assumed of the signal to track [Hz]
static float32_t rise_time = 50.e-3F;     // dynamic of the loop [s].
static float32_t Ts = 100.0e-6F;          // sampling time [s]
```

We define the variable `pll` which is an instance of `PllSinus` object.
```c++
static PllSinus pll;
```

### 2. `PllSinus` initialization.
In the **`setup_routine()`** of the OwnTech Power API,
you must initialize the `PllSinus` with its parameters.

```c++
pll.init(Ts, amplitude, f0, rise_time);
```

### 3. `PllSinus` execution.
In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`

!!! note
    Remind that the `loop_critical_task()` is called at the sampling time you define and
    must be equal to $T_s$.

```
pll_datas = pll.calculateWithReturn(signal_to_track);
```

`pll_datas` is a [structure](../../structPllDatas) which kept the results of the PllSinus calculation for one step.

the PllData structure has 3 fields:
```c++
struct PllDatas {
    float32_t w;     // estimated pulsation [rad/s]
    float32_t angle; // estimated angle [rad]
    float32_t error; // angle error [rad]
};
```

