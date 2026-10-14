#include "./helper.h"
#include "./code.h"

#include <vector>
#include <iostream>

using namespace std;

int main() {

	//print("hellow world");
	string Data = readFile();

	vector<Chunk> lines = readData(Data);
	print(Data);

	for (int i = 0; i < lines.size(); i++) {
		cout << (string)lines[i] << endl;
	}

	return 0;
}