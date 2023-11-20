#include "Markdown.hpp"

#include <algorithm>

Markdown::Markdown() {}

Markdown::~Markdown() {}

std::weak_ptr<Character>  Markdown::create_character(const char character)
{
  return add(std::make_shared<Character>(character));
}

std::weak_ptr<Word> Markdown::create_word(const std::string& word)
{
  return add(std::make_shared<Word>(word));
}

std::weak_ptr<Title>  Markdown::create_title(const std::string& title, int level)
{
  return add(std::make_shared<Title>(title, level));
}

std::weak_ptr<MarkdownLinks>  Markdown::create_word_linked(const std::string& word)
{
  return add_links(create_word(word), kWord);
}

std::weak_ptr<MarkdownLinks>  Markdown::create_title_linked(const std::string& title, int level)
{
  return add_links(create_title(title, level), kTitle);
}

std::weak_ptr<MarkdownCompose>  Markdown::create_line()
{
  return add_compose(kLine);
}

std::weak_ptr<MarkdownLinks>  Markdown::create_paragraph()
{
  return add_links(add_compose(kParagraph), kParagraph);
}

std::weak_ptr<MarkdownCompose>  Markdown::create_document()
{
  return add_compose(kDocument);
}

std::shared_ptr<MarkdownObject> Markdown::remove_object(std::weak_ptr<MarkdownObject> object)
{
  Hash  hash = std::hash<std::shared_ptr<MarkdownObject> >()(object.lock());

  if (this->objects_.find(hash) != this->objects_.end())
  {
    std::shared_ptr<MarkdownObject>  object = this->objects_[hash];

    this->objects_.erase(hash);
    return object;
  }
  return nullptr;
}

const Markdown::Objects&  Markdown::get_objects() const
{
  return this->objects_;
}

template <typename T>
std::weak_ptr<T>   Markdown::add(std::shared_ptr<T> object)
{
  Hash  hash = std::hash<std::shared_ptr<T> >()(object);

  if (this->objects_.find(hash) == this->objects_.end())
    this->objects_[hash] = object;
  return object;
}

template <typename T>
std::weak_ptr<MarkdownLinks>  Markdown::add_links(std::weak_ptr<T> object, MarkdownType type)
{
  return this->add(new MarkdownLinks(object, type));
}

std::weak_ptr<MarkdownCompose>  Markdown::add_compose(MarkdownType type)
{
  return this->add(std::make_shared<MarkdownCompose>(type));
}
