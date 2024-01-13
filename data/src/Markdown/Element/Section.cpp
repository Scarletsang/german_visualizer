#include "Section.hpp"

#include <vector>
#include <memory>

#include "Element.hpp"
#include "Encoder.hpp"
#include "Collection.hpp"
#include "Title.hpp"

Section::Section()
  : Collection<MarkdownElement>(), title_() {}

Section::Section(std::shared_ptr<Title> title)
  : Collection<MarkdownElement>(), title_(title) {}

Section::Section(const Section& object)
  : Collection<MarkdownElement>(object), title_(object.title_) {}

Section::~Section() {}

Section& Section::operator=(const Section& other)
{
  if (this != &other)
  {
    Collection<MarkdownElement>::operator=(other);
    title_ = other.title_;
  }
  return *this;
}

int Section::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

bool  Section::is_atom() const
{
  return false;
}

void  Section::set_title(std::shared_ptr<Title> title)
{
  title_ = title;
}

std::shared_ptr<Title> Section::title() const
{
  return title_;
}
