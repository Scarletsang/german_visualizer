#ifndef MARKDOWN_OBJECTS_OBJECT_HPP
#define MARKDOWN_OBJECTS_OBJECT_HPP

#include <string>
#include <memory>

#include "Types.hpp"

class MarkdownObject
{
  public:
    MarkdownObject();
    MarkdownObject(const MarkdownObject& other);
    MarkdownObject& operator=(const MarkdownObject& other) = delete;
    virtual ~MarkdownObject();

    template <typename View_, typename Object>
    class Perspective
    {
      public:
        Perspective();
        Perspective(const Perspective& other);
        Perspective& operator=(const Perspective& other) = delete;
        virtual ~Perspective();

        friend class View_;
        using View = View_;

        virtual View_ apply(const Object& object) const;
    };

    class View
    {
      public:
        View(MarkdownObject&);
        View(const View& other) = delete;
        View& operator=(const View& other) = delete;
        virtual ~View();

      private:
        MarkdownObject& object_;
    };

    template <typename T>
    typename T::View        view(T& perspective) const;

    virtual MarkdownType    get_type() const;
};
/////////////////////////////////////////////////
////////////   template implementation   ////////
/////////////////////////////////////////////////

template <typename View_, typename Object>
MarkdownObject::Perspective<View_, Object>::Perspective() {}

template <typename View_, typename Object>
MarkdownObject::Perspective<View_, Object>::Perspective(const Perspective& other) {}

template <typename View_, typename Object>
MarkdownObject::Perspective<View_, Object>::~Perspective() {}

template <typename View_, typename Object>
View_ MarkdownObject::Perspective<View_, Object>::apply(const Object& object) const
{
  throw std::runtime_error("Perspective: apply method not implemented");
}

template <typename T>
typename T::View MarkdownObject::view(T& perspective) const
{
  return perspective.apply(*this);
}

#endif