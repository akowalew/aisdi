#define BOOST_TEST_MODULE VectorTest
#include <boost/test/unit_test.hpp>

#include <boost/mpl/list.hpp>

#include "aisdi/Vector.hpp"

using TestTypes = boost::mpl::list<int, long, unsigned char>;

BOOST_AUTO_TEST_CASE_TEMPLATE(DefaultConstructedShouldHasSizeZero, T, TestTypes)
{
	const auto vector = aisdi::Vector<T>{};

	BOOST_CHECK(vector.size() == 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(FillConstructedShouldHasDesiredItems, T, TestTypes)
{
	const auto count = 5;
	const auto item = 0xAB;
	const auto vector = aisdi::Vector<T>(count, item);

	BOOST_CHECK(vector.size() == count);

	for(auto i = 0; i < count; ++i)
	{
		BOOST_CHECK(vector[i] == item);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(DefaultFillConstructedShouldHasDefaultItems, T, TestTypes)
{
	const auto count = 5;
	const auto vector = aisdi::Vector<T>(count);

	BOOST_CHECK(vector.size() == count);

	const auto defaultItem = T{};
	for(auto i = 0; i < count; ++i)
	{
		BOOST_CHECK(vector[i] == defaultItem);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ListInitializedShouldHasDesiredItems, T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}, T{4}, T{5}};
	const auto vector = aisdi::Vector<T>{il};

	BOOST_CHECK(vector.size() == il.size());

	auto i = 0;
	for(const auto& item : il)
	{
		BOOST_CHECK(vector[i] == item);
		i++;
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CopyConstructedShouldBeEqual, T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{1, 2, 3, 4, 5};
	const auto vector2 = vector1;

	BOOST_CHECK(vector1.size() == vector2.size());

	for(std::size_t i = 0; i < vector1.size(); ++i)
	{
		BOOST_CHECK(vector1[i] == vector2[i]);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ShouldBeEqualWithItself, T, TestTypes)
{
	{
		const auto vector = aisdi::Vector<T>{1, 2, 3, 4, 5};
	
		BOOST_CHECK(vector == vector);
		BOOST_CHECK(!(vector != vector));
	}

	{
		const auto vector = aisdi::Vector<T>{};

		BOOST_CHECK(vector == vector);
		BOOST_CHECK(!(vector == vector));
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ShouldBeEqualWhenSameItems, T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{1, 2, 3, 4, 5};
	const auto vector2 = vector1;

	BOOST_CHECK(vector1 == vector2);
	BOOST_CHECK(!(vector1 != vector2));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ShouldNotBeEqualWhenVariousItems, T, TestTypes)
{
	{
		const auto vector1 = aisdi::Vector<T>{1, 2, 3, 4, 5};
		const auto vector2 = aisdi::Vector<T>{6, 7, 8, 9, 10};

		BOOST_CHECK(vector1 != vector2);
		BOOST_CHECK(!(vector1 == vector2));
	}

	{
		const auto vector1 = aisdi::Vector<T>{1, 2, 3, 4, 5};
		const auto vector2 = aisdi::Vector<T>{};
	
		BOOST_CHECK(vector1 != vector2);
		BOOST_CHECK(!(vector1 == vector2));	
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ShouldNotBeEqualWhenVariousSizes, T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{1, 2, 3, 4, 5};
	const auto vector2 = aisdi::Vector<T>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	BOOST_CHECK(vector1.size() != vector2.size());
	BOOST_CHECK(vector1 != vector2);
	BOOST_CHECK(!(vector1 == vector2));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(EqualityShouldBeCommutative, T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{1, 2, 3, 4, 5};
	const auto vector2 = aisdi::Vector<T>{6, 7, 8, 9, 10};

	BOOST_CHECK(vector1 != vector2);
	BOOST_CHECK(vector2 != vector1);
	BOOST_CHECK(!(vector1 == vector2));
	BOOST_CHECK(!(vector2 == vector1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(AppendShouldIncrementSize, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{};
	const auto previousSize = vector.size();

	vector.append(T{}); // append dummy item

	BOOST_CHECK(vector.size() == (previousSize + 1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(AppendedItemShouldBeSame, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{};

	const auto item = T{0xAB};
	vector.append(item); // append dummy item

	BOOST_CHECK(vector[vector.size() - 1] == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IfSizeZeroThanShouldBeEmpty, T, TestTypes)
{
	const auto vector = aisdi::Vector<T>{};

	BOOST_CHECK(vector.size() == 0);
	BOOST_CHECK(vector.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IfSizeNotZeroThanShouldNotBeEmpty, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{};

	const auto item = T{0xAB};
	vector.append(item); // append dummy item

	BOOST_CHECK(vector.size() != 0);
	BOOST_CHECK(!vector.empty());
}