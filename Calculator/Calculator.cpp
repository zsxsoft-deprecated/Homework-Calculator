#include "stdafx.h"
#include "Calculator.h"
#include "BigInteger.h"
#include <sstream>
#include <cstdarg>
#include <memory>
#include <queue>
#include <iomanip>
#define BIGINT(n, s) (n == 10 ? BigInteger<>(s) : BigInteger<1, n>(s));

int precedence(const char sign) {
	switch (sign) {
		case '(':
			return -1;
		case '+':
			return 1;
		case '-':
			return 1;
		case '*':
			return 2;
		case '/':
			return 2;
		case '%':
			return 2;
		case '^':
			return 3;
		default:
			return 0;
	}
}

Calculator::CharType Calculator::GetType(const char c) const {
	if (c < 0) {
		return UNEXPECTED_CHAR;
	}
	if (c >= '0' && c <= (Radix >= 10 ? '9' : Radix + '0')) {
		return DIGIT;
	}
	if (c == '.') {
		return DOT;
	}
	if (c == '(') {
		return OPEN_BRACKET;
	}
	if (c == ')') {
		return CLOSE_BRACKET;
	}
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^') {
		return SIGN;
	}
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
		return IGNORE;
	}
	if (c == '\0') {
		return END;
	}
	return UNEXPECTED_CHAR;
}


template<typename T> bool Calculator::DoCalculate(const CharData data, std::string item1, std::string item2) {
	return DoCalculate(data, T(item1), T(item2));
}

bool Calculator::DoCalculate(const CharData data, double Int1, double Int2) {
	auto Int3 = .0;
	switch (data.Char) {
		case '+':
			Int3 = Int2 + Int1;
			break;
		case '-':
			Int3 = Int2 - Int1;
			break;
		case '*':
			Int3 = Int2 * Int1;
			break;
		case '/':
			if (Int1 == 0.0) {
				ThrowError("Cannot divide by zero", data);
				return false;
			}
			Int3 = Int2 / Int1;
			break;
		case '^':
			Int3 = pow(Int2, Int1);
			break;
		case '%':
			ThrowError("Cannot use % in double", data);
			break;
		default:;
	}

	DoubleOutput.push(Int3);
	return true;
}

template<typename T> bool Calculator::DoCalculate(const CharData data, T Int1, T Int2) {

	auto Int3 = T();
	switch (data.Char) {
		case '+':
			Int3 = Int2 + Int1;
			break;
		case '-':
			Int3 = Int2 - Int1;
			break;
		case '*':
			Int3 = Int2 * Int1;
			break;
		case '/':
			if (Int1 == 0) {
				ThrowError("Cannot divide by zero", data);
				return false;
			}
			Int3 = Int2 / Int1;
			break;
		case '^':
			Int3 = Int2.Pow(Int1);

			break;
		case '%':
			Int3 = Int2 % Int1;

			break;
		default:;
	}
	Output.push(Int3.ToString());

	return true;

}

bool Calculator::DoCalculate(const CharData data) {

	if (UseDouble) {
		if (DoubleOutput.empty()) return false;
		auto Int1 = DoubleOutput.top();
		DoubleOutput.pop();
		if (DoubleOutput.empty()) return false;
		auto Int2 = DoubleOutput.top();
		DoubleOutput.pop();
		return DoCalculate(data, Int1, Int2);
	}

	if (Output.empty()) return false;
	auto item1 = Output.top();
	Output.pop();
	if (Output.empty()) return false;
	auto item2 = Output.top();
	Output.pop();
	if (Radix == 2) {
		return DoCalculate<BigInteger<1, 1>>(data, item1, item2);
	}
	if (Radix == 8) {
		return DoCalculate<BigInteger<1, 7>>(data, item1, item2);
	}
	if (Radix == 16) {
		return DoCalculate<BigInteger<2, 15>>(data, item1, item2);
	}
	if (Radix == 10) {
		return DoCalculate<BigInteger<>>(data, item1, item2);
	}
	return false;
}


Calculator::Calculator(std::string data, int radix) : Radix(radix) {

	bool state;
	std::stringstream digitBuffer;
	auto prevStatus = NONE;
	auto hasDot = false;
	auto position = -1;

	while (true) {
		auto c = data[++position];
		CharData inputData(c, position - 1);

		auto thisStatus = GetType(c);
		if (prevStatus == DIGIT) {
			if (thisStatus != DIGIT && thisStatus != DOT) {
				auto str = digitBuffer.str();
				if (UseDouble) {
					auto dbl = atof(str.c_str());
					DoubleOutput.push(dbl);
				}
				else {
					std::string ret;
					if (radix == 2) {
						ret = BigInteger<1, 1>(str).ToString();
					}
					else if (radix == 8) {
						ret = BigInteger<1, 7>(str).ToString();
					}
					else if (radix == 10) {
						ret = BigInteger<>(str).ToString();
					}
					else if (radix == 16) {
						ret = BigInteger<2, 15>(str).ToString();
					}
					//BigInteger<>(str).ToString()
					Output.push(ret);
				}
				digitBuffer.str("");
				hasDot = false;
			}
		}
		if (thisStatus == END) break;

		switch (thisStatus) {
			case DOT:
				if (hasDot) {
					ThrowError("Unexpected dot", inputData);
					return;
				}
				if (!UseDouble) {
					ConvertBigIntegerToDouble();
				}
				UseDouble = true;
				hasDot = true;
				digitBuffer << c;
				break;
			case DIGIT:
				if (prevStatus != NONE && prevStatus != OPEN_BRACKET && prevStatus != DIGIT && prevStatus != DOT && prevStatus != SIGN) {
					ThrowError("Unexpected digit", inputData);
					return;
				}
				digitBuffer << c;
				break;

			case SIGN:
				if ((c == '-' || c == '+') && (prevStatus == SIGN || prevStatus == NONE)) {
					auto next = data[position + 1];
					if (GetType(next) == DIGIT) {
						digitBuffer << c;
					}
					continue;
				}
				if (prevStatus != NONE && prevStatus != CLOSE_BRACKET && prevStatus != DIGIT) {
					ThrowError("Unexpected sign", inputData);
					return;
				}
				while (!Stack.empty()) {
					auto item = Stack.top();
					if (precedence(c) <= precedence(item.Char)) {
						state = DoCalculate(item);
						Stack.pop();
						if (!state) {
							ThrowError("Unknown Error", inputData);
							return;
						}
					}
					else {
						break;
					}
				}
				Stack.push(inputData);
				break;

			case IGNORE:
				break;

			case OPEN_BRACKET:
				if (prevStatus != NONE && prevStatus != OPEN_BRACKET && prevStatus != SIGN) {
					ThrowError("Unexpected open bracket", inputData);
					return;
				}
				Stack.push(inputData);
				break;

			case CLOSE_BRACKET:
				if (prevStatus != CLOSE_BRACKET && prevStatus != DIGIT) {
					ThrowError("Unexpected close bracket", inputData);
					return;
				}
				if (Stack.empty()) {
					ThrowError("Unexpected close bracket", inputData);
				}
				while (!Stack.empty()) {

					auto item = Stack.top();
					Stack.pop();
					if (GetType(item.Char) == OPEN_BRACKET) {
						break;
					}
					DoCalculate(item);

				}

				break;


			case UNEXPECTED_CHAR:
				ThrowError("Unexpected character", inputData);
				return;
			default:;
		}
		if (thisStatus != IGNORE) {
			prevStatus = thisStatus;
		}
	}

	while (!Stack.empty()) {
		auto item = Stack.top();
		Stack.pop();
		if (GetType(item.Char) == OPEN_BRACKET) {
			ThrowError("Bracket unclosed", item);
			return;
		}
		state = DoCalculate(item);
		if (!state) {
			break;
		}
	}

	if (Output.empty() && DoubleOutput.empty()) {
		ThrowError("No Answer");
		return;
	}

	if (UseDouble) {
		auto ret = DoubleOutput.top();
		DoubleOutput.pop();
		DoubleResult = ret;
	}
	else {
		auto ret = Output.top();
		Output.pop();
		StringResult = ret;
	}

}

std::string Calculator::Data() const {
	if (UseDouble) {
		std::ostringstream ss;
		ss << std::setprecision(15) << DoubleResult;
		return ss.str();
	}

	return StringResult;

}

std::string Calculator::Error() const {
	using std::endl;
	std::stringstream ss;
	ss << "Error: " << ErrorData.Data << endl;
	if (ErrorData.HasChar) {
		ss << "Character: " << ErrorData.Char.Char << endl;
		ss << "In Position " << ErrorData.Char.Position << endl;
	}
	return ss.str();
}

Calculator::~Calculator() {
	while (!DoubleOutput.empty()) {
		DoubleOutput.pop();
	}
	while (!Output.empty()) {
		Output.pop();
	}
	while (!Stack.empty()) {
		Stack.pop();
	}
}


void Calculator::ThrowError(const std::string Data) {
	HasError = true;
	ErrorData.Data = Data;
}


void Calculator::ThrowError(const std::string Data, const CharData Char) {
	HasError = true;
	ErrorData.HasChar = true;
	ErrorData.Data = Data;
	ErrorData.Char = Char;
}

void Calculator::ConvertBigIntegerToDouble() {
	std::queue<double> temp;
	while (!Output.empty()) {
		auto top = Output.top();
		auto dbl = atof(top.c_str());
		temp.push(dbl);
		Output.pop();
	}
	while (!temp.empty()) {
		auto top = temp.front();
		DoubleOutput.push(top);
		temp.pop();
	}
}
