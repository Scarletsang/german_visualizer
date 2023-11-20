#ifndef MARKDOWN_OBJECTS_COMPOSE_HPP
# define MARKDOWN_OBJECTS_COMPOSE_HPP

#include <vector>
#include <memory>

#include "Object.hpp"
#include "Types.hpp"

class MarkdownCompose : public MarkdownObject
{
  public:
    typedef std::vector<std::weak_ptr<MarkdownObject> >  Objects;
    MarkdownCompose();
    MarkdownCompose(MarkdownType type);
    MarkdownCompose(const MarkdownCompose& other);
    virtual ~MarkdownCompose();

    virtual MarkdownType  get_type() const;
    const Objects&        get_objects() const;
    void                  set_type(MarkdownType type);

    void                  add(std::weak_ptr<MarkdownObject> object);
    void                  add(Objects& object);

  protected:
    Objects       objects_;
    MarkdownType  type_;
};

class MarkdownLinks : public virtual MarkdownCompose
{
  public:
    MarkdownLinks();
    MarkdownLinks(MarkdownType type);
    MarkdownLinks(std::weak_ptr<MarkdownObject> target, MarkdownType type);
    MarkdownLinks(const MarkdownLinks& other);
    virtual ~MarkdownLinks();

    virtual MarkdownType          get_type() const;
    std::weak_ptr<MarkdownObject> get_target() const;
    void                          set_target(std::weak_ptr<MarkdownObject> target);

  protected:
    std::weak_ptr<MarkdownObject> target_;
};

#endif