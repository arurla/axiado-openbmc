/*
 * Copyright (c) 2021-23 Axiado Corporation (or its affiliates). 
 * All rights reserved. Use, modification and redistribution of this file
 * is subject to your possession of a valid End User License Agreement (EULA)
 * for the Axiado Product of which these sources are part of and your 
 * compliance with all applicable terms and conditions of such
 *  licence agreement.
 */

/**
 * @file: ax-demo-test.cpp
 * This file defines random test data sent to DBus objects for testing Ax Agent.
 */

#include <sdbusplus/bus.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>
#include <sstream>
#include <cstdint>
#include <unistd.h>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cstdlib>


#define MAX_ARGUMENTS 6


#define MAX_TEST_DATA 10


#define MIN_TEST_DATA 3


static constexpr auto bus_name = "com.axiado.ax200.Demo";


static constexpr auto interface_name = "com.axiado.ax200.Demo";


static constexpr auto bus_method = "SendDemoData";


bool isRandomValSet = false;

using namespace std;

const unordered_map<string, string> dbus_paths = {
	
	{ "rw", "/com/axiado/ax200/Demo/RWAttack" },
	
	{ "nw", "/com/axiado/ax200/Demo/nwAnom" },
	
	{ "hsm", "/com/axiado/ax200/Demo/Gen_HSMKey" }
};

/**
 * @brief Prints usage of ax-demo-test application with input parameters
 * 
 */
void print_help_section(char *argv)
{
	cout << "Usage: " << argv
		 << " [argument1] [argument2] (Optional: -f | -r param) [argument3]"
		 << endl;
	cout << "Options:" << endl;
	cout << "  -h   Show help section" << endl;
	cout << "  rw   Send test data to Ransomware Attack Dbus object" << endl;
	cout << "  hsm  Send test data to Generate HSM key Dbus object" << endl;
	cout
		<< "  nw   Send test data to Network Anomaly Dbus object. Default number of types of data is "
		<< MIN_TEST_DATA << endl;
	cout
		<< "  -f   Flag to generate fixed number of types of data for Network anomaly"
		<< endl;
	cout
		<< "  -r   Flag to generate random number of types of bytes for Network anomaly"
		<< endl;
	cout
		<< "  param Number of types of data for Network Anomaly and it must be passed with -f or -r flag. Range: "
		<< MIN_TEST_DATA << " - " << MAX_TEST_DATA << endl;
	cout
		<< "Example: " << argv
		<< "rw nw -f 6 -- Generates data for Ransomware attack and network anomaly with fixed 6 types of data"
		<< endl;
	cout
		<< "Example: " << argv
		<< "nw -r 9 hsm  -- Generates data for key generation and network anomaly with random number of types each time"
		<< endl;
	exit(0);
}

/**
 * @brief Checks if the custom data input is within the accepted range
 * 
 * @param custom_input Input given by user
 * @param argv input parameters from command line
 */
void input_range_check(int custom_input, char *argv[])
{
	if ((custom_input > MAX_TEST_DATA) || (custom_input < MIN_TEST_DATA)) {
		cout << "Random data range cannot be less than " << MIN_TEST_DATA
			 << " or greater than " << MAX_TEST_DATA << endl;
		print_help_section(argv[0]);
	}
}

/**
 * @brief Parse the custom arguments for Network Anomaly 
 * 
 * @param custom_nw_anom_data Number of bytes generated for network anomaly
 * @param argv input parameters from command line
 * @param i Position of nw argument
 * @return int new position for parsing
 */
int custom_data(int *custom_nw_anom_data, char *argv[], int i)
{
	if ((string(argv[i + 1]) == "-f") || (string(argv[i + 1]) == "-r")) {
		try {
			*custom_nw_anom_data = stoi(argv[i + 2]);
			input_range_check(*custom_nw_anom_data, argv);
			if (string(argv[i + 1]) == "-r") {
				isRandomValSet = true;
				cout
					<< "Using random number of types of data for Network Anomaly test data with range 4 - "
					<< *custom_nw_anom_data << endl;
			} else {
				cout
					<< "Using fixed number of types of data for Network Anomaly test data: "
					<< *custom_nw_anom_data << endl;
			}
			return i + 2;
		} catch (const exception &e) {
			cout << "Invalid argument: " << argv[i + 2]
				 << " Expected an integer" << endl;
			print_help_section(argv[0]);
		}
	}
	return i;
}

/**
 * @brief Get the input paths object
 * 
 * @param num_args Number of arguments passed 
 * @param input_paths Unordered set 
 * @param argv  input parameters from command line
 */
void get_input_paths(int num_args, unordered_set<string> &input_paths,
					 int *custom_nw_anom_data, char *argv[])
{
	int i;
	string input_dbus_path;
	for (i = 1; i < num_args; i++) {
		try {
			input_dbus_path = dbus_paths.at(string(argv[i]));
		} catch (const out_of_range &e) {
			
			cout << "Invalid argument: " << argv[i] << endl;
			print_help_section(argv[0]);
		}
		
		if (input_paths.find(input_dbus_path) != input_paths.end()) {
			cout << "Duplicate argument: " << argv[i] << endl;
			print_help_section(argv[0]);
		}
		input_paths.insert(input_dbus_path);

		if (string(argv[i]) == "nw" && (i + 2 < num_args)) {
			
			i = custom_data(custom_nw_anom_data, argv, i);
		}
	}
}

int main(int argc, char *argv[])
{
	unordered_set<string> input_paths;
	int custom_nw_anom_data = MIN_TEST_DATA;

	if ((argc == 1 || argc > MAX_ARGUMENTS) || string(argv[1]) == "-h") {
		print_help_section(argv[0]);
	}

	get_input_paths(argc, input_paths, &custom_nw_anom_data, argv);

	
	random_device rd;
	mt19937 gen(rd());
	custom_nw_anom_data++; /* Another type for prediction */
	uniform_int_distribution<> dis(0, 100);
	uniform_int_distribution<> dis2(MIN_TEST_DATA + 1, 5);
	if (isRandomValSet) {
		dis2 =
			uniform_int_distribution<>(MIN_TEST_DATA + 1, custom_nw_anom_data);
	}

	int counter_rw = 0;
	int counter_nw = 0;

	using namespace sdbusplus;
	auto b = bus::new_default_system();

	while (1) {
		srand(time(0));
		for (const auto &path : input_paths) {
			int i = 0;
			int num_bytes = (path == dbus_paths.at("rw")) ? 3 : 4;
			if (path == dbus_paths.at("nw")) {
				num_bytes = (isRandomValSet) ? dis2(gen) : custom_nw_anom_data;
			}

			std::vector<uint8_t> bytes(num_bytes);

			
			for (i = 0; i < (num_bytes - 1); i++) {
				bytes[i] = static_cast<uint8_t>(dis(gen));
			}

			if (path == dbus_paths.at("rw")) {
				bytes[i] = (counter_rw % 40) / 20;
				counter_rw++;
			} else if (path == dbus_paths.at("nw")) {
				bytes[i] = (counter_nw % 40) / 20;
				counter_nw++;
			} else {
				bytes[i] = static_cast<uint8_t>(dis(gen));
			}

			
			auto m = b.new_method_call(bus_name, path.c_str(), interface_name,
									   bus_method);

			
			m.append(bytes);
			
			auto reply = b.call(m);
			
		}
		sleep(1);
	}
	return 0;
}
