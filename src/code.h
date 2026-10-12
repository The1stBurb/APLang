#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <format>

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

class ELM {
public:
	string type;
	string data;

	ELM(string Type, string Data) : type(Type), data(Data) {}

	void append(string Data) {
		data += Data;
	}
	bool append(string Data, string Type) {
		bool res = Type == type;
		if (res) {
			append(Data);
		}
		return res;
	}

	bool append(char Data, string Type) {
		return append(string(1,Data), Type);
	}

	operator string() const {
		return format("({}: {})", type, to_raw(data));
	}
};

class LINE {
public:
	vector<ELM> data;

	LINE() : data({}) {}

	size_t size() {
		return data.size();
	}
	void append(ELM elm) {
		data.push_back(elm);
	}
	void append(string Data, string Type) {
		data.push_back(ELM(Type, Data));
	}
	void append(char Data, string Type) {
		data.push_back(ELM(Type, string(1, Data)));
	}

	ELM& top() {
		return data[size() - 1];
	}

	operator string() const {
		string s = "[";
		for (int i = 0; i < data.size(); i++) {
			s += (string)data[i] + ", ";
		}
		return s + "]";
	}
};

string charType(char ch) {
	if (std::isdigit(static_cast<unsigned char>(ch))) return "num";

	else if (std::isalnum(static_cast<unsigned char>(ch))) return "alpha";

	else if (ch == '(') return "lparen";
	else if (ch == ')') return "rparen";

	else if (ch == '{') return "lbrace";
	else if (ch == '}') return "rbrace";

	else if (ch == '[') return "lbrack";
	else if (ch == ']') return "rbrack";

	else if (ch == '"') return "quote";
	else if (ch == '\'')return "squote";
	else if (ch == '=') return "eq";
	else if (ch == '+') return "plus";
	else if (ch == '-') return "minus";
	else if (ch == '/') return "fslash";
	else if (ch == '*') return "star";

	else if (ch == ',')return "comma";

	else if (ch == '\n')return "newline";
	else if (ch == ' ')return "space";

	return "none";
}


class CHUNK {
public:
	char endChar;
	int& pos;
	string data;
	char& tok;
	LINE line;
	bool stop = false;

	CHUNK(string Data, int& Pos, char EndChar) : data(Data), pos(Pos), endChar(EndChar), tok(data[pos]) {}

	void move() { pos++; }
	char currentToken() { tok = (data[pos]); return tok; }

	void mainStep() {
		currentToken();

		string type = charType(tok);
		print(type, tok, pos);

		if (tok == endChar) {
			stop = !false;
			return;
		}

		if (line.size() == 0) {
			line.append(tok, type);
		}
		else {
			ELM& elm = line.top();
			if (!elm.append(tok, type)) { line.append(tok, type); }
		}

		move();

	}
	void main() {
		while (!stop && pos < data.size()) {
			mainStep();
		}
		pos++;
	}

	operator string() const {
		return "Chunk: "+(string)line;
	}
};

vector<LINE> readData(const string& data) {
	int pos = 0;
	vector<LINE> lines;
	CHUNK chunk(data, pos, ';');
	while (pos < data.size()) {
		chunk.main();
	}
	lines.push_back(chunk.line);
	return lines;
}