# Android HAL for SMI230 Sensor

## Table of Contents
 - [Introduction](#Intro)
 - [License](#License)
 - [Architecture](#Architecture)
 - [Operation Examples](#examples)

## Introduction <a name=Intro></a>

SMI230 HAL is module running in user space, under framework of Android Hardware Abstraction Layer.
It provides capability to bridge SMI230 sensor driver in kernel and Android APPs.
For more information please refer to [HAL sensors-1.0](https://source.android.com/devices/sensors/hal-interface)

SMI230 HAL supports data synchronization feature of SMI230 sensor only and shall be used with [SMI230 kernel driver](https://github.com/boschmemssolutions/SMI230-Linux-Driver) based on Linux Input Framework.

## License <a name=License></a>
See [LICENSE](smi230/LICENSE) file

## Architecture <a name=Architecture></a>
```
                 Android APPS
                      |
                 Android HAL
                      |
                  SMI230 HAL
                      |
-------------------------------------------------------
                 |          |
               sysfs       dev
                 \          /
               input-subsystem
	              |
                smi230_driver
                      |
-------------------------------------------------------
                  Hardware
```
## Operation Examples <a name=examples></a>

SMI230 HAL is a shared lib, which is dynamically linked to other Android programs.
* For convenient testing, it can be compiled as an executable to directly run on command line.

Integrate this repository into your build setup into location
```
 android/hardware/libhardware/modules/
```
extend your build setup with smi230-hal and compile it.

For more information please refer to [android build system](https://source.android.com/setup/build/building)
