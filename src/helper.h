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
