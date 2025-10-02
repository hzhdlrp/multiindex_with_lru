#include "src/tests/lru_basic_tests.h"
#include "src/implements/lru_time_index_container.h"
#include "src/benchmarks/lru_basic_benchmarks.h"
#include "src/benchmarks/lru_google_benchmarks.h"

int main() {
    test_lru_users<LRUCacheContainer_TimeIndex>();
    test_lru_products<LRUCacheContainer_TimeIndex>();
    std::cout << "all tests success" << std::endl;

    benchmark::simple_benchmark<LRUCacheContainer_TimeIndex>("output_time_index.txt");
    benchmark::google_benchmark<LRUCacheContainer_TimeIndex>("google_output_time_index.txt");
    return 0;
}