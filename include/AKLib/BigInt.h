#pragma once

#include "AKLib/common.h"
#include "AKLib/String.h"
#include "AKLib/Vector.h"

NSB(AKLib)

template <int BYTE_COUNT>
class BigInt
{
public:
	unsigned char number[BYTE_COUNT] = { 0 };

	inline BigInt(long long n = 0);
	inline BigInt(const char* s);
	template <int BYTE_COUNT_B>
	inline BigInt(const BigInt<BYTE_COUNT_B>& other);
	template <int BYTE_COUNT_B>
	inline BigInt& operator = (const BigInt<BYTE_COUNT_B>& other);
	inline BigInt& operator = (const char* s);
	inline bool IsNegative() const;
	inline long long ToInt() const;
	inline static BigInt Max();
	inline void AddAt(unsigned char* buffer, int idx, unsigned char value);
	inline void ShiftL();
	inline void ShiftR();
	inline void FixSign();
	inline String ToString(int base = 10);
	template <int BYTE_COUNT_B>
	inline BigInt& operator += (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt& operator -= (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt& operator *= (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt& operator /= (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt& operator %= (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt& operator >>= (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt& operator <<= (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt operator + (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt operator - (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt operator * (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt operator / (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt operator % (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt operator >> (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline BigInt operator << (const BigInt<BYTE_COUNT_B> b);
	inline BigInt operator ~ ();
	inline BigInt operator - ();
	inline BigInt& operator ++ ();
	inline BigInt& operator -- ();
	template <int BYTE_COUNT_B>
	inline bool operator < (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline bool operator > (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline bool operator <= (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline bool operator >= (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline bool operator == (const BigInt<BYTE_COUNT_B> b);
	template <int BYTE_COUNT_B>
	inline bool operator != (const BigInt<BYTE_COUNT_B> b);

	inline BigInt& operator += (const long long b);
	inline BigInt& operator -= (const long long b);
	inline BigInt& operator *= (const long long b);
	inline BigInt& operator /= (const long long b);
	inline BigInt& operator %= (const long long b);
	inline BigInt& operator >>= (const long long b);
	inline BigInt& operator <<= (const long long b);
	inline BigInt operator + (const long long b);
	inline BigInt operator - (const long long b);
	inline BigInt operator * (const long long b);
	inline BigInt operator / (const long long b);
	inline BigInt operator % (const long long b);
	inline BigInt operator >> (const long long b);
	inline BigInt operator << (const long long b);
	inline bool operator < (const long long b);
	inline bool operator > (const long long b);
	inline bool operator <= (const long long b);
	inline bool operator >= (const long long b);
	inline bool operator == (const long long b);
	inline bool operator != (const long long b);
};

template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> operator + (const long long a, const BigInt<BYTE_COUNT> b);
template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> operator - (const long long a, const BigInt<BYTE_COUNT> b);
template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> operator * (const long long a, const BigInt<BYTE_COUNT> b);
template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> operator / (const long long a, const BigInt<BYTE_COUNT> b);
template <int BYTE_COUNT>
inline BigInt<BYTE_COUNT> operator % (const long long a, const BigInt<BYTE_COUNT> b);

inline unsigned char UAddC(unsigned char a, unsigned char b, bool& carry);
inline void UMul(unsigned char& resHigh, unsigned char& resLow);
inline unsigned char UDiv(unsigned char dividend, unsigned char divisor, unsigned char& prevRem);

typedef BigInt<1> bint8_t;
typedef BigInt<2> bint16_t;
typedef BigInt<4> bint32_t;
typedef BigInt<8> bint64_t;
typedef BigInt<16> bint128_t;
typedef BigInt<32> bint256_t;
typedef BigInt<64> bint512_t;

NSE()

#include "AKLib/BigInt.inl"
