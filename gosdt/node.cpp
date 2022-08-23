#include "node.hpp"

namespace GOSDT {

    Node::Node()
    : upper_bound(std::numeric_limits<f32>::max()), lower_bound(-std::numeric_limits<f32>::max())
    {}

    Node::Node(Bitset capture_set, f32 ub, f32 lb, f32 mcr, usize cm, Node *parent)
    : capture_set(std::move(capture_set)), upper_bound(ub), lower_bound(lb), max_cost_reduction(mcr),
    cost_minimizer(cm)
    {
        if (parent != nullptr)
            parents.push_back(parent);
    }
}