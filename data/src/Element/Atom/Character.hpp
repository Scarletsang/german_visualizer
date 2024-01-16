#pragma once

#include "Element/Atom.hpp"
#include "Encoder.hpp"

class Character : public Atom<char>
{
  public:
    Character();
    Character(char character);
    Character(const Character& object);
    virtual ~Character();

    Character& operator=(const Character& other);

    virtual int encode(Encoder& encoder) const override;
    virtual bool  is_atom() const override;
};
