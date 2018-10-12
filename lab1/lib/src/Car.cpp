#include "aisdi/Car.hpp"

#include <iostream>

namespace aisdi {

Car::Car(int fuel)
	:	_fuel(fuel)
{
	std::clog << "[Car] Initialized with fuel: " << fuel << std::endl;
}

void 
Car::accelerate(int speed)
{
	std::clog << "[Car] Accelerating to: " << speed << std::endl;

	_fuel -= speed;

	std::clog << "[Car] Current fuel level: " << _fuel << std::endl;
}

bool
Car::empty() const
{
	return (_fuel == 0);
}

} // namespace aisdi