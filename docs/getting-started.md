# Getting started

The control library is in the `src/` directory.
It has been written in c++. It has mainly been developped to be used with 
The [OwnTech Power API](https://github.com/owntech-foundation/Core) based on [Zephyr](https://www.zephyrproject.org/)
and integrated with [PlatformIO](https://platformio.org/).

Here we show step-by-step an example on how to use a `Pid()` assuming the
[installation](https://github.com/owntech-foundation/control_library#installation)
step is done. 


The use of the `Pid` is based on **3 steps**.

1. Pid object instanciation (declaration).
2. Pid initialisation.
3. Pid execution.


## Pid object and parameters instanciation.

For each _`Controller`_ like (`Pid`, `Rst`, `Pr`) we have to define a parameter structure.

We define constants used to initialize the parameter structure.
```c++
static float32_t Ti = 7.5175e-5F;
static float32_t Td = 0.0F;
static float32_t N = 0.0F;
static float32_t upper_bound = 1.0F;
static float32_t lower_bound = 0.0F;
static float32_t Ts = 100.0e-6F;
```

We define the parameter structure.
```c++
static PidParams pid_params(Ts, kp, Ti, Td, N, lower_bound, upper_bound);
```

We define the `Pid` object.
```c++
static Pid pid;
```

## Pid initialization.
In the **`setup_routine()`** of the OwnTech Power API,
you must initialize the `Pid` with its parameters.

```c++
pid.init(pid_params);
```

## Pid execution.
In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`
which have two arguments: 
1. the reference
2. the measure.

```
new_command = pid.calculateWithReturn(reference, measurement);
```
