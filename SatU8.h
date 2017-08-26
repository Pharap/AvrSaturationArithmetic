#include <stdint.h>

class SatU8
{
private:
	uint8_t value;
		
public:
	SatU8(void) : value() {}
	
	SatU8(const uint8_t & value) : value(value) {}

	operator uint8_t(void) const;

	SatU8 & operator++(void);
	SatU8 & operator--(void);
};

SatU8 operator+(const SatU8 & left, const SatU8 & right);
SatU8 operator-(const SatU8 & left, const SatU8 & right);
SatU8 operator*(const SatU8 & left, const SatU8 & right);

SatU8 operator+(const SatU8 & left, const uint8_t & right);
SatU8 operator-(const SatU8 & left, const uint8_t & right);
SatU8 operator*(const SatU8 & left, const uint8_t & right);

SatU8 operator+(const uint8_t & left, const SatU8 & right);
SatU8 operator-(const uint8_t & left, const SatU8 & right);
SatU8 operator*(const uint8_t & left, const SatU8 & right);