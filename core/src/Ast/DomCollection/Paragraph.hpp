#pragma once

#include "Encoder.hpp"
#include "Ast/DomCollection.hpp"
#include "Ast/DomCollection/Sentence.hpp"

class Paragraph : public DomCollection
{
  public:
    Paragraph();
    Paragraph(const Paragraph& object);
    virtual ~Paragraph();

    Paragraph& operator=(const Paragraph& other);

    virtual int encode(Encoder& encoder) const override;

    const std::vector<std::shared_ptr<Sentence>>& data() const;
};
