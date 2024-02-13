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

  void Number::set_integer(int number)
  {
    this->type_ = kInteger;
    this->data_.integer = number;
  }

  void Number::set_float(float number)
  {
    this->type_ = kFloat;
    this->data_.floats = number;
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
  : data_() {}

Number::Number(int number)
  : data_(atom::Number(number)) {}

Number::Number(float number)
  : data_(atom::Number(number)) {}

Number::Number(atom::Number number)
  : data_(number) {}

Number::Number(const Number& object)
  : data_(object.data_) {}

Number::~Number() {}

Number& Number::operator=(const Number& other)
{
  data_ = other.data_;
  return (*this);
}

int Number::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

ElementType Number::type() const
{
  return ElementType::kAtom;
}

void  Number::set_data(atom::Number number)
{
  data_ = number;
}

const atom::Number& Number::data() const
{
  return data_;
}
