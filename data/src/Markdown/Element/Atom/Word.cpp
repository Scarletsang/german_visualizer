#include "Word.hpp"

Word::Word()
  : Atom<std::string>() {}

Word::Word(const std::string& character)
  : Atom<std::string>(character) {}

Word::Word(const Word& object)
  : Atom<std::string>(object) {}

Word::~Word() {}

Word& Word::operator=(const Word& other)
{
  if (this != &other)
    Atom<std::string>::operator=(other);
  return *this;
}

int Word::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

bool  Word::is_atom() const
{
  return true;
}
