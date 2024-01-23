#include "Weighted.hpp"

WeightedEdge::WeightedEdge()
  : TaggedEdge<float>() {}

WeightedEdge::WeightedEdge(std::shared_ptr<Edge> edge)
  : TaggedEdge<float>(edge) {}

WeightedEdge::WeightedEdge(std::shared_ptr<Edge> edge, double weight)
  : TaggedEdge<float>(edge, weight) {}

WeightedEdge::WeightedEdge(const WeightedEdge& edge)
  : TaggedEdge<float>(edge) {}

WeightedEdge& WeightedEdge::operator=(const WeightedEdge& edge)
{
  TaggedEdge<float>::operator=(edge);
  return *this;
}

WeightedEdge::~WeightedEdge() {}

int WeightedEdge::encode(Encoder& encoder) const
{
  encoder.encode(*this);
  return EXIT_SUCCESS;
}
