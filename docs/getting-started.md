# Getting started

Control library has been written in c++, the library is a set of object you can instanciate. 

Each _`Controller`_ is in a different file (`pid.cpp, rst.cpp, pr.cpp`).

It has mainly been developped to be used with 
The [OwnTech Power API](https://github.com/owntech-foundation/Core) based on [Zephyr](https://www.zephyrproject.org/)
and integrated with [PlatformIO](https://platformio.org/).

## Installation

_Control library_ has been designed to be integrated as a PlatformIO library.

To use it, you need to add the line below in the `platformio.ini` file.

!!!note "platformio.ini"

    ```ini
    lib_deps=
        control_lib = https://github.com/owntech-foundation/control_library.git
    ```


## Using the `Pid()` _`Controller`_.

The use of the `Pid` is based on **3 steps**.

1. Pid object instanciation (declaration).
2. Pid initialisation.
3. Pid execution.

## 1. `Pid` object and parameters instanciation.

For each _`Controller`_ like (`Pid`, `Rst`, `Pr`) we have to define a parameter structure.

We define constants used to initialize the parameter structure.
```c++
#include "pid.h"

static float32_t Ti = 7.5175e-5F;
static float32_t Td = 0.0F;
static float32_t N = 0.0F;
static float32_t upper_bound = 1.0F;
static float32_t lower_bound = 0.0F;
static float32_t Ts = 100.0e-6F;
```

We define the parameter structure. Each parameter is defined [here](../../structPidParams).
```c++
static PidParams pid_params(Ts, kp, Ti, Td, N, lower_bound, upper_bound);
```


We define the variable `pid` which is a `Pid` object.
```c++
static Pid pid;
```

## 2. `Pid` initialization.
In the **`setup_routine()`** of the OwnTech Power API,
you must initialize the `Pid` with its parameters.

```c++
pid.init(pid_params);
```

## 3. `Pid` execution.
In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`
which have two arguments: 

1. the reference
2. the measure.

!!! note
    Remind that the `loop_critical_task()` is called every 100µs, in our examples.

```
new_command = pid.calculateWithReturn(reference, measurement);
```

`new_command` is the result of the pid calculation for one step.

