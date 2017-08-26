#include <stdint.h>

class SatU16
{
private:
	uint16_t value;
		
public:
	SatU16(void) : value() {}
	
	SatU16(const uint16_t & value) : value(value) {}

	operator uint16_t(void) const;

	SatU16 & operator++(void);
	SatU16 & operator--(void);
};

SatU16 operator+(const SatU16 & left, const SatU16 & right);
SatU16 operator-(const SatU16 & left, const SatU16 & right);

SatU16 operator+(const SatU16 & left, const uint16_t & right);
SatU16 operator-(const SatU16 & left, const uint16_t & right);

SatU16 operator+(const uint16_t & left, const SatU16 & right);
SatU16 operator-(const uint16_t & left, const SatU16 & right);