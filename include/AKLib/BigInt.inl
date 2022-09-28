
#include <string.h>

NSB(AKLib)

inline unsigned char UAddC(unsigned char a, unsigned char b, bool& carry)
{
	unsigned char bigC = (a & 1) + (b & 1) + (carry ? 1 : 0);
	unsigned char res = (a >> 1) + (b >> 1) + (bigC >> 1);
	carry = (res & (1 << 7)) == 0 ? false : true;
	res = (res << 1) | (bigC & 1);
	return res;
}

inline void UMul(unsigned char& resHigh, unsigned char& resLow)
{
	unsigned char low1 = 0x0f & resHigh;
	unsigned char high1 = (0xf0 & resHigh) >> 4;
	unsigned char low2 = 0x0f & resLow;
	unsigned char high2 = (0xf0 & resLow) >> 4;

	unsigned char ll = low1 * low2;
	unsigned char hh = high1 * high2;

	unsigned char lh = low1 * high2;
	unsigned char hl = high1 * low2;

	unsigned char lhh = (lh & 0xf0) >> 4;
	unsigned char lhl = (lh & 0x0f) << 4;

	unsigned char hlh = (hl & 0xf0) >> 4;
	unsigned char hll = (hl & 0x0f) << 4;

	unsigned char resL = lhl;
	unsigned char resH = lhh;
	resH += hlh;
	bool carry = false;
	resL = UAddC(resL, hll, carry);
	if (carry) {
		++resH;
		carry = false;
	}
	resL = UAddC(resL, ll, carry);
	resH = UAddC(resH, hh, carry);
	resHigh = resH;
	resLow = resL;
}

inline unsigned char UDiv(unsigned char dividend, unsigned char divisor, unsigned char& prevRem)
{
	if (divisor == 0) return 0;

	unsigned char quot = 0, rem = 0;

	if (prevRem)
	{
		quot = (unsigned char)-1 / divisor;
		rem = (unsigned char)-1 % divisor + 1;
		if (rem == divisor)
		{
			++quot;
			rem = 0;
		}

		quot *= prevRem;
		rem *= prevRem;
	}

	quot += dividend / divisor;
	rem += dividend % divisor;

	quot += rem / divisor;
	rem %= divisor;

	prevRem = rem;
	return quot;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT>::BigInt(long long n)
{
	BigInt<BYTE_COUNT>& a = *this;

	bool neg = n < 0;

	for (int i = 0; i < BYTE_COUNT; ++i)
	{
		a.number[i] = neg ? -1 : 0;
	}

	for (int i = 0; i < BYTE_COUNT && i < 8; ++i)
	{
		a.number[i] = (n >> (i * 8)) & 0xff;
	}

	if (neg) a.number[BYTE_COUNT - 1] |= 1 << 7;
	else a.number[BYTE_COUNT - 1] &= ~(1 << 7);
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT>::BigInt(const char* s) 
{
	BigInt<BYTE_COUNT>& a = *this;

	for (int i = 0; i < BYTE_COUNT; ++i)
	{
		a.number[i] = 0;
	}

	if (s == NULL) 
		return;
	bool neg = s[0] == '-';
	size_t size = strlen(s);

	if (neg && s[1] == '0') return;
	else if (s[0] == '0') return;

	for (int i = neg ? 1 : 0; i < size; ++i) 
	{
		a *= 10;
		a += s[i] - '0';
	}
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline BigInt<BYTE_COUNT>::BigInt(const BigInt<BYTE_COUNT_B>& other)
{
	int count = BYTE_COUNT > BYTE_COUNT_B ? BYTE_COUNT_B : BYTE_COUNT;
	bool sign = other.IsNegative();

	for (int i = 0; i < count; ++i)
		number[i] = other.number[i];

	if (sign) number[count - 1] &= ~(1 << 7); // discard the copied sign bit if the other number was signed

	if (count < BYTE_COUNT)
	{
		for (int i = count; i < BYTE_COUNT; ++i)
		{
			number[i] = sign ? -1 : 0;
		}
	}
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator = (const BigInt<BYTE_COUNT_B>& other)
{
	int count = BYTE_COUNT > BYTE_COUNT_B ? BYTE_COUNT_B : BYTE_COUNT;
	bool sign = other.IsNegative();

	for (int i = 0; i < count; ++i)
		number[i] = other.number[i];

	if (sign) number[count - 1] &= ~(1 << 7); // discard the copied sign bit if the other number was signed

	if (count < BYTE_COUNT)
	{
		for (int i = count; i < BYTE_COUNT; ++i)
		{
			number[i] = sign ? -1 : 0;
		}
	}
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator = (const char* s) 
{
	BigInt<BYTE_COUNT>& a = *this;

	for (int i = 0; i < BYTE_COUNT; ++i)
	{
		a.number[i] = 0;
	}

	if (s == NULL) return a;
	bool neg = s[0] == '-';
	size_t size = strlen(s);

	if (neg && s[1] == '0') return a;
	else if (s[0] == '0') return a;

	for (int i = neg ? 1 : 0; i < size; ++i)
	{
		a *= 10;
		a += s[i] - '0';
	}
}

template <int BYTE_COUNT>
bool BigInt<BYTE_COUNT>::IsNegative() const
{
	return number[BYTE_COUNT - 1] >> 7;
}

template <int BYTE_COUNT>
inline long long BigInt<BYTE_COUNT>::ToInt() const
{
	long long ret = 0;

	for (int i = 0; i < BYTE_COUNT && i < 8; ++i)
	{
		ret |= ((long long)number[i] << (i * 8));
	}

	return ret;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::Max()
{
	BigInt<BYTE_COUNT> a;

	for (int i = 0; i < BYTE_COUNT; ++i)
		a.number[i] = 0xff;

	a.number[BYTE_COUNT - 1] &= ~(1 << 7);
	return a;
}

template <int BYTE_COUNT>
void BigInt<BYTE_COUNT>::AddAt(unsigned char* buffer, int idx, unsigned char value)
{
	bool carry = false;
	buffer[idx] = UAddC(buffer[idx], value, carry);

	while (carry)
	{
		++idx;
		if (buffer[idx] != (unsigned long long) - 1)
		{
			++buffer[idx];
			break;
		}

		buffer[idx] = 0;
	}
}

template <int BYTE_COUNT>
inline void BigInt<BYTE_COUNT>::ShiftL()
{
	char carry = 0;
	for (int i = 0; i < BYTE_COUNT; ++i)
	{
		unsigned char temp = number[i];
		number[i] <<= 1;
		if (carry) number[i] |= carry;
		carry = temp & 0x80 ? 1 : 0;
	}
}

template <int BYTE_COUNT>
inline void BigInt<BYTE_COUNT>::ShiftR()
{
	char carry = 0;
	for (int i = BYTE_COUNT - 1; i >= 0; --i)
	{
		unsigned char temp = number[i];
		number[i] >>= 1;
		if (carry) number[i] |= carry;
		carry = temp & 1 ? 0x80 : 0;
	}
}

template <int BYTE_COUNT>
inline void BigInt<BYTE_COUNT>::FixSign()
{
	bool neg = IsNegative();
	unsigned char comp = 0;

	if (neg) comp = -1;

	for (size_t i = BYTE_COUNT - 1; i != 0; --i) 
	{
		if (number[i] != comp) break;
		bool sign = number[i - 1] >> 7;
		if (neg != sign) number[i] = comp;
	}
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator += (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;

	int count = BYTE_COUNT > BYTE_COUNT_B ? BYTE_COUNT_B : BYTE_COUNT;

	bool aNeg = a.IsNegative();
	bool bNeg = b.IsNegative();

	bool carry = false;

	for (int i = 0; i < count; ++i)
		a.number[i] = UAddC(a.number[i], b.number[i], carry);

	int j = count;

	// continues the carry if the number being added is smaller 
	// also mostly helps if a negative number is added
	while (carry && j < BYTE_COUNT) 
	{
		a.number[j] = UAddC(a.number[j], 0, carry);
		++j;
	}

	if ((aNeg == bNeg) && (aNeg != a.IsNegative()))
	{
		if (aNeg) a.number[BYTE_COUNT - 1] |= (unsigned char)1 << 7;
		else a.number[BYTE_COUNT - 1] &= ~((unsigned char)1 << 7);
	}
	
	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator -= (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<BYTE_COUNT_B> bb(b);

	bb = -bb;
	a += bb;
	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator *= (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<BYTE_COUNT_B> bb(b);

	bool neg = a.IsNegative() != b.IsNegative();

	if (a.IsNegative()) a = -a;
	if (b.IsNegative()) bb = -bb;

	unsigned char buffer[BYTE_COUNT * BYTE_COUNT_B] = { 0 };

	for (int j = BYTE_COUNT_B - 1; j >= 0; --j)
	{
		for (int i = BYTE_COUNT - 1; i >= 0; --i)
		{
			unsigned char high = a.number[i];
			unsigned char low = bb.number[j];
			UMul(high, low);
			a.AddAt(buffer, i + j, low);
			a.AddAt(buffer, i + j + 1, high);
		}
	}

	for (int i = 0; i < BYTE_COUNT; ++i)
		a.number[i] = buffer[i];

	if (neg) a = -a;

	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator /= (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<BYTE_COUNT_B> bb(b);
	BigInt<BYTE_COUNT> quot;

	if (bb == 0)
	{
		a = 0;
		return a;
	}

	bool neg = (a.IsNegative() != b.IsNegative());

	if (a.IsNegative()) a = -a;
	if (b.IsNegative()) bb = -bb;

	if (a < bb)
	{
		if (neg) a = -a;
		a = 0;
		return a;
	}

	BigInt<BYTE_COUNT> dvsr(bb);

	while (a >= bb)
	{
		bb.ShiftL(); // direct call to increase performance
	}

	while (bb > a)
	{
		bb.ShiftR();
	}

	while (bb >= dvsr)
	{
		quot.ShiftL();
		if (bb <= a)
		{
			quot.number[0] |= 1;
			a -= bb;
		}
		bb.ShiftR();
	}

	if (neg)
	{
		quot = -quot;
		a = -a;
	}

	for (int i = 0; i < BYTE_COUNT; ++i)
		a.number[i] = quot.number[i];

	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator %= (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<BYTE_COUNT_B> bb(b);

	if (a.IsNegative()) a = -a;
	if (b.IsNegative()) bb = -bb;
	
	a -= bb * (a / bb);
	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator <<= (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<BYTE_COUNT_B> bb(b);

	for (int i = 0; i < bb; ++i)
	{
		a.ShiftL();
	}

	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator >>= (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<BYTE_COUNT_B> bb(b);

	for (int i = 0; i < bb; ++i)
	{
		a.ShiftR();
	}

	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator + (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT> a(*this);
	a += b;
	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator - (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT> a(*this);
	a -= b;
	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator * (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT> a(*this);
	a *= b;
	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator / (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT> a(*this);
	a /= b;
	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator % (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT> a(*this);
	a %= b;
	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator >> (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT> a(*this);
	a >>= b;
	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator << (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT> a(*this);
	a <<= b;
	return a;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline bool BigInt<BYTE_COUNT>::operator < (const BigInt<BYTE_COUNT_B> b)
{
	const int count = BYTE_COUNT < BYTE_COUNT_B ? BYTE_COUNT_B : BYTE_COUNT;
	BigInt<count> a(*this);
	BigInt<count> bb(b);

	if (a.IsNegative() && !b.IsNegative()) return true;

	for (int i = count - 1; i >= 0; --i)
	{
		if (a.number[i] < bb.number[i]) return true;
		if (a.number[i] != bb.number[i]) return false;
	}

	return false;
}
template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline bool BigInt<BYTE_COUNT>::operator > (const BigInt<BYTE_COUNT_B> b)
{
	const int count = BYTE_COUNT < BYTE_COUNT_B ? BYTE_COUNT_B : BYTE_COUNT;
	BigInt<count> a(*this);
	BigInt<count> bb(b);

	if (!a.IsNegative() && b.IsNegative()) return true;

	for (int i = count - 1; i >= 0; --i)
	{
		if (a.number[i] > bb.number[i]) return true;
		if (a.number[i] != bb.number[i]) return false;
	}

	return false;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline bool BigInt<BYTE_COUNT>::operator <= (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;
	return !(a > b);
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline bool BigInt<BYTE_COUNT>::operator >= (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;
	return !(a < b);
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline bool BigInt<BYTE_COUNT>::operator == (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;

	for (int i = BYTE_COUNT - 1; i >= 0; --i)
	{
		if (!(a.number[i] == b.number[i])) return false;
	}

	return true;
}

template <int BYTE_COUNT>
template <int BYTE_COUNT_B>
inline bool BigInt<BYTE_COUNT>::operator != (const BigInt<BYTE_COUNT_B> b)
{
	BigInt<BYTE_COUNT>& a = *this;
	return !(a == b);
}

template <int BYTE_COUNT>
BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator ~ ()
{
	BigInt<BYTE_COUNT> a(*this);
	for (int i = 0; i < BYTE_COUNT; ++i)
		a.number[i] = ~a.number[i];
	return a;
}

template <int BYTE_COUNT>
BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator - ()
{
	BigInt<BYTE_COUNT> a(*this);
	a = ~a + 1;
	return a;
}

template <int BYTE_COUNT>
BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator ++ ()
{
	BigInt<BYTE_COUNT>& a = *this;
	a += 1;
	return a;
}

template <int BYTE_COUNT>
BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator -- ()
{
	BigInt<BYTE_COUNT>& a = *this;
	a -= 1;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator += (const long long b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<8> bb(b);
	a += bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator -= (const long long b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<8> bb(b);
	a -= bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator *= (const long long b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<8> bb(b);
	a *= bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator /= (const long long b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<8> bb(b);
	a /= bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator %= (const long long b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<8> bb(b);
	a %= bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator >>= (const long long b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<8> bb(b);
	a >>= bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT>& BigInt<BYTE_COUNT>::operator <<= (const long long b)
{
	BigInt<BYTE_COUNT>& a = *this;
	BigInt<8> bb(b);
	a <<= bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator + (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	a += bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator - (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	a -= bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator * (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	a *= bb;
	return a;
}

template <int BYTE_COUNT>
inline String BigInt<BYTE_COUNT>::ToString(int base)
{
	Vector<char> characters;

	BigInt<BYTE_COUNT> copy(*this);

	bool neg = copy.IsNegative();

	if (neg) copy = -copy;

	// checks if base is one of the supported bases and if not then defaults to base 10
	static const BigInt<BYTE_COUNT> divisor = base == 16 ? 16 : (base == 10 ? 10 : (base == 8 ? 8 : (base == 2 ? 2 : 10)));

	int i = 0;

	do
	{
		int idx = (copy % divisor).ToInt();
		char c = 0;
		if (idx >= 10) c = 'a' + idx - 10;
		else c = '0' + idx;
		copy /= divisor;
		characters.push_back(c);
		++i;
	} while (copy > 0);

	/*if (buffer[0] != '0' && i != 0)
	{
		--buffer[0];
	}*/

	Vector<char> ret;

	if (neg) ret.push_back('-');

	for (int i = characters.size() - 1; i >= 0; --i) 
	{
		ret.push_back(characters[i]);
	}
 
	return String(ret.data(), ret.size());
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator / (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	a /= bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator % (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	a %= bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator >> (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	a >>= bb;
	return a;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> BigInt<BYTE_COUNT>::operator << (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	a <<= bb;
	return a;
}

template <int BYTE_COUNT>
inline bool BigInt<BYTE_COUNT>::operator < (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	return a < bb;
}

template <int BYTE_COUNT>
inline bool BigInt<BYTE_COUNT>::operator > (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	return a > bb;
}

template <int BYTE_COUNT>
inline bool BigInt<BYTE_COUNT>::operator <= (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	return a <= bb;
}

template <int BYTE_COUNT>
inline bool BigInt<BYTE_COUNT>::operator >= (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	return a >= bb;
}

template <int BYTE_COUNT>
inline bool BigInt<BYTE_COUNT>::operator == (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	return a == bb;
}

template <int BYTE_COUNT>
inline bool BigInt<BYTE_COUNT>::operator != (const long long b)
{
	BigInt<BYTE_COUNT> a(*this);
	BigInt<8> bb(b);
	return a != bb;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> operator + (const long long a, const BigInt<BYTE_COUNT> b)
{
	BigInt<8> aa(a);
	return aa + b;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> operator - (const long long a, const BigInt<BYTE_COUNT> b)
{
	BigInt<8> aa(a);
	return aa - b;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> operator * (const long long a, const BigInt<BYTE_COUNT> b)
{
	BigInt<8> aa(a);
	return aa * b;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> operator / (const long long a, const BigInt<BYTE_COUNT> b)
{
	BigInt<8> aa(a);
	return aa / b;
}

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> operator % (const long long a, const BigInt<BYTE_COUNT> b)
{
	BigInt<8> aa(a);
	return aa % b;
}

template <int BYTE_COUNT>
inline bool operator < (const long long a, const BigInt<BYTE_COUNT> b)
{
	return BigInt<8>(a) < b;
}

template <int BYTE_COUNT>
inline bool operator > (const long long a, const BigInt<BYTE_COUNT> b)
{
	return BigInt<8>(a) > b;
}

template <int BYTE_COUNT>
inline bool operator <= (const long long a, const BigInt<BYTE_COUNT> b)
{
	return BigInt<8>(a) <= b;
}

template <int BYTE_COUNT>
inline bool operator >= (const long long a, const BigInt<BYTE_COUNT> b)
{
	return BigInt<8>(a) >= b;
}

template <int BYTE_COUNT>
inline bool operator == (const long long a, const BigInt<BYTE_COUNT> b)
{
	return BigInt<8>(a) == b;
}

template <int BYTE_COUNT>
inline bool operator != (const long long a, const BigInt<BYTE_COUNT> b)
{
	return BigInt<8>(a) != b;
}

NSE()
