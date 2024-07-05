# control_library

Control and dsp algorithms for owntech converters

This library has been integrated and tested on [TWIST](https://github.com/owntech-foundation/TWIST) converters using [SPIN](https://github.com/owntech-foundation/SPIN) controller.

The integration is based on the dedicated [OwnTech Power API](https://github.com/owntech-foundation/Core).

__Remarks__: The library is currently not optimized to make it easier to read (we hope).

## Features

The library provides the following functionalities mainly through C++ objects.

### Controllers.

Controllers provided are:
 * `Pid()`: Standard form of the PID regulator.
 * `Pr()`: Proportional Resonant regulator.
 * `Rst()`: Discrete form of Polynomial regulator.

`Pid()`, `Pr()` and `Rst()` inherit from the `Controller()` class which define the same interface.

### Filters

 * `PllSinus()`: Software PLL (Phased Lock Loop)
 * Digital filters: `LowPassFirstOrdreFilter()`, `NotchFilter()`

### Three phase system transformations.

It provides also three phase system transformation through a `static` class : `Transform` from `transform.h` header. 
* `Idqo = Transform::to_dqo(Iabc, angle);`
* `Iabc = Transofrm::to_abc(Idqo, angle);`
* `Iabo = Transform::clarke(Iabc);`
* `Iabc = Transform::clarke_inverse(Iabo);`
* `Idqo = Transform::rotation_to_dqo(Iabo, angle)`
* `Iabo = Transform::rotation_to_clarke(Idqo, angle)`

It includes also types:
* `threephase_t`
* `dqo_t`
* `clarke_t`


## Installation

We describe here the process to use it with the [Power API](https://github.com/owntech-foundation/Core) which has been designed to use PlatformIO.

The installation here recall the procedure of using a library in [PlatformIO](https://docs.platformio.org/en/latest/librarymanager/index.html).

To use the library, you need to add the line above in the `platformio.ini` file.

```ini
lib_deps=
    control_lib = https://github.com/owntech-foundation/control_library.git
```

You can find various examples in the [OwnTech examples library](https://github.com/owntech-foundation/examples) 

## Links:

Links which inspired this work:

* PID:
  * K. Astrom et T. Hagglund, Advanced PID Control.
* PR:
  * A. Gï, « Digital Resonant Current Controllers For Voltage Source Converters ». [Thesis](http://agyepes.webs.uvigo.es/files/Thesis.pdf)
* RST:
  * I. D. Landau, C. Cyrot, A. Voda, et D. Rey, « Robust digital control of flexible structures using the combined pole placement/sensitivity function shaping method », in Proceedings of 1994 American Control Conference - ACC ’94, Baltimore, MD, USA: IEEE, 1994, p. 283‑288. doi: 10.1109/ACC.1994.751743.
  * I. D. Landau et G. Zito, « Digital Control Systems - New edition (I. D. Landau &amp; G. Zito) », 2020, doi: 10.13140/RG.2.2.19321.49764.
* PLL:
  * F. D. Freijedo, J. Doval-Gandoy, O. Lopez, et J. Cabaleiro, « Robust phase locked loops optimized for DSP implementation in power quality applications », in 2008 34th Annual Conference of IEEE Industrial Electronics, Orlando, FL: IEEE, nov. 2008, p. 3052‑3057. doi: 10.1109/IECON.2008.4758447.
 
## Thanks

Thanks to the OwnTech contributors:

* [Luiz](https://github.com/luizvilla)
* [Ayoub](https://github.com/Ayoub-Farah)
* [Guillaume](https://github.com/guigur)
* [Jean](https://github.com/jalinei)
* [Clement](https://github.com/cfoucher-laas)
