#include "SatS16.h"

#include <stdint.h>

SatS16::operator int16_t(void) const
{
	return this->value;
}

SatS16 & SatS16::operator++(void)
{
	asm volatile
	(
		"ADIW %[result], 1 \n"
		"BRPL end%= \n"
		"SBR %A[result], 0xFF \n"
		"LDI %B[result], 0x7F \n"
		"end%=: \n"
		:
		[result] "+w" (this->value)
	);
	return *this;
}
	
SatS16 & SatS16::operator--(void)
{
	asm volatile
	(
		"SBCI %[result], 1 \n"
		"BRCC end%= \n"
		"CLR %A[result] \n"
		"LDI %B[result], 0x80 \n"
		"end%=: \n"
		:
		[result] "+w" (this->value)
	);
	return *this;
}

SatS16 operator+(const SatS16 & left, const SatS16 & right)
{
	SatS16 result;
	asm volatile
	(
		"MOVW %[result], %[left] \n"
		"CLV \n"
		"ADC %A[result], %A[right] \n"
		"ADC %B[result], %B[right] \n"
		"BRVC end%= \n"
		"SBR %A[result], 0xFF \n"
		"LDI %B[result], 0x7F \n"
		"end%=: \n"
		:
		[result] "+d" (result)
		:
		[left] "r" (left),
		[right] "r" (right)
	);
	return result;
}

SatS16 operator-(const SatS16 & left, const SatS16 & right)
{
	SatS16 result;
	asm volatile
	(
		"MOVW %[result], %[left] \n"
		"CLC \n"
		"SBC %B[result], %B[right] \n"
		"SBC %A[result], %A[right] \n"
		"BRCC end%= \n"
		"CLR %A[result] \n"
		"LDI %B[result], 0x80 \n"
		"end%=: \n"
		:
		[result] "+d" (result)
		:
		[left] "r" (left),
		[right] "r" (right)
	);
	return result;
}

SatS16 operator+(const SatS16 & left, const int8_t & right)
{
	return left + SatS16(right);
}

SatS16 operator-(const SatS16 & left, const int8_t & right)
{
	return left - SatS16(right);
}

SatS16 operator+(const int8_t & left, const SatS16 & right)
{
	return SatS16(left) - right;
}

SatS16 operator-(const int8_t & left, const SatS16 & right)
{
	return SatS16(left) - right;
}
