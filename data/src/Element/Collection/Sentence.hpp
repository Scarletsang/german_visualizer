#pragma once

#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Element/Element.hpp"
#include "Element/Collection.hpp"
#include "Element/Atom/Word.hpp"
#include "Element/Atom/Number.hpp"
#include "Element/Atom/Character.hpp"

class Sentence : public Collection<MarkdownElement>
{
  public:
    Sentence();
    Sentence(const Sentence& object);
    virtual ~Sentence();

    Sentence& operator=(const Sentence& other);

    virtual int encode(Encoder& encoder) const override;
    virtual bool  is_atom() const override;

    using Collection<MarkdownElement>::add_element;

    void  add_word(std::shared_ptr<Word> word);
    void  add_number(std::shared_ptr<Number> number);
    void  add_character(std::shared_ptr<Character> character);

};
