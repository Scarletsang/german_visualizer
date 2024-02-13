#include "Delimiter.hpp"

Delimiter::Delimiter()
  : data_() {}

Delimiter::Delimiter(const std::string& delimiter)
  : data_(delimiter) {}

Delimiter::Delimiter(const Delimiter& object)
  : data_(object.data_) {}

Delimiter::~Delimiter() {}

Delimiter& Delimiter::operator=(const Delimiter& other)
{
  if (this != &other)
    data_ = other.data_;
  return *this;
}

int Delimiter::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

ElementType Delimiter::type() const
{
  return ElementType::kAtom;
}

void  Delimiter::set_data(const std::string& delimiter)
{
  data_ = delimiter;
}

const std::string& Delimiter::data() const
{
  return data_;
}
