#ifndef MARKDOWN_OBJECTS_OBJECT_HPP
#define MARKDOWN_OBJECTS_OBJECT_HPP

#include <string>

#include "Types.hpp"

class Perspective;

class MarkdownObject
{
  public:
    MarkdownObject();
    MarkdownObject(const MarkdownObject& other);
    virtual ~MarkdownObject();
    virtual MarkdownObject* view(Perspective& perspective) const;
    virtual MarkdownType    get_type() const;

  private:
    MarkdownObject& operator=(const MarkdownObject& other);
};

class Perspective
{
  public:
    Perspective();
    Perspective(const Perspective& other);
    virtual ~Perspective();

    virtual MarkdownObject* apply(const MarkdownObject& object) const;
};

#endif