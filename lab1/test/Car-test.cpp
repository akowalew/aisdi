#include <iostream>

#include <cassert>

#include "aisdi/Car.hpp"

int main()
{
	using namespace aisdi;

	const auto fuel = 100;
	auto car = Car{fuel};
	assert(!car.empty());

	const auto speed = 25;
	car.accelerate(speed);
	car.accelerate(speed);
	car.accelerate(speed);
	car.accelerate(speed);
	assert(car.empty());
}

