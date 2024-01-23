#include "Undirected.hpp"

#include <memory>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"
#include "Ast/GraphCollection/Edge.hpp"

UndirectedEdge::UndirectedEdge()
  : Edge() {}

UndirectedEdge::UndirectedEdge(std::shared_ptr<MarkdownElement> sender)
  : Edge(sender) {}

UndirectedEdge::UndirectedEdge(std::shared_ptr<MarkdownElement> sender, std::shared_ptr<MarkdownElement> receiver)
  : Edge(sender, receiver) {}

UndirectedEdge::UndirectedEdge(const UndirectedEdge& edge)
  : Edge(edge) {}

UndirectedEdge& UndirectedEdge::operator=(const UndirectedEdge& edge)
{
  if (this == &edge)
    return *this;
  Edge::operator=(edge);
  return *this;
}

UndirectedEdge::~UndirectedEdge() {}

int UndirectedEdge::encode(Encoder& encoder) const
{
  encoder.encode(*this);
  return EXIT_SUCCESS;
}

bool UndirectedEdge::is_sender(std::shared_ptr<MarkdownElement> element) const
{
  return (element == first_) || (element == second_);
}

bool UndirectedEdge::is_receiver(std::shared_ptr<MarkdownElement> element) const
{
  return (element == second_) || (element == first_);
}
