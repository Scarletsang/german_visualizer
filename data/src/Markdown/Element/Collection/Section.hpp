#pragma once

#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Element/Element.hpp"
#include "Element/Collection.hpp"
#include "Element/Title.hpp"

class Section : public Collection<MarkdownElement>
{
  public:
    Section();
    Section(std::shared_ptr<Title> title);
    Section(const Section& object);
    virtual ~Section();

    Section& operator=(const Section& other);

    virtual int encode(Encoder& encoder) const override;
    virtual bool  is_atom() const override;

    void  set_title(std::shared_ptr<Title> title);

    std::shared_ptr<Title> title() const;

  protected:
    std::shared_ptr<Title> title_;
};
