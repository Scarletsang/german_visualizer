#pragma once

#include "Maybe.hpp"

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

      libcpp::Maybe<int>  get_integer() const;
      libcpp::Maybe<float>  get_float() const;

    protected:
      union data data_;
      enum type type_;
  };
}; // namespace atom
