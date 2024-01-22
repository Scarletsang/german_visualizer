#pragma once

#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Ast/Element.hpp"
#include "Ast/DomCollection.hpp"
#include "Ast/Title.hpp"

class Section : public DomCollection<MarkdownElement>
{
  public:
    Section();
    Section(std::shared_ptr<Title> title);
    Section(const Section& object);
    virtual ~Section();

    Section& operator=(const Section& other);

    virtual int encode(Encoder& encoder) const override;

    void  set_title(std::shared_ptr<Title> title);

    std::shared_ptr<Title> title() const;

  protected:
    std::shared_ptr<Title> title_;
};
