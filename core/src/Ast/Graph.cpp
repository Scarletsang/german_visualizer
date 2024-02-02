#include "Graph.hpp"

int Graph::encode(Encoder& encoder) const
{
    return encoder.encode(*this);
}

ElementType Graph::type() const
{
    return ElementType::kGraphCollection;
}
