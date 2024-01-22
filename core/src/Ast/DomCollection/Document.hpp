#pragma once

#include "Encoder.hpp"
#include "Ast/DomCollection.hpp"
#include "Ast/Element.hpp"

class Document : public DomCollection<MarkdownElement>
{
  public:
    Document();
    Document(const Document& object);
    virtual ~Document();

    Document& operator=(const Document& other);

    virtual int encode(Encoder& encoder) const override;
};
