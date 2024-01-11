#ifndef PERSPECTIVE_HPP
# define PERSPECTIVE_HPP

#include <stdexcept>

class MarkdownObject;

/**
 * @brief To accesssing an object, you have to use a perspective to view it
*/
template <typename Object, typename _View>
class Perspective
{
  public:
    Perspective();
    Perspective(const Perspective& other);
    Perspective& operator=(const Perspective& other) = delete;
    virtual ~Perspective();

    using View = _View;

    View apply(const Object& object) const;
};

/**
 * @brief A view is a way to see an object. Each perspective
 * correspond to a view that it will create.
*/
class View
{
  public:
    View(const View& other) = delete;
    View& operator=(const View& other) = delete;
    virtual ~View() {};
};

/////////////////////////////////////////////////
////////////   template implementation   ////////
/////////////////////////////////////////////////

template <typename Object, typename _View>
Perspective<Object, _View>::Perspective() {}

template <typename Object, typename _View>
Perspective<Object, _View>::Perspective(const Perspective& other) {}

template <typename Object, typename _View>
Perspective<Object, _View>::~Perspective() {}

template <typename Object, typename _View>
Perspective<Object, _View>::View Perspective<Object, _View>::apply(const Object& object) const
{
  return _View(object);
}

#endif