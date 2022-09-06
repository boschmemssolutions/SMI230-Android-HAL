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
## Usage

SMI230 HAL is a shared lib, compatable with android sensor 1.0 specification.
There are two ways to compile/build the lib.

### build with Android SDK

This way, the HAL lib is dynamically linked to other Android libs in its SDK.
For convenient testing, it can also be compiled as an executable to directly run on command line.

Integrate this repository into your build setup into location
```
 android/hardware/libhardware/modules/
```
extend your build setup with smi230-hal and compile it.

For more information please refer to [android build system](https://source.android.com/setup/build/building)

### build independantly without Android SDK

The essecial header files from Android have already been placed in the repo and adapted for compiling the lib. The local libs instead of Android SDK libs would be used to link to the HAL lib in the build process.
Go to the folder where Makefile is located, and first clean the folder by issuing command:
```
make clean
```
Then simply issue command:
```
make
```
the Hal lib will be built in seconds.
Note: the Makefile is written for cross compiling on the host and with toolchain specified. If a different environment is used, a little change is needed to fit into new conditions.
