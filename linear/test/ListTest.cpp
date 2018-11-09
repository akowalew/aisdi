#define BOOST_TEST_MODULE ListTest
#include <boost/test/unit_test.hpp>

#include <boost/mpl/list.hpp>

#include "aisdi/List.hpp"

using TestTypes = boost::mpl::list<int, long, unsigned char>;

BOOST_AUTO_TEST_CASE_TEMPLATE(
	WhenDefaultConstructing_ThenItIsEmpty,
	T, TestTypes)
{
	const auto list = aisdi::List<T>{};

    BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyList_WhenGettingSize_ThenItIsZero,
	T, TestTypes)
{
	const auto list = aisdi::List<T>{};

    BOOST_CHECK(list.size() == 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyInitializer_WhenListInitializing_ThenItIsEmpty,
	T, TestTypes)
{
	auto il = std::initializer_list<T>{};
	const auto list = aisdi::List<T>{il};

	BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenInitializer_WhenListInitializing_ThenItHasSameItems,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	const auto list = aisdi::List<T>{il};

	BOOST_CHECK(list.size() == il.size());
	BOOST_CHECK(std::equal(il.begin(), il.end(), list.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherEmptyContainer_WhenCopyConstructing_ThenItIsAlsoEmpty,
	T, TestTypes)
{
	const auto list1 = aisdi::List<T>{};
	const auto list2 = list1;

	BOOST_CHECK(list2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherContainer_WhenCopyConstructing_ThenTheyAreEqual,
	T, TestTypes)
{
	const auto list1 = aisdi::List<T>{1, 2, 3};
	const auto list2 = list1;

	BOOST_CHECK(list1 == list2);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherEmptyContainer_WhenMoveConstructing_ThenBothAreEmpty,
	T, TestTypes)
{
	const auto list1 = aisdi::List<T>{};
	const auto list2 = std::move(list1);

	BOOST_CHECK(list1.empty());
	BOOST_CHECK(list2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherContainer_WhenMoveConstructing_ThenItHasHisContents,
	T, TestTypes)
{
	auto list1 = aisdi::List<T>{T{1}, T{2}, T{3}};
	const auto list2 = std::move(list1);

	BOOST_CHECK(list1.empty());
	// TODO: Missing checks
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherEmptyContainer_WhenAssigning_ThenItIsAlsoEmpty,
	T, TestTypes)
{
	const auto list1 = aisdi::List<T>{};
	auto list2 = aisdi::List<T>{};
	list2 = list1;

	BOOST_CHECK(list2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherContainer_WhenAssigning_ThenTheyAreSame,
	T, TestTypes)
{
	const auto list1 = aisdi::List<T>{1, 2, 3};
	auto list2 = aisdi::List<T>{};
	list2 = list1;

	BOOST_CHECK(list2 == list1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenOtherEmptyContainer_WhenMoveAssigning_ThenTheyAreBothEmpty,
	T, TestTypes)
{
	auto list1 = aisdi::List<T>{};
	auto list2 = aisdi::List<T>{};
	list2 = std::move(list1);

	BOOST_CHECK(list2.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenMoveAssigning_ThenItHasHisContents,
	T, TestTypes)
{
	auto list1 = aisdi::List<T>{1, 2, 3};
	auto list2 = aisdi::List<T>{};
	list2 = std::move(list1);

	BOOST_CHECK(list1.empty());
	// TODO: Missing checks
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenGettingSize_ThenItIsZero,
	T, TestTypes)
{
	const auto list = aisdi::List<T>{};

	BOOST_CHECK(list.size() == 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenComparedToItself_ThenItIsEqual,
	T, TestTypes)
{
	const auto list = aisdi::List<T>{};

	BOOST_CHECK(list == list);
	BOOST_CHECK(!(list != list));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenComparedToItself_ThenItIsEqual,
	T, TestTypes)
{
	const auto list = aisdi::List<T>{1, 2, 3};

	BOOST_CHECK(list == list);
	BOOST_CHECK(!(list != list));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainers_WhenCompared_ThenTheyAreEqual,
	T, TestTypes)
{
	const auto list1 = aisdi::List<T>{};
	const auto list2 = aisdi::List<T>{};

	BOOST_CHECK(list1 == list2);
	BOOST_CHECK(!(list1 != list2));
	BOOST_CHECK(list2 == list1);
	BOOST_CHECK(!(list2 != list1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenSameContainers_WhenCompared_ThenTheyAreEqual,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	const auto list1 = aisdi::List<T>{il};
	const auto list2 = aisdi::List<T>{il};

	BOOST_CHECK(list1 == list2);
	BOOST_CHECK(!(list1 != list2));
	BOOST_CHECK(list2 == list1);
	BOOST_CHECK(!(list2 != list1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenVariousSizeContainers_WhenCompared_ThenTheyAreNotEqual,
	T, TestTypes)
{
	{
		const auto list1 = aisdi::List<T>{1, 2, 3};
		const auto list2 = aisdi::List<T>{1, 2, 3, 4, 5, 6};

		BOOST_CHECK(!(list1 == list2));
		BOOST_CHECK(list1 != list2);
		BOOST_CHECK(!(list2 == list1));
		BOOST_CHECK(list2 != list1);
	}

	{
		const auto list1 = aisdi::List<T>{1, 2, 3};
		const auto list2 = aisdi::List<T>{4, 5, 6, 1, 2, 3};

		BOOST_CHECK(!(list1 == list2));
		BOOST_CHECK(list1 != list2);
		BOOST_CHECK(!(list2 == list1));
		BOOST_CHECK(list2 != list1);
	}

	{
		const auto list1 = aisdi::List<T>{1, 2, 3};
		const auto list2 = aisdi::List<T>{};

		BOOST_CHECK(!(list1 == list2));
		BOOST_CHECK(list1 != list2);
		BOOST_CHECK(!(list2 == list1));
		BOOST_CHECK(list2 != list1);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenVariousItemsContainers_WhenCompared_ThenTheyAreNotEqual,
	T, TestTypes)
{
	const auto list1 = aisdi::List<T>{1, 2, 3};
	const auto list2 = aisdi::List<T>{6, 7, 8};

	BOOST_CHECK(!(list1 == list2));
	BOOST_CHECK(list1 != list2);
	BOOST_CHECK(!(list2 == list1));
	BOOST_CHECK(list2 != list1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenAppending_ThenItemIsPlacedAtEnd,
	T, TestTypes)
{
	auto list = aisdi::List<T>{};

	const auto item = T{4};
	list.append(item);

	BOOST_CHECK(list.size() == 1);
	BOOST_CHECK(*std::rbegin(list) == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenAppending_ThenItemIsPlacedAtEnd,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	auto list = aisdi::List<T>{il};
	const auto previousSize = list.size();

	const auto item = T{4};
	list.append(item);

	BOOST_CHECK(list.size() == (previousSize + 1));
	BOOST_CHECK(*std::rbegin(list) == item);
	BOOST_CHECK(std::equal(il.begin(), il.end(), list.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenPrepending_ThenItemIsPlacedAtBegin,
	T, TestTypes)
{
	auto list = aisdi::List<T>{};

	const auto item = T{4};
	list.prepend(item);

	BOOST_CHECK(list.size() == 1);
	BOOST_CHECK(*list.begin() == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenPrepending_ThenItemIsPlacedAtBegin,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	auto list = aisdi::List<T>{il};
	const auto previousSize = list.size();

	const auto item = T{4};
	list.prepend(item);

	BOOST_CHECK(list.size() == (previousSize + 1));
	BOOST_CHECK(*list.begin() == item);
	BOOST_CHECK(std::equal(il.begin(), il.end(), std::next(list.begin())));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenInsertingAtBegin_ThenItemIsPrepended,
	T, TestTypes)
{
	auto list = aisdi::List<T>{};

	const auto item = T{4};
	const auto pos = list.begin();
	const auto newPos = list.insert(pos, item);

	BOOST_CHECK(list.size() == 1);
	BOOST_CHECK(newPos == list.begin());
	BOOST_CHECK(*newPos == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenEmptyContainer_WhenInsertingAtEnd_ThenItemIsAppended,
	T, TestTypes)
{
	auto list = aisdi::List<T>{};

	const auto item = T{4};
	const auto pos = list.end();
	const auto newPos = list.insert(pos, item);

	BOOST_CHECK(list.size() == 1);
	BOOST_CHECK(newPos == std::prev(list.end()));
	BOOST_CHECK(*newPos == item);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenInsertingAtBegin_ThenItemIsInPrepended,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	auto list = aisdi::List<T>{il};
	const auto prevSize = list.size();

	const auto item = T{4};
	const auto pos = list.begin();
	const auto newPos = list.insert(pos, item);

	BOOST_CHECK(list.size() == (prevSize + 1));
	BOOST_CHECK(newPos == list.begin());
	BOOST_CHECK(*newPos == item);
	BOOST_CHECK(std::equal(std::next(newPos), list.end(), il.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenInsertingInMiddle_ThenItemIsInserted,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}, T{4}, T{5}};
	auto list = aisdi::List<T>{il};
	const auto prevSize = list.size();

	const auto item = T{6};
	const auto shift = 2;
	const auto pos = (list.begin() + shift);
	const auto newPos = list.insert(pos, item);

	BOOST_CHECK(list.size() == (prevSize + 1));
	BOOST_CHECK(newPos == (list.begin() + shift));
	BOOST_CHECK(*newPos == item);
	BOOST_CHECK(std::equal(list.begin(), newPos, il.begin()));
	BOOST_CHECK(std::equal(std::next(newPos), list.end(), (il.begin() + shift)));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenInsertingAtEnd_ThenItemIsAppended,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	auto list = aisdi::List<T>{il};
	const auto prevSize = list.size();

	const auto item = T{4};
	const auto pos = list.end();
	const auto newPos = list.insert(pos, item);

	BOOST_CHECK(list.size() == (prevSize + 1));
	BOOST_CHECK(newPos == std::prev(list.end()));
	BOOST_CHECK(*newPos == item);
	BOOST_CHECK(std::equal(list.begin(), newPos, il.begin()));
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
	auto il = {T{1}, T{2}, T{3}};
	const auto lastItem = *std::rbegin(il);

	auto list = aisdi::List<T>{il};
	const auto previousSize = list.size();

	const auto item = list.popBack();

	// TODO: Add check about call count to destructor (prove destroing)

	BOOST_CHECK(list.size() == (previousSize - 1));
	BOOST_CHECK(item == lastItem);
	BOOST_CHECK(std::equal(list.begin(), list.end(), il.begin()));
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
	auto il = {T{1}, T{2}, T{3}};
	const auto firstItem = *std::begin(il);

	auto list = aisdi::List<T>{il};
	const auto previousSize = list.size();

	const auto item = list.popFront();

	// TODO: Add check about call count to destructor (prove destroing)

	BOOST_CHECK(list.size() == (previousSize - 1));
	BOOST_CHECK(item == firstItem);
	BOOST_CHECK(std::equal(list.begin(), list.end(), std::next(il.begin())));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenAnyContainer_WhenFirstEqualsLast_WhenErasing_ThenNothingHappens,
	T, TestTypes)
{
	{
		auto list = aisdi::List<T>{};

		const auto first = list.begin();
		const auto last = first;
		list.erase(first, last);

		BOOST_CHECK(list.empty());
	}

	{
		const auto list1 = aisdi::List<T>{1, 2, 3};
		auto list2 = list1;

		const auto first = list2.begin();
		const auto last = first;
		list2.erase(first, last);

		BOOST_CHECK(list2 == list1);
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
	auto il = {T{1}, T{2}, T{3}};
	auto list = aisdi::List<T>{il};

	const auto first = list.begin();
	const auto last = std::next(first);
	const auto afterLastRemoved = list.erase(first, last);

	BOOST_CHECK(afterLastRemoved == list.begin());
	BOOST_CHECK(list.size() == (il.size() - 1));
	BOOST_CHECK(std::equal(list.begin(), list.end(), std::next(il.begin())));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingAtBeginning_ThenItemsAreRemovedFromFront,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	auto list = aisdi::List<T>{il};

	const auto count = 2;
	const auto first = list.begin();
	const auto last = (first + count);
	const auto afterLastRemoved = list.erase(first, last);

	BOOST_CHECK(afterLastRemoved == list.begin());
	BOOST_CHECK(list.size() == (il.size() - count));
	BOOST_CHECK(std::equal(list.begin(), list.end(), il.begin() + count));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingLastItem_ThenItIsPoppedFromBack,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	auto list = aisdi::List<T>{il};

	const auto last = list.end();
	const auto first = std::prev(last);
	const auto afterLastRemoved = list.erase(first, last);

	BOOST_CHECK(afterLastRemoved == list.end());
	BOOST_CHECK(list.size() == (il.size() - 1));
	BOOST_CHECK(std::equal(il.begin(), std::prev(il.end()), list.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingAtEnd_ThenItemsAreRemovedFromBack,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	auto list = aisdi::List<T>{il};

	const auto count = 2;
	const auto last = list.end();
	const auto first = (last - count);
	const auto afterLastRemoved = list.erase(first, last);

	BOOST_CHECK(afterLastRemoved == list.end());
	BOOST_CHECK(list.size() == (il.size() - count));
	BOOST_CHECK(std::equal(il.begin(), il.end() - count, list.begin()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingOneInMiddle_ThenItemIsRemoved,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	auto list = aisdi::List<T>{il};

	const auto shift = 1;
	const auto first = (list.begin() + shift);
	const auto last = std::next(first);
	const auto afterLastRemoved = list.erase(first, last);

	BOOST_CHECK(afterLastRemoved == (list.begin() + shift));
	BOOST_CHECK(list.size() == (il.size() - 1));
	BOOST_CHECK(std::equal(list.begin(), list.begin() + shift,
		il.begin()));
	BOOST_CHECK(std::equal(list.begin() + shift + 1, list.end(),
		il.begin() + shift + 1));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingInMiddle_ThenItemsAreRemoved,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}, T{4}, T{5}};
	auto list = aisdi::List<T>{il};

	const auto shift = 1;
	const auto count = 2;
	const auto first = (list.begin() + shift);
	const auto last = (first + count);
	const auto afterLastRemoved = list.erase(first, last);

	BOOST_CHECK(afterLastRemoved == (list.begin() + shift));
	BOOST_CHECK(list.size() == (il.size() - count));
	BOOST_CHECK(std::equal(list.begin(), list.begin() + shift,
		il.begin()));
	BOOST_CHECK(std::equal(list.begin() + shift + count, list.end(),
		il.begin() + shift + count));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
	GivenContainer_WhenErasingFromBeginToEnd_ThenItIsEmpty,
	T, TestTypes)
{
	auto il = {T{1}, T{2}, T{3}};
	auto list = aisdi::List<T>{il};

	const auto first = list.begin();
	const auto last = list.end();
	const auto afterLastRemoved = list.erase(first, last);

	BOOST_CHECK(afterLastRemoved == list.end());
	BOOST_CHECK(list.empty());
}
