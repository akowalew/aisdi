#include <hayai.hpp>

#include <cstdlib>

#include <aisdi/TreeMap.hpp>

constexpr auto Iterations = 1000;

class InsertionBenchmark
    :   public ::hayai::Fixture
{
public:
	aisdi::TreeMap<int, int> container;
};

BENCHMARK_F(InsertionBenchmark, AccessTest, 10, Iterations)
{
    container[rand() % 1000000000] = (rand() % 10000);
}


class ErasingBenchmark
    :   public ::hayai::Fixture
{
public:
    void SetUp() override
    {
    	for(auto i = 0; i < Iterations; ++i)
    	{
		    container[rand() % 1000000000] = (rand() % 10000);
    	}
    }

	aisdi::TreeMap<int, int> container;
};

BENCHMARK_F(ErasingBenchmark, PopFrontTest, 10, Iterations)
{
    container.erase(container.begin());
}

BENCHMARK_F(ErasingBenchmark, PopBackTest, 10, Iterations)
{
    container.erase(std::prev(container.end()));
}


class CopyingBenchmark
    :   public ::hayai::Fixture
{
public:
    void SetUp() override
    {
    	for(auto i = 0; i < Iterations; ++i)
    	{
            container[rand() % 1000000000] = (rand() % 10000);
    	}
    }

	aisdi::TreeMap<int, int> container;
};

BENCHMARK_F(CopyingBenchmark, CopyConstructorTest, 100, 1)
{
    auto other = container;
    static_cast<void>(other);
}


class SearchingBenchmark
    :   public ::hayai::Fixture
{
public:
    void SetUp() override
    {
        for(auto i = 0; i < Iterations; ++i)
        {
            container[rand() % 1000000000] = (rand() % 10000);
        }
    }

    aisdi::TreeMap<int, int> container;
};

BENCHMARK_F(SearchingBenchmark, CopyConstructorTest, 100, 1)
{
    container.find(rand() % 1000000000);
}


class ComparingBenchmark
    :   public ::hayai::Fixture
{
public:
    void SetUp() override
    {
    	for(auto i = 0; i < Iterations; ++i)
    	{
            container[rand() % 1000000000] = (rand() % 10000);
            other[rand() % 1000000000] = (rand() % 10000);
    	}
    }

	aisdi::TreeMap<int, int> container;
	aisdi::TreeMap<int, int> other;
};

BENCHMARK_F(ComparingBenchmark, EqualityTest, 1000, 1)
{
    const auto equals = (container == other);
    static_cast<void>(equals);
}
