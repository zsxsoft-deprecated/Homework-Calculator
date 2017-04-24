#pragma once
#include <string>
#include <vector>
#include <algorithm>
template <int A, int B>
struct get_power {
	static const long value = A * get_power<A, B - 1>::value;
};
template <int A>
struct get_power<A, 0> {
	static const long value = 1;
};
template <int A>
struct plus {
	static const long value = get_power<10, A>::value - 1;
};
template <size_t _MaxInt = 4, int _MaxSave = plus<_MaxInt>::value>
class BigInteger {
public:

	template<typename U> friend std::istream& operator>> (std::istream &, BigInteger<_MaxInt, _MaxSave>&);
	template<typename U> friend std::ostream& operator<< (std::ostream &, BigInteger<_MaxInt, _MaxSave>&);

	template<typename U> friend bool operator!=(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs);
	template<typename U> friend bool operator==(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs);
	template<typename U> friend BigInteger<_MaxInt, _MaxSave> operator+(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs);
	template<typename U> friend BigInteger<_MaxInt, _MaxSave> operator-(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs);
	template<typename U> friend BigInteger<_MaxInt, _MaxSave> operator*(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs);
	template<typename U> friend BigInteger<_MaxInt, _MaxSave> operator/(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs);

	std::vector<int> Data;

	BigInteger();
	explicit BigInteger(const char*);
	explicit BigInteger(std::string);
	explicit BigInteger(int);
	explicit BigInteger(BigInteger<_MaxInt, _MaxSave>*);
	std::string ToString() const;

	bool IsPositive() const;
	void SetLength(int);
	void FixZero();
	void RemoveUselessZero();
	int Length() const;

	BigInteger<_MaxInt, _MaxSave> Abs() const;
	BigInteger<_MaxInt, _MaxSave> Factorial() const;
	BigInteger<_MaxInt, _MaxSave> Sqrt() const;
	BigInteger<_MaxInt, _MaxSave> Pow(const BigInteger<_MaxInt, _MaxSave>&) const;

	BigInteger<_MaxInt, _MaxSave> operator= (const char*);
	BigInteger<_MaxInt, _MaxSave> operator= (int);
	BigInteger<_MaxInt, _MaxSave> operator= (const std::string);

	bool operator> (const BigInteger<_MaxInt, _MaxSave>&) const;
	bool operator>= (const BigInteger<_MaxInt, _MaxSave>&) const;
	bool operator< (const BigInteger<_MaxInt, _MaxSave>&) const;
	bool operator<= (const BigInteger<_MaxInt, _MaxSave>&) const;
	bool operator== (const BigInteger<_MaxInt, _MaxSave>&) const;
	bool operator!= (const BigInteger<_MaxInt, _MaxSave>&) const;

	bool operator> (const int&) const;
	bool operator>= (const int&) const;
	bool operator< (const int&) const;
	bool operator<= (const int&) const;
	bool operator== (const int&) const;
	bool operator!= (const int&) const;

	BigInteger<_MaxInt, _MaxSave> operator+ (const BigInteger<_MaxInt, _MaxSave>&) const;
	BigInteger<_MaxInt, _MaxSave> operator+ (const int) const;
	BigInteger<_MaxInt, _MaxSave> operator++ ();
	BigInteger<_MaxInt, _MaxSave> operator++ (int);
	BigInteger<_MaxInt, _MaxSave> operator+= (const BigInteger<_MaxInt, _MaxSave>&);
	BigInteger<_MaxInt, _MaxSave> operator- (const BigInteger<_MaxInt, _MaxSave>&) const;
	BigInteger<_MaxInt, _MaxSave> operator- (const int) const;
	BigInteger<_MaxInt, _MaxSave> operator- () const;
	BigInteger<_MaxInt, _MaxSave> operator-- ();
	BigInteger<_MaxInt, _MaxSave> operator-- (int);
	BigInteger<_MaxInt, _MaxSave> operator-= (const BigInteger<_MaxInt, _MaxSave>&);
	BigInteger<_MaxInt, _MaxSave> operator* (const BigInteger<_MaxInt, _MaxSave>&) const;
	BigInteger<_MaxInt, _MaxSave> operator* (const int) const;
	BigInteger<_MaxInt, _MaxSave> operator*= (const BigInteger<_MaxInt, _MaxSave>&);
	BigInteger<_MaxInt, _MaxSave> operator*= (const int);
	BigInteger<_MaxInt, _MaxSave> operator/ (const BigInteger<_MaxInt, _MaxSave>&) const;
	BigInteger<_MaxInt, _MaxSave> operator/ (const int) const;
	BigInteger<_MaxInt, _MaxSave> operator/= (const BigInteger<_MaxInt, _MaxSave>&);
	BigInteger<_MaxInt, _MaxSave> operator% (const BigInteger<_MaxInt, _MaxSave>&) const;


	std::pair<size_t, int> GetClassInfo() const;

	void Clean();
	~BigInteger();

private:
	bool positive = true;
};

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave>::BigInteger() {}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave>::BigInteger(const char *num) {
	*this = num;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave>::BigInteger(std::string num) {
	*this = num.c_str();
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave>::BigInteger(int num) {
	*this = num;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave>::BigInteger(BigInteger<_MaxInt, _MaxSave>* num) {
	this->positive = num->IsPositive();
	copy(num->Data.begin(), num->Data.end(), Data.begin());
}

template <size_t _MaxInt, int _MaxSave>
std::string BigInteger<_MaxInt, _MaxSave>::ToString() const {
	std::string res = "";
	auto len = Length();
	for (auto i = 0; i < len; i++) {
		auto part = std::to_string(Data[i]);
		auto length = static_cast<int>(part.length());
		while (length < _MaxInt && i < len - 1) part = "0" + part, length++;
		res = part + res;
	}
	if (res == "") res = "0";
	if (!IsPositive() && res != "0") res = "-" + res;
	return res;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator=(const char* in) {

	auto l = static_cast<int>(strlen(in));
	auto len = l / _MaxInt;
	auto start = 0;
	auto maxInt = static_cast<int>(_MaxInt);

	if (l % maxInt) len++;
	if (in[0] == '-') {
		positive = false;
		start++;
	}
	else if (in[0] == '+') {
		start++;
	}
	SetLength(len);
	Data.clear();

	for (auto i = l - 1; i >= start; i -= maxInt) {
		auto t = 0;
		auto k = i - maxInt + 1;
		if (k < start) k = start;
		for (auto j = k; j <= i; j++) {
			if (in[j] < '0' || in[j] > '9') throw std::exception("Invalid input"); // @TODO
			t = t * 10 + in[j] - '0';
		}
		Data.push_back(t > 0 ? t : -t);
	}
	RemoveUselessZero();
	FixZero();
	return *this;

}

template <size_t _MaxInt, int _MaxSave>
std::istream &operator>>(std::istream &in, BigInteger<_MaxInt, _MaxSave>&num) {
	std::string str;
	in >> str;
	num = str;
	return in;
}

template <size_t _MaxInt, int _MaxSave>
std::ostream &operator<<(std::ostream &out, BigInteger<_MaxInt, _MaxSave>&num) {
	out << num.ToString();
	return out;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator=(int num) {
	char temp[20];
	sprintf_s(temp, 20, "%d", num);
	*this = temp;
	return *this;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator=(const std::string num) {
	const char *tmp;
	tmp = num.c_str();
	*this = tmp;
	return *this;
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::IsPositive()  const {
	return positive;
}

template <size_t _MaxInt, int _MaxSave>
void BigInteger<_MaxInt, _MaxSave>::SetLength(int n) {
	Data.resize(n);
}

template <size_t _MaxInt, int _MaxSave>
int BigInteger<_MaxInt, _MaxSave>::Length() const {
	return Data.size();
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::Abs() const {
	auto ret = BigInteger<_MaxInt, _MaxSave>(*this);
	ret.positive = true;
	return ret;
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator>(const BigInteger<_MaxInt, _MaxSave>& num) const {
	return num < *this;
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator<=(const BigInteger<_MaxInt, _MaxSave>& num) const {
	return !(*this > num);
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator>=(const BigInteger<_MaxInt, _MaxSave>& num) const {
	return !(*this < num);
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator<(const BigInteger<_MaxInt, _MaxSave>& num) const {
	if (IsPositive() != num.IsPositive()) return num.IsPositive();
	auto len = Length();
	if (Length() != num.Length())
		return Length() < num.Length();
	for (auto i = len - 1; i >= 0; i--)
		if (Data[i] != num.Data[i])
			return positive ? (Data[i] < num.Data[i]) : (!(Data[i] < num.Data[i]));
	return !positive;
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator!=(const BigInteger<_MaxInt, _MaxSave>& num) const {
	return !(*this == num);
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator==(const BigInteger<_MaxInt, _MaxSave>& num) const {
	if (IsPositive() != num.IsPositive()) return false;
	if (Length() != num.Length()) return false;
	auto len = Length();
	for (auto i = 0; i < len; i++) {
		if (Data[i] != num.Data[i]) return false;
	}
	return true;
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator<(const int& num) const {
	return *this < BigInteger<_MaxInt, _MaxSave>(num);
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator>(const int& num) const {
	return *this > BigInteger<_MaxInt, _MaxSave>(num);
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator<=(const int& num) const {
	return !(*this > num);
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator>=(const int& num) const {
	return !(*this < num);
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator!=(const int& num) const {
	return !(*this == num);
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator+(const BigInteger<_MaxInt, _MaxSave>& num) const {

	if (this->IsPositive() && !num.IsPositive()) {
		auto ret = BigInteger<_MaxInt, _MaxSave>(num);
		ret.positive = true;
		return ret > *this ? ret - *this : *this - ret;
	}
	if (!this->IsPositive() && num.IsPositive()) {
		auto ret = BigInteger<_MaxInt, _MaxSave>(*this);
		ret.positive = true;
		return ret > num ? ret - num : num - ret;
	}
	auto ret = BigInteger<_MaxInt, _MaxSave>(*this);
	auto numLength = num.Length();
	auto length = std::max(numLength, Length()) + 1;
	ret.SetLength(length);
	for (auto i = 0; i < length; i++) {
		ret.Data[i] += (i >= numLength ? 0 : num.Data[i]);
		if (ret.Data[i] > _MaxSave) {
			++ret.Data[i + 1];
			ret.Data[i] -= _MaxSave + 1;
		}
	}
	if (ret.Data[length - 1] == 0) ret.SetLength(length - 1);
	ret.FixZero();
	return ret;
}

template <size_t _MaxInt, int _MaxSave>
bool BigInteger<_MaxInt, _MaxSave>::operator==(const int& num) const {
	return *this == BigInteger<_MaxInt, _MaxSave>(num);
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator+(const int num) const {
	return *this + BigInteger<_MaxInt, _MaxSave>(num);
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator++() {
	*this = *this + 1;
	return *this;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator++(int) {
	auto old = *this;
	++(*this);
	return old;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator+=(const BigInteger<_MaxInt, _MaxSave>& num) {
	*this = *this + num;
	return *this;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator-(const BigInteger<_MaxInt, _MaxSave>& num) const {

	auto flag = true;
	BigInteger<_MaxInt, _MaxSave> t1, t2;
	if (this->IsPositive() && !num.IsPositive()) {
		return *this + -num;
	}
	if (num.IsPositive() && !this->IsPositive()) {
		return -(-*this + num);
	}
	if (!this->IsPositive() && !num.IsPositive()) {
		t1 = -*this;
		t2 = -num;
		flag = false;
	}
	else {
		t1 = *this;
		t2 = num;
	}

	if (t2 > t1) {
		std::swap(t1, t2);
		flag = !flag;
	}

	auto length = t1.Length();
	t1.SetLength(length);
	t2.SetLength(length);
	for (auto i = 0; i < length; i++) {
		if (t1.Data[i] < t2.Data[i]) {
			auto j = i + 1;
			while (t1.Data[j] == 0)	j++;
			--t1.Data[j--];
			while (j > i)
				t1.Data[j--] += _MaxSave;
			t1.Data[i] += _MaxSave + 1 - t2.Data[i];
		}
		else
			t1.Data[i] -= t2.Data[i];
	}

	if (!flag) t1.positive = false;
	t1.FixZero();
	t1.RemoveUselessZero();
	return t1;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator-(const int num) const {
	return *this - static_cast<BigInteger<_MaxInt, _MaxSave>>(num);
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator-() const {
	auto ret = BigInteger<_MaxInt, _MaxSave>(*this);
	ret.positive = !ret.IsPositive();
	return ret;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator--() {
	*this = *this - 1;
	return *this;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator--(int) {
	auto old = *this;
	--(*this);
	return old;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator-=(const BigInteger<_MaxInt, _MaxSave>& num) {
	*this = *this - num;
	return *this;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator*(const BigInteger<_MaxInt, _MaxSave>& num) const {
	BigInteger<_MaxInt, _MaxSave> ret;
	int temp, temp1;
	auto length = Length();
	auto rvalueLength = num.Length();
	auto retLength = length + rvalueLength;
	ret.SetLength(retLength);
	for (auto i = 0; i < length; i++) {
		int j, next = 0;
		for (j = 0; j < rvalueLength; j++) {
			temp = Data[i] * num.Data[j] + ret.Data[i + j] + next;
			if (temp > _MaxSave) {
				temp1 = temp - temp / (_MaxSave + 1) * (_MaxSave + 1);
				next = temp / (_MaxSave + 1);
				ret.Data[i + j] = temp1;
			}
			else {
				next = 0;
				ret.Data[i + j] = temp;
			}
		}
		if (next != 0)
			ret.Data[i + j] = next;
	}
	ret.RemoveUselessZero();

	if ((IsPositive() && !num.IsPositive()) || (!IsPositive() && num.IsPositive())) {
		ret.positive = false;
	}
	ret.FixZero();
	return ret;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator*(const int num)const {
	auto x = BigInteger<_MaxInt, _MaxSave>(num);
	auto z = *this;
	return x * z;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator*=(const BigInteger<_MaxInt, _MaxSave>& num) {
	*this = *this * num;
	return *this;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator*=(const int num) {
	*this = *this * num;
	return *this;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator/(const BigInteger<_MaxInt, _MaxSave>& num) const {

	auto positive = true;
	auto t1 = *this, t2 = num;
	auto t1Len = Length(), t2Len = t2.Length();
	auto times = t1Len - t2Len;

	if ((IsPositive() && !num.IsPositive())) {
		t2 = -t2;
		positive = false;
	}
	else if (!IsPositive() && num.IsPositive()) {
		t1 = -t1;
		positive = false;
	}
	else if (!IsPositive() && !num.IsPositive()) {
		t2 = -t2;
		t1 = -t1;
	}

	if (t1 == t2) return BigInteger<_MaxInt, _MaxSave>(1);
	if (t1 < t2) return BigInteger<_MaxInt, _MaxSave>(0);
	if (t2 == 0) throw std::exception("paramter2 equals to 0");

	BigInteger<_MaxInt, _MaxSave> ret;
	if (!positive) ret.positive = false;
	ret.SetLength(t1.Length() + 1);
	t2.SetLength(times + t2.Length());
	for (auto i = t1Len - 1; i >= 0; i--) {
		if (i >= times) t2.Data[i] = t2.Data[i - times];
		else t2.Data[i] = 0;
	}

	t2Len = t1Len;
	for (auto i = 0; i <= times; i++) {
		while (t1 >= t2) {
			t1 -= t2;
			++ret.Data[times - i];
		}
		for (auto j = 1; j < t2Len; j++) {
			t2.Data[j - 1] = t2.Data[j];
		}
		t2.Data[--t2Len] = 0;
		t2.RemoveUselessZero();
	}

	auto retLength = ret.Length();
	for (auto i = 0; i < retLength - 1; i++) {
		if (ret.Data[i] >= _MaxSave) {
			ret.Data[i + 1] += ret.Data[i] / _MaxSave;
			ret.Data[i] %= _MaxSave;
		}
	}
	ret.RemoveUselessZero();
	return ret;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator/(const int num) const {
	return *this / static_cast<BigInteger<_MaxInt, _MaxSave>>(num);
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator/=(const BigInteger<_MaxInt, _MaxSave>& num) {
	*this = *this / num;
	return *this;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::operator%(const BigInteger<_MaxInt, _MaxSave>& num) const {
	return *this - *this / num * num;
}

template <size_t _MaxInt, int _MaxSave>
std::pair<size_t, int> BigInteger<_MaxInt, _MaxSave>::GetClassInfo() const
{
	return std::pair<auto, auto>(_MaxInt, _MaxSave);
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::Pow(const BigInteger<_MaxInt, _MaxSave>& num) const {
	auto ret = BigInteger<_MaxInt, _MaxSave>(1);
	for (auto i = BigInteger<_MaxInt, _MaxSave>(0); i < num; ++i) ret = ret*(*this);
	return ret;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::Factorial() const {
	auto ret = BigInteger<_MaxInt, _MaxSave>(1);
	for (auto i = BigInteger<_MaxInt, _MaxSave>(1); i <= *this; ++i) ret *= i;
	return ret;
}

template <size_t _MaxInt, int _MaxSave>
void BigInteger<_MaxInt, _MaxSave>::Clean() {
	Data.clear();
}

template <size_t _MaxInt, int _MaxSave>
void BigInteger<_MaxInt, _MaxSave>::RemoveUselessZero() {
	while (Data.size() > 0 && Data[Data.size() - 1] == 0) {
		Data.pop_back();
	}
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> BigInteger<_MaxInt, _MaxSave>::Sqrt()const {
	if (*this < 0)
		return BigInteger<_MaxInt, _MaxSave>(-1);
	if (*this <= 1)
		return *this;

	auto l = BigInteger<_MaxInt, _MaxSave>(0), r = *this;
	BigInteger<_MaxInt, _MaxSave> mid;
	while (r - l > 1) {
		mid = (l + r) / 2;
		if (mid * mid > *this)
			r = mid;
		else
			l = mid;
	}
	return l;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave>::~BigInteger<_MaxInt, _MaxSave>() {}

template <size_t _MaxInt, int _MaxSave>
void BigInteger<_MaxInt, _MaxSave>::FixZero() {
	if (this->Length() == 1 && this->Data[0] == 0 && !this->IsPositive()) {
		this->positive = true;
	}
}

template <size_t _MaxInt, int _MaxSave>
bool operator==(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs) {
	return rhs == num;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> operator+(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs) {
	return rhs + num;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> operator-(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs) {
	return rhs - num;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> operator*(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs) {
	return rhs * num;
}

template <size_t _MaxInt, int _MaxSave>
BigInteger<_MaxInt, _MaxSave> operator/(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs) {
	return rhs / num;
}

template <size_t _MaxInt, int _MaxSave>
bool operator!=(const int& num, const BigInteger<_MaxInt, _MaxSave>& rhs) {
	return rhs != num;
}