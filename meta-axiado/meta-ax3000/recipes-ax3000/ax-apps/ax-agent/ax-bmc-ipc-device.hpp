/*
 * Copyright (c) 2021-23 Axiado Corporation (or its affiliates). 
 * All rights reserved. Use, modification and redistribution of this file
 * is subject to your possession of a valid End User License Agreement (EULA)
 * for the Axiado Product of which these sources are part of and your 
 * compliance with all applicable terms and conditions of such
 *  licence agreement.
 */

/**
 * @file: ax-bmc-ipc-device.hpp
 * Header file that defines IPC device class.
 */

#ifndef _AX_BMC_IPC_DEVICE_HPP
#define _AX_BMC_IPC_DEVICE_HPP

#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <sdbusplus/bus.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include "ax-ipc-endpoints.hpp"

using namespace std;

#define AX_IPC_IOC_MAGIC  0x66
#define AX_IPC_IOC_SEND	  _IOW(AX_IPC_IOC_MAGIC, 0, ipc_pkt_ptr_t)
#define AX_IPC_IOC_RECV	  _IOR(AX_IPC_IOC_MAGIC, 1, ipc_pkt_ptr_t)
#define AX_IPC_IOC_NOTIFY _IOW(AX_IPC_IOC_MAGIC, 2, int)

/* Define the logging levels */
enum LogLevel { ERROR, WARNING, INFO, DEBUG };
const LogLevel LOG_LEVEL = DEBUG;

/* Define the logging function */
template <typename... Args>
void bmc_log(LogLevel level, const std::string &message, Args &&... args)
{
	/* Check if the logging level is high enough to output the log message */
	if (level <= LOG_LEVEL) {
		/* Format the log message with the timestamp, level, and variables */
		std::ostringstream formatted_message_stream;
		switch (level) {
		case ERROR:
			formatted_message_stream << "[ERROR] ";
			break;
		case WARNING:
			formatted_message_stream << "[WARNING] ";
			break;
		case INFO:
			formatted_message_stream << "[INFO] ";
			break;
		case DEBUG:
			formatted_message_stream << "[DEBUG] ";
			break;
		}
		formatted_message_stream << message;
		(formatted_message_stream << ... << args);

		/* Output the log message to the console */
		std::cout << formatted_message_stream.str() << std::endl;
	}
}

/**
 * @brief Function pointer for signal handler. 
 * This is defined by signal header file.
 * 
 */
typedef void (*ax_signal_handler_t)(int, siginfo_t *, void *);

/**
 * @brief Represents a TLV structure that is packed and aligned to 8 bytes.
 */
typedef struct ipc_pkt_ptr_s {
	volatile uint32_t endpoint_id; /**< The endpoint ID for the TLV */
	volatile uint32_t event; /**< The event for the TLV */
	volatile uint32_t length; /**< The length of the TLV data */
	volatile uint32_t rsvd; /**< The rsvd word for alignment */
	volatile uint64_t data; /**< The data for the TLV */
} ipc_pkt_ptr_t __attribute__((aligned(8)));

/**
 * @brief Class for IPC devices. 
 * Each object represents a device representation for an application.
 * 
 */
class IPCDevice {
private:
	string dev_name; /** Device name for the object. 
					  * This name will be used to open the object. 
				      * Passed in the constructor by the caller */
	uint32_t ax_ep; /** Endpoint to be registered. 
		              * Passed in the constructor by the caller */
	int fd = 0; /** fd returned after opening the device */

	/**
	 * @brief Open the device and save the file descriptor. 
	 * Register the signal handler with the signal name attribute.
	 * Signal handler is common for all devices. 
	 * Call ioctl to specify notify signal for current endpoint.
	 * 
	 */
	void registerDevice();

public:
	IPCDevice() = delete;

	/**
	 * @brief Destroy the IPCDevice object. Close the device
	 * 
	 */
	virtual ~IPCDevice();

	/**
	 * @brief Construct a new IPCDevice object
	 * 
	 * @param device Device name for the object. 
	 * This name will be used to open the object
	 * @param endpoint Endpoint to be registered
	 */
	IPCDevice(string device, uint32_t endpoint);

	/**
	 * @brief Get the dev_name attribute for the device object calling this method
	 * 
	 * @return string 
	 * @retval Device name passed to the constructor
	 */
	string getDevname() const;

	/**
	 * @brief Get the Fd value
	 * 
	 * @return int 
	 * @retval File descriptor for the device object calling this method
	 */
	int getFd() const;

	/**
	 * @brief Send the data to the device
	 * 
	 * @return int
	 * @retval 0 successfully sent
	 * @retval -1 Failed to send
	 */
	int sendData();

	/**
	 * @brief It is called by the signal handler to 
	 * send data to the relevant dbus object
	 * 
	 * @param pkt_data Data received through IPC
	 * @return int 
	 * @retval 0 Success 
	 */
	int sendDataToDbus(ipc_pkt_ptr_t pkt_data);
};

#endif /**end of _AX_BMC_IPC_DEVICE_HPP */