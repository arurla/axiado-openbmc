/*
 * Copyright (c) 2021-23 Axiado Corporation (or its affiliates). 
 * All rights reserved. Use, modification and redistribution of this file
 * is subject to your possession of a valid End User License Agreement (EULA)
 * for the Axiado Product of which these sources are part of and your 
 * compliance with all applicable terms and conditions of such
 *  licence agreement.
 */

/**
 * @file: ax-bmc-ipc-main.cpp
 * This file creates all the IPC device objects for ax agent.
 */

#include "ax-bmc-ipc-device.hpp"
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Used to keep track of every device object 
 * and parameters passed in the constructor.
 * 
 */
typedef struct ipc_dev_info_s {
	string dev_name; /** Device name used for opening the device */
	IPCDevice *dev_obj; /** Store the pointer to device object after creating */
	uint32_t ax_ep; /** Signal ID for the device object */
} ipc_dev_info_t;


vector<ipc_dev_info_t> ipc_devs = {
	{ "M0", NULL, AX_GET_EPID_INDEX(AX_EP_BMC_HSM_KEYGEN_M0) },
	{ "ML0", NULL, AX_GET_EPID_INDEX(AX_EP_BMC_RWATTACK_ML0) },
	{ "ML1", NULL, AX_GET_EPID_INDEX(AX_EP_BMC_NW_ANOMALY_ML1) }
};

/**
 * @brief Compare endpoint to received signal
 * 
 * @param dev IPC device
 * @param received_sig Signal that triggered the handler
 * @return true : Endpoint matched
 * @return false : Endpoint not matched
 */
bool compareEndpoint(const ipc_dev_info_s &dev, const uint32_t received_sig)
{
	return dev.ax_ep == received_sig;
}

/**
 * @brief Get the active object using the signal registered for the device
 * 
 * @param received_sig signal that triggered the handler
 * @return IPCDevice* IPC device active object
 */
IPCDevice *ax_get_dev_obj(uint32_t received_sig)
{
	auto it = std::find_if(ipc_devs.begin(), ipc_devs.end(),
						   [&](const ipc_dev_info_s &dev) {
							   return compareEndpoint(dev, received_sig);
						   });

	if (it != ipc_devs.end()) {
		return it->dev_obj;
	}
	bmc_log(ERROR, "Dev Object not found! Received signal: ", received_sig);
	return NULL;
}

int main()
{
	
	for (auto &j : ipc_devs) {
		j.dev_obj = new IPCDevice(j.dev_name, j.ax_ep);
		bmc_log(INFO, "IPC Device ", j.dev_name, " created with endpoint ",
				j.ax_ep, " at ", j.dev_obj);
	}

	while (1) { /* TODO: Replace this with cpp func */
		sleep(1);
	}
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
