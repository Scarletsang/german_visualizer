#pragma once

#include <iostream>
#include <optional>

#include "Ast/Element.hpp"
#include "Encoder.hpp"

namespace atom
{
  class Number
  {
    public:
      enum type
      {
        kInteger,
        kFloat
      };
      union data
      {
        int integer;
        float floats;
      };
      Number();
      Number(int number);
      Number(float number);
      Number(const Number& object);
      virtual ~Number();

      Number& operator=(const Number& other);

      void  set_integer(int number);
      void  set_float(float number);

      std::optional<int>  get_integer() const;
      std::optional<float>  get_float() const;

      friend std::ostream& operator<<(std::ostream& os, const Number& number);
    protected:
      union data data_;
      enum type type_;
  };

}; // namespace atom

class Number : public MarkdownElement
{
  public:
    Number();
    Number(int number);
    Number(float number);
    Number(atom::Number number);
    Number(const Number& object);
    virtual ~Number();

    Number& operator=(const Number& other);

    virtual int encode(Encoder& encoder) const override;
    virtual ElementType  type() const override;

    void  set_data(atom::Number number);
    const atom::Number& data() const;
  
  protected:
    atom::Number  data_;
};
