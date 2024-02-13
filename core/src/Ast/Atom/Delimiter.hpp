#pragma once

#include <string>

#include "Ast/Element.hpp"
#include "Encoder.hpp"

class Delimiter : public MarkdownElement
{
  public:
    Delimiter();
    Delimiter(const std::string& delimiter);
    Delimiter(const Delimiter& object);
    virtual ~Delimiter();

    Delimiter& operator=(const Delimiter& other);

    virtual int encode(Encoder& encoder) const override;
    virtual ElementType  type() const override;

    void  set_data(const std::string& delimiter);
    const std::string& data() const;
  
  protected:
    std::string  data_;
};
