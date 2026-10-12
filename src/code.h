#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "./helper.h"

using namespace std;

string readFile() {
	string filePath = "C:\\Users\\ECoop\\Desktop\\SCHOOL 2026\\cs2420-f26\\APLang\\src\\code.txt";
	ifstream file(filePath);

	if (!file.is_open()) {
		cerr << "Error: Could not open the file!" << endl;
		return "";
	}

	stringstream buffer;
	buffer << file.rdbuf(); // Read the entire file buffer
	string file_contents = buffer.str();

	return file_contents;
}

class LINE {
public:
	vector<string>
};

vector<LINE> readData() {}