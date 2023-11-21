#ifndef MARKDOWN_OBJECTS_OBJECT_HPP
#define MARKDOWN_OBJECTS_OBJECT_HPP

#include <string>
#include <memory>

#include "Types.hpp"

/**
 * @brief This represents an object in the markdown document.
*/
class MarkdownObject
{
  public:
    MarkdownObject();
    MarkdownObject(const MarkdownObject& other);
    MarkdownObject& operator=(const MarkdownObject& other) = delete;
    virtual ~MarkdownObject();

    /**
     * @brief To accesssing an object, you have to use a perspective to view it
    */
    template <typename View_, typename Object>
    class Perspective
    {
      public:
        Perspective();
        Perspective(const Perspective& other);
        Perspective& operator=(const Perspective& other) = delete;
        virtual ~Perspective();

        // Each perspective correspond to a view that it will create
        friend class View_;
        using View = View_;

        virtual View_ apply(const Object& object) const;
    };

    /**
     * @brief A view is a way to see an object
    */
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

    /**
     * @brief This method applies a perspective to this object and returns a View.
    */
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