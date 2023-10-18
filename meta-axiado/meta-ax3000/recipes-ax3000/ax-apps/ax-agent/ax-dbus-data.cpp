/*
 * Copyright (c) 2021-23 Axiado Corporation (or its affiliates).
 * All rights reserved. Use, modification and redistribution of this file
 * is subject to your possession of a valid End User License Agreement (EULA)
 * for the Axiado Product of which these sources are part of and your
 * compliance with all applicable terms and conditions of such
 *  licence agreement.
 */

/**
 * @file: ax-dbus-data.cpp
 * This file defines common paths for Ax Agent and API for calling a send data
 * on dbus path
 */

#include "ax-dbus-data.hpp"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/message.hpp>

#include <iostream>

using namespace std;

bool send_dbus_data(vector<uint8_t>& bytes, const string& path)
{
    using namespace sdbusplus;
    auto bus = bus::new_default_system();

    
    auto m = bus.new_method_call(AX_AGENT_BUSNAME, path.c_str(),
                                 AX_AGENT_INTERFACE, AX_AGENT_SEND_DATA);

    
    m.append(bytes);

    auto reply = bus.call(m);
    if (reply.is_method_error())
    {
        std::cerr << "Method call error: " << reply.get_error() << std::endl;
        return false;
    }
    return true;
}
