#ifndef MARKDOWN_OBJECTS_COMPOSE_HPP
# define MARKDOWN_OBJECTS_COMPOSE_HPP

#include <vector>
#include <memory>

#include "Object.hpp"
#include "Perspective.hpp"
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

    virtual MarkdownType  get_type() const override;
    void                  set_type(MarkdownType type);

    class View : public ::View<MarkdownCompose>
    {
      public:
        View(MarkdownCompose& data);
        virtual ~View();

        const MarkdownCompose::Objects& objects() const;
        void                            add(std::weak_ptr<MarkdownObject> object);
        void                            add(MarkdownCompose::Objects& object);
    };

    class Perspective : public ::Perspective<MarkdownCompose, View>
    {
      public:
        virtual ~Perspective();
    };

  protected:
    Objects       objects_;
    MarkdownType  type_;
};

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

    class View : public ::View<MarkdownLinks>
    {
      public:
        View(MarkdownCompose& data);
        virtual ~View();

        std::weak_ptr<MarkdownObject> get_target() const;
        void                          set_target(std::weak_ptr<MarkdownObject> target);

        const MarkdownCompose::Objects& objects() const;
        void                            add(std::weak_ptr<MarkdownObject> object);
        void                            add(MarkdownCompose::Objects& object);
    };

    class Perspective : public ::Perspective<MarkdownLinks, View>
    {
      public:
        virtual ~Perspective();
    };

    virtual MarkdownType          get_type() const override;

  protected:
    std::weak_ptr<MarkdownObject> target_;
};

#endif