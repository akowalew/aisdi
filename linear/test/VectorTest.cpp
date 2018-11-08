#define BOOST_TEST_MODULE VectorTest
#include <boost/test/unit_test.hpp>

#include <boost/mpl/list.hpp>

#include "aisdi/Vector.hpp"

using TestTypes = boost::mpl::list<int, long, unsigned char>;

BOOST_AUTO_TEST_CASE_TEMPLATE(
	WhenDefaultConstructing_ThenItIsEmpty,
	T, TestTypes)
{
	const auto vector = aisdi::Vector<T>{};

	BOOST_CHECK(vector.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyInitializer_WhenListInitializing_ThenItIsEmpty,
	T, TestTypes)
{
	const auto il = std::initializer_list<T>{};
	const auto vector = aisdi::Vector<T>{il};

	BOOST_CHECK(vector.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenInitializer_WhenListInitializing_ThenItHasSameItems,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	const auto vector = aisdi::Vector<T>{il};

	BOOST_CHECK(vector.size() == il.size());
	BOOST_CHECK(std::equal(il.begin(), il.end(), vector.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherEmptyContainer_WhenCopyConstructing_ThenItIsAlsoEmpty,
	T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{};
	const auto vector2 = vector1;

	BOOST_CHECK(vector2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherContainer_WhenCopyConstructing_ThenTheyAreEqual,
	T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{1, 2, 3};
	const auto vector2 = vector1;

	BOOST_CHECK(vector1 == vector2);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherEmptyContainer_WhenMoveConstructing_ThenBothAreEmpty,
	T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{};
	const auto vector2 = std::move(vector1);

	BOOST_CHECK(vector1.empty());
	BOOST_CHECK(vector2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherContainer_WhenMoveConstructing_ThenItHasHisContents,
	T, TestTypes)
{
	auto vector1 = aisdi::Vector<T>{T{1}, T{2}, T{3}};
	const auto vector2 = std::move(vector1);

	BOOST_CHECK(vector1.empty());
	// TODO: Missing checks
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherEmptyContainer_WhenAssigning_ThenItIsAlsoEmpty,
	T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{};
	auto vector2 = aisdi::Vector<T>{};
	vector2 = vector1;

	BOOST_CHECK(vector2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherContainer_WhenAssigning_ThenTheyAreSame,
	T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{1, 2, 3};
	auto vector2 = aisdi::Vector<T>{};
	vector2 = vector1;

	BOOST_CHECK(vector2 == vector1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherEmptyContainer_WhenMoveAssigning_ThenTheyAreBothEmpty,
	T, TestTypes)
{
	auto vector1 = aisdi::Vector<T>{};
	auto vector2 = aisdi::Vector<T>{};
	vector2 = std::move(vector1);

	BOOST_CHECK(vector2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenMoveAssigning_ThenItHasHisContents,
	T, TestTypes)
{
	auto vector1 = aisdi::Vector<T>{1, 2, 3};
	auto vector2 = aisdi::Vector<T>{};
	vector2 = std::move(vector1);

	BOOST_CHECK(vector1.empty());
	// TODO: Missing checks
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenGettingSize_ThenItIsZero,
	T, TestTypes)
{
	const auto vector = aisdi::Vector<T>{};

	BOOST_CHECK(vector.size() == 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenComparedToItself_ThenItIsEqual,
	T, TestTypes)
{
	const auto vector = aisdi::Vector<T>{};

	BOOST_CHECK(vector == vector);
	BOOST_CHECK(!(vector != vector));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenComparedToItself_ThenItIsEqual,
	T, TestTypes)
{
	const auto vector = aisdi::Vector<T>{1, 2, 3};

	BOOST_CHECK(vector == vector);
	BOOST_CHECK(!(vector != vector));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainers_WhenCompared_ThenTheyAreEqual,
	T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{};
	const auto vector2 = aisdi::Vector<T>{};

	BOOST_CHECK(vector1 == vector2);
	BOOST_CHECK(!(vector1 != vector2));
	BOOST_CHECK(vector2 == vector1);
	BOOST_CHECK(!(vector2 != vector1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenSameContainers_WhenCompared_ThenTheyAreEqual,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	const auto vector1 = aisdi::Vector<T>{il};
	const auto vector2 = aisdi::Vector<T>{il};

	BOOST_CHECK(vector1 == vector2);
	BOOST_CHECK(!(vector1 != vector2));
	BOOST_CHECK(vector2 == vector1);
	BOOST_CHECK(!(vector2 != vector1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenVariousSizeContainers_WhenCompared_ThenTheyAreNotEqual,
	T, TestTypes)
{
	{
		const auto vector1 = aisdi::Vector<T>{1, 2, 3};
		const auto vector2 = aisdi::Vector<T>{1, 2, 3, 4, 5, 6};

		BOOST_CHECK(!(vector1 == vector2));
		BOOST_CHECK(vector1 != vector2);
		BOOST_CHECK(!(vector2 == vector1));
		BOOST_CHECK(vector2 != vector1);
	}

	{
		const auto vector1 = aisdi::Vector<T>{1, 2, 3};
		const auto vector2 = aisdi::Vector<T>{4, 5, 6, 1, 2, 3};

		BOOST_CHECK(!(vector1 == vector2));
		BOOST_CHECK(vector1 != vector2);
		BOOST_CHECK(!(vector2 == vector1));
		BOOST_CHECK(vector2 != vector1);
	}

	{
		const auto vector1 = aisdi::Vector<T>{1, 2, 3};
		const auto vector2 = aisdi::Vector<T>{};

		BOOST_CHECK(!(vector1 == vector2));
		BOOST_CHECK(vector1 != vector2);
		BOOST_CHECK(!(vector2 == vector1));
		BOOST_CHECK(vector2 != vector1);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenVariousItemsContainers_WhenCompared_ThenTheyAreNotEqual,
	T, TestTypes)
{
	const auto vector1 = aisdi::Vector<T>{1, 2, 3};
	const auto vector2 = aisdi::Vector<T>{6, 7, 8};

	BOOST_CHECK(!(vector1 == vector2));
	BOOST_CHECK(vector1 != vector2);
	BOOST_CHECK(!(vector2 == vector1));
	BOOST_CHECK(vector2 != vector1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenAppending_ThenItemIsPlacedAtEnd,
	T, TestTypes)
{
	auto vector = aisdi::Vector<T>{};

	const auto item = T{4};
	vector.append(item);

	BOOST_CHECK(vector.size() == 1);
	BOOST_CHECK(*std::rbegin(vector) == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenAppending_ThenItemIsPlacedAtEnd,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};
	const auto previousSize = vector.size();

	const auto item = T{4};
	vector.append(item);

	BOOST_CHECK(vector.size() == (previousSize + 1));
	BOOST_CHECK(*std::rbegin(vector) == item);
	BOOST_CHECK(std::equal(il.begin(), il.end(), vector.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenPrepending_ThenItemIsPlacedAtBegin,
	T, TestTypes)
{
	auto vector = aisdi::Vector<T>{};

	const auto item = T{4};
	vector.prepend(item);

	BOOST_CHECK(vector.size() == 1);
	BOOST_CHECK(*vector.begin() == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenPrepending_ThenItemIsPlacedAtBegin,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};
	const auto previousSize = vector.size();

	const auto item = T{4};
	vector.prepend(item);

	BOOST_CHECK(vector.size() == (previousSize + 1));
	BOOST_CHECK(*vector.begin() == item);
	BOOST_CHECK(std::equal(il.begin(), il.end(), std::next(vector.begin())));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenInsertingAtBegin_ThenItemIsPrepended,
	T, TestTypes)
{
	auto vector = aisdi::Vector<T>{};

	const auto item = T{4};
	const auto pos = vector.begin();
	const auto newPos = vector.insert(pos, item);

	BOOST_CHECK(vector.size() == 1);
	BOOST_CHECK(newPos == vector.begin());
	BOOST_CHECK(*newPos == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenInsertingAtEnd_ThenItemIsAppended,
	T, TestTypes)
{
	auto vector = aisdi::Vector<T>{};

	const auto item = T{4};
	const auto pos = vector.end();
	const auto newPos = vector.insert(pos, item);

	BOOST_CHECK(vector.size() == 1);
	BOOST_CHECK(newPos == std::prev(vector.end()));
	BOOST_CHECK(*newPos == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenInsertingAtBegin_ThenItemIsInPrepended,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};
	const auto prevSize = vector.size();

	const auto item = T{4};
	const auto pos = vector.begin();
	const auto newPos = vector.insert(pos, item);

	BOOST_CHECK(vector.size() == (prevSize + 1));
	BOOST_CHECK(newPos == vector.begin());
	BOOST_CHECK(*newPos == item);
	BOOST_CHECK(std::equal(std::next(newPos), vector.end(), il.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenInsertingInMiddle_ThenItemIsInserted,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}, T{4}, T{5}};
	auto vector = aisdi::Vector<T>{il};
	const auto prevSize = vector.size();

	const auto item = T{6};
	const auto shift = 2;
	const auto pos = (vector.begin() + shift);
	const auto newPos = vector.insert(pos, item);

	BOOST_CHECK(vector.size() == (prevSize + 1));
	BOOST_CHECK(newPos == (vector.begin() + shift));
	BOOST_CHECK(*newPos == item);
	BOOST_CHECK(std::equal(vector.begin(), newPos, il.begin()));
	BOOST_CHECK(std::equal(std::next(newPos), vector.end(), (il.begin() + shift)));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenInsertingAtEnd_ThenItemIsAppended,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};
	const auto prevSize = vector.size();

	const auto item = T{4};
	const auto pos = vector.end();
	const auto newPos = vector.insert(pos, item);

	BOOST_CHECK(vector.size() == (prevSize + 1));
	BOOST_CHECK(newPos == std::prev(vector.end()));
	BOOST_CHECK(*newPos == item);
	BOOST_CHECK(std::equal(vector.begin(), newPos, il.begin()));
}

// BOOST_AUTO_TEST_CASE_TEMPLATE(
// 	GivenEmptyContainer_WhenPoppingBack_ThenExceptionIsThrown,
// 	T, TestTypes)
// {
// 	// ...
// }

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenPoppingBack_ThenLastItemIsRemovedAndReturned,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	const auto lastItem = *std::rbegin(il);

	auto vector = aisdi::Vector<T>{il};
	const auto previousSize = vector.size();

	const auto item = vector.popBack();

	// TODO: Add check about call count to destructor (prove destroing)

	BOOST_CHECK(vector.size() == (previousSize - 1));
	BOOST_CHECK(item == lastItem);
	BOOST_CHECK(std::equal(vector.begin(), vector.end(), il.begin()));
}

// BOOST_AUTO_TEST_CASE_TEMPLATE(
// 	GivenEmptyContainer_WhenPoppingFront_ThenExceptionIsThrown,
// 	T, TestTypes)
// {
// 	// ...
// }

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenPoppingFront_ThenFirstItemIsRemovedAndReturned,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	const auto firstItem = *std::begin(il);

	auto vector = aisdi::Vector<T>{il};
	const auto previousSize = vector.size();

	const auto item = vector.popFront();

	// TODO: Add check about call count to destructor (prove destroing)

	BOOST_CHECK(vector.size() == (previousSize - 1));
	BOOST_CHECK(item == firstItem);
	BOOST_CHECK(std::equal(vector.begin(), vector.end(), std::next(il.begin())));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenAnyContainer_WhenFirstEqualsLast_ThenNothingHappens,
	T, TestTypes)
{
	{
		auto vector = aisdi::Vector<T>{};

		const auto first = vector.begin();
		const auto last = first;
		vector.erase(first, last);

		BOOST_CHECK(vector.empty());
	}

	{
		auto vector = aisdi::Vector<T>{};

		const auto first = vector.begin() - 100;
		const auto last = first;
		vector.erase(first, last);

		BOOST_CHECK(vector.empty());
	}

	{
		const auto vector1 = aisdi::Vector<T>{1, 2, 3};
		auto vector2 = vector1;

		const auto first = vector2.begin();
		const auto last = first;
		vector2.erase(first, last);

		BOOST_CHECK(vector2 == vector1);
	}

	{
		const auto vector1 = aisdi::Vector<T>{1, 2, 3};
		auto vector2 = vector1;

		const auto first = vector2.begin() - 100;
		const auto last = first;
		vector2.erase(first, last);

		BOOST_CHECK(vector2 == vector1);
	}
}

// BOOST_AUTO_TEST_CASE_TEMPLATE(
// 	GivenEmptyContainer_WhenErasingAnything_ThenExceptionIsThrown,
// 	T, TestTypes)
// {
// 	// ...
// }

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingFirstItem_ThenItIsPoppedFromFront,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};

	const auto first = vector.begin();
	const auto last = std::next(first);
	const auto afterLastRemoved = vector.erase(first, last);

	BOOST_CHECK(afterLastRemoved == vector.begin());
	BOOST_CHECK(vector.size() == (il.size() - 1));
	BOOST_CHECK(std::equal(vector.begin(), vector.end(), std::next(il.begin())));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingAtBeginning_ThenItemsAreRemovedFromFront,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};

	const auto count = 2;
	const auto first = vector.begin();
	const auto last = (first + count);
	const auto afterLastRemoved = vector.erase(first, last);

	BOOST_CHECK(afterLastRemoved == vector.begin());
	BOOST_CHECK(vector.size() == (il.size() - count));
	BOOST_CHECK(std::equal(vector.begin(), vector.end(), il.begin() + count));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingLastItem_ThenItIsPoppedFromBack,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};

	const auto last = vector.end();
	const auto first = std::prev(last);
	const auto afterLastRemoved = vector.erase(first, last);

	BOOST_CHECK(afterLastRemoved == vector.end());
	BOOST_CHECK(vector.size() == (il.size() - 1));
	BOOST_CHECK(std::equal(il.begin(), std::prev(il.end()), vector.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingAtEnd_ThenItemsAreRemovedFromBack,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};

	const auto count = 2;
	const auto last = vector.end();
	const auto first = (last - count);
	const auto afterLastRemoved = vector.erase(first, last);

	BOOST_CHECK(afterLastRemoved == vector.end());
	BOOST_CHECK(vector.size() == (il.size() - count));
	BOOST_CHECK(std::equal(il.begin(), il.end() - count, vector.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingOneInMiddle_ThenItemIsRemoved,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};

	const auto shift = 1;
	const auto first = (vector.begin() + shift);
	const auto last = std::next(first);
	const auto afterLastRemoved = vector.erase(first, last);

	BOOST_CHECK(afterLastRemoved == (vector.begin() + shift));
	BOOST_CHECK(vector.size() == (il.size() - 1));
	BOOST_CHECK(std::equal(vector.begin(), vector.begin() + shift,
		il.begin()));
	BOOST_CHECK(std::equal(vector.begin() + shift + 1, vector.end(),
		il.begin() + shift + 1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingInMiddle_ThenItemsAreRemoved,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}, T{4}, T{5}};
	auto vector = aisdi::Vector<T>{il};

	const auto shift = 1;
	const auto count = 2;
	const auto first = (vector.begin() + shift);
	const auto last = (first + count);
	const auto afterLastRemoved = vector.erase(first, last);

	BOOST_CHECK(afterLastRemoved == (vector.begin() + shift));
	BOOST_CHECK(vector.size() == (il.size() - count));
	BOOST_CHECK(std::equal(vector.begin(), vector.begin() + shift,
		il.begin()));
	BOOST_CHECK(std::equal(vector.begin() + shift + count, vector.end(),
		il.begin() + shift + count));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingFromBeginToEnd_ThenItIsEmpty,
	T, TestTypes)
{
	const auto il = {T{1}, T{2}, T{3}};
	auto vector = aisdi::Vector<T>{il};

	const auto first = vector.begin();
	const auto last = vector.end();
	const auto afterLastRemoved = vector.erase(first, last);

	BOOST_CHECK(afterLastRemoved == vector.end());
	BOOST_CHECK(vector.empty());
}
