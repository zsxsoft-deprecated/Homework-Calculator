#include "stdafx.h"
#include "InputParser.h"
#include <fstream>


void InputParser::ParseInput(std::istream& in, std::ostream& out, bool prompt) {
	std::string input;
	bool settingRadix = false;
	auto radix = 10;
	while (!in.eof()) {
		if (prompt && !settingRadix) out << "> ";
		in >> input;
		if (input == "MODE") {
			settingRadix = true;
			continue;
		}
		if (settingRadix) {
			settingRadix = false;
			if (input == "10") {
				radix = 10;
				continue;
			}
			if (input == "2") {
				radix = 2;
				continue;
			}
			if (input == "8") {
				radix = 8;
				continue;
			}
			if (input == "16") {
				radix = 16;
				continue;
			}
		}

		auto ret = Calculator(input, radix);
		if (!ret.HasError) {
			out << ret.Data() << std::endl;
		}
		else {
			if (prompt) {
				out << ret.Error() << std::endl;
			}
			else {
				out << ret.ErrorData.Data;
				if (ret.ErrorData.HasChar) out << ret.ErrorData.Char.Position << ": " << ret.ErrorData.Char.Char;
				out << std::endl;
			}

		}
	}
}
