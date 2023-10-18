/*
 * Copyright (c) 2021-23 Axiado Corporation (or its affiliates). 
 * All rights reserved. Use, modification and redistribution of this file
 * is subject to your possession of a valid End User License Agreement (EULA)
 * for the Axiado Product of which these sources are part of and your 
 * compliance with all applicable terms and conditions of such
 *  licence agreement.
 */

#include "ax-bmc-ipc-device.hpp"
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern IPCDevice *ax_get_dev_obj(uint32_t);


static constexpr auto bmcAgentBusName = "com.axiado.ax200.Demo";
static constexpr auto bmcAgentInterfaceName = "com.axiado.ax200.Demo";
static constexpr auto bmcAgentDbusPath = "/com/axiado/ax200/Demo";
static constexpr auto bmcAgentDbusMethod = "SendDemoData";

static std::unordered_map<uint32_t, string> endpt_dbus_obj_map = {
	{ AX_EP_BMC_HSM_KEYGEN_M0, "/Gen_HSMKey" },
	{ AX_EP_BMC_RWATTACK_ML0, "/RWAttack" },
	{ AX_EP_BMC_NW_ANOMALY_ML1, "/nwAnom" }
};

int sendDataToDbus(ipc_pkt_ptr_t pkt_data)
{
	uint8_t *bytes_array = (uint8_t *)(&(pkt_data.data));
	const size_t num_bytes = static_cast<size_t>(pkt_data.length);
	uint32_t endpt = pkt_data.endpoint_id;
	if (!bytes_array || !num_bytes) {
		bmc_log(DEBUG, "Msg is null! bytes_arr: ", bytes_array,
				" num_bytes: ", num_bytes);
		return -1;
	}

	bmc_log(DEBUG, "bytes_arr value: ", *bytes_array,
			" num_bytes: ", pkt_data.length);

	std::vector<uint8_t> bytes_vector(bytes_array, bytes_array + num_bytes);

	bmc_log(DEBUG, "Size of packet: ", bytes_vector.size());

	if (LOG_LEVEL == DEBUG) {
		
		for (const auto &byte : bytes_vector) {
			cout << hex << static_cast<int>(byte) << " ";
		}
		cout << " " << endl;
	}

	using namespace sdbusplus;
	auto b = bus::new_default_system();
	auto m = b.new_method_call(
		bmcAgentBusName, (bmcAgentDbusPath + endpt_dbus_obj_map[endpt]).c_str(),
		bmcAgentInterfaceName, bmcAgentDbusMethod);

	
	m.append(bytes_vector);
	auto reply = b.call(m);

	return 0;
}

void sig_event_handler(int received_sig, siginfo_t *info, void *unused)
{
	bmc_log(DEBUG, "Unused: ", unused);
	bmc_log(DEBUG, "Unused: ", info->si_code);
	bmc_log(DEBUG, "Signal_val: ", received_sig);
	

	ipc_pkt_ptr_t pkt;

	
	IPCDevice *dev_obj = ax_get_dev_obj(received_sig);
	int dev_descriptor = dev_obj->getFd();
	bmc_log(INFO, "Dev descriptor: ", dev_descriptor);

	
	if (ioctl(dev_descriptor, AX_IPC_IOC_RECV, &pkt) < 0) {
		bmc_log(ERROR, "Failed to receive TLV for fd: ", dev_descriptor);
		return;
	}

	if (!(pkt.length)) {
		bmc_log(DEBUG, "Msg is null!", pkt.length);
		return;
	}
	sendDataToDbus(pkt);
}

IPCDevice::IPCDevice(string device, uint32_t endpoint)
	: dev_name(device), ax_ep(endpoint)
{
	registerDevice();
}

IPCDevice::~IPCDevice()
{
	close(fd);
}

string IPCDevice::getDevname() const
{
	return dev_name;
}

int IPCDevice::getFd() const
{
	return fd;
}

void IPCDevice::registerDevice()
{
	struct sigaction ax_sig_action;
	string dev = "/dev/axdev" + dev_name;
	fd = open(dev.c_str(), O_RDWR);
	if (fd < 0) {
		bmc_log(ERROR, "Failed to open ", dev_name, " device file");
		return;
	}
	bmc_log(INFO, "Successfully opened ", dev_name, " device file");

	sigemptyset(&ax_sig_action.sa_mask);
	ax_sig_action.sa_flags = (SA_SIGINFO | SA_RESTART);
	ax_sig_action.sa_sigaction = sig_event_handler;

	bmc_log(DEBUG, "Signal flags ", ax_sig_action.sa_flags, " created");
	bmc_log(DEBUG, "Signal action ", ax_sig_action.sa_sigaction, " created");
	bmc_log(INFO, "FD: ", fd, " for device ", dev);

	
	int err = sigaction((int)ax_ep, &ax_sig_action, NULL);
	if (err) {
		bmc_log(ERROR, "Signal registration failed! err: ", err);
		close(fd);
		return;
	}

	if (ioctl(fd, AX_IPC_IOC_NOTIFY, &ax_ep)) {
		bmc_log(ERROR, "Notify ioctl failed! fd: ", fd);
		close(fd);
		return;
	}
	return;
}


int IPCDevice::sendData()
{
	
	ipc_pkt_ptr_t pkt;
	
	pkt.endpoint_id = 0xf0f0f0f0;
	pkt.event = 0xfefefefe;
	pkt.length = 0x4;
	pkt.data = 0xdeadbeef;

	if (ioctl(fd, AX_IPC_IOC_SEND, &pkt) < 0) {
		bmc_log(ERROR, "Failed to send TLV! fd: ", fd);
		return -1;
	}
	return 0;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
