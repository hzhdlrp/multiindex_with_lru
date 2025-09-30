#pragma once

#include "../lru_container_concept.h"

namespace {

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

template <template <typename> LRUCacheType LRUCacheContainer>
void simple_benchmark() {
    using UserCache = LRUCacheContainer<
        User,
        indexed_by<
            ordered_unique<tag<id_tag>, member<User, int, &User::id>>,
            ordered_unique<tag<email_tag>, member<User, std::string, &User::email>>,
            ordered_non_unique<tag<name_tag>, member<User, std::string, &User::name>>
        >
    >;
}

}