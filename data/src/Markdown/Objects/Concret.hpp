#ifndef MARKDOWN_OBJECTS_OBJECTS_HPP
#define MARKDOWN_OBJECTS_OBJECTS_HPP

#include <string>

#include "Object.hpp"
#include "Types.hpp"
#include "Compose.hpp"

template <typename T>
class ConcretObject : public MarkdownObject
{
  public:
    ConcretObject();
    ConcretObject(const T& object);
    ConcretObject(const ConcretObject& other);
    virtual ~ConcretObject();

    class View : public MarkdownObject::View
    {
      public:
        View(ConcretObject& object);
        virtual ~View();
        T&  data();
    };

    class Perspective : public MarkdownObject::Perspective<View, ConcretObject>
    {
      public:
        virtual ~Perspective();        
        virtual View apply(ConcretObject& object) const;
    };

    virtual MarkdownType    get_type() const override;

  protected:
    T             data_;
    MarkdownType  type_;
};

/////////////////////////////////////////////////////
////////////   template implementation   ////////////
/////////////////////////////////////////////////////

template <typename T>
ConcretObject<T>::ConcretObject() : MarkdownObject() {}

template <typename T>
ConcretObject<T>::ConcretObject(const T& object)
  : MarkdownObject(), data_(object) {}

template <typename T>
ConcretObject<T>::ConcretObject(const ConcretObject& other)
  : MarkdownObject(other), data_(other.data_) {}

template <typename T>
ConcretObject<T>::~ConcretObject() {}

template <typename T>
ConcretObject<T>::Perspective::~Perspective() {}

template <typename T>
ConcretObject<T>::View ConcretObject<T>::Perspective::apply(ConcretObject<T>& object) const
{
  return object.data_;
}

template <typename T>
MarkdownType ConcretObject<T>::get_type() const
{
  return type_;
}

template <typename T>
ConcretObject<T>::View::View(ConcretObject<T>& object)
  : MarkdownObject::View(object) {}

template <typename T>
ConcretObject<T>::View::~View() {}

template <typename T>
T& ConcretObject<T>::View::data()
{
  return object_.data_;
}

#endif