#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <bitset>

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255)
{
	return (a << 24) + (b << 16) + (g << 8) + r;
}

void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a)
{
	//apply a mask so we only get a Byte from the bit-shifted value
	r = (color >>  0) & 255;
	g = (color >>  8) & 255;
	b = (color >> 16) & 255;
	a = (color >> 24) & 255;
}

int main()
{
	const uint8_t r = 32;
	const uint8_t g = 32;
	const uint8_t b = 128;
	const uint8_t a = 255;

	uint8_t uR, uG, uB, uA;

	uint32_t test = pack_color(r, g, b);
	std::cout << test << '\n';

	unpack_color(test, uR, uG, uB, uA);
	std::cout << std::bitset<8>(uR) << '\t';
	std::cout << std::bitset<8>(uG) << '\t';
	std::cout << std::bitset<8>(uB) << '\t';
	std::cout << std::bitset<8>(uA) << '\n';

	return 0;
}