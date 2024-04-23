# PID _`Controller`_.
## Introduction.
The PID :material-information-outline:{title="K. Astrom et T. Hagglund, Advanced PID Control."}
 `Controller` is implemented here in a **Standard form**.

<figure markdown="span">
![pid block diagram](images/pid_diagram.svg){width=600}
<figcaption>bloc diagram of standard form of a continuous PID with filtered derivative</figcaption>
</figure>


### Parameters:
Here the continuous transfer function representation of the PID.

$$command = K_p . \left( \varepsilon + \dfrac{1}{T_i.s} .  \varepsilon + \dfrac{T_d.s}{1 + \frac{T_d}{N}.s}.\varepsilon \right)$$

where:

* $K_p$ is the proportionnal gain
* $T_i$ is the integration time
* $T_d$ is derivative time
* $N$ help to filter the derivative (typical values $\in [2, 20]$).



=== warning "controller are sampled"
    We show here the continuous form of the function we want to implement.
    But calculation are sampled.
    [Relationship to Laplace transform](https://en.wikipedia.org/wiki/Z-transform#Relationship_to_Laplace_transform)

### Discretization:
This `Pid()` object is implemented using a **_backward euler integration method_**.

We have the following discrete equation implemented:

$$ 
\begin{align}
\epsilon_k &=  y_{ref_{k}} - y_{meas_{k}} \\ \\ 
i_k &= i_{k-1}  + T_s . \epsilon_k \\ \\
d_k &= \dfrac{\epsilon_k - \epsilon_{k-1}}{T_s} \\ \\
f_{d_k} &= (1 - e^{-T_s/\tau}) . d_k  + e^{-T_s/\tau} . f_{d_{k-1}}
\end{align}
$$

$$ u_k = K_p .\left( \epsilon_k +  \dfrac{1}{T_i} . i_k + T_d . f_{d_k}\right) $$

## Use of the PID `Controller`.
The use of the `Pid` is based on **3 steps**.

1. Object instanciation (declaration).
2. Initialisation.
3. Execution.

=== "1. Object and parameters instanciation."

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

=== "2. Initialization."
    In the **`setup_routine()`** of the OwnTech Power API,
    you must initialize the `Pid` with its parameters.

    ```c++
    pid.init(pid_params);
    ```

=== "3. Execution."
    In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`
    which have two arguments: 

    1. the reference
    2. the measure.

    !!! note
        Remind that the `loop_critical_task()` is called at the sampling time you define and
        must be equal to $T_s$.

    ```
    new_command = pid.calculateWithReturn(reference, measurement);
    ```

    `new_command` is the result of the pid calculation for one step.

## Example
You can find a _pid_ use with the [buck voltage mode example](../../../examples/TWIST/DC_AC/grid_following) which
regulates a DC voltage.

