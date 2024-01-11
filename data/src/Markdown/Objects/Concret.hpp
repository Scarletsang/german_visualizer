#ifndef MARKDOWN_OBJECTS_OBJECTS_HPP
#define MARKDOWN_OBJECTS_OBJECTS_HPP

#include <string>

#include "Object.hpp"
#include "Perspective.hpp"
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

    virtual MarkdownType    get_type() const override;

    class View : public ::View
    {
      public:
        View(ConcretObject& object);
        virtual ~View();
        virtual T&  data();
      
      private:
        ConcretObject& object_;
    };

    class Perspective : public ::Perspective<ConcretObject, View>
    {
      public:
        virtual ~Perspective();
    };

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
MarkdownType ConcretObject<T>::get_type() const
{
  return type_;
}

template <typename T>
ConcretObject<T>::View::View(ConcretObject<T>& object)
  : ::Perspective<ConcretObject>::View(object) {}

template <typename T>
ConcretObject<T>::View::~View() {}

template <typename T>
T& ConcretObject<T>::View::data()
{
  return object_.data_;
}

#endif