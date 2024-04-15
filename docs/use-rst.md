# Use The polynomial RST controller.

The use of the `RST is based on **3 steps**.

1. Rst object instanciation (declaration).
2. Rst initialisation.
3. Rst execution.

## 1. Rst object and parameters instanciation.

For each _`Controller`_ like (`Pid`, `Rst`, `Pr`) we have to define a parameter structure.

We define constants used to initialize the parameter structure.
```c++
#include "rst.h"

const uint8_t nr = 3;
const float R[] = { 0.8914, -1.1521, 0.3732 };

const uint8_t ns = 6;
const float S[] = { 0.2, 0.0852, -0.0134, -0.0045, -0.1785, -0.0888 };

const uint8_t nt = 3;
const float T[] = { 1.0, -1.3741, 0.4867 };

static float32_t upper_bound = 1.0F;
static float32_t lower_bound = -1.0F;

static float32_t Ts = 100.0e-6F;
```

We define the parameter structure. Each parameter is defined [here](../../structRstParams).
```c++
static RstParams params = RstParams(Ts, nr, R, ns, S, nt, T, lower_bound, upper_bound);
```


We define the variable `my_rst` which is a `Rst` object.
```c++
static RST my_rst;
```

## 2. RST initialization.
In the **`setup_routine()`** of the OwnTech Power API,
you must initialize the `Rst` with its parameters.

```c++
my_rst.init(params);
```

## 3. RST execution.
In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`
which have two arguments: 

1. the reference
2. the measure.

Remind that the `loop_critical_task()` is called every 100µs.

```
new_command = my_rst.calculateWithReturn(reference, measurement);
```

`new_command` is the result of the rst calculation for one step.

