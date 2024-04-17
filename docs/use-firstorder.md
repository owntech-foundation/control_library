# Use First Order Low Pass Filter


![first order filter bode plot](images/first_order_filter-light.svg#only-light){ align=center style="border-radius: 5px;" loading=lazy}
![first order filter bode plot](images/first_order_filter-dark.svg#only-dark){ align=center style="border-radius: 5px;" loading=lazy}


Here, the transfer function representation of the first order filter (where $s$ is the
Laplace variable) :  

$$H(s) = \dfrac{1}{1+\tau.s}$$ 

Where:

* $\tau$ is the constant time in [s].

!!! warning "transfer function is sampled"
    We show here the continuous transfer function of the function we want to implement.
    But calculation are sampled.
    [Relationship to Laplace transform](https://en.wikipedia.org/wiki/Z-transform#Relationship_to_Laplace_transform)

## Use
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