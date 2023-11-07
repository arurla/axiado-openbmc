/*
 * Copyright (c) 2021-23 Axiado Corporation (or its affiliates).
 * All rights reserved. Use, modification and redistribution of this file
 * is subject to your possession of a valid End User License Agreement (EULA)
 * for the Axiado Product of which these sources are part of and your
 * compliance with all applicable terms and conditions of such
 *  licence agreement.
 */

/**
 * @file: ax-demo.cpp
 * This file defines Ax Agent class for DBus objects.
 */

#include "config.h"

#include <com/axiado/ax200/Demo/client.hpp>
#include <com/axiado/ax200/Demo/server.hpp>
#include <sdbusplus/server.hpp>

#include <cassert>
#include <iostream>
#include <string_view>

using namespace std;

using DemoInterface =
    sdbusplus::server::object_t<sdbusplus::com::axiado::ax200::server::Demo>;

class AxAgent : public DemoInterface
{
  public:
    /**
     * @brief Construct a new Ax Agent object
     *
     * @param bus dbus object
     * @param path
     */
    AxAgent(sdbusplus::bus_t& bus, const char* path) : DemoInterface(bus, path)
    {}

    /**
     * @brief Send demo data to the property
     *
     * @param data byte vector that will be sent over dbus
     */
    void sendDemoData(std::vector<uint8_t> data) override
    {
        assert(demoData(data) == demoData());
    }
};

int main()
{
    static_assert(std::string_view(
                      sdbusplus::com::axiado::ax200::client::Demo::interface) ==
                  std::string_view(AxAgent::interface));

    auto bus = sdbusplus::bus::new_default();
    sdbusplus::server::manager_t m{bus, AX_AGENT_PATH};

    
    bus.request_name("com.axiado.ax200.Demo");

    
    AxAgent axAgentObject_rwAttack{bus, RW_ATTACK_PATH};
    AxAgent axAgentObject_nwAnom{bus, NW_ANOM_PATH};
    AxAgent axAgentObject_hsmKey{bus, HSM_KEYGEN_PATH};

    
    while (1)
    {
        bus.process_discard(); /* discard any unhandled messages. */
        bus.wait();
    }
    return 0;
}
