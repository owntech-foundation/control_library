# Use First Order Low Pass Filter

## Introduction.

![first order filter bode plot](images/first_order_filter-light.svg#only-light){ align=center style="border-radius: 5px;" loading=lazy}
![first order filter bode plot](images/first_order_filter-dark.svg#only-dark){ align=center style="border-radius: 5px;" loading=lazy}

### Parameters
Here, the transfer function representation of the first order filter (where $s$ is the
Laplace variable) :  

$$H(s) = \dfrac{1}{1+\tau.s}$$ 

Where:

* $\tau$ is the constant time in [s].

!!! warning "transfer function is sampled"
    We show here the continuous transfer function of the function we want to implement.
    But calculation are sampled.
    [Relationship to Laplace transform](https://en.wikipedia.org/wiki/Z-transform#Relationship_to_Laplace_transform)

### Discretization
Using the $z$-transform we get the following form:

$$
\begin{align}
H(z^{-1}) = \dfrac{b_1.z^{-1}}{1+a_1.z^{-1}} \\ \\
H(z) = \dfrac{b_1}{z + a_1} \\
\end{align}
$$

As there's a direct relation between $z^{-1}$ and $q^{-1}$ the delay operator, we can
write the reccuring equations we will use in the code.


We give here the recurrence equation used to filter the $\text{input}$ signal.

$$ 
out_k = b_1 . \text{in}_k - a_1 . out_{k-1}
$$

where:

$$
\begin{align}
a_1 &= -exp\left(-\dfrac{Ts}{\tau}\right) \\ \\
b_1 &= 1 + a_1
\end{align}
$$

## Use.

!!! Example "3 steps to use the first order filter"

    === "1. Declaration."
        ```cpp
            #include "filters.h"

            const float32_t tau = 1e-3;               // constant time
            const float32_t Ts = 100e-6;              // sampling time
            myfilter = LowPassFirstOrderFilter(Ts, tau);
        ```

    === "2. Initialisation."
        Before to run, we advise to `reset` the filter, in the **`setup_routine()`** of the
        OwnTech Power API.

        ```
        myfilter.reset();
        ```

    === "3. Execution."
        In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`.

        ```cpp
        signal_filtered = myfilter.calculateWithReturn(signal_to_filter);
        ```

        It returns the data filtered.
        !!! note
            Remind that the `loop_critical_task()` is called at the sampling time you define and
            must be equal to $T_s$.
