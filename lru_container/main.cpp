#include "src/tests/lru_time_index_tests.h"
#include "src/implements/lru_time_index_container.h"

int main() {
    test_lru_users<LRUCacheContainer_TimeIndex>();
    test_lru_products<LRUCacheContainer_TimeIndex>();
    std::cout << "all tests success" << std::endl;
    return 0;
}