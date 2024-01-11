#ifndef MARKDOWN_OBJECTS_OBJECT_HPP
#define MARKDOWN_OBJECTS_OBJECT_HPP

#include <string>

#include "Types.hpp"
#include "Perspective.hpp"

/**
 * @brief This represents an object in the markdown document.
*/
class MarkdownObject
{
  public:
    MarkdownObject();
    MarkdownObject(const MarkdownObject& other);
    MarkdownObject(MarkdownObject&& other) noexcept = default;
    MarkdownObject& operator=(const MarkdownObject& other) = delete;
    virtual ~MarkdownObject();

    /**
     * @brief This method applies a perspective to this object and returns a View.
    */
    template <typename T>
    typename T::View        view(T& perspective) const;

    virtual MarkdownType    get_type() const = 0;
};
/////////////////////////////////////////////////
////////////   template implementation   ////////
/////////////////////////////////////////////////

template <typename T>
typename T::View MarkdownObject::view(T& perspective) const
{
  return perspective.apply(*this);
}

#endif