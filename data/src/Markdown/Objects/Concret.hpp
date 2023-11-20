#ifndef MARKDOWN_OBJECTS_OBJECTS_HPP
#define MARKDOWN_OBJECTS_OBJECTS_HPP

#include <string>

#include "Object.hpp"
#include "Types.hpp"
#include "Compose.hpp"

class Character : public MarkdownObject
{
  public:
    Character();
    Character(const char character);
    Character(const Character& other);
    virtual ~Character();

    virtual MarkdownType  get_type() const;

  protected:
    char character_;
};

class Word : public MarkdownObject
{
  public:
    Word();
    Word(const std::string& word);
    Word(const Word& other);
    virtual ~Word();

    virtual MarkdownType  get_type() const;

  protected:
    std::string word_;
};

class Title : public MarkdownObject
{
  public:
    Title();
    Title(const std::string& word);
    Title(const std::string& word, int level);
    Title(const Title& other);
    virtual ~Title();

    virtual MarkdownType  get_type() const;

  protected:
    std::string title_;
    int         level_;
};

#endif