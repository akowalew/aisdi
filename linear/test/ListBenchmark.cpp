#include <hayai.hpp>

#include <cstdlib>
 
#include <aisdi/List.hpp>

constexpr auto Iterations = 100000;

class InsertionBenchmark
    :   public ::hayai::Fixture
{
public:
	aisdi::List<int> container;
};
 
BENCHMARK_F(InsertionBenchmark, AppendTest, 10, Iterations)
{
    container.append(rand() % 10000);
}

BENCHMARK_F(InsertionBenchmark, PrependTest, 10, Iterations)
{
    container.prepend(rand() % 10000);
}

class ErasingBenchmark
    :   public ::hayai::Fixture
{
public:
    void SetUp() override
    {
    	for(auto i = 0; i < Iterations; ++i)
    	{
    		container.append(rand() % 10000);
    	}
    }

	aisdi::List<int> container;
};
 
BENCHMARK_F(ErasingBenchmark, PopBackTest, 10, Iterations)
{
    container.popBack();
}

BENCHMARK_F(ErasingBenchmark, PopFrontTest, 10, Iterations)
{
    container.popFront();
}

class CopyingBenchmark
    :   public ::hayai::Fixture
{
public:
    void SetUp() override
    {
        for(auto i = 0; i < Iterations; ++i)
        {
            container.append(rand() % 10000);
        }
    }

    aisdi::List<int> container;
};

BENCHMARK_F(CopyingBenchmark, CopyConstructorTest, 100, 1)
{
    auto other = container;
    static_cast<void>(other);
}

class ComparingBenchmark
    :   public ::hayai::Fixture
{
public:
    void SetUp() override
    {
        for(auto i = 0; i < Iterations; ++i)
        {
            container.append(rand() % 10000);
            other.append(rand() % 10000);
        }
    }

    aisdi::List<int> container;
    aisdi::List<int> other;
}; 

BENCHMARK_F(ComparingBenchmark, EqualityTest, 1000, 1)
{
    const auto equals = (container == other);
    static_cast<void>(equals);
}
