/*
 * Copyright (c) 2021-23 Axiado Corporation (or its affiliates). 
 * All rights reserved. Use, modification and redistribution of this file
 * is subject to your possession of a valid End User License Agreement (EULA)
 * for the Axiado Product of which these sources are part of and your 
 * compliance with all applicable terms and conditions of such
 *  licence agreement.
 */

/**
 * @file: ax-ipc-endpoints.hpp
 * Header file that defines all the supported IPC endpoints.
 */

#ifndef _AX_IPC_ENDPOINTS_HPP
#define _AX_IPC_ENDPOINTS_HPP

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
	/**
	 *  @attention
	 *  Keep this value starting at 0x0, CPU ID is used as Table / Array Index in certain parts of the code  
	 **/
	M0_CPU_ID = 0x0ul, /** < CPU ID for M0  */
	A53_CPU_ID, /** < CPU ID for A53 */
	FIREWALL_M55_CPU_ID, /** < CPU ID for firewall M55 */
	/**
	 * @note
	 * All M55 should be consecutive to hold the check in ax_resume_cpu api 
	 * If there is any change in CPU IDs for ML islands, then reflect those changes 
	 * in CPU_ID_TO_ML_ISLAND macro defined in this file.
	 */
	ML_M55_0_CPU_ID, /**< CPU ID for ML0 M55 */
	ML_M55_1_CPU_ID, /**< CPU ID for ML1 M55 */
	ML_M55_2_CPU_ID, /**< CPU ID for ML2 M55 */
	ML_M55_3_CPU_ID, /**< CPU ID for ML3 M55 */
	RT660_CPU_ID, /**< CPU ID for RT660 core */

	/**
	 * @note
	 * Append new CPU IDs before MAX_CPU_ID.
	 */
	MAX_CPU_ID, /** < Max number of cpus */
} CPU_ID;

/**
 * @brief Axiado endpoints list
 * 
 * These endpoints can be used across the CPUs as per the need. Endpoints are the services/threads
 * that is usually having the logic to process the messages.
 *  
 * \section format Endpoint Format
 * 
 * Each endpoint is a 32bit unsigned integer number. Each endpoint number is represented by 
 * a 8bit CPU code and a 24bit identification for the specific endpoint.
 * 
 * 	+--------+------------------------+
 *	| 8 bit	 |      24 bit            |
 * 	| CPU    |      Endpoint ID       |
 * 	+--------+------------------------+
 * Developers are generally free to define the endpoint IDs. Allocation of the CPU blocks is 
 * centralized to avoid conflicting endpoints.
 * 
 * Feel free to add your endpoint as required if there is the endpoint to be registered to 
 * the CPU.
 * 
 * \section how_to How to define the Endpoint identification?
 * 1. The endpoint should have a pre-fix 'AX_EP'
 * 2. Each endpoint needs to be added in the respective CPU block and It should be followed 
 *    by the CPU identifier. i.e. AX_EP_SYSMGR or AX_EP_ML0. We have following possible CPU list
 * 	2a. SYSMGR
 *	2b. ML0
 * 	2c. ML1
 * 	2d. ML2
 * 	2e. ML3
 * 	2f. FWL
 * 	2g. A53
 * 3. It should be followed by the endpoint identification macro,
 * 	i.e. 
 *	AX_EP_SYSMGR_A53_PROXY - Endpoint is A53_Proxy running on SYSMGR CPU
 * 	AX_EP_ML0_SYSMGR_PROXY - Endpoint is SYSMGR_proxy running on ML0 CPU
 *
 * 4. Also, add comments for each of the error code as to when can it be used.
 */

/**
 * @brief Generate a CPU specific endpoint identification
 * 
 * For each of the CPU, CPU_IDs are already defined in the 
 * ax_hal/include/ax200/cpu/ax_hal_cpu_id.h
 *
 * @param cpu CPU identifier (8-bit) 
 * @param code The identifier for the endpoint (24-bit)
 */
#define AX_DEFINE_ENDPOINT(cpu, code)                                          \
	(((cpu << 24) & 0xFF000000) | (code & 0xFFFFFF))

/**
 * @brief Get the endpoint id index
 */
#define AX_GET_EPID_INDEX(endpoint) ((endpoint)&0xFFFFFF)

/******************************************************/
/**
 * @brief System manager endpoint IDs
 *
 * This section describes all the system manager endpoints which is to be serviced on 
 * system manager.
 */

/** ML0 Proxy on System manager */
#define AX_EP_SYSMGR_ML0_PROXY AX_DEFINE_ENDPOINT(M0_CPU_ID, 0x00)

/** ML1 Proxy on System manager */
#define AX_EP_SYSMGR_ML1_PROXY AX_DEFINE_ENDPOINT(M0_CPU_ID, 0x01)

/** ML2 Proxy on System manager */
#define AX_EP_SYSMGR_ML2_PROXY AX_DEFINE_ENDPOINT(M0_CPU_ID, 0x02)

/** ML3 Proxy on System manager */
#define AX_EP_SYSMGR_ML3_PROXY AX_DEFINE_ENDPOINT(M0_CPU_ID, 0x03)

/** Firewall Proxy on System manager */
#define AX_EP_SYSMGR_FWL_PROXY AX_DEFINE_ENDPOINT(M0_CPU_ID, 0x04)

/** A53 Proxy on System manager */
#define AX_EP_SYSMGR_A53_PROXY AX_DEFINE_ENDPOINT(M0_CPU_ID, 0x05)

/** Sysmc on System manager */
#define AX_EP_SYSMGR_SYSMC AX_DEFINE_ENDPOINT(M0_CPU_ID, 0x06)

/** CSP on System manager */
#define AX_EP_SYSMGR_CSP AX_DEFINE_ENDPOINT(M0_CPU_ID, 0x07)

/******************************************************/
/**
 * @brief ML0 endpoint IDs
 *
 * This section describes all the ML0 endpoints information
 */

/** System manager proxy on ML0 */
#define AX_EP_ML0_SYSMGR_PROXY AX_DEFINE_ENDPOINT(ML0_CPU_ID, 0x00)

/******************************************************/
/**
 * @brief ML1 endpoint IDs
 *
 * This section describes all the ML1 endpoints information
 */
/** System manager proxy on ML1 */
#define AX_EP_ML1_SYSMGR_PROXY AX_DEFINE_ENDPOINT(ML1_CPU_ID, 0x00)

/******************************************************/
/**
 * @brief ML2 endpoint IDs
 *
 * This section describes all the ML2 endpoints information
 */
/** System manager proxy on ML2 */
#define AX_EP_ML2_SYSMGR_PROXY AX_DEFINE_ENDPOINT(ML2_CPU_ID, 0x00)

/******************************************************/
/**
 * @brief ML3 endpoint IDs
 *
 * This section describes all the ML3 endpoints information
 */
/** System manager proxy on ML3 */
#define AX_EP_ML3_SYSMGR_PROXY AX_DEFINE_ENDPOINT(ML3_CPU_ID, 0x00)

/******************************************************/
/**
 * @brief Firewall endpoint IDs
 *
 * This section describes all the Firewall endpoints information
 */
/** System manager proxy on Firewall */
#define AX_EP_FWL_SYSMGR_PROXY AX_DEFINE_ENDPOINT(FWL_CPU_ID, 0x00)

/******************************************************/
/**
 * @brief A53 endpoint IDs
 *
 * This section describes all the A53 endpoints information
 */
/** System manager proxy on A53 */
#define AX_EP_A53_SYSMGR_PROXY AX_DEFINE_ENDPOINT(A53_CPU_ID, 0x00)

/** BMC application HSM Key generation on A53 */
#define AX_EP_BMC_HSM_KEYGEN_M0 AX_DEFINE_ENDPOINT(A53_CPU_ID, 0x01)

/** BMC application RansomWare Attack on A53 */
#define AX_EP_BMC_RWATTACK_ML0 AX_DEFINE_ENDPOINT(A53_CPU_ID, 0x02)

/** BMC application Network Anomaly Detection on A53 */
#define AX_EP_BMC_NW_ANOMALY_ML1 AX_DEFINE_ENDPOINT(A53_CPU_ID, 0x03)

#ifdef __cplusplus
}
#endif /** __cplusplus */
#endif /**end of _AX_IPC_ENDPOINTS_HPP */
