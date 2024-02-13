#include "Character.hpp"

Character::Character()
  : data_(0) {}

Character::Character(char character)
  : data_(character) {}

Character::Character(const Character& object)
  : data_(object.data_) {}

Character::~Character() {}

Character& Character::operator=(const Character& other)
{
  if (this != &other)
    data_ = other.data_;
  return *this;
}

int Character::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

ElementType Character::type() const
{
  return ElementType::kAtom;
}

void Character::set_data(char character)
{
  data_ = character;
}

const char& Character::data() const
{
  return data_;
}
