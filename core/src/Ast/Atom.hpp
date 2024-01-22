#pragma once

#include <string>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"

template <typename T>
class Atom : public MarkdownElement
{
  public:
    Atom();
    Atom(T Atom);
    Atom(const Atom& object);
    virtual ~Atom();

    Atom& operator=(const Atom& other);

    virtual int encode(Encoder& encoder) const override;
    virtual ElementType  type() const override;

    T& data();
    const T& data() const;

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
ElementType Atom<T>::type() const
{
  return ElementType::kAtom;
}

template <typename T>
T& Atom<T>::data()
{
  return data_;
}

template <typename T>
const T& Atom<T>::data() const
{
  return data_;
}
