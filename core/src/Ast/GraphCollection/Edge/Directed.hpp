#pragma once

#include <memory>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"
#include "Ast/GraphCollection/Edge.hpp"

class DirectedEdge : public Edge
{
  public:
    DirectedEdge();
    DirectedEdge(std::shared_ptr<MarkdownElement> sender);
    DirectedEdge(std::shared_ptr<MarkdownElement> sender, std::shared_ptr<MarkdownElement> receiver);
    DirectedEdge(const DirectedEdge& edge);
    DirectedEdge& operator=(const DirectedEdge& edge);
    virtual ~DirectedEdge();

    virtual int encode(Encoder& encoder) const;

    virtual bool  is_sender(std::shared_ptr<MarkdownElement> element) const;
    virtual bool  is_receiver(std::shared_ptr<MarkdownElement> element) const;
};
