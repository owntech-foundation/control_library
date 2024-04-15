# Filters module.

In the filters module there's multiple digital filter.

The software Phase Locked Loop is also in this module but there's a dedicated
[section](../use-pll).


## First Order Low Pass Filter

Here, the transfer function representation of the first order filter:<br>
$H(s) = \dfrac{1}{1+\tau.s}$ where $\tau$ is the constant time in [s].


### Declaration.
```cpp
    const float32_t tau = 1e-3;               // constant time
    const float32_t Ts = 100e-6;              // sampling time
    myfilter = LowPassFirstOrderFilter(Ts, tau);
```

### Initialisation.
Before to run, we advise to `reset` the filter, in the **`setup_routine()`** of the
OwnTech Power API.

```
myfilter.reset();
```

### Execution.
In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`.

Remind that the `loop_critical_task()` is called every 100µs.
```cpp
signal_filtered = myfilter.calculateWithReturn(signal_to_filter);
```

It returns the data filtered.


## Notch Filter 

Here, the transfer function representation of the notch filter:<br>

$H(s) = \dfrac{s^2 + \omega_0^2}{s^2 + \Delta \omega.s + \omega_0^2}$ 

Where $\omega_0$ is the notch pulsation,

and $\Delta \omega$ is the bandwidth of the filter.

### Declaration.
```cpp
    const float32_t f0 = 50.0;               // notch frequency [Hz]
    const float32_t bandwidth = 5.0;         // notch bandwidth [Hz]
    const float32_t Ts = 100e-6;              // sampling time [s]
    myfilter = NotchFilter(Ts, f0, bandwidth);
```

### Initialisation.
Before to run, we advise to `reset` the filter, in the **`setup_routine()`** of the
OwnTech Power API.

```
myfilter.reset();
```

### Execution.
In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`.

Remind that the `loop_critical_task()` is called every 100µs.
```cpp
signal_filtered = myfilter.calculateWithReturn(signal_to_filter);
```

It returns the data filtered.
