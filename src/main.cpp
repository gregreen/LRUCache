
#include "lru_cache.h"
#include <iostream>
#include <limits>
#include <vector>


int main(int argc, char* argv[]) {
    uint32_t capacity = 4;
    double empty_value = std::numeric_limits<double>::quiet_NaN();
    LRUCache::LRUCache<int, double> cache(capacity, empty_value);

    cache.set(0, 1.);
    cache.set(1, 2.);
    cache.set(2, 3.);
    cache.set(3, 4.);
    cache.set(5, 6.);
    
    std::cout << cache.get(0) << std::endl;
    std::cout << cache.get(1) << std::endl;
    std::cout << cache.get(2) << std::endl;
    std::cout << cache.get(3) << std::endl;
    std::cout << cache.get(4) << std::endl;
    std::cout << cache.get(5) << std::endl;


    LRUCache::CachedFunction<int, double> f(
        [](int x) -> double {
            return x*x;
        },
        capacity
    );

    for(int i=-5; i<=5; i++) {
        std::cout << f(i) << std::endl;
    }
    for(int i=5; i>=-5; i--) {
        std::cout << f(i) << std::endl;
    }
    
    LRUCache::CachedFunction<std::vector<double>, double, LRUCache::VectorHasher<double>> g(
        [](const std::vector<double>& vec) -> double {
            double ret = 1.;
            for(auto v : vec) {
                ret *= v;
            }
            return ret;
        },
        capacity
    );

    for(int i=1; i<8; i++) {
        std::vector<double> v;
        for(int j=1; (j<=i) && (j<=5); j++) {
            v.push_back((double)j);
        }
        std::cout << g(v) << std::endl;
    }

    return 0;
}

