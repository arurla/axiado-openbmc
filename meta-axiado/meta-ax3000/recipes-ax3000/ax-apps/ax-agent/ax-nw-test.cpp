/*
 * Copyright (c) 2021-23 Axiado Corporation (or its affiliates). 
 * All rights reserved. Use, modification and redistribution of this file
 * is subject to your possession of a valid End User License Agreement (EULA)
 * for the Axiado Product of which these sources are part of and your 
 * compliance with all applicable terms and conditions of such
 *  licence agreement.
 */

/**
 * @file: ax-nw-test.cpp
 * This file defines random test data sent to DBus objects for testing Ax Agent.
 */

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <cstring>
#include <unistd.h>

using namespace std;

int main()
{
	vector<string> values{ "49", "48", "46", "49", "48",  "46",	 "48", "46",
						   "57", "0",  "0",	 "0",  "1",	  "0",	 "0",  "0",
						   "0",	 "0",  "0",	 "0",  "49",  "48",	 "46", "49",
						   "48", "46", "48", "46", "49",  "48",	 "0",  "0",
						   "50", "53", "48", "0",  "255", "255", "0",  "0",
						   "0",	 "0",  "0",	 "0",  "0",	  "0",	 "0",  "0",
						   "52", "53", "53", "53" };

	while (1) {
		srand(time(0));
		for (int i = 0; i < 2; i++) {
			vector<string> final_command{};
			vector<string> tempheader{ "2", "1", "82" };
			vector<string> tempcommand{
				"busctl set-property com.axiado.ax200.Demo /com/axiado/ax200/Demo/nwAnom com.axiado.ax200.Demo DemoData ay ",
				"56", "1"
			};
			int number_of_attacks = rand() % 7 + 1;
			int number_of_bytes = (number_of_attacks * 55) + 1;
			cout << number_of_attacks << endl;
			tempcommand[1] = to_string(number_of_bytes) + " ";
			tempcommand[2] = to_string(number_of_attacks) + " ";
			final_command.insert(final_command.end(), tempcommand.begin(),
								 tempcommand.end());
			for (int j = 0; j < number_of_attacks; j++) {
				vector<string> tempvals{};
				tempheader[0] = to_string(j) + " ";
				tempheader[1] = to_string(rand() % 200 + 1) + " ";
				tempheader[2] = to_string(rand() % 127 + 1) + " ";
				final_command.insert(final_command.end(), tempheader.begin(),
									 tempheader.end());
				for (size_t i = 0; i < values.size(); i++) {
					if (stoi(values[i]) != 46 && stoi(values[i]) != 0) {
						tempvals.push_back(
							to_string(rand() % (57 - 48 + 1) + 49) + " ");
					} else {
						tempvals.push_back(values[i] + " ");
					}
				}
				final_command.insert(final_command.end(), tempvals.begin(),
									 tempvals.end());
			}

			string a = std::accumulate(final_command.begin(),
									   final_command.end(), std::string(""));

			int size = a.length();
			char *array = new char[size + 1];
			strcpy(array, a.c_str());
			// cout << array << endl;
			int sys_return = system(array);
			free(array);
			if (sys_return != 0) {
				cout << "Call is wrong" << endl;
			}
		}
		sleep(1);
	}
	return 0;
}
