# Use Notch Filter 
## Introduction.
The notch filter allow to remove a band of frequencies from an input signal.

![notch filter bode plot](images/notch_filter-light.svg#only-light){ align=center style="border-radius: 5px;" loading=lazy}
![notch filter bode plot](images/notch_filter-dark.svg#only-dark){ align=center style="border-radius: 5px;" loading=lazy}

### Parameters.
Here, the transfer function representation of the notch filter:<br>

$$H(s) = \dfrac{s^2 + (2 \pi f_0)^2}{s^2 + 2 \pi \Delta f .s + (2 \pi f_0)^2}$$ 

Where:

* $f_0$ is the notch frequency,

* $\Delta f$ is the bandwidth of the filter.
### Discretization.

$$
out_k = b_0 . in_k + b_1 . in_{k-1} + b_2 . in_{k-2} - a_1 . out_{k-1} - a_2 . out_{k-2}.
$$

Where:

$$
\begin{align}
b_0 &= \dfrac{1}{1+ \pi . \Delta f . T_s} \\ \\
b_1 &= -2 . b_0 .\cos(2\pi.f_0.T_s)  \\ \\
b_2 &= b_0 \\ \\
a_1 &= b_1 \\ \\
a_2 &= 2 b_0 - 1
\end{align}
$$

## Use of the `NotchFilter` object.

=== "Declaration."
    ```cpp
        #include "filters.h"

        const float32_t f0 = 50.0;               // notch frequency [Hz]
        const float32_t bandwidth = 5.0;         // notch bandwidth [Hz]
        const float32_t Ts = 100e-6;              // sampling time [s]
        myfilter = NotchFilter(Ts, f0, bandwidth);
    ```

=== "Initialisation."
    Before to run, we advise to `reset` the filter, in the **`setup_routine()`** of the
    OwnTech Power API.

    ```
    myfilter.reset();
    ```

=== "Execution."
    In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`.

    ```cpp
    signal_filtered = myfilter.calculateWithReturn(signal_to_filter);
    ```

    It returns the data filtered.

    !!! note
        Remind that the `loop_critical_task()` is called at the sampling time you define and
        must be equal to $T_s$.
