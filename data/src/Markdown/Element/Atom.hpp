#pragma once

#include <string>

#include "Element.hpp"
#include "Encoder.hpp"
#include "Element/Atom/Number.hpp"

template <typename T>
class Atom : public MarkdownElement
{
  public:
    Atom();
    Atom(T Atom);
    Atom(const Atom& object);
    virtual ~Atom();

    Atom& operator=(const Atom& other);

    virtual int encode(Encoder& encoder) const;
    virtual bool  is_atom() const;

    T& data();

  protected:
    T data_;
};

template <typename T>
Atom<T>::Atom() : data_() {}

template <typename T>
Atom<T>::Atom(T data) : data_(data) {}

template <typename T>
Atom<T>::Atom(const Atom& object) : data_(object.data_) {}

template <typename T>
Atom<T>::~Atom() {}

template <typename T>
Atom<T>& Atom<T>::operator=(const Atom& other)
{
  if (this != &other)
  {
    data_ = other.data_;
  }
  return *this;
}

template <typename T>
int Atom<T>::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

template <typename T>
bool Atom<T>::is_atom() const
{
  return true;
}

template <typename T>
T& Atom<T>::data()
{
  return data_;
}

using Word = Atom<std::string>;
using Character = Atom<char>;
using Number = Atom<atom::Number>;
