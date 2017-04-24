#include "stdafx.h"
#include "HugeIntTest.h"
#include "HugeInt.h"
#include <cassert>
#include <sstream>
#include <iostream>

void HugeIntTest::Test()
{
	// 基本功能
	{
		auto a = static_cast<HugeInt>(1000);
		auto b = HugeInt("100");
		auto c = HugeInt(-2333);
		auto d = HugeInt(1000) - 900;
		std::stringstream stream;

		stream << a << " " << b << c;
		assert(stream.str() == "1000 100-2333");
	}
	// 不进位的正数四则运算
	{
		assert(HugeInt(100) * 10 == HugeInt(1000));
		assert(HugeInt(100) + HugeInt(900) == 1000);
		assert(HugeInt(1000) / 10 == HugeInt(100));
		assert(HugeInt(1000) - 900 == 100);
	}
	// 不进位的自然数四则运算
	{
		assert(HugeInt("1") - HugeInt("-2") == HugeInt("3"));
		assert(HugeInt("1") - HugeInt("2") == HugeInt("-1"));
		assert(HugeInt("-1") - HugeInt("2") == HugeInt("-3"));
		assert(HugeInt("-1") - HugeInt("-2") == HugeInt("1"));
		assert(HugeInt("2") - HugeInt("-1") == HugeInt("3"));
		assert(HugeInt("2") - HugeInt("1") == HugeInt("1"));
		assert(HugeInt("-2") - HugeInt("1") == HugeInt("-3"));
		assert(HugeInt("-2") - HugeInt("-1") == HugeInt("-1"));
		assert(HugeInt("2") * HugeInt("1") == HugeInt("2"));
		assert(HugeInt("2") / HugeInt("1") == HugeInt("2"));
		assert(HugeInt("-2") * HugeInt("-1") == HugeInt("2"));
		assert(HugeInt("-2") / HugeInt("-1") == HugeInt("2"));
		assert(HugeInt("2") * HugeInt("-1") == HugeInt("-2"));
		assert(HugeInt("2") / HugeInt("-1") == HugeInt("-2"));
		assert(HugeInt("-2") * HugeInt("1") == HugeInt("-2"));
		assert(HugeInt("-2") / HugeInt("1") == HugeInt("-2"));
	}
	// 带进位的正数四则运算
	{
		assert(HugeInt("2312321") - HugeInt("1251521521521") == HugeInt("-1251519209200"));
		assert(HugeInt("2333333333333333333333") + HugeInt("999999999999999999999999999") == HugeInt("1000002333333333333333333332"));
		assert(HugeInt("2333333333333333333333") * HugeInt("999999999999999999999999999") == HugeInt("2333333333333333333332999997666666666666666666667"));
		assert(HugeInt("7042432142142214") / HugeInt("1048432642142214") == HugeInt(6));
	}
	// 带进位的自然数四则运算
	{
		assert(HugeInt("100000000") - HugeInt("31232142421") == -(HugeInt("-100000000") + HugeInt("31232142421")));
		assert(HugeInt("-100000000") + HugeInt("31232142421") == HugeInt("31132142421"));
		assert(HugeInt("-100000000") + HugeInt("-31232142421") == HugeInt("-31332142421"));
		assert(HugeInt("-32132321") * HugeInt("-313232321") == HugeInt("10064881485947041"));
	}
	// 比较
	{
		assert(1 == HugeInt("1"));
		assert(HugeInt("1") == 1);
		assert(HugeInt(-1) == -1);
		assert(HugeInt("1") == HugeInt(1));
		assert(HugeInt("1") >= HugeInt(1));
		assert(HugeInt("1") <= HugeInt(1));
		assert(HugeInt("2") != HugeInt(1));
		assert(HugeInt("2") >= HugeInt(1));
		assert(HugeInt("2") > HugeInt(1));
		assert(HugeInt("-1") < HugeInt(1));
		assert(HugeInt("-1") <= HugeInt(1));
		assert(HugeInt("10000000000000000000000000000000000000") > HugeInt("1"));
		assert(HugeInt("0") < HugeInt("1"));
		assert(HugeInt("0") == HugeInt("-0"));
		assert(HugeInt("-0") < HugeInt("1"));
		assert(HugeInt("-1") < HugeInt("1"));
		assert(HugeInt("-1") <= HugeInt("1"));
		assert(HugeInt("1") > HugeInt("-1"));
		assert(HugeInt("1") > -1);
	}
	// 连续运算
	{
		assert(
			HugeInt("123456789012345678901234567890") + HugeInt("123456789012345678901234567890") +
			HugeInt("123456789012345678901234567890") + HugeInt("123456789012345678901234567890") +
			HugeInt("123456789012345678901234567890") == HugeInt("123456789012345678901234567890") * 5
		);
		assert(
			HugeInt("3452345232343242342342352356235324233262346742352342341242345235235623563242342352346756856532412432423234234324") +
			HugeInt("-4128973621893612879356281793561278954128903789120739012863427891532789153278915378912564789125") -
			HugeInt("51351234123421312312412412412412214512521512512512512") +
			HugeInt("-3465423895723489564238946237894569823564893684392464234") -
			HugeInt("-345623456234623465423524354235234523454235")
			==
			HugeInt("3452345232343242338213378734341711353906064948791063387109924670985037985125819336787126144739773873548287922688")
		);
		assert(HugeInt("1") * HugeInt("10") * HugeInt("1000") + HugeInt("1") == HugeInt("10001"));
		assert(HugeInt("1") * HugeInt("10") + HugeInt("1000") * HugeInt("1") == HugeInt("1010"));
	}
	// 运算符
	{
		auto a = HugeInt("1");
		assert(a++ == HugeInt("1"));
		assert(++a == HugeInt("3"));
		assert(a-- == HugeInt("3"));
		assert(--a == HugeInt("1"));
	}
	// 高级功能
	{
		assert(HugeInt("12341233195117467310308") % HugeInt("100000007") == 4124124);
		assert(HugeInt("30").Factorial() == HugeInt("265252859812191058636308480000000"));
		assert(HugeInt("2").Pow(HugeInt("128")) == HugeInt("340282366920938463463374607431768211456"));
#ifdef _DEBUG
		std::cout << "Calculating sqrt(49382617284)" << std::endl;
		assert(HugeInt("49382617284").Sqrt() == HugeInt("222222"));
		std::cout << "Calculated!" << std::endl;
#endif
	}


}
