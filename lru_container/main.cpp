#include "src/tests/lru_basic_tests.h"
#include "src/implements/lru_time_index_container.h"
#include "src/benchmarks/lru_basic_benchmarks.h"
#include "src/benchmarks/lru_google_benchmarks.h"
#include "src/implements/lru_list_container.h"

int main() {
    test_lru_users<LRUCacheContainer_TimeIndex>();
    test_lru_products<LRUCacheContainer_TimeIndex>();
    std::cout << "all tests success" << std::endl;

    benchmark::simple_benchmark<LRUCacheContainer_TimeIndex>("output_time_index.txt");
    benchmark::google_benchmark<LRUCacheContainer_TimeIndex>("google_output_time_index.txt");

    test_lru_users<LRUCacheContainer_List>();
    test_lru_products<LRUCacheContainer_List>();
    std::cout << "all tests success" << std::endl;

    benchmark::simple_benchmark<LRUCacheContainer_List>("output_time_index.txt");
    benchmark::google_benchmark<LRUCacheContainer_List>("google_output_time_index.txt");
    return 0;
}