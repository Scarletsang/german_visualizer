#pragma once

#include "Ast/Element.hpp"
#include "Encoder.hpp"

class Character : public MarkdownElement
{
  public:
    Character();
    Character(char character);
    Character(const Character& object);
    virtual ~Character();

    Character& operator=(const Character& other);

    virtual int encode(Encoder& encoder) const override;
    virtual ElementType  type() const override;

    void  set_data(char character);
    const char& data() const;

  protected:
    char  data_;
};
