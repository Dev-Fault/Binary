#include <iostream>
#include <vector>
#include "binary.h"

int main()
{
	Binary binary("b0101");
	Binary binary2("b1010");
	Binary binary3("#64");

	std::cout << "Logical Comparisons" << std::endl << std::endl;
	std::cout << "Value 1: " << binary.ToBinaryString() << std::endl;
	std::cout << "Value 2: " << binary2.ToBinaryString() << std::endl << std::endl;
	std::cout << "OR" << std::endl;
	std::cout << (binary | binary2).ToBinaryString() << std::endl << std::endl;
	std::cout << "AND" << std::endl;
	std::cout << (binary & binary2).ToBinaryString() << std::endl << std::endl;
	std::cout << "XOR" << std::endl;
	std::cout << (binary ^ binary2).ToBinaryString() << std::endl << std::endl;

	std::cout << "Shifts and rotations" << std::endl << std::endl;
	std::cout << "Value 1: " << binary.ToBinaryString() << std::endl << std::endl;
	std::cout << "Left shift" << std::endl;
	std::cout << (binary << 1).ToBinaryString() << std::endl << std::endl;
	std::cout << "Right shift" << std::endl;
	std::cout << (binary >> 1).ToBinaryString() << std::endl << std::endl;
	std::cout << "Left rotation" << std::endl;
	binary.RotateLeft(1);
	std::cout << binary.ToBinaryString() << std::endl << std::endl;
	binary.RotateRight(2);
	std::cout << "Right rotation" << std::endl;
	std::cout << binary.ToBinaryString() << std::endl << std::endl;

	std::cout << "Representations" << std::endl << std::endl;
	std::cout << "Binary: " << binary3.ToBinaryString() << std::endl << std::endl;
	std::cout << "Hex: " << binary3.ToHexString() << std::endl << std::endl;
	std::cout << "Integer: " << binary3.ToIntegerString() << std::endl << std::endl;
	return 0;
}