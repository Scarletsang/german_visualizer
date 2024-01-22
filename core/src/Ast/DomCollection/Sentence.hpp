#pragma once

#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Ast/Element.hpp"
#include "Ast/DomCollection.hpp"
#include "Ast/Atom/Word.hpp"
#include "Ast/Atom/Number.hpp"
#include "Ast/Atom/Character.hpp"
#include "Ast/Atom/Delimiter.hpp"

class Sentence : public DomCollection<MarkdownElement>
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
