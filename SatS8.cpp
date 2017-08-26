#include "SatS8.h"

#include <stdint.h>

SatS8::operator int8_t(void) const
{
	return this->value;
}

SatS8 & SatS8::operator++(void)
{
	asm volatile
	(
		"INC %A[result] \n"
		"BRPL end%= \n"
		"LDI %A[result], 0x7F \n"
		"end%=: \n"
		:
		[result] "+d" (this->value)
	);
	return *this;
}
	
SatS8 & SatS8::operator--(void)
{
	asm volatile
	(
		"SBCI %A[result], 1 \n" 
		"BRMI end%= \n"
		"LDI %A[result], 0x80 \n"
		"end%=: \n"
		:
		[result] "+d" (this->value)
	);
	return *this;
}

SatS8 operator+(const SatS8 & left, const SatS8 & right)
{
	SatS8 result;
	asm volatile
	(
		"MOV %A[result], %A[left] \n"
		"CLV \n"
		"ADC %A[result], %A[right] \n"
		"BRVC end%= \n"
		"LDI %A[result], 0x7F \n"
		"end%=: \n"
		:
		[result] "+d" (result)
		:
		[left] "r" (left),
		[right] "r" (right)
	);
	return result;
}

SatS8 operator-(const SatS8 & left, const SatS8 & right)
{
	SatS8 result;
	asm volatile
	(
		"MOV %A[result], %A[left] \n"
		"CLV \n"
		"SBC %A[result], %A[right] \n"
		"BRVC end%= \n"
		"LDI %A[result], 0x80 \n"
		"end%=: \n"
		:
		[result] "+d" (result)
		:
		[left] "r" (left),
		[right] "r" (right)
	);
	return result;
}

SatS8 operator*(const SatS8 & left, const SatS8 & right)
{
	SatS8 result;
	int8_t temp;
	int8_t high;
	int8_t low;
	asm volatile
	(
		"MULS %A[left], %A[right] \n"
		"MOV %A[high], r1 \n"
		"MOV %A[low], r0 \n"
		
		"TST %A[high] \n"
		"BREQ highIsZero%= \n"
		
		"CPI %A[high], 0xFF \n"
		"BREQ highIsFull%= \n"
		
		"else%=: \n"
		// Return low
		"MOV %A[result], %A[low] \n"
		"RJMP end%= \n"
		
		"highIsZero%=: \n"
		"TST %A[low] \n"
		"BRMI lowIsNeg1%= \n"
		
		// If low not negative
		// Return low
		"MOV %A[result], %A[low] \n"
		"RJMP end%= \n"
		
		// If low negative
		"lowIsNeg1%=: \n"
		// Return 127
		"LDI %A[result], 0x7F \n"
		"RJMP end%= \n"
		
		
		"highIsFull%=: \n"
		"TST %A[low] \n"
		"BRMI lowIsNeg2%= \n"
		
		// If low not negative
		// Return -128
		"LDI %A[result], 0x80 \n"
		"RJMP end%= \n"
		
		// If low negative
		"lowIsNeg2%=: \n"
		// Return low
		"MOV %A[result], %A[low] \n"
		"RJMP end%= \n"
		
		"end%=: \n"
		"CLR r1 \n"
		/*"CPI %A[temp], 0 \n"
		"BRNE overflowed%= \n"
		"MOV %A[result], r0 \n"
		"RJMP end%= \n"
		"overflowed%=: \n"
		"SBR %A[result], 0xFF \n"*/
		:
		[result] "+r" (result),
		[temp] "+d" (temp),
		[high] "+d" (high),
		[low] "+d" (low)
		:
		[left] "r" (left),
		[right] "r" (right)
		:
		"r0", "r1"
	);
	return result;
}


#define SatS8Op_SatS8_int8_t(op) \
	SatS8 operator op(const SatS8 & left, const int8_t & right) \
	{ \
		return left op SatS8(right); \
	}
	
#define SatS8Op_int8_t_SatS8(op) \
	SatS8 operator op(const int8_t & left, const SatS8 & right) \
	{ \
		return SatS8(left) op right; \
	}

#define SatS8Op(op) \
	SatS8Op_SatS8_int8_t(op) \
	SatS8Op_int8_t_SatS8(op)
	
SatS8Op( + )
SatS8Op( - )
SatS8Op( * )

#undef SatS8Op
#undef SatS8Op_int8_t_SatS8
#undef SatS8Op_SatS8_int8_t