#include "Number.hpp"

#include <iostream>
#include <optional>

namespace atom
{
  Number::Number()
    : data_(), type_(kInteger) {}

  Number::Number(int number)
    : type_(kInteger)
  {
    this->data_.integer = number;
  }

  Number::Number(float number)
    : type_(kFloat)
  {
    this->data_.floats = number;
  }

  Number::Number(const Number& object)
    : data_(object.data_), type_(object.type_) {}

  Number::~Number() {}

  Number& Number::operator=(const Number& other)
  {
    this->type_ = other.type_;
    this->data_ = other.data_;
    return (*this);
  }

  std::optional<int> Number::get_integer() const
  {
    if (this->type_ == kInteger)
      return (this->data_.integer);
    return (std::nullopt);
  }

  std::optional<float> Number::get_float() const
  {
    if (this->type_ == kFloat)
      return (this->data_.floats);
    return (std::nullopt);
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
