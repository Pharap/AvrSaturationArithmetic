#include <stdint.h>

class SatS16
{
private:
	uint16_t value;
		
public:
	SatS16(void) : value() {}
	
	SatS16(const uint16_t & value) : value(value) {}

	operator int16_t(void) const;

	SatS16 & operator++(void);
	SatS16 & operator--(void);
};

SatS16 operator+(const SatS16 & left, const SatS16 & right);
SatS16 operator-(const SatS16 & left, const SatS16 & right);

SatS16 operator+(const SatS16 & left, const int16_t & right);
SatS16 operator-(const SatS16 & left, const int16_t & right);

SatS16 operator+(const int16_t & left, const SatS16 & right);
SatS16 operator-(const int16_t & left, const SatS16 & right);