#include "Number.hpp"

namespace atom
{
  Number::Number()
    : data_(), type_(kInteger) {}

  Number::Number(int number)
    : data_({ .integer = number }), type_(kInteger) {}

  Number::Number(float number)
    : data_({ .floats = number }), type_(kFloat) {}

  Number::Number(const Number& object)
    : data_(object.data_), type_(object.type_) {}

  Number::~Number() {}

  Number& Number::operator=(const Number& other)
  {
    this->type_ = other.type_;
    this->data_ = other.data_;
    return (*this);
  }

  libcpp::Maybe<int> Number::get_integer() const
  {
    if (this->type_ == kInteger)
      return (libcpp::Maybe<int>(this->data_.integer));
    return (libcpp::Maybe<int>());
  }

  libcpp::Maybe<float> Number::get_float() const
  {
    if (this->type_ == kFloat)
      return (libcpp::Maybe<float>(this->data_.floats));
    return (libcpp::Maybe<float>());
  }
}; // namespace atom
