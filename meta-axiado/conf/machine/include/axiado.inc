# Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.
# Use, modification and redistribution of this file is subject to your possession
# of a valid End User License Agreement (EULA) for the Axiado Product of which
# these sources are part of and your compliance with all applicable terms and
# conditions of such licence agreement.

SOC_FAMILY = "axiado-ax3000"
include conf/machine/include/soc-family.inc
include conf/machine/include/arm/arch-arm64.inc

PREFERRED_PROVIDER_virtual/kernel ?= "linux-axiado"
PREFERRED_PROVIDER_virtual/bootloader ?= "u-boot-axiado"
PREFERRED_PROVIDER_u-boot ?= "u-boot-axiado"

KERNEL_IMAGETYPE ?= "Image"
INITRAMFS_IMAGE = "obmc-phosphor-initramfs"
INITRAMFS_FSTYPES = "cpio.xz"
IMAGE_FSTYPES = "${INITRAMFS_FSTYPES} wic.xz mmc-ext4-tar"
INITRAMFS_IMAGE_BUNDLE = "1"
IMAGE_CLASSES:append = " image_types_ax3000"

DEFAULTTUNE:ax3000 ?= "aarch64"
 
WKS_FILE ?= "emmc-axiado.wks.in"
WKS_FILE_DEPENDS ?= "virtual/kernel"
WKS_FILE_DEPENDS += "e2fsprogs-native"

FLASH_KERNEL_IMAGE ?= "Image"
