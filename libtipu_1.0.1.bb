# Copyright (C) 2016 Dorian Ivancic <dorian.ivancic@gmail.com>
# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "Apalis iMX6 LVDB toggler"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRCREV = "fc2157855009f0ba69d0bd204d89fdb0dea24e67"
SRC_URI = "git://github.com/divancic/libtipu;protocol=http"

S = "${WORKDIR}/git"

INSANE_SKIP_${PN} = "ldflags"

do_configure () {
  sed 's:/usr/local:${D}/usr:' -i Makefile
  sed 's:/lib/libtipu:/lib:' -i Makefile
}

do_compile () {
  make 
}

do_install() {
  make install
}
