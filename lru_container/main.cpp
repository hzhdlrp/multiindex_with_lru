#include "src/lru_tests.h"

int main() {
    test_lru_users();
    test_lru_products();
    std::cout << "all tests success" << std::endl;
    return 0;
}