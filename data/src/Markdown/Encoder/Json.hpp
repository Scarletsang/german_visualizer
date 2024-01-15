#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <type_traits>

#include "Encoder.hpp"
#include "Encoder/Json/Settings.hpp"
#include "Elements.hpp"

class JsonEncoder : public Encoder
{
  public:
    JsonEncoder(std::ostream& stream, const JsonEncoderSettings& settings = {});
    JsonEncoder(const JsonEncoder& object);
    virtual ~JsonEncoder();

    virtual int encode(const Character& character) override;
    virtual int encode(const Word& word) override;
    virtual int encode(const Number& number) override;

    // group of atoms
    virtual int encode(const Sentence& sentence) override;
    // level and one line
    virtual int encode(const Title& title) override;

    // group of lines
    virtual int encode(const Paragraph& paragraph) override;
    // group of a title, and a group of elements with the same level
    virtual int encode(const Section& section) override;
    // group of sections
    virtual int encode(const Document& document) override;

    virtual int encode(const MarkdownElement& element) override;

    template <typename T, typename = std::enable_if<std::is_base_of<MarkdownElement, T>::value> >
    int encode(const std::vector<std::weak_ptr<T> >& elements);

    template <typename T, typename = std::enable_if<std::is_base_of<MarkdownElement, T>::value> >
    int encode(const std::vector<std::shared_ptr<T> >& elements);

  protected:
    std::ostream& output_;
    JsonEncoderSettings settings_;

    virtual void  write_indent();
    virtual void  write_newline_indent();
    virtual void  write_key(const std::string& key);
    virtual void  write_pair(const std::string& key, 
                             const std::string& value,
                             bool comma);
    virtual void  write_pair(const std::string& key, 
                             char value,
                             bool comma);
    virtual void  write_pair(const std::string& key, 
                             const atom::Number& value,
                             bool comma);
    virtual void  write_pair(const std::string& key, 
                             int value,
                             bool comma);
    virtual void  write_pair(const std::string& key,
                             const std::shared_ptr<MarkdownElement>& value,
                             bool comma);

    virtual void  write_sanitize(const std::string& string);
    virtual void  write_sanitize(char string);
};

/////////////////////////////////////////////////////
////////////   template implementation   ////////////
/////////////////////////////////////////////////////

template <typename T, typename>
int JsonEncoder::encode(const std::vector<std::weak_ptr<T> >& elements)
{
  output_ << '[';
  {
    settings_.increase_level();
    for (auto it = elements.begin(); it != elements.end(); ++it)
    {
      write_newline_indent();
      (*it).lock()->encode(*this);
      if (it + 1 != elements.end())
        output_ << ',';
    }
    settings_.decrease_level();
  }
  write_newline_indent();
  output_ << ']';
  return EXIT_SUCCESS;
}

template <typename T, typename>
int JsonEncoder::encode(const std::vector<std::shared_ptr<T> >& elements)
{
  output_ << '[';
  {
    settings_.increase_level();
    for (auto it = elements.begin(); it != elements.end(); ++it)
    {
      write_newline_indent();
      (*it)->encode(*this);
      if (it + 1 != elements.end())
        output_ << ',';
    }
    settings_.decrease_level();
  }
  write_newline_indent();
  output_ << ']';
  return EXIT_SUCCESS;
}
