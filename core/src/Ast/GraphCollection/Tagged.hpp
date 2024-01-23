#pragma once

#include <memory>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"
#include "Ast/GraphCollection/Edge.hpp"

template <typename T>
class TaggedEdge : public MarkdownElement
{
  public:
    TaggedEdge();
    TaggedEdge(std::shared_ptr<Edge> edge);
    TaggedEdge(std::shared_ptr<Edge> edge, const T& tag);
    TaggedEdge(const TaggedEdge& edge);
    TaggedEdge& operator=(const TaggedEdge& edge);
    virtual ~TaggedEdge();

    virtual int encode(Encoder& encoder) const;
    virtual ElementType type() const;

    void  set_data(std::shared_ptr<Edge> edge);
    void  set_tag(const T& tag);

    std::shared_ptr<Edge> data();
    const T& tag() const;
    T&  tag();

  protected:
    std::shared_ptr<Edge> edge_;
    T tag_;
};

/////////////////////////////////////////////////////
////////////   template implementation   ////////////
/////////////////////////////////////////////////////

template <typename T>
TaggedEdge<T>::TaggedEdge()
  : edge_(nullptr), tag_() {}

template <typename T>
TaggedEdge<T>::TaggedEdge(std::shared_ptr<Edge> edge)
  : edge_(edge), tag_() {}

template <typename T>
TaggedEdge<T>::TaggedEdge(std::shared_ptr<Edge> edge, const T& tag)
  : edge_(edge), tag_(tag) {}

template <typename T>
TaggedEdge<T>::TaggedEdge(const TaggedEdge& edge)
  : edge_(edge.edge_), tag_(edge.tag_) {}

template <typename T>
TaggedEdge<T>& TaggedEdge<T>::operator=(const TaggedEdge& edge)
{
  edge_ = edge.edge_;
  tag_ = edge.tag_;
  return *this;
}

template <typename T>
TaggedEdge<T>::~TaggedEdge() {}

template <typename T>
int TaggedEdge<T>::encode(Encoder& encoder) const
{
  encoder.encode(*this);
  return EXIT_SUCCESS;
}

template <typename T>
ElementType TaggedEdge<T>::type() const
{
  return ElementType::kGraphCollection;
}

template <typename T>
void TaggedEdge<T>::set_data(std::shared_ptr<Edge> edge)
{
  edge_ = edge;
}

template <typename T>
void TaggedEdge<T>::set_tag(const T& tag)
{
  tag_ = tag;
}

template <typename T>
std::shared_ptr<Edge> TaggedEdge<T>::data()
{
  return edge_;
}

template <typename T>
const T& TaggedEdge<T>::tag() const
{
  return tag_;
}

template <typename T>
T& TaggedEdge<T>::tag()
{
  return tag_;
}
