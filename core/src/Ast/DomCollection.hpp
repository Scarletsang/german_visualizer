#pragma once

#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"

template <typename T>
class DomCollection : public MarkdownElement
{
  public:
    using element = std::shared_ptr<T>;
    DomCollection();
    DomCollection(const DomCollection& object);
    virtual ~DomCollection();

    DomCollection& operator=(const DomCollection& other);

    virtual int encode(Encoder& encoder) const override;
    virtual ElementType  type() const override;

    virtual int  add_element(element element);
    virtual int  add_elements(std::vector<element>& elements);

    const std::vector<element>& data() const;
    std::vector<element>& data();

  protected:
    std::vector<element> data_;
};

/////////////////////////////////////////////////////
////////////   template implementation   ////////////
/////////////////////////////////////////////////////

template <typename T>
DomCollection<T>::DomCollection() : data_() {}

template <typename T>
DomCollection<T>::DomCollection(const DomCollection& object)
  : data_(object.data_) {}

template <typename T>
DomCollection<T>::~DomCollection() {}

template <typename T>
DomCollection<T>& DomCollection<T>::operator=(const DomCollection& other)
{
  if (this != &other)
    data_ = other.data_;
  return *this;
}

template <typename T>
int DomCollection<T>::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

template <typename T>
ElementType  DomCollection<T>::type() const
{
  return ElementType::kDomCollection;
}

template <typename T>
int  DomCollection<T>::add_element(element element)
{
  data_.push_back(element);
  return EXIT_SUCCESS;
}

template <typename T>
int  DomCollection<T>::add_elements(std::vector<element>& elements)
{
  for (auto& element : elements)
    data_.push_back(element);
  return EXIT_SUCCESS;
}

template <typename T>
const std::vector<typename DomCollection<T>::element>& DomCollection<T>::data() const
{
  return data_;
}

template <typename T>
std::vector<typename DomCollection<T>::element>& DomCollection<T>::data()
{
  return data_;
}
