#pragma once

#include "Encoder.hpp"
#include "Element/Collection.hpp"
#include "Element/Collection/Sentence.hpp"

class Paragraph : public Collection<Sentence>
{
  public:
    Paragraph();
    Paragraph(const Paragraph& object);
    virtual ~Paragraph();

    Paragraph& operator=(const Paragraph& other);

    virtual int encode(Encoder& encoder) const override;
    virtual bool  is_atom() const override;
};
