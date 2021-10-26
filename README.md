# Binary Class For C++

A binary class that uses an underlying vector of bools for space consideration.

![Thumbnail](/images/thumbnail.png)

## Example

Inside main.cpp is an example of how the binary class can be used.

## Purpose of this class

My goal for designing this class was to allow for an easy way to look at the binary data of the different primitive types. To achieve this goal you can create an instance of the class from integer constants, any primitive integer variable signed or unsigned, as well as through hex and binary delimited strings. When creating an instance through the string constructor strings delimited with the # character denote hex, and strings delimited with the b character denote binary. Underscores are completely ignored allowing you to visualize the data easier. As an example constructing a binary instance by using the strings "#5" and "b0101" will yield a binary class with the unsigned value of 5. To make this value signed you pass in a secondary bool parameter to tell the class whether to treat values as signed or unsigned. This class is capable of doing arithmetic with other instances of the class as well as logical comparisons, shifts, rotations and more. To view the data inside the class you can use the string converter functions ToBinaryString, ToHexString, and ToIntegerString. To set a single bit or clear a single bit call SetBit, or ClearBit, with the index of the bit.
