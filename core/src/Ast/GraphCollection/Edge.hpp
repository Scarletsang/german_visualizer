#pragma once

#include <memory>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"

class Edge : public MarkdownElement
{
  public:
    Edge();
    Edge(std::shared_ptr<MarkdownElement> first);
    Edge(std::shared_ptr<MarkdownElement> first, std::shared_ptr<MarkdownElement> second);
    Edge(const Edge& edge);
    Edge& operator=(const Edge& edge);
    virtual ~Edge();

    virtual int encode(Encoder& encoder) const = 0;
    virtual ElementType type() const;

    virtual bool  is_sender(std::shared_ptr<MarkdownElement> element) const = 0;
    virtual bool  is_receiver(std::shared_ptr<MarkdownElement> element) const = 0;

    void set_first(std::shared_ptr<MarkdownElement> first);
    void set_second(std::shared_ptr<MarkdownElement> second);

  protected:
    std::shared_ptr<MarkdownElement> first_;
    std::shared_ptr<MarkdownElement> second_;
};
