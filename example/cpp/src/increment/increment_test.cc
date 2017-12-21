#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <string>

#if defined PRE_INCREMENT
#define FOREACH_SAFE(container, it)                                 \
    for (auto it = (container).begin(), __nit = it;                 \
         (it != (container).end()) && (++__nit, true); it = __nit)
#elif defined POST_INCREMENT
#define FOREACH_SAFE(container, it)                                 \
    for (auto it = (container).begin(), __nit = it;                 \
         (it != (container).end()) && (__nit++, true); it = __nit)
#endif

void test_vector(std::vector<std::string> &vec) {
    std::cout << "vector: " << std::endl;
    FOREACH_SAFE(vec, it) {
        std::cout << *it << std::endl;
    }
}

void test_set(std::set<std::string> &set) {
    std::cout << "set: " << std::endl;
    FOREACH_SAFE(set, it) {
        std::cout << *it << std::endl;
    }
}

void test_map(std::map<std::string, std::string> &map) {
    std::cout << "map: " << std::endl;
    FOREACH_SAFE(map, it) {
        std::cout << "key: " << it->first
                  << " value: " << it->second << std::endl;
    }
}

void test_hashmap(std::unordered_map<std::string, std::string> &hashmap) {
    std::cout << "hashmap: " << std::endl;
    FOREACH_SAFE(hashmap, it) {
        std::cout << "key: " << it->first
                  << " value: " << it->second << std::endl;
    }
}

int main () {
    std::vector<std::string> vec;
    for (int i = 0; i < 3; i++) {
        vec.push_back(std::to_string(i));
    }
    test_vector(vec);

    std::set<std::string> set;
    for (int i = 0; i < 3; i++) {
        set.insert(std::to_string(i));
    }
    test_set(set);

    std::map<std::string, std::string> map;
    for (int i = 0; i < 3; i++) {
        map.insert({std::to_string(i), std::to_string(i)});
    }
    test_map(map);

    std::unordered_map<std::string, std::string> hashmap;
    for (int i = 0; i < 3; i++) {
        hashmap.insert({std::to_string(i), std::to_string(i)});
    }
    test_hashmap(hashmap);
}
