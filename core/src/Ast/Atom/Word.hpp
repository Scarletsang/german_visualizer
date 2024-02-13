#pragma once

#include <string>

#include "Ast/Element.hpp"
#include "Encoder.hpp"

class Word : public MarkdownElement
{
  public:
    Word();
    Word(const std::string& word);
    Word(const Word& object);
    virtual ~Word();

    Word& operator=(const Word& other);

    virtual int encode(Encoder& encoder) const override;
    virtual ElementType  type() const override;

    void  set_data(const std::string& word);
    const std::string& data() const;
  
  protected:
    std::string  data_;
};
