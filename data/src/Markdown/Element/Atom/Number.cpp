#include "Number.hpp"

#include <iostream>

#include "libcpp/Maybe.hpp"

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

  std::ostream& operator<<(std::ostream& os, const Number& number)
  {
    if (number.type_ == Number::kInteger)
      os << number.data_.integer;
    else if (number.type_ == Number::kFloat)
      os << number.data_.floats;
    return os;
  }
}; // namespace atom

Number::Number()
  : Atom<atom::Number>() {}

Number::Number(int number)
  : Atom<atom::Number>(atom::Number(number)) {}

Number::Number(float number)
  : Atom<atom::Number>(atom::Number(number)) {}

Number::Number(const Number& object)
  : Atom<atom::Number>(object) {}

Number::~Number() {}

Number& Number::operator=(const Number& other)
{
  Atom<atom::Number>::operator=(other);
  return (*this);
}

int Number::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

bool Number::is_atom() const
{
  return true;
}
