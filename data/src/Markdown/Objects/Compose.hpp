#ifndef MARKDOWN_OBJECTS_COMPOSE_HPP
# define MARKDOWN_OBJECTS_COMPOSE_HPP

#include <vector>
#include <memory>

#include "Object.hpp"
#include "Types.hpp"


/**
 * @brief An object the contains objects
*/
class MarkdownCompose : public MarkdownObject
{
  public:
    typedef std::vector<std::weak_ptr<MarkdownObject> >  Objects;
    MarkdownCompose();
    MarkdownCompose(MarkdownType type);
    MarkdownCompose(const MarkdownCompose& other);
    virtual ~MarkdownCompose();

    friend class compose_perspectives::ComposePerspective;
    using Perspective = compose_perspectives::ComposePerspective;

    virtual MarkdownType  get_type() const override;
    void                  set_type(MarkdownType type);

  protected:
    Objects       objects_;
    MarkdownType  type_;
};

namespace compose_perspectives
{
  class ComposeView : public MarkdownObject::View
  {
    public:
      ComposeView(MarkdownCompose& data);
      virtual ~ComposeView();

      const MarkdownCompose::Objects& objects() const;
      virtual void                    add(std::weak_ptr<MarkdownObject> object);
      virtual void                    add(MarkdownCompose::Objects& object);
  };

  class ComposePerspective : public MarkdownObject::Perspective<ComposeView, MarkdownCompose>
  {
    public:
      virtual ~ComposePerspective();
      virtual ComposeView apply(const MarkdownCompose& object) const override;
  };
}

/**
 * @brief An object that contains objects but also has a main object
*/
class MarkdownLinks : public virtual MarkdownCompose
{
  public:
    MarkdownLinks();
    MarkdownLinks(MarkdownType type);
    MarkdownLinks(std::weak_ptr<MarkdownObject> target, MarkdownType type);
    MarkdownLinks(const MarkdownLinks& other);
    virtual ~MarkdownLinks();

    friend class compose_perspectives::LinksPerspective;
    using Perspective = compose_perspectives::LinksPerspective;

    virtual MarkdownType          get_type() const override;

  protected:
    std::weak_ptr<MarkdownObject> target_;
};

namespace compose_perspectives
{
  // TODO
  class LinksView : public MarkdownObject::View
  {
    public:
      LinksView(MarkdownCompose& data);
      virtual ~LinksView();

      std::weak_ptr<MarkdownObject> get_target() const;
      void                          set_target(std::weak_ptr<MarkdownObject> target);

      virtual void                  add(std::weak_ptr<MarkdownObject> object) override;
      virtual void                  add(Objects& object) override;
  };

  class LinksPerspective : public MarkdownObject::Perspective<LinksView, MarkdownLinks>
  {
    public:
      virtual ~LinksPerspective();
      virtual LinksView apply(MarkdownLinks& object) const;
  };
}

#endif