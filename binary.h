#pragma once

#include <vector>
#include <string>
#include <map>
#include <exception>

class Binary
{
private:
	static const unsigned int byteSize			= 8;
	static const unsigned int wordSize			= 16;
	static const unsigned int doubleWordSize	= 32;
	static const unsigned int quadWordSize		= 64;	

	bool isSigned = false;
	std::vector<bool> data;
	
	void TwosCompliment();

public:

	typedef std::vector<bool>::size_type size_type;

	Binary();
	Binary(char);
	Binary(unsigned char);
	Binary(short);
	Binary(unsigned short);
	Binary(int);
	Binary(unsigned int);
	Binary(long int);
	Binary(unsigned long int);
	Binary(const std::string&, bool = false);
	Binary(const char* value, bool = false);
	Binary(const std::vector<bool>&, bool = false);
	Binary(const Binary& binary);
	
	bool SignBitSet() const;
	bool IsSigned() const;
	bool IsNegative() const;
	void SetBit(int);
	void ClearBit(int);
	void SetAllBits();
	void ClearAllBits();
	void RotateRight(int);
	void RotateLeft(int);
	void Decrement();
	void Increment();
	void SetSize(std::vector<bool>::size_type);
	void ConvertToSignedValue();
	void ConvertToUnsignedValue();
	void PushBack(bool value) { data.push_back(value); }
	std::vector<bool>::size_type Size() const { return data.size(); }

	std::vector<bool> ToVector() const;
	char ToChar() const;
	unsigned char ToUnsignedChar() const;
	short ToShort() const;
	unsigned short ToUnsignedShort() const;
	int ToInt() const;
	unsigned int ToUnsignedInt() const;
	long int ToLongInt() const;
	unsigned long int ToUnsignedLongInt() const;
	std::string ToBinaryString(const std::string& = "") const;
	std::string ToHexString() const;
	std::string ToIntegerString() const;

	bool operator [](int index) const 
	{ 
		return data[index]; 
	}

	Binary operator +(const Binary&) const;
	Binary operator -(const Binary&) const;
	Binary operator *(const Binary&) const;
	Binary operator /(const Binary&) const;
	Binary operator %(const Binary&) const;
	
	Binary& operator ++();
	Binary operator ++(int);
	Binary& operator --();
	Binary operator --(int);
	
	Binary operator &(const Binary&) const;
	Binary operator |(const Binary&) const;
	Binary operator ^(const Binary&) const;
	Binary operator >>(int) const;
	Binary operator <<(int) const;
	Binary operator ~() const;
	Binary operator -() const;

	void operator +=(const Binary&);
	void operator -=(const Binary&);
	void operator *=(const Binary&);
	void operator /=(const Binary&);
	void operator %=(const Binary&);
	void operator &=(const Binary&);
	void operator |=(const Binary&);
	void operator ^=(const Binary&);
	void operator >>=(int);
	void operator <<=(int);

	bool operator ==(const Binary&) const;
	bool operator !=(const Binary&) const;
	bool operator <(const Binary&) const;
	bool operator >(const Binary&) const;
	bool operator <=(const Binary&) const;
	bool operator >=(const Binary&) const;	
};