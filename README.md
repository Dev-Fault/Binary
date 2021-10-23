# Binary Class For C++

A binary class that uses an underlying vector of bools for space consideration.

![Thumbnail](/images/thumbnail.png)

## Example

Inside main.cpp is an example of how the binary class can be used.

## How to use

The binary class has many constructors for creating a new binary value. You can use any integer type and it will automatically determine whether the type is signed or unsigned. Using a string allows you to write binary or hex as a literal. Binary values are delimited by the b character and hex by the # character. As an example "b0101" will construct a binary instance with the value of 5; "#5" will construct a value of 5 as well. To tell the class to use either of these as a signed or unsigned value there is an optional bool parameter to set the isSigned value of the class. Setting the sign will not change the sign bit but only change how values are converted back and forth from different types. To set individual bits you can use the SetBit and ClearBit function along with the index of the bit you wish to set. There are many public functions to manipulate the underlying binary data.