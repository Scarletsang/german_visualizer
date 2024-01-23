#include "Directed.hpp"

#include <memory>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"
#include "Ast/GraphCollection/Edge.hpp"

DirectedEdge::DirectedEdge()
  : Edge() {}

DirectedEdge::DirectedEdge(std::shared_ptr<MarkdownElement> sender)
  : Edge(sender) {}

DirectedEdge::DirectedEdge(std::shared_ptr<MarkdownElement> sender, std::shared_ptr<MarkdownElement> receiver)
  : Edge(sender, receiver) {}

DirectedEdge::DirectedEdge(const DirectedEdge& edge)
  : Edge(edge) {}

DirectedEdge& DirectedEdge::operator=(const DirectedEdge& edge)
{
  if (this == &edge)
    return *this;
  Edge::operator=(edge);
  return *this;
}

DirectedEdge::~DirectedEdge() {}

int DirectedEdge::encode(Encoder& encoder) const
{
  encoder.encode(*this);
  return EXIT_SUCCESS;
}

bool DirectedEdge::is_sender(std::shared_ptr<MarkdownElement> element) const
{
  return element == first_;
}

bool DirectedEdge::is_receiver(std::shared_ptr<MarkdownElement> element) const
{
  return element == second_;
}
