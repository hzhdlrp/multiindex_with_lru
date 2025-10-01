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

const std::vector<size_t> CACHE_SIZES = {1000, 10000, 100000};
const size_t OPERATIONS_NUMBER = 100000;
const int MAX_ID_SIZE = 50000;

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
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> action_dist(0.0, 1.0);
std::uniform_int_distribution<int> id_dist(0, MAX_ID_SIZE);

User generate_user() {
    std::string email = "email" + std::to_string(id_dist(gen));
    std::string name = "name" + std::to_string(id_dist(gen));
    return User{id_dist(gen), email, name};
}

int generate_id() {
    return id_dist(gen);
}

std::string generate_name() {
    return "name" + std::to_string(id_dist(gen));
}

std::string generate_email() {
    return "email" + std::to_string(id_dist(gen));
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

    output_file << std::left << std::setw(20) << "Operations count" 
                << std::setw(16) << "Cache size" 
                << std::setw(12) << "Time (ms)" 
                << std::endl;
    output_file << std::string(50, '-') << std::endl;

    for (const size_t size : CACHE_SIZES) {
        UserCache cache(size);
        for (size_t i = 0; i < size; ++i) {
            cache.emplace(generator::generate_user());
        }

        size_t reading_operations_number = OPERATIONS_NUMBER * 4 / 5;
        size_t writing_operations_number = OPERATIONS_NUMBER / 5;

        auto start_time = std::chrono::high_resolution_clock::now();
        

        for (size_t i = 0; i < reading_operations_number; ++i) {
            cache.template get<name_tag>().find(generator::generate_name());
            cache.template get<email_tag>().find(generator::generate_email());
            cache.template get<id_tag>().find(generator::generate_id());
        }

        for (size_t i = 0; i < writing_operations_number; ++i) {
            cache.emplace(generator::generate_user());
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        output_file << std::left << std::setw(20) << OPERATIONS_NUMBER
                    << std::setw(16) << size 
                    << std::setw(12) << elapsed.count()
                    << std::endl;
        output_file << std::string(50, '-') << std::endl;
    }


}

}