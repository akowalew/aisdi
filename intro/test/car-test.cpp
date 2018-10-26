#include "catch2/catch.hpp"

#include "aisdi/Car.hpp"

TEST_CASE("Car's acceleration will be tested", "[accelerate]")
{
	using namespace aisdi;

	const auto fuel = 100;
	auto car = Car{fuel};
	REQUIRE(!car.empty());

	const auto speed = 25;
	car.accelerate(speed);
	car.accelerate(speed);
	car.accelerate(speed);
	car.accelerate(speed);
	REQUIRE(car.empty());
}
