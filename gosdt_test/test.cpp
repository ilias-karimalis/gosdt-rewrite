#include "../gosdt/graph.hpp"

#include <iostream>
#include <unordered_map>

int main() {

    auto node_map = std::unordered_map<gosdt::Bitset, gosdt::Node, std::hash<gosdt::Bitset>>();
    auto b1 = gosdt::Bitset(3, true);
    auto b2 = gosdt::Bitset(3, false);
    auto b3 = gosdt::Bitset(3, true); b3.set(2, false);

    node_map.emplace(b1, gosdt::Node());
    node_map.emplace(b2, gosdt::Node());
    node_map.emplace(b3, gosdt::Node());

    auto n1 = node_map.find(b1);
    n1->second.upper_bound = 32;

    std::cout << node_map.find(b1)->second.upper_bound << std::endl;



}