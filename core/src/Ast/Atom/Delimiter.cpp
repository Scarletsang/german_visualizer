#include "Delimiter.hpp"

Delimiter::Delimiter()
  : Atom<std::string>() {}

Delimiter::Delimiter(const std::string& delimiter)
  : Atom<std::string>(delimiter) {}

Delimiter::Delimiter(const Delimiter& object)
  : Atom<std::string>(object) {}

Delimiter::~Delimiter() {}

Delimiter& Delimiter::operator=(const Delimiter& other)
{
  if (this != &other)
    Atom<std::string>::operator=(other);
  return *this;
}

int Delimiter::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

bool  Delimiter::is_atom() const
{
  return true;
}
