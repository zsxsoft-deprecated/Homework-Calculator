// Calculator.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Calculator.h"
#include <iostream>
#include <fstream>
#include <string>
#include "InputParser.h"
#include "HugeIntTest.h"

int main(int argc, char* argv[]) {

	HugeIntTest::Test();
	if (argc <= 2) {
		InputParser::ParseInput(std::cin, std::cout, true);
	}
	else {
		std::fstream out;
		out.open(argv[argc - 1], std::ios::out);
		for (auto i = 1; i < argc - 1; i++) {
			auto fileName = argv[i];
			std::fstream file;
			file.open(fileName, std::ios::in);
			if (!file) {
				return 1;
			}
			InputParser::ParseInput(file, out, false);
			file.close();
		}
		out.close();
	}
	return 0;
}

