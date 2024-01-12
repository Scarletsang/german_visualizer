#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Element.hpp"
#include "Encoder.hpp"
#include "Atom.hpp"

class Sentence : public MarkdownElement
{
  public:
    using element = std::weak_ptr<MarkdownElement>;
    Sentence();
    Sentence(const Sentence& object);
    virtual ~Sentence();

    Sentence& operator=(const Sentence& other);

    virtual int encode(Encoder& encoder, std::istream& input) const;
    virtual bool  is_atom() const;

    void  add_word(std::weak_ptr<Word> word);
    void  add_number(std::weak_ptr<Number> number);
    void  add_character(std::weak_ptr<Character> character);

    const std::vector<element>& data() const;
    std::vector<element>& data();

  private:
    std::vector<element> data_;
};
