#include <stdint.h>

class SatS8
{
private:
	uint8_t value;
		
public:
	SatS8(void) : value() {}
	
	SatS8(const uint8_t & value) : value(value) {}

	operator int8_t(void) const;

	SatS8 & operator++(void);
	SatS8 & operator--(void);
};

SatS8 operator+(const SatS8 & left, const SatS8 & right);
SatS8 operator-(const SatS8 & left, const SatS8 & right);
SatS8 operator*(const SatS8 & left, const SatS8 & right);

SatS8 operator+(const SatS8 & left, const int8_t & right);
SatS8 operator-(const SatS8 & left, const int8_t & right);
SatS8 operator*(const SatS8 & left, const int8_t & right);

SatS8 operator+(const int8_t & left, const SatS8 & right);
SatS8 operator-(const int8_t & left, const SatS8 & right);
SatS8 operator*(const int8_t & left, const SatS8 & right);
