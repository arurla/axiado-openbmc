/*
Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.

Use, modification and redistribution of this file is subject to your possession
of a valid End User License Agreement (EULA) for the Axiado Product of which
these sources are part of and your compliance with all applicable terms and
conditions of such licence agreement.
*/

#ifndef _AX_OPENBMC_VERSION_H
#define _AX_OPENBMC_VERSION_H

#define OPENBMC_VERSION_MAJOR 0
#define OPENBMC_VERSION_MINOR 6
#define OPENBMC_VERSION_PATCH 0
#define OPENBMC_VERSION_BUILD 3

#define OPENBMC_VERSION                                                                                        \
	((OPENBMC_VERSION_MAJOR << 24) | (OPENBMC_VERSION_MINOR << 16) | (OPENBMC_VERSION_PATCH << 8) |                \
	 (OPENBMC_VERSION_BUILD))

uint32_t get_OPENBMC_version();

#endif /* _AX_OPENBMC_VERSION_H */
