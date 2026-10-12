#pragma once

#include <string>
#include <iostream>
#include <vector>


template <typename... Args>
void print(Args... args) {
	((std::cout << args << ' '), ...) << '\n';
}

std::vector<float> concat(std::vector<float> vec1, std::vector<float> vec2) {
	std::vector<float> combined = vec1;

	combined.insert(
		combined.end(),
		vec2.begin(),
		vec2.end()
	);
	return combined;
}

float lerp(float a, float b, float t) {
	if (t < 0) { return a; }
	else if (t > 1) { return b; }
	return a + (b - a) * t;
}

std::string to_raw(const std::string& input) {
	std::string raw;
	for (char c : input) {
		switch (c) {
		case '\n': raw += "\\n"; break;
		case '\r': raw += "\\r"; break;
		case '\t': raw += "\\t"; break;
		case '\\': raw += "\\\\"; break;
		default:   raw += c;    break;
		}
	}
	return raw;
}