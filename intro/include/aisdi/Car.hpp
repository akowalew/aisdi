#pragma once

namespace aisdi {

class Car
{
public:
	Car(int fuel);

	void accelerate(int speed);

	bool empty() const;

private:
	int _fuel;
};

} // namespace aisdi