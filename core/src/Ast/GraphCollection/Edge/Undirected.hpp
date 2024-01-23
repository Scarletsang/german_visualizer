#pragma once

#include <memory>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"
#include "Ast/GraphCollection/Edge.hpp"

class UndirectedEdge : public Edge
{
  public:
    UndirectedEdge();
    UndirectedEdge(std::shared_ptr<MarkdownElement> sender);
    UndirectedEdge(std::shared_ptr<MarkdownElement> sender, std::shared_ptr<MarkdownElement> receiver);
    UndirectedEdge(const UndirectedEdge& edge);
    UndirectedEdge& operator=(const UndirectedEdge& edge);
    virtual ~UndirectedEdge();

    virtual int encode(Encoder& encoder) const;

    virtual bool  is_sender(std::shared_ptr<MarkdownElement> element) const;
    virtual bool  is_receiver(std::shared_ptr<MarkdownElement> element) const;
};
