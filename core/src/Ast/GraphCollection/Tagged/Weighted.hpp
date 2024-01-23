#pragma once

#include <memory>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"
#include "Ast/GraphCollection/Edge.hpp"
#include "Ast/GraphCollection/Tagged.hpp"

class WeightedEdge : public TaggedEdge<float>
{
  public:
    WeightedEdge();
    WeightedEdge(std::shared_ptr<Edge> edge);
    WeightedEdge(std::shared_ptr<Edge> edge, double weight);
    WeightedEdge(const WeightedEdge& edge);
    WeightedEdge& operator=(const WeightedEdge& edge);
    virtual ~WeightedEdge();

    virtual int encode(Encoder& encoder) const;
};
