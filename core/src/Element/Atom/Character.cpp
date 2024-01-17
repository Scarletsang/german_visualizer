#include "Character.hpp"

Character::Character()
  : Atom<char>() {}

Character::Character(char character)
  : Atom<char>(character) {}

Character::Character(const Character& object)
  : Atom<char>(object) {}

Character::~Character() {}

Character& Character::operator=(const Character& other)
{
  if (this != &other)
    Atom<char>::operator=(other);
  return *this;
}

int Character::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

bool  Character::is_atom() const
{
  return true;
}
