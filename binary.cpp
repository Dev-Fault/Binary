#include "binary.h"
#include <iostream>

const std::map<unsigned int, std::string> intToHexMap
{
	{0, "0"},
	{1, "1"},
	{2, "2"},
	{3, "3"},
	{4, "4"},
	{5, "5"},
	{6, "6"},
	{7, "7"},
	{8, "8"},
	{9, "9"},
	{10, "A"},
	{11, "B"},
	{12, "C"},
	{13, "D"},
	{14, "E"},
	{15, "F"}
};

const std::map<char, std::string> hexToBinaryMap
{
	{'0', "0000"},
	{'1', "0001"},
	{'2', "0010"},
	{'3', "0011"},
	{'4', "0100"},
	{'5', "0101"},
	{'6', "0110"},
	{'7', "0111"},
	{'8', "1000"},
	{'9', "1001"},
	{'A', "1010"},
	{'B', "1011"},
	{'C', "1100"},
	{'D', "1101"},
	{'E', "1110"},
	{'F', "1111"}
};

struct AdditionResult
{
	bool sum = false;
	bool carry = false;
};

struct SubtractResult
{
	bool difference = false;
	bool borrow = false;
};

std::string ReverseString(const std::string& input)
{
	std::string result;

	for (std::string::const_reverse_iterator i = input.rbegin(); i != input.rend(); i++)
	{
		result.push_back(*i);
	}

	return result;
}

#pragma region Binary Arithmetic

bool AND(bool a, bool b)
{
	return a && b;
}

bool OR(bool a, bool b)
{
	return a || b;
}

bool XOR(bool a, bool b)
{
	return AND(OR(a, b), !AND(a, b));
}

AdditionResult Add(bool a, bool b)
{
	AdditionResult additionResult;
	additionResult.sum = a != b;
	additionResult.carry = a && b;
	return additionResult;
}

SubtractResult Sub(bool a, bool b)
{
	SubtractResult subtractResult;
	subtractResult.difference = XOR(a, b);
	subtractResult.borrow = AND(!a, b);
	return subtractResult;
}

#pragma endregion

#pragma region Constructors

Binary::Binary() {}

Binary::Binary(const Binary& binary)
{
	data = binary.data;
	isSigned = binary.isSigned;
}

Binary::Binary(char value)
{
	isSigned = true;
	bool negative = value < 0;
	value = abs(value);
	for (int i = 0; i != byteSize; i++)
	{
		data.push_back(value & 1);
		value = value >> 1;
	}
	data.push_back(false);

	if (negative) TwosCompliment();
}

Binary::Binary(unsigned char value)
{
	isSigned = false;
	for (int i = 0; i != byteSize; i++)
	{
		data.push_back(value & 1);
		value = value >> 1;
	}
}

Binary::Binary(short value)
{
	isSigned = true;
	bool negative = value < 0;
	value = abs(value);
	for (int i = 0; i != wordSize; i++)
	{
		data.push_back(value & 1);
		value = value >> 1;
	}
	data.push_back(false);

	if (negative) TwosCompliment();
}

Binary::Binary(unsigned short value)
{
	isSigned = false;
	for (int i = 0; i != wordSize; i++)
	{
		data.push_back(value & 1);
		value = value >> 1;
	}
}

Binary::Binary(int value)
{
	isSigned = true;
	bool negative = value < 0;
	value = abs(value);
	for (int i = 0; i != doubleWordSize; i++)
	{
		data.push_back(value & 1);
		value = value >> 1;
	}

	if (negative) TwosCompliment();
}

Binary::Binary(unsigned int value)
{
	isSigned = false;
	for (int i = 0; i != doubleWordSize; i++)
	{
		data.push_back(value & 1);
		value = value >> 1;
	}
}

Binary::Binary(long int value)
{
	isSigned = true;
	bool negative = value < 0;
	value = abs(value);
	for (int i = 0; i != quadWordSize; i++)
	{
		data.push_back(value & 1);
		value = value >> 1;
	}
	data.push_back(false);

	if (negative) TwosCompliment();
}

Binary::Binary(unsigned long int value)
{
	isSigned = false;
	for (int i = 0; i != quadWordSize; i++)
	{
		data.push_back(value & 1);
		value = value >> 1;
	}
}

Binary::Binary(const std::string& value, bool isSignedValue)
{
	if (value.empty()) return;
	isSigned = isSignedValue;

	if (value[0] == 'b' || value[0] == 'B')
	{
		std::string reverseValue = ReverseString(value.substr(1, value.size() - 1));
		for (int i = 0; i != reverseValue.size(); i++)
		{
			if (reverseValue[i] == '0')
			{
				data.push_back(false);
			}
			else if (reverseValue[i] == '1')
			{
				data.push_back(true);
			}
			else if (reverseValue[i] == '_' || reverseValue[i] == ' ')
			{
				continue;
			}
			else
			{
				data.clear();
				for (int i = 0; i != byteSize; i++)
				{
					data.push_back(false);
				}
				return;
			}
		}
	}
	else if (value[0] == '#')
	{
		std::string binaryString("b");
		for (std::string::const_iterator i = value.begin() + 1; i != value.end(); i++)
		{
			if (!(*i == '_' || *i == ' '))
			{
				try
				{
					binaryString += hexToBinaryMap.at(*i);
				}
				catch (std::out_of_range e)
				{
					data.clear();
					for (int i = 0; i != byteSize; i++)
					{
						data.push_back(false);
					}
				}
			}	
		}
		Binary binary(binaryString);
		data = binary.data;
	}
	else
	{
		return;
	}
}

Binary::Binary(const char* value, bool isSignedValue)
{
	std::string str(value);
	Binary temp(str, isSignedValue);
	*this = temp;
}

Binary::Binary(const std::vector<bool>& value, bool isSignedValue)
{
	isSigned = isSignedValue;
	data = value;
}

#pragma endregion

#pragma region Operators

Binary Binary::operator+(const Binary& binary) const
{
	Binary result;
	if (IsSigned() && binary.IsSigned())
	{
		long int left, right = 0;
		left = ToLongInt();
		right = binary.ToLongInt();
		result = left + right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else if (IsSigned() && !binary.IsSigned())
	{
		long int left = 0;
		long unsigned int right = 0;
		left = ToLongInt();
		right = binary.ToUnsignedLongInt();
		result = left + right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else if (!IsSigned() && binary.IsSigned())
	{
		long unsigned int left = 0;
		long int right = 0;
		left = ToUnsignedLongInt();
		right = binary.ToLongInt();
		result = left + right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else
	{
		long unsigned int left, right = 0;
		left = ToUnsignedLongInt();
		right = binary.ToUnsignedLongInt();
		result = left + right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
}

Binary Binary::operator-(const Binary& binary) const
{
	Binary result;
	if (IsSigned() && binary.IsSigned())
	{
		long int left, right = 0;
		left = ToLongInt();
		right = binary.ToLongInt();
		result = left - right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else if (IsSigned() && !binary.IsSigned())
	{
		long int left = 0;
		long unsigned int right = 0;
		left = ToLongInt();
		right = binary.ToUnsignedLongInt();
		result = left - right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else if (!IsSigned() && binary.IsSigned())
	{
		long unsigned int left = 0;
		long int right = 0;
		left = ToUnsignedLongInt();
		right = binary.ToLongInt();
		result = left - right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else
	{
		long unsigned int left, right = 0;
		left = ToUnsignedLongInt();
		right = binary.ToUnsignedLongInt();
		result = left - right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
}

Binary Binary::operator *(const Binary& binary) const
{
	Binary result;
	if (IsSigned() && binary.IsSigned())
	{
		long int left, right = 0;
		left = ToLongInt();
		right = binary.ToLongInt();
		result = left * right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else if (IsSigned() && !binary.IsSigned())
	{
		long int left = 0;
		long unsigned int right = 0;
		left = ToLongInt();
		right = binary.ToUnsignedLongInt();
		result = left * right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else if (!IsSigned() && binary.IsSigned())
	{
		long unsigned int left = 0;
		long int right = 0;
		left = ToUnsignedLongInt();
		right = binary.ToLongInt();
		result = left * right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else
	{
		long unsigned int left, right = 0;
		left = ToUnsignedLongInt();
		right = binary.ToUnsignedLongInt();
		result = left * right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
}

Binary Binary::operator /(const Binary& binary) const
{
	Binary result;
	if (IsSigned() && binary.IsSigned())
	{
		long int left, right = 0;
		left = ToLongInt();
		right = binary.ToLongInt();
		result = left / right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else if (IsSigned() && !binary.IsSigned())
	{
		long int left = 0;
		long unsigned int right = 0;
		left = ToLongInt();
		right = binary.ToUnsignedLongInt();
		result = left / right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else if (!IsSigned() && binary.IsSigned())
	{
		long unsigned int left = 0;
		long int right = 0;
		left = ToUnsignedLongInt();
		right = binary.ToLongInt();
		result = left / right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else
	{
		long unsigned int left, right = 0;
		left = ToUnsignedLongInt();
		right = binary.ToUnsignedLongInt();
		result = left / right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
}

Binary Binary::operator %(const Binary& binary) const
{
	Binary result;
	if (IsSigned() && binary.IsSigned())
	{
		long int left, right = 0;
		left = ToLongInt();
		right = binary.ToLongInt();
		result = left % right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else if (IsSigned() && !binary.IsSigned())
	{
		long int left = 0;
		long unsigned int right = 0;
		left = ToLongInt();
		right = binary.ToUnsignedLongInt();
		result = left % right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else if (!IsSigned() && binary.IsSigned())
	{
		long unsigned int left = 0;
		long int right = 0;
		left = ToUnsignedLongInt();
		right = binary.ToLongInt();
		result = left % right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
	else
	{
		long unsigned int left, right = 0;
		left = ToUnsignedLongInt();
		right = binary.ToUnsignedLongInt();
		result = left % right;
		std::vector<bool>::size_type size = Size() >= binary.Size() ? Size() : binary.Size();
		result.SetSize(size);
		return result;
	}
}

void Binary::operator +=(const Binary& binary)
{
	*this = *this + binary;
}

void Binary::operator -=(const Binary& binary)
{
	*this = *this - binary;
}

void Binary::operator *=(const Binary& binary)
{
	*this = *this * binary;
}

void Binary::operator /=(const Binary& binary)
{
	*this = *this / binary;
}

void Binary::operator %=(const Binary& binary)
{
	*this = *this % binary;
}

void Binary::operator &=(const Binary& binary)
{
	*this = *this & binary;
}

void Binary::operator |=(const Binary& binary)
{
	*this = *this | binary;
}

void Binary::operator ^=(const Binary& binary)
{
	*this = *this ^ binary;
}

void Binary::operator >>=(int n)
{
	*this = *this >> n;
}

void Binary::operator <<=(int n)
{
	*this = *this << n;
}

Binary& Binary::operator ++()
{
	Increment();
	return *this;
}

Binary Binary::operator ++(int)
{
	Binary temp = *this;
	++(*this);
	return temp;
}

Binary& Binary::operator --()
{
	Decrement();
	return *this;
}

Binary Binary::operator --(int)
{
	Binary temp = *this;
	--(*this);
	return temp;
}

bool Binary::operator ==(const Binary& binary) const
{
	if (data.size() < binary.data.size())
	{
		int endPoint = 0;
		for (int i = 0; i != data.size(); i++)
		{
			if (data[i] != binary.data[i]) return false;
			endPoint++;
		}

		for (int i = endPoint; i != binary.data.size(); i++)
		{
			if (binary.data[i]) return false;
		}

		return true;
	}
	else if (data.size() > binary.data.size())
	{
		int endPoint = 0;
		for (int i = 0; i != binary.data.size(); i++)
		{
			if (binary.data[i] != data[i]) return false;
			endPoint++;
		}

		for (int i = endPoint; i != data.size(); i++)
		{
			if (data[i]) return false;
		}

		return true;
	}
	else
	{
		for (int i = 0; i != data.size(); i++)
		{
			if (data[i] != binary.data[i]) return false;
		}
		return true;
	}
}

bool Binary::operator !=(const Binary& binary) const
{
	return !(*this == binary);
}

bool Binary::operator <(const Binary& binary) const
{
	if (IsNegative() && !binary.IsNegative())
	{
		return true;
	}
	else if (!IsNegative() && binary.IsNegative())
	{
		return false;
	}
	else if (IsNegative() && binary.IsNegative())
	{
		return ToLongInt() < binary.ToLongInt();
	}
	else
	{
		return ToUnsignedLongInt() < binary.ToUnsignedLongInt();
	}
}

bool Binary::operator >(const Binary& binary) const
{
	if (IsNegative() && !binary.IsNegative())
	{
		return false;
	}
	else if (!IsNegative() && binary.IsNegative())
	{
		return true;
	}
	else if (IsNegative() && binary.IsNegative())
	{
		return ToLongInt() > binary.ToLongInt();
	}
	else
	{
		return ToUnsignedLongInt() > binary.ToUnsignedLongInt();
	}
}

bool Binary::operator <=(const Binary& binary) const
{
	return *this < binary || *this == binary;
}

bool Binary::operator >=(const Binary& binary) const
{
	return *this > binary || *this == binary;
}

Binary Binary::operator >>(int value) const
{
	Binary result = *this;
	std::vector<bool> temp;

	if (value < 0)
	{
		return *this << abs(value);
	}

	for (int v = 0; v != value; v++)
	{
		for (int i = 1; i != result.Size(); i++)
		{
			temp.push_back(result[i]);
		}
		temp.push_back(false);
		result = Binary(temp, isSigned);
		temp.clear();
	}	

	return result;
}

Binary Binary::operator <<(int value) const
{
	Binary result = *this;
	std::vector<bool> temp;

	if (value < 0)
	{
		return *this >> abs(value);
	}

	for (int v = 0; v != value; v++)
	{
		temp.push_back(false);
		for (int i = 0; i != result.Size(); i++)
		{
			temp.push_back(result[i]);
		}
		temp.resize(result.Size());
		result = Binary(temp, isSigned);
		temp.clear();
	}

	return result;
}

Binary Binary::operator &(const Binary& binary) const
{
	Binary result;

	if (data.size() <= binary.data.size())
	{
		for (int i = 0; i != data.size(); i++)
		{
			result.data.push_back(data[i] && binary.data[i]);
		}
	}
	else
	{
		for (int i = 0; i != binary.data.size(); i++)
		{
			result.data.push_back(data[i] && binary.data[i]);
		}
	}

	return result;
}

Binary Binary::operator |(const Binary& binary) const
{
	Binary result;

	if (data.size() <= binary.data.size())
	{
		for (int i = 0; i != data.size(); i++)
		{
			result.data.push_back(data[i] || binary.data[i]);
		}
	}
	else
	{
		for (int i = 0; i != binary.data.size(); i++)
		{
			result.data.push_back(data[i] || binary.data[i]);
		}
	}

	return result;
}

Binary Binary::operator ^(const Binary& binary) const
{
	Binary result;

	if (data.size() <= binary.data.size())
	{
		for (int i = 0; i != data.size(); i++)
		{
			result.data.push_back(data[i] ^ binary.data[i]);
		}
	}
	else
	{
		for (int i = 0; i != binary.data.size(); i++)
		{
			result.data.push_back(data[i] ^ binary.data[i]);
		}
	}

	return result;
}

Binary Binary::operator ~() const
{
	Binary result;

	for (int i = 0; i != data.size(); i++)
	{
		result.data.push_back(!data[i]);
	}

	return result;
}

Binary Binary::operator -() const
{
	Binary result = *this;
	result.TwosCompliment();
	result.isSigned = true;
	return result;
}

#pragma endregion

#pragma region User Converisons

char Binary::ToChar() const
{
	char value = 0;

	if (SignBitSet())
	{
		Binary positiveBinary = -*this;
		for (int i = 0; i != positiveBinary.data.size(); i++)
		{
			if (positiveBinary.data[i]) value += pow(2, i);
		}
		value *= -1;
	}
	else
	{
		for (int i = 0; i != data.size(); i++)
		{
			if (data[i]) value += pow(2, i);
		}
	}

	return value;
}

unsigned char Binary::ToUnsignedChar() const
{
	unsigned char value = 0;

	for (int i = 0; i != data.size(); i++)
	{
		if (data[i]) value += pow(2, i);
	}

	return value;
}

short Binary::ToShort() const
{
	short value = 0;

	if (SignBitSet())
	{
		Binary positiveBinary = -*this;
		for (int i = 0; i != positiveBinary.data.size(); i++)
		{
			if (positiveBinary.data[i]) value += pow(2, i);
		}
		value *= -1;
	}
	else
	{
		for (int i = 0; i != data.size(); i++)
		{
			if (data[i]) value += pow(2, i);
		}
	}

	return value;
}

unsigned short Binary::ToUnsignedShort() const
{
	unsigned short value = 0;

	for (int i = 0; i != data.size(); i++)
	{
		if (data[i]) value += pow(2, i);
	}

	return value;
}

int Binary::ToInt() const
{
	int value = 0;

	if (SignBitSet())
	{
		Binary positiveBinary = -*this;
		for (int i = 0; i != positiveBinary.data.size(); i++)
		{
			if (positiveBinary.data[i]) value += pow(2, i);
		}
		value *= -1;
	}
	else
	{
		for (int i = 0; i != data.size(); i++)
		{
			if (data[i]) value += pow(2, i);
		}
	}

	return value;
}

unsigned int Binary::ToUnsignedInt() const
{
	unsigned int value = 0;

	for (int i = 0; i != data.size(); i++)
	{
		if (data[i]) value += pow(2, i);
	}

	return value;
}

long int Binary::ToLongInt() const
{
	long int value = 0;

	if (SignBitSet())
	{
		Binary positiveBinary = -*this;
		for (int i = 0; i != positiveBinary.data.size(); i++)
		{
			if (positiveBinary.data[i]) value += pow(2, i);
		}
		value *= -1;
	}
	else
	{
		for (int i = 0; i != data.size(); i++)
		{
			if (data[i]) value += pow(2, i);
		}
	}

	return value;
}

unsigned long int Binary::ToUnsignedLongInt() const
{
	unsigned long int value = 0;

	for (int i = 0; i != data.size(); i++)
	{
		if (data[i]) value += pow(2, i);
	}

	return value;
}

std::string Binary::ToBinaryString(const std::string& seperator) const
{
	std::string result;

	for (int i = 0; i != data.size(); i++)
	{
		result += data[i] == true ? "1" : "0";
		if (i != data.size() - 1 && (i + 1) % 4 == 0) result += seperator;
	}

	return ReverseString(result);
}

std::string Binary::ToIntegerString() const
{
	if (IsSigned())
	{
		return std::to_string(this->ToLongInt());
	}
	else
	{
		return std::to_string(this->ToUnsignedLongInt());
	}
}

std::string Binary::ToHexString() const
{
	std::string result;
	int dataIndex = 0;
	while (dataIndex != data.size())
	{
		std::vector<bool> halfByte;
		for (int i = 0; i != 4; i++)
		{
			if (dataIndex >= data.size())
			{
				break;
			}
			halfByte.push_back(data[dataIndex]);
			dataIndex++;
		}
		Binary binary(halfByte);
		unsigned int value = binary.ToUnsignedInt();
		result += intToHexMap.at(value);
	}

	return ReverseString(result);
}

std::vector<bool> Binary::ToVector() const
{
	return data;
}

#pragma endregion

#pragma region Actions

void Binary::TwosCompliment()
{
	for (int i = 0; i != data.size(); i++)
	{
		data[i] = !data[i];
	}
	Increment();
}

void Binary::Increment()
{
	AdditionResult additionResult;
	additionResult.carry = true;

	for (int i = 0; i != data.size(); i++)
	{
		additionResult = Add(data[i], additionResult.carry);
		data[i] = additionResult.sum;
	}
}

void Binary::Decrement()
{
	SubtractResult subtractResult;
	subtractResult.borrow = true;

	for (int i = 0; i != data.size(); i++)
	{
		subtractResult = Sub(data[i], subtractResult.borrow);
		data[i] = subtractResult.difference;
	}
}

void Binary::SetBit(int n)
{
	if (n >= 0 && n < data.size())
	{
		data[n] = true;
	}
}

void Binary::ClearBit(int n)
{
	if (n >= 0 && n < data.size())
	{
		data[n] = false;
	}
}

void Binary::SetAllBits()
{
	for (int i = 0; i != data.size(); i++)
	{
		data[i] = true;
	}
}

void Binary::ClearAllBits()
{
	for (int i = 0; i != data.size(); i++)
	{
		data[i] = false;
	}
}

void Binary::RotateRight(int n)
{
	if (data.empty())
	{
		return;
	}
	for (int r = 0; r != n; r++)
	{
		bool firstBit = data[0];
		*this = *this >> 1;
		data[data.size() - 1] = firstBit;
	}
}

void Binary::RotateLeft(int n)
{
	if (data.empty())
	{
		return;
	}
	for (int r = 0; r != n; r++)
	{
		bool lastBit = data[data.size() - 1];
		*this = *this << 1;
		data[0] = lastBit;
	}
}

void Binary::SetSize(std::vector<bool>::size_type size)
{
	if (size < 0)
	{
		return;
	}
	std::vector<bool> newData;
	for (int i = 0; i != size; i++)
	{
		if (i < data.size())
		{
			newData.push_back(data[i]);
		}
		else
		{
			newData.push_back(false);
		}
	}
	data = newData;
}

void Binary::ConvertToSignedValue()
{
	isSigned = true;
}

void Binary::ConvertToUnsignedValue()
{
	isSigned = false;
}

#pragma endregion

#pragma region Properties

bool Binary::IsSigned() const
{
	return isSigned;
}

bool Binary::SignBitSet() const
{
	if (data.empty())
	{
		return false;
	}
	else
	{
		return data[data.size() - 1];
	}
}

bool Binary::IsNegative() const
{
	return IsSigned() && SignBitSet();
}

#pragma endregion
