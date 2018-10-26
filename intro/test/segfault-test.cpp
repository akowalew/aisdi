#include "catch2/catch.hpp"

#include <memory>

#include "aisdi/Car.hpp"

TEST_CASE("System crash will be evaluated", "[crash]")
{
	using namespace aisdi;

	const auto fuel = 100;
	auto car = std::make_unique<Car>(fuel);
	car.reset();

	// It will cause a SEGFAULT error due to use of invalidated pointer
	const auto speed = 50;
	car->accelerate(speed);
}
