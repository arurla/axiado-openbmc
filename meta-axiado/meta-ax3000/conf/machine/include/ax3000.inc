# Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.
# Use, modification and redistribution of this file is subject to your possession
# of a valid End User License Agreement (EULA) for the Axiado Product of which
# these sources are part of and your compliance with all applicable terms and
# conditions of such licence agreement.

MACHINE_FEATURES += "\
        obmc-bmc-state-mgmt \
        obmc-chassis-state-mgmt \
        obmc-host-ipmi \
        obmc-host-state-mgmt \
        obmc-phosphor-chassis-mgmt \
        obmc-phosphor-fan-mgmt \
        obmc-phosphor-flash-mgmt \
        "

VIRTUAL-RUNTIME_skeleton_workbook = "${MACHINE}-config"

PREFERRED_PROVIDER_virtual/obmc-chassis-mgmt = "packagegroup-ax3000-apps"
PREFERRED_PROVIDER_virtual/obmc-fan-mgmt = "packagegroup-ax3000-apps"
PREFERRED_PROVIDER_virtual/obmc-flash-mgmt = "packagegroup-ax3000-apps"
PREFERRED_PROVIDER_virtual/obmc-inventory-data ?= "${VIRTUAL-RUNTIME_skeleton_workbook}"
PREFERRED_PROVIDER_virtual/obmc-system-mgmt = "packagegroup-ax3000-apps"

VIRTUAL-RUNTIME_obmc-host-state-manager ?= "x86-power-control"
VIRTUAL-RUNTIME_obmc-chassis-state-manager ?= "x86-power-control"

PREFERRED_PROVIDER_virtual/obmc-host-ipmi-hw ?= "phosphor-ipmi-kcs"

IMAGE_FEATURES:remove = "obmc-settings-mgmt \
			 obmc-inventory \
			 obmc-leds \
			 obmc-net-ipmi \
			 obmc-network-mgmt \
			 obmc-fru-ipmi \
			 obmc-fan-control \
			 obmc-fan-mgmt \
			 obmc-host-state-mgmt \
			 obmc-chassis-mgmt \
			 obmc-chassis-state-mgmt \
			 "
IMAGE_FEATURES:append = " allow-root-login"

# We don't need any legacy sysv rc compatability
PACKAGECONFIG:remove = "sysvinit"

# We don't enable kernel modules
PACKAGECONFIG:remove = "kmod"
