/*
 * Copyright (c) 2021-23 Axiado Corporation (or its affiliates).
 * All rights reserved. Use, modification and redistribution
 * of this file is subject to your possession of a valid
 * End User License Agreement (EULA) for the Axiado Product
 * of which these sources are part of and your compliance with
 * all applicable terms and conditions of such
 * licence agreement.
 */

/**
 * @file: ax-dbus-data.hpp
 * This file defines common paths for Ax Agent
 * and API for calling a send data on dbus paths.
 */
#include "config.h"

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Send data to DBus path via common DBus method
 * shared by all DBus object in Ax-agent: SendDemoData
 *
 * @param bytes Data to be sent in Byte vector format
 * @param path DBus object path
 * @return true Data sent to DBus path
 * @return false Error in sending data
 */
bool send_dbus_data(std::vector<uint8_t>& bytes, const std::string& path);
