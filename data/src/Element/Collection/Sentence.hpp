#pragma once

#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Element/Element.hpp"
#include "Element/Collection.hpp"
#include "Element/Atom/Word.hpp"
#include "Element/Atom/Number.hpp"
#include "Element/Atom/Character.hpp"
#include "Element/Atom/Delimiter.hpp"

class Sentence : public Collection<MarkdownElement>
{
  public:
    Sentence();
    Sentence(const Sentence& object);
    virtual ~Sentence();

    Sentence& operator=(const Sentence& other);

    virtual int encode(Encoder& encoder) const override;
    virtual bool  is_atom() const override;

    int  add_element(std::shared_ptr<Word> word);
    int  add_element(std::shared_ptr<Number> number);
    int  add_element(std::shared_ptr<Character> character);
    int  add_element(std::shared_ptr<Delimiter> delimiter);
    int  add_element(std::shared_ptr<MarkdownElement> element) override;
};
