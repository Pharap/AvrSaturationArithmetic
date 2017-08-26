#include "SatU8.h"

#include <stdint.h>

SatU8::operator uint8_t(void) const
{
	return this->value;
}

SatU8 & SatU8::operator++(void)
{
	asm volatile
	(
		// ++result;
		"INC %A[result] \n"
		// if(sreg.z == 0) goto end;
		"BRNE end%= \n"
		// else result = 0xFF;
		"SBR %A[result], 0xFF \n" 
		"end%=: \n"
		:
		[result] "+r" (this->value)
	);
	return *this;
}
	
SatU8 & SatU8::operator--(void)
{
	asm volatile
	(
		// --result;
		"SBCI %A[result], 1 \n"
		// if(sreg.c == 0) goto end;
		"BRCC end%= \n"
		// else result = 0;
		"CLR %A[result] \n"
		"end%=: \n"
		:
		[result] "+r" (this->value)
	);
	return *this;
}

SatU8 operator+(const SatU8 & left, const SatU8 & right)
{
	SatU8 result;
	asm volatile
	(
		// result = left;
		"MOV %A[result], %A[left] \n"
		"CLC \n"
		// result += right;
		"ADC %A[result], %A[right] \n"
		// if(sreg.c == 0) goto end;
		"BRCC end%= \n"
		// else result != 0xFF;
		"SBR %A[result], 0xFF \n"
		"end%=: \n"
		:
		[result] "+r" (result)
		:
		[left] "r" (left),
		[right] "r" (right)
	);
	return result;
}

SatU8 operator-(const SatU8 & left, const SatU8 & right)
{
	SatU8 result;
	asm volatile
	(
		// result = left;
		"MOV %A[result], %A[left] \n"
		"CLC \n"
		// result -= right;
		"SBC %A[result], %A[right] \n"
		// if(sreg.c == 0) goto end;
		"BRCC end%= \n"
		// else result = 0;
		"CLR %A[result] \n"
		"end%=: \n"
		:
		[result] "+r" (result)
		:
		[left] "r" (left),
		[right] "r" (right)
	);
	return result;
}

SatU8 operator*(const SatU8 & left, const SatU8 & right)
{
	SatU8 result;
	uint8_t temp;
	asm volatile
	(
		"MUL %A[left], %A[right] \n"
		"MOV %A[temp], r1 \n"
		"TST %A[temp] \n"
		"BRNE overflowed%= \n"
		"MOV %A[result], r0 \n"
		"RJMP end%= \n"
		"overflowed%=: \n"
		"SBR %A[result], 0xFF \n"
		"end%=: \n"
		"CLR r1 \n"
		:
		[result] "+r" (result),
		[temp] "+r" (temp)
		:
		[left] "r" (left),
		[right] "r" (right)
		:
		"r0", "r1"
	);
	return result;
}

#define SatU8Op_SatU8_uint8_t(op) \
	SatU8 operator op(const SatU8 & left, const uint8_t & right) \
	{ \
		return left op SatU8(right); \
	}
	
#define SatU8Op_uint8_t_SatU8(op) \
	SatU8 operator op(const uint8_t & left, const SatU8 & right) \
	{ \
		return SatU8(left) op right; \
	}

#define SatU8Op(op) \
	SatU8Op_SatU8_uint8_t(op) \
	SatU8Op_uint8_t_SatU8(op)
	
SatU8Op( + )
SatU8Op( - )
SatU8Op( * )

#undef SatU8Op
#undef SatU8Op_uint8_t_SatU8
#undef SatU8Op_SatU8_uint8_t