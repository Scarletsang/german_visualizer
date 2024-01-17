#pragma once

#include <string>

#include "Element/Atom.hpp"
#include "Encoder.hpp"

class Word : public Atom<std::string>
{
  public:
    Word();
    Word(const std::string& word);
    Word(const Word& object);
    virtual ~Word();

    Word& operator=(const Word& other);

    virtual int encode(Encoder& encoder) const override;
    virtual bool  is_atom() const override;
};
