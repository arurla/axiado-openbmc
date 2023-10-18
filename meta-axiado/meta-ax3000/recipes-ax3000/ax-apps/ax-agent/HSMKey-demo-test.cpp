/*
 * Copyright (c) 2021-23 Axiado Corporation (or its affiliates).
 * All rights reserved. Use, modification and redistribution of this file
 * is subject to your possession of a valid End User License Agreement (EULA)
 * for the Axiado Product of which these sources are part of and your
 * compliance with all applicable terms and conditions of such
 *  licence agreement.
 */

/**
 * @file: HSMKey-demo-test.cpp
 * This application is to test HSM key generation features from WebUi
 */

#include <sdbusplus/bus.hpp>
#include <boost/asio.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/asio/property.hpp>
#include <sdbusplus/bus/match.hpp>
#include <iostream>
#include <unistd.h>
#include <random>
#include <sstream>
#include <cstdint>
#include <vector>
#include <string>
#include <sdbusplus/message.hpp>
#include <com/axiado/ax200/Demo/server.hpp>
#include <com/axiado/ax200/Demo/client.hpp>
#include <sdbusplus/server.hpp>
#include "ax-bmc-ipc-device.hpp"



constexpr auto service = "com.axiado.ax200.Demo";


constexpr auto objectPath = "/com/axiado/ax200/Demo/Gen_HSMKey";


constexpr auto interface = "com.axiado.ax200.Demo";


constexpr auto defSetInterface = "org.freedesktop.DBus.Properties";


constexpr auto propertyStr = "GenHSM";


constexpr auto propertyInt = "DemoData";


constexpr auto bus_method = "SendDemoData";

std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<> dis(0, 255);

std::uniform_int_distribution<> hsm(1, 10);

const int MAX_ARGUMENTS = 2;
int arg = 0;

namespace rules = sdbusplus::bus::match::rules;
using propertyVariantType = std::variant<std::string, bool, uint8_t>;
using propertyChangesType = std::map<std::string, propertyVariantType>;
using namespace sdbusplus;
auto b = bus::new_default_system();

/**
 * @brief Prints usage of hsmkey-test application with input parameters
 *
 */
void print_help_section(char *argv)
{
    std::cout << "Usage: " << argv << " [argument1] " << std::endl;
    std::cout << "Expected [argument1]" << std::endl;
    std::cout << "  1. delay between req and resp for HSM Keys test" << std::endl;
    std::cout << "  2. Provide unsigned integer input" << std::endl;
    std::cout << "  3. Provided input considered as in milliseconds" << std::endl;
    std::cout << "  4. -h shows help section" << std::endl;
    std::cout << "Note: Default 500 ms delay considered" << std::endl;
    exit(0);
}

bool isNumeric(std::string str) {
    int length = static_cast<int>(str.length());
    for (int i = 0; i < length; i++){
        if (isdigit(str[i]) == false)
            return false;
    }
    return true;
}

void setDemoDataProperty(std::vector<uint8_t>& bytes, int random_hsm)
{
    for (int i = 0; i < random_hsm; i++){
        bytes[i] = static_cast<uint8_t>(dis(gen));
    }

    auto m = b.new_method_call(service,
             objectPath,
             interface, bus_method);
    
    m.append(bytes);
    auto reply=b.call(m);
}

void setDefaultGenHSM()
{
    std::variant<std::string> defaultValue = "off";
    auto setMethod = b.new_method_call(service, objectPath,
                     defSetInterface, "Set");
    setMethod.append(interface, propertyStr, defaultValue);
    b.call(setMethod);
    std::cout << "Property set to defaultvalue: off"<< std::endl;
}

void monitorGenHSMProperty(int arg)
{
    boost::asio::io_context io;
    auto bus = std::make_shared<sdbusplus::asio::connection>(io);
    int delay_millisec = arg;
    sdbusplus::bus::match_t propertyMatch(
            *bus, rules::propertiesChanged(objectPath, interface),
            [delay_millisec](sdbusplus::message::message& msg) {
            std::string interfaceName;
            propertyChangesType changedProperties;
            try
            {
                msg.read(interfaceName, changedProperties);
                for (const auto& [p, v] : changedProperties)
                {
                    if (p == propertyStr)
                    {

                        
                        int random_hsm = hsm(gen);
                        std::vector<uint8_t> bytes(random_hsm);

                        if( std::get<std::string>(v) == "Start")
                        {
                            std::cout<<"Property GenHSM Changes to----> "<<
                            std::get<std::string>(v) << std::endl;

                            
                            auto delay_microsec = delay_millisec * 1000;

                            
                            if( delay_microsec <= 2700000 )
                            {
                                bmc_log(INFO, "Delay in Microseconds: ", delay_microsec);
                                usleep(delay_microsec);
                                
                                srand(time(0));
                                
                                setDemoDataProperty(bytes, random_hsm);
                            }else{
                                bmc_log(INFO, "Provide minimum delay than: ", delay_millisec);
                            }

                            
                            setDefaultGenHSM();
                        }
                    }
                    else if (p == propertyInt)
                    {
                        //PropertyInt change code here.
                        //          std::cout <<"Unknown propertyInt {PROP} "<< std::endl;
                    }
                    else
                    {
                        std::cout <<"Unknown property {PROP} "<< std::endl;
                    }
                }
            }
            catch (const std::exception& ex)
            {
                std::cout <<"Failed to read msg: " << ex.what() << std::endl;
            }
        });
        io.run();
}

int main(int argc, char* argv[])
{

    if ( argc == 1 )
    {
        
        arg = 500;
    }
    else if((argc > MAX_ARGUMENTS) || (argc == 2 && std::string(argv[1]) == "-h")) {
        print_help_section(argv[0]);
        return 0;
    }
    else if ( argc == 2 ){
        if (isNumeric(std::string(argv[1]))){
            arg = std::stoi(argv[1]);
        }
        else{
            print_help_section(argv[0]);
            return 0;
        }
    }

    
    monitorGenHSMProperty(arg);

    return 0;
}
