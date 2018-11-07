#define BOOST_TEST_MODULE VectorTest
#include <boost/test/unit_test.hpp>

#include <boost/mpl/list.hpp>

#include "aisdi/Vector.hpp"

using TestTypes = boost::mpl::list<int, long, unsigned char>;

BOOST_AUTO_TEST_CASE_TEMPLATE(DefaultConstructedShouldBeEmpty, T, TestTypes)
{
	const auto vector = aisdi::Vector<T>{};

	BOOST_CHECK(vector.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(WhenCountIsZero_DefaultFillConstructedShouldBeEmpty, T, TestTypes)
{
	const auto count = 0;
	const auto vector = aisdi::Vector<T>(count);

	BOOST_CHECK(vector.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(DefaultFillConstructedShouldHasDefaultItems, T, TestTypes)
{
	const auto count = 5;
	const auto vector = aisdi::Vector<T>(count);

	BOOST_CHECK(vector.size() == count);
	const auto defaultItem = T{};
	BOOST_CHECK(std::all_of(vector.begin(), vector.end(),
		[&defaultItem](auto& item) { return item == defaultItem; }));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(WhenCountIsZero_FillConstructedShouldBeEmpty, T, TestTypes)
{
	const auto count = 0;
	const auto item = 0xAB;
	const auto vector = aisdi::Vector<T>(count, item);

	BOOST_CHECK(vector.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(FillConstructedShouldHasDesiredItems, T, TestTypes)
{
	const auto count = 5;
	const auto item = 0xAB;
	const auto vector = aisdi::Vector<T>(count, item);

	BOOST_CHECK(vector.size() == count);
	BOOST_CHECK(std::all_of(vector.begin(), vector.end(),
		[&item](auto& item2) { return item == item2; }));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(WhenListIsEmpty_ListInitializedShouldBeEmpty, T, TestTypes)
{
	const auto il = std::initializer_list<T>{};
	const auto vector = aisdi::Vector<T>{il};

	BOOST_CHECK(vector.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ListInitializedShouldHasDesiredItems, T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	const auto vector = aisdi::Vector<T>{il};

	BOOST_CHECK(vector.size() == il.size());
	BOOST_CHECK(std::equal(il.begin(), il.end(), vector.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(WhenOtherIsEmpty_CopyConstructedShouldBeEmpty, T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{};
	const auto vector2 = vector1;

	BOOST_CHECK(vector2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CopyConstructedShouldBeTheSame, T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{1, 2, 3};
	const auto vector2 = vector1;

	BOOST_CHECK(vector1 == vector2);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(WhenOtherEmpty_MoveContructedAndOtherShouldBeEmpty, T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{};
	const auto vector2 = std::move(vector1);

	BOOST_CHECK(vector1.empty());
	BOOST_CHECK(vector2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(MoveContructedShouldNotBeAndOtherShouldBeEmpty, T, TestTypes)
{
	auto vector1 = aisdi::Vector<T>{T{1}, T{2}, T{3}};
	const auto vector2 = std::move(vector1);

	BOOST_CHECK(vector1.empty());
	BOOST_CHECK(!vector2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(WhenOtherEmpty_CopyInitializedShouldBeEmpty, T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{};
	auto vector2 = aisdi::Vector<T>{};
	vector2 = vector1;

	BOOST_CHECK(vector2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CopyInitializedShouldBeTheSame, T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{1, 2, 3};
	auto vector2 = aisdi::Vector<T>{};
	vector2 = vector1;

	BOOST_CHECK(vector2 == vector1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(WhenOtherEmpty_MoveInitializedShouldBeEmpty, T, TestTypes)
{
	auto vector1 = aisdi::Vector<T>{};
	auto vector2 = aisdi::Vector<T>{};
	vector2 = std::move(vector1);

	BOOST_CHECK(vector2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(MoveInitializedShouldNotBeAndOtherShouldBeEmpty, T, TestTypes)
{
	auto vector1 = aisdi::Vector<T>{1, 2, 3};
	auto vector2 = aisdi::Vector<T>{};
	vector2 = std::move(vector1);

	BOOST_CHECK(!vector2.empty());
	BOOST_CHECK(vector1.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(WhenEmpty_AppendShouldIncreaseSize, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{};
	const auto previousSize = vector.size();

	const auto item = T{4};
	vector.append(item);

	BOOST_CHECK(vector.size() == (previousSize + 1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(AppendShouldIncreaseSize, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{1, 2, 3};
	const auto previousSize = vector.size();

	const auto item = T{4};
	vector.append(item);

	BOOST_CHECK(vector.size() == (previousSize + 1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(WhenEmpty_AppendedItemShouldBeAtTheEnd, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{};

	const auto item = T{4};
	vector.append(item); // append dummy item

	BOOST_CHECK(vector[vector.size() - 1] == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(AppendedItemShouldBeAtTheEnd, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{1, 2, 3};

	const auto item = T{4};
	vector.append(item); // append dummy item

	BOOST_CHECK(vector[vector.size() - 1] == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(AppendShouldNotModifyCurrentItems, T, TestTypes)
{
	const auto pattern = aisdi::Vector<T>{1, 2, 3};
	auto vector = pattern;

	const auto item = T{4};
	vector.append(item); // append dummy item

	BOOST_CHECK(
		std::equal(pattern.begin(), pattern.end(),
			vector.data()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IfSizeZeroThanShouldBeEmpty, T, TestTypes)
{
	const auto vector = aisdi::Vector<T>{};

	BOOST_CHECK(vector.size() == 0);
	BOOST_CHECK(vector.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IfSizeNotZeroThanShouldNotBeEmpty, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{1, 2, 3};

	BOOST_CHECK(vector.size() != 0);
	BOOST_CHECK(!vector.empty());
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
		BOOST_CHECK(!(vector != vector));
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

BOOST_AUTO_TEST_CASE_TEMPLATE(PopBackShouldDecrementSize, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{1, 2, 3};
	const auto previousSize = vector.size();

	vector.popBack();

	BOOST_CHECK(vector.size() == (previousSize - 1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(PopBackShouldReturnLastItem, T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};

	const auto item = vector.popBack();

	BOOST_CHECK(item == *std::prev(il.end()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(PopBackShouldNotModifyOtherItems, T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};

	vector.popBack();

	BOOST_CHECK(std::equal(vector.begin(), vector.end(), il.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(PopFrontShouldDecrementSize, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{1, 2, 3};
	const auto previousSize = vector.size();

	vector.popFront();

	BOOST_CHECK(vector.size() == (previousSize - 1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(PopFrontShouldReturnFirstItem, T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};

	const auto item = vector.popFront();

	BOOST_CHECK(item == *il.begin());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(PopFrontShouldNotModifyOtherItems, T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};

	vector.popFront();

	BOOST_CHECK(std::equal(vector.begin(), vector.end(),
		std::next(il.begin())));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(PrependShouldIncrementSize, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{1, 2, 3};
	const auto previousSize = vector.size();

	const auto item = T{4};
	vector.prepend(item);

	BOOST_CHECK(vector.size() == (previousSize + 1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(PrependedItemShouldBeAtTheBegin, T, TestTypes)
{
	auto vector = aisdi::Vector<T>{1, 2, 3};

	const auto item = T{4};
	vector.prepend(item); // append dummy item

	BOOST_CHECK(vector[0] == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(PrependShouldNotModifyCurrentItems, T, TestTypes)
{
	const auto pattern = aisdi::Vector<T>{1, 2, 3};
	auto vector = pattern;

	const auto item = T{4};
	vector.prepend(item); // append dummy item

	BOOST_CHECK(std::equal(pattern.begin(), pattern.end(), 
		std::next(vector.begin())));
}

