# libtipu

## Introduction

This library is used for turning on/off IPU output on LVDS for the IMX6 processor. It has been tested on Toradex IMX6 Apalis.

## Compilation

First make sure you set the proper values for CC and LD variables (see the Makefile header) (I used toolchain which is built by Boot2QT). When you set the proper variables simply do:

```
make CROSS=Y
```

Or you can simply use included bitbake recipe.

## Installation
```
make install
```
Make install will install shared and static libraries into lib dir (/usr/local/lib/libtipu) and testing binary into bin dir (/usr/local/bin).

## Usage

See tipu.c, but basically you have to calls: ipu_on() and ipu_off().
