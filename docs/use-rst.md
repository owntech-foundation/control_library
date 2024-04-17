# Polynomial RST `Controller`.
## Introduction.
The polynomial R-S-T `Controller`:material-information-outline:{ title="I. D. Landau, « The R-S-T digital controller design and applications », Control Engineering Practice, vol. 6, nᵒ 2, p. 155‑165, févr. 1998, doi: 10.1016/S0967-0661(98)00016-1."}
is a two degree of freedom **digital** controller (allowing to fix a regulation dynamic uncoupling from the setting point dynamic). 

It allows to implement various digital filters in the measurement part ($R$) or in the
integration part ($\frac{1}{S}$).

![rst block diagram](images/rst_diagram.svg#only-light){width=100%}
![rst block diagram](images/rst_diagram-dark.svg#only-dark){width=100%}

### Parameters:
Here the equation of the RST controller.

$$S(q^{-1}).u = T(q^{-1}).y_{ref} - R(q^{-1}).y$$

where:

* $S(q^{-1}),\ T(q^{-1})\ \text{and}\ R(q^{-1})$ are polynomial of delay operator $q^{-1}$

* $S(q^{-1}) = s_0 + s_1.q^{-1} + ... + s_{ns}.q^{-ns}$
* $R(q^{-1}) = r_0 + r_1.q^{-1} + ... + r_{nr}.q^{-nr}$
* $T(q^{-1}) = t_0 + t_1.q^{-1} + ... + t_{nt}.q^{-nt}$


## Use of the RST `Controller`.
The use of the `RST is based on **3 steps**.

1. `RST` object instanciation (declaration).
2. `RST` initialisation.
3. `RST` execution.
### 1. `RST` object and parameters instanciation.

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

### 2. `RST` initialization.
In the **`setup_routine()`** of the OwnTech Power API,
you must initialize the `Rst` with its parameters.

```c++
my_rst.init(params);
```

### 3. `RST` execution.
In the **`loop_critical_task()`** you can call the method `calculateWithReturn()`
which have two arguments: 

1. the reference
2. the measure.

!!! note
    Remind that the `loop_critical_task()` is called at the sampling time you define and
    must be equal to $T_s$.

```
new_command = my_rst.calculateWithReturn(reference, measurement);
```

`new_command` is the result of the rst calculation for one step.

