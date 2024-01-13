#pragma once

#include "Encoder.hpp"
#include "Element/Collection.hpp"
#include "Element/Element.hpp"

class Document : public Collection<MarkdownElement>
{
  public:
    Document();
    Document(const Document& object);
    virtual ~Document();

    Document& operator=(const Document& other);

    virtual int encode(Encoder& encoder) const override;
    virtual bool  is_atom() const override;
};
