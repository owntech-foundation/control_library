# Use the SOGI PLL.

## Introduction

The Second Order Generalized Integrator based PLL `SOGI-PLL`:material-information-outline:{title="Mihai Ciobotaru, Remus Teodorescu and Frede Blaabjerg, « A New Single-Phase PLL Structure Based on Second Order Generalized Integrator », 2006."} is a filter used to retrieve phase information from a mono phase or three phased system.
It is included in the `filters.cpp` module.

Digital implementation of the SOGI PLL is done using the `impulse invariant`:material-information-outline:{title="Alejandro Gómez Yepes, « Digital Resonant Current Controllers for Voltage Source Converters », 2011."}  using $R_{1h}$ discretization method.


![SOGI PLL block diagram](./images/sogi_pll.drawio){width=700px}
<figcaption>block diagram of SOGI PLL</figcaption>

 - `SOGI QSG` stands for Quadrature Signal Generator. It takes an input signal and output the signal in $\alpha\beta$ frame. This `QSG` is particularily useful for single phase systems where a simple Clark transform can not be used.
 - `SRF PLL` stands for Synchronous Reference Frame Phase Lock Loop. It takes an input signal in the $\alpha\beta$ frame and retrieve the angle $\theta$ and the pulsation $\omega$

### Parameters:

The SOGI PLL is discretized form is the following:


$R_{1hd} = Kr \times Ts \left(. \cos(\phi') . \dfrac{1- z^{-1}.cos(\omega .T_s)}{1 - 2.z^{-1}cos(\omega T_s)+z^{-2}} -sin(\phi').\dfrac{z^{-1}.sin(\omega .T_s)}{1 - 2.z^{-1}cos(\omega T_s)+z^{-2}}\right)${width=700px}


Kr parameter augment SOGI PLL dynamic but decrease its selectivity.

### Retrieve Quadrature signal

Following equations are expanded :

$$cos(\theta_k) = cos(\theta_{k-1} +\omega.Ts)$$

$$sin(\theta_k) = sin(\theta_{k-1} +\omega.Ts)$$

Hence we derive the quadrature sinus term

$$sin(\theta_k) = -\dfrac{cos(\omega . Ts)}{sin(\omega .Ts)}.cos(\theta_k) + \dfrac{1}{sin(\omega . Ts)}.cos(\theta_{k-1})$$


## Use of the `SOGI` filter.

The use of the `Sogi is based on **3 steps**.

1. Object instanciation (declaration).
2. Initialisation.
3. Execution.

!!!Example
    === "1. Object and parameters declaration."

        We define constants used to initialize the parameter structure.
        ```c++
        #include "filters.h"

        static float32_t kr = 500;                // Sogi filter gain defining dynamics
        static float32_t Ts = 100.0e-6F;          // sampling time of the control period [s]
        ```

        We define the variable `sogi_filter` which is an instance of `Sogi` object.
        ```c++
        static Sogi sogi_filter;
        ```

    === "2. Initialization."
        In the **`setup_routine()`** of the OwnTech Power API,
        you must initialize the `sogi_filter` with its parameters.

        ```c++
        sogi_filter.init(Ts, kr);
        ```

    === "3. Execution."
        In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`

        ```
        sogi_data = sogi_filter.calculateWithReturn(signal_to_track);
        ```

        `sogi_data` is a [structure](../../structSogiData) which kept the results of the sogi_filter calculation for one step.

        the SogiData structure has 3 fields:
        ```c++
        struct SogiData {
            float32_t theta;      // estimated angle [rad]
            float32_t alpha;      // cosinus term
            float32_t beta;       // sinus term
        };
        ```

        !!! note
            Remind that the `loop_critical_task()` is called at the sampling time you define and
            must be equal to $T_s$.

## Example
You can find a _Sogi_ use with the [grid following example](../../../examples/TWIST/DC_AC/grid_following) which
requires a synchronisation to inject current in parallel with a voltage source.
