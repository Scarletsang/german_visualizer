#include "Compose.hpp"
#include "Types.hpp"


/////////////////////////////////////////////
////////////   MarkdownCompose   ////////////
/////////////////////////////////////////////

MarkdownCompose::MarkdownCompose() : type_(kInvalid) {}

MarkdownCompose::MarkdownCompose(MarkdownType type) : type_(type) {}

MarkdownCompose::MarkdownCompose(const MarkdownCompose& other)
  : type_(other.type_), objects_(other.objects_) {}

MarkdownCompose::~MarkdownCompose() {}

MarkdownType MarkdownCompose::get_type() const
{
  return type_;
}

const MarkdownCompose::Objects& MarkdownCompose::get_objects() const
{
  return objects_;
}

void  MarkdownCompose::set_type(MarkdownType type)
{
  type_ = type;
}

void  MarkdownCompose::add(std::weak_ptr<MarkdownObject> object)
{
  objects_.push_back(object);
}

void  MarkdownCompose::add(Objects& objects)
{
  objects_.insert(objects_.end(), objects.begin(), objects.end());
}

///////////////////////////////////////////
////////////   MarkdownLinks   ////////////
///////////////////////////////////////////

MarkdownLinks::MarkdownLinks() : MarkdownCompose(kInvalid) {}

MarkdownLinks::MarkdownLinks(MarkdownType type) : MarkdownCompose(type) {}

MarkdownLinks::MarkdownLinks(std::weak_ptr<MarkdownObject> target, MarkdownType type)
  : MarkdownCompose(type), target_(target) {}

MarkdownLinks::MarkdownLinks(const MarkdownLinks& other)
  : MarkdownCompose(other), target_(other.target_) {}

MarkdownLinks::~MarkdownLinks() {}

MarkdownType  MarkdownLinks::get_type() const
{
  return type_;
}

std::weak_ptr<MarkdownObject> MarkdownLinks::get_target() const
{
  return target_;
}

void  MarkdownLinks::set_target(std::weak_ptr<MarkdownObject> target)
{
  target_ = target;
}
