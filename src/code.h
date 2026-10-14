#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <format>
#include <variant>

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
	ELM(string Type, char Data) : type(Type), data(string(1, Data)) {}

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

	bool append(ELM Elm) {
		return append(Elm.data, Elm.type);
	}

	operator string() const {
		return format("{}:'{}'", type, to_raw(data));
	}
};
class LINE {
public:
	vector<variant<ELM, LINE>> data;

	LINE() : data({}) {}

	size_t size() {
		return data.size();
	}
	void append(ELM elm) {
		data.push_back(elm);
	}
	bool append(string Data, string Type) {
		data.push_back(ELM(Type, Data));
		return true;
	}
	bool append(char Data, string Type) {
		data.push_back(ELM(Type, string(1, Data)));
		return true;
	}
	void append(LINE line) {
		data.push_back(line);
	}

	auto& top() {
		return data[size() - 1];
	}

	operator string() const {
		string s = "[";
		for (int i = 0; i < data.size(); i++) {
			visit([&](const auto& element) {
				s += (string)element + ", ";
			}, data[i]);
			//s += (string)data[i] + ", ";
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
string beginChars = "([{'\"";
char endBegins(char ch) {
	if (ch == '(') return ')';
	else if (ch == '[') return ']';
	else if (ch == '{') return '}';
	else if (ch == '\'') return '\'';
	else if (ch == '"') return '"';

	return '0';
}


class CHUNKOLD {
public:
	char endChar;
	string endType;

	int& pos;
	string data;
	char& tok;

	LINE line;
	bool stop = false;

	CHUNKOLD(string Data, int& Pos, char EndChar) : data(Data), pos(Pos), endChar(EndChar), endType(charType(EndChar)), tok(data[pos]) {}

	void move() { pos++; }
	char currentToken() { tok = (data[pos]); return tok; }

	void mainStep() {
		currentToken();

		string type = charType(tok);
		//print(type, tok, pos);

		if (tok == endChar) {
			stop = !false;
			move();
			return;
		}

		if (
			(type == "newline" || type == "space") && !(endType == "squote" || endType == "quote")
			) {
			move();
			return;
		}

		if (line.size() == 0) {
			line.append(tok, type);
		}
		else if (beginChars.find(tok) != string::npos) {
			CHUNKOLD tchunk(data, pos, endBegins(tok));
			tchunk.main();
			line.append(tchunk.line);
			return;
		}
		else {
			auto& elm = line.top();
			bool res = false;
			visit([&](auto& element) {
				res = element.append(tok, type);
			}, elm);
			if (!res) { line.append(tok, type); }
		}

		move();

	}
	void main() {
		while (!stop && pos < data.size()) {
			mainStep();
		}
		//pos--;
	}

	operator string() const {
		return "Chunk: "+(string)line;
	}
};

vector<LINE> readDataOld(const string& data) {
	int pos = 0;
	vector<LINE> lines;
	while (pos < data.size()) {
		CHUNKOLD chunk(data, pos, ';');
		chunk.main();
		lines.push_back(chunk.line);
	}
	return lines;
}

class Chunk {
public:
	vector<variant<ELM, Chunk>> data;
	string type;

	Chunk(string Type) : type(Type), data({}) {}

	void append(char Data) {
		append(ELM(charType(Data), Data));
	}
	void append(string Data, string Type) {
		append(ELM(Type, Data));
	}
	void append(ELM elm) {
		if (data.size() == 0) { data.push_back(elm); }
		else {
			auto& telm = top();
			if (holds_alternative<ELM>(telm)) {
				bool res = get<ELM>(telm).append(elm);
				if (!res) { data.push_back(elm); }
			}
			else if (holds_alternative<Chunk>(telm)) {
				data.push_back(elm);
			}
			else {
				data.push_back(elm);
			}
		}
	}
	void append(Chunk chunk) {
		data.push_back(chunk);
	}

	variant<ELM, Chunk>& top() {
		return data[data.size() - 1];
	}
	operator string() const {
		string s = "";
		for (int i = 0; i < data.size(); i++) {
			variant<ELM, Chunk> d = data[i];
			s += (holds_alternative<ELM>(d) ? (string)get<ELM>(d) : (string)get<Chunk>(d)) + ", ";
		}
		return format("{}:[{}]", type, s);
	}
};


Chunk readDecide(const string& data, int& pos, char type);

Chunk readSemicolon(const string& data, int& pos) {
	Chunk ch("Semicolon");
	while (pos < data.size()) {
		char tok = data[pos++];
		if (tok == ';') break;
		else if (beginChars.find(tok) != string::npos) {
			char end = endBegins(tok);
			Chunk res = readDecide(data, pos, end);
			ch.append(res);
		}
		else {
			ch.append(tok);
		}
	}
	return ch;
}
Chunk readPara(const string& data, int& pos) {
	Chunk ch("Para");
	while (pos < data.size()) {
		char tok = data[pos++];
		if (tok == ')') break;
		else if (beginChars.find(tok) != string::npos) {
			char end = endBegins(tok);
			Chunk res = readDecide(data, pos, end);
			ch.append(res);
		}
		else {
			ch.append(tok);
		}
	}
	return ch;
}
Chunk readBrace(const string& data, int& pos) {
	Chunk ch("Brace");
	while (pos < data.size()) {
		char tok = data[pos++];
		if (tok == '}') break;
		else if (beginChars.find(tok) != string::npos) {
			char end = endBegins(tok);
			Chunk res = readDecide(data, pos, end);
			ch.append(res);
		}
		else {
			ch.append(tok);
		}
	}
	return ch;
}
Chunk readBrack(const string& data, int& pos) {
	Chunk ch("Brack");
	while (pos < data.size()) {
		char tok = data[pos++];
		if (tok == ']') break;
		else if (beginChars.find(tok) != string::npos) {
			char end = endBegins(tok);
			Chunk res = readDecide(data, pos, end);
			ch.append(res);
		}
		else {
			ch.append(tok);
		}
	}
	return ch;
}
Chunk readStringS(const string& data, int& pos) {
	Chunk ch("StringS");
	while (pos < data.size()) {
		char tok = data[pos++];
		if (tok == '\'') break;
		else {
			ch.append(tok);
		}
	}
	return ch;
}

Chunk readDecide(const string& data, int& pos, char type) {
	if (type == ';')return readSemicolon(data, pos);
	else if (type == ')')return readPara(data, pos);
	else if (type == '}')return readBrace(data, pos);
	else if (type == ']')return readBrack(data, pos);
	else if (type == '\'')return readStringS(data, pos);
	return Chunk("Oopsie!");
}


vector<Chunk> readData(const string& data) {
	int pos = 0;
	vector<Chunk> chunks = {};

	while (pos < data.size()) {
		Chunk res = readSemicolon(data, pos);
		chunks.push_back(res);
	}

	return chunks;
}