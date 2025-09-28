#include <iostream>
#include <cassert>
#include <string>

#include "lru_cache_container.h"

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

void test_lru_users() {
    using UserCache = LRUCacheContainer<
        User,
        indexed_by<
            ordered_unique<tag<id_tag>, member<User, int, &User::id>>,
            ordered_unique<tag<email_tag>, member<User, std::string, &User::email>>,
            ordered_non_unique<tag<name_tag>, member<User, std::string, &User::name>>
        >
    >;
    
    UserCache cache(3); // capacity == 3
    
    cache.emplace(User{1, "alice@test.com", "Alice"});
    cache.emplace(User{2, "bob@test.com", "Bob"});
    cache.emplace(User{3, "charlie@test.com", "Charlie"});
    
    // find by id
    auto by_id = cache.get<id_tag>().find(1);
    assert(by_id != cache.get<id_tag>().end());
    assert(by_id->get().name == "Alice");
    
    // find by email
    auto by_email = cache.get<email_tag>().find("bob@test.com");
    assert(by_email != cache.get<email_tag>().end());
    assert(by_email->get().id == 2);
    
    //find by name
    auto by_name = cache.get<name_tag>().find("Charlie");
    assert(by_name != cache.get<name_tag>().end());
    assert(by_name->get().email == "charlie@test.com");
    
    //find by email
    auto it = cache.find<email_tag, std::string>("alice@test.com");
    assert(it != cache.get<email_tag>().end());
    
    //find by id
    cache.find<id_tag, int>(1); 

    // capacity == 3, Alice, Charlie was recently used -> Bob will be ousted 
    cache.emplace(User{4, "david@test.com", "David"}); 
    
    assert((!cache.contains<id_tag, int>(2))); // Bob outsed
    assert((cache.contains<id_tag, int>(1))); 
    assert((cache.contains<id_tag, int>(3))); 
    assert((cache.contains<id_tag, int>(4))); 

    std::cout << "test_lru_users correct" << std::endl;
    
}

void test_lru_products() {

    struct Product {
        std::string sku;
        std::string name;
        double price;
    };
    
    struct sku_tag {};
    struct name_tag {};
    
    using ProductCache = LRUCacheContainer<
        Product,
        indexed_by<
            ordered_unique<tag<sku_tag>, member<Product, std::string, &Product::sku>>,
            ordered_unique<tag<name_tag>, member<Product, std::string, &Product::name>>
        >
    >;
    
    ProductCache cache(2);
    
    cache.emplace(Product{"A1", "Laptop", 999.99});
    cache.emplace(Product{"A2", "Mouse", 29.99});
    
    auto laptop = cache.find<sku_tag, std::string>("A1");
    assert(laptop != cache.get<sku_tag>().end());
    
    // A1 was used, so A2 should be ousted
    cache.emplace(Product{"A3", "Keyboard", 79.99});
    
    assert((cache.contains<sku_tag, std::string>("A1"))); // used
    assert((cache.contains<sku_tag, std::string>("A3"))); // new
    assert((!cache.contains<sku_tag, std::string>("A2"))); // ousted
    
    // 
    assert((cache.get<name_tag>().find("Keyboard") != cache.get<name_tag>().end()));
    assert((cache.get<name_tag>().find("Mouse") == cache.get<name_tag>().end()));
    
    std::cout << "test_lru_products correct" << std::endl;
}
