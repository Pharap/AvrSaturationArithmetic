#include "SatU16.h"

#include <stdint.h>

SatU16::operator uint16_t(void) const
{
	return this->value;
}

SatU16 & SatU16::operator++(void)
{
	asm volatile
	(
		"ADIW %[result], 1 \n"
		"BRNE end%= \n"
		"SBR %A[result], 0xFF \n"
		"SBR %B[result], 0xFF \n"
		"end%=: \n"
		:
		[result] "+w" (this->value)
	);
	return *this;
}
	
SatU16 & SatU16::operator--(void)
{
	asm volatile
	(
		"SBIW %[result], 1 \n"
		"BRCC end%= \n"
		"CLR %A[result] \n"
		"CLR %B[result] \n"
		"end%=: \n"
		:
		[result] "+w" (this->value)
	);
	return *this;
}

SatU16 operator+(const SatU16 & left, const SatU16 & right)
{
	SatU16 result;
	asm volatile
	(
		"MOVW %[result], %[left] \n"
		"CLC \n"
		"ADC %A[result], %A[right] \n"
		"ADC %B[result], %B[right] \n"
		"BRCC end%= \n"
		"SBR %A[result], 0xFF \n"
		"SBR %B[result], 0xFF \n"
		"end%=: \n"
		:
		[result] "+r" (result)
		:
		[left] "r" (left),
		[right] "r" (right)
	);
	return result;
}

SatU16 operator-(const SatU16 & left, const SatU16 & right)
{
	SatU16 result;
	asm volatile
	(
		"MOVW %[result], %[left] \n"
		"CLC \n"
		"SBC %B[result], %B[right] \n"
		"SBC %A[result], %A[right] \n"
		"BRCC end%= \n" // if(sreg.c == 0) goto end;
		"CLR %A[result] \n" // else result = 0;
		"CLR %B[result] \n"
		"end%=: \n"
		:
		[result] "+r" (result)
		:
		[left] "r" (left),
		[right] "r" (right)
	);
	return result;
}

SatU16 operator+(const SatU16 & left, const uint16_t & right)
{
	return left + SatU16(right);
}

SatU16 operator-(const SatU16 & left, const uint16_t & right)
{
	return left - SatU16(right);
}

SatU16 operator+(const uint16_t & left, const SatU16 & right)
{
	return SatU16(left) - right;
}

SatU16 operator-(const uint16_t & left, const SatU16 & right)
{
	return SatU16(left) - right;
}
