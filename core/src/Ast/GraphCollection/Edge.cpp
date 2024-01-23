#include "Edge.hpp"

#include <memory>

#include "Ast/Type.hpp"

Edge::Edge()
  : first_(nullptr), second_(nullptr) {}

Edge::Edge(std::shared_ptr<MarkdownElement> first)
  : first_(first), second_(nullptr) {}

Edge::Edge(std::shared_ptr<MarkdownElement> first, std::shared_ptr<MarkdownElement> second)
  : first_(first), second_(second) {}

Edge::Edge(const Edge& edge)
  : first_(edge.first_), second_(edge.second_) {}

Edge& Edge::operator=(const Edge& edge)
{
  first_ = edge.first_;
  second_ = edge.second_;
  return *this;
}

Edge::~Edge() {}

ElementType Edge::type() const
{
  return ElementType::kGraphCollection;
}

void Edge::set_first(std::shared_ptr<MarkdownElement> first)
{
  first_ = first;
}

void Edge::set_second(std::shared_ptr<MarkdownElement> second)
{
  second_ = second;
}
