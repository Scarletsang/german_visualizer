#ifndef MARKDOWN_MEMORY_HPP
# define MARKDOWN_MEMORY_HPP

#include <map>
#include <memory>

#include "Objects/Object.hpp"
#include "Objects/Compose.hpp"
#include "Objects/Concret.hpp"

class Markdown
{
  public:
    typedef size_t  Hash;
    typedef std::map<Hash, std::shared_ptr<MarkdownObject> > Objects;
    Markdown();
    ~Markdown();

    std::weak_ptr<Character>        create_character(const char character);
    std::weak_ptr<Word>             create_word(const std::string& word);
    std::weak_ptr<Title>            create_title(const std::string& title, int level);

    std::weak_ptr<MarkdownLinks>    create_word_linked(const std::string& word);
    std::weak_ptr<MarkdownLinks>    create_title_linked(const std::string& title, int level);

    std::weak_ptr<MarkdownCompose>  create_line();
    std::weak_ptr<MarkdownLinks>    create_paragraph();
    std::weak_ptr<MarkdownCompose>  create_document();

    std::shared_ptr<MarkdownObject> remove_object(std::weak_ptr<MarkdownObject>);

    const Objects&                  get_objects() const;

  private:
    Objects  objects_;

    template <typename T>
    std::weak_ptr<T>                add(std::shared_ptr<T> object);

    template <typename T>
    std::weak_ptr<MarkdownLinks>    add_links(std::weak_ptr<T> object, MarkdownType type);

    std::weak_ptr<MarkdownCompose>  add_compose(MarkdownType type);
};

#endif