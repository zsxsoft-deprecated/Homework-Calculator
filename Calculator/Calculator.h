#pragma once
#include <string>
#include <stack>
#include "BigInteger.h"

class Calculator {
public:
	typedef enum {
		NONE, DOT, OPEN_BRACKET, CLOSE_BRACKET, SIGN, IGNORE, DIGIT, UNEXPECTED_CHAR, END
	} CharType;

	struct CharData {
		char Char;
		int Position = 0;
		CharData(char c, int p) : Char(c), Position(p) {};
	};
	struct errorUnion {
		std::string Data;
		bool HasChar = false;
		CharData Char = CharData('\0', 0);
		errorUnion() {}
	} ErrorData;

	bool UseDouble = false;
	bool HasError = false;
	std::string StringResult;
	double DoubleResult;

	explicit Calculator(std::string data, int radix);
	std::string Data() const;
	std::string Error() const;

	~Calculator();
private:
	
	std::stack<CharData> Stack;
	std::stack<std::string> Output;
	std::stack<double> DoubleOutput;
	bool DoCalculate(const CharData);
	void ThrowError(const std::string);
	void ThrowError(const std::string, const CharData);
	void ConvertBigIntegerToDouble();
	CharType GetType(const char c) const;
	bool DoCalculate(const CharData, double, double);
	template <class T> bool DoCalculate(const CharData, std::string, std::string);
	template <class T> bool DoCalculate(const CharData, T, T);
	int Radix = 10;
};

