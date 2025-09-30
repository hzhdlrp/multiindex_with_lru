#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

#include "../lru_container_concept.h"

namespace benchmark {

struct id_tag {};
struct email_tag {};
struct name_tag {};

struct User {
    int id;
    std::string email;
    std::string name;
    
    bool operator==(const User& other) const {
        return id == other.id && email == other.email && name == other.name;
    }
};

namespace generator {
unsigned long long id_cntr = 0;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> action_dist(0.0, 1.0);
std::uniform_int_distribution<size_t> id_dist(0, 1000);

User generate_user() {
    std::string email = "email" + std::to_string(id_dist(gen));
    std::string name = "name" + std::to_string(id_dist(gen));
    return User{++id_cntr, email, name};
}

void clear_id_cntr() {
    id_cntr = 0;
}
} // generator

template<
    template<typename, typename, typename> class LRUCacheContainer
>
void simple_benchmark(std::string &&output_filename) {

    using UserCache = LRUCacheContainer<
        User,
        indexed_by<
            ordered_unique<tag<id_tag>, member<User, int, &User::id>>,
            ordered_unique<tag<email_tag>, member<User, std::string, &User::email>>,
            ordered_non_unique<tag<name_tag>, member<User, std::string, &User::name>>
        >,
        std::allocator<User>
    >;

    lru_concept_assert_for_one_tag(UserCache, id_tag, int, User);
    lru_concept_assert_for_one_tag(UserCache, email_tag, std::string, User);
    lru_concept_assert_for_one_tag(UserCache, name_tag, std::string, User);

    std::ofstream output_file(output_filename);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output file: " << output_filename << std::endl;
        return;
    }

    output_file << std::left << std::setw(12) << "Keys count" 
                << std::setw(16) << "Cache size" 
                << std::setw(12) << "Time (ms)" 
                << std::endl;
    output_file << std::string(50, '-') << std::endl;
    
    const std::vector<size_t> cache_sizes = {1000, 10000, 100000};
    const size_t operations_number = 100000;

    for (const size_t size : cache_sizes) {
        UserCache cache(size);
        for (size_t i = 0; i < size; ++i) {
            cache.emplace(generator::generate_user());
        }

        auto start_time = std::chrono::high_resolution_clock::now();
    }


}

}