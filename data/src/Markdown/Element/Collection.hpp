#pragma once

#include <vector>
#include <memory>

#include "Element.hpp"
#include "Encoder.hpp"

template <typename T>
class Collection : public MarkdownElement
{
  public:
    using element = std::shared_ptr<T>;
    Collection();
    Collection(const Collection& object);
    virtual ~Collection();

    Collection& operator=(const Collection& other);

    virtual int encode(Encoder& encoder) const;
    virtual bool  is_atom() const;

    void  add_element(element line);

    const std::vector<element>& data() const;
    std::vector<element>& data();

  protected:
    std::vector<element> data_;
};

/////////////////////////////////////////////////////
////////////   template implementation   ////////////
/////////////////////////////////////////////////////

template <typename T>
Collection<T>::Collection() : data_() {}

template <typename T>
Collection<T>::Collection(const Collection& object)
  : data_(object.data_) {}

template <typename T>
Collection<T>::~Collection() {}

template <typename T>
Collection<T>& Collection<T>::operator=(const Collection& other)
{
  if (this != &other)
    data_ = other.data_;
  return *this;
}

template <typename T>
int Collection<T>::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

template <typename T>
bool  Collection<T>::is_atom() const
{
  return false;
}

template <typename T>
void  Collection<T>::add_element(element line)
{
  data_.push_back(line);
}

template <typename T>
const std::vector<typename Collection<T>::element>& Collection<T>::data() const
{
  return data_;
}

template <typename T>
std::vector<typename Collection<T>::element>& Collection<T>::data()
{
  return data_;
}

using Paragraph = Collection<Sentence>;
using Document = Collection<MarkdownElement>;
