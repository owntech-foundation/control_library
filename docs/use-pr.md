# Use Proportionnal Resonant controller.

The use of the `Pr is based on **3 steps**.

1. Pr object instanciation (declaration).
2. Pr initialisation.
3. Pr execution.

## 1. Pr object and parameters instanciation.

For each _`Controller`_ like (`Pid`, `Rst`, `Pr`) we have to define a parameter structure.

We define constants used to initialize the parameter structure.
```c++
#include "pr.h"

static float32_t Kp = 0.001F;
static float32_t Kr = 300.0F;
static float32_t w0 = 2 * PI * 50.0F;
static float32_t phase_shift = 0.0F;
static float32_t upper_bound = 1.0F;
static float32_t lower_bound = -1.0F;
static float32_t Ts = 100.0e-6F;
```

We define the parameter structure. Each parameter is defined [here](structPrParams.md).
```c++
static  PrParams params = PrParams(Ts, Kp, Kr, w0, phase_shift, lower_bound, upper_bound);
```


We define the variable `prop_res` which is a `Pr` object.
```c++
static Pr prop_res;
```

## 2. Pr initialization.
In the **`setup_routine()`** of the OwnTech Power API,
you must initialize the `Pr` with its parameters.

```c++
prop_res.init(params);
```

## 3. Pr execution.
In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`
which have two arguments: 

1. the reference
2. the measure.

Remind that the `loop_critical_task()` is called every 100µs.

```
new_command = prop_res.calculateWithReturn(reference, measurement);
```

`new_command` is the result of the pr calculation for one step.

