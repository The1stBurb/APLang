#include "./helper.h"
#include "./code.h"

#include <vector>
#include <iostream>

using namespace std;

int main() {

	//print("hellow world");
	string Data = readFile();

	vector<LINE> lines = readData(Data);

	cout << (string)lines[0] << endl;

	return 0;
}