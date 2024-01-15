#pragma once

#include <string>

#include "Element/Atom.hpp"
#include "Encoder.hpp"

class Delimiter : public Atom<std::string>
{
  public:
    Delimiter();
    Delimiter(const std::string& delimiter);
    Delimiter(const Delimiter& object);
    virtual ~Delimiter();

    Delimiter& operator=(const Delimiter& other);

    virtual int encode(Encoder& encoder) const override;
    virtual bool  is_atom() const override;
};
