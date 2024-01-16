#pragma once

#include <memory>

#include "Encoder.hpp"
#include "Element/Element.hpp"
#include "Element/Collection.hpp"

class Title : public MarkdownElement
{
  public:
    Title();
    Title(int level);
    Title(int level, std::shared_ptr<Paragraph> paragraph);
    Title(const Title& object);
    virtual ~Title();

    Title& operator=(const Title& other);

    virtual int encode(Encoder& encoder) const override;
    virtual bool  is_atom() const override;

    int level() const;
    std::shared_ptr<Paragraph> data() const;

    void  set_level(int level);
    void  set_data(std::shared_ptr<Paragraph> paragraph);

  protected:
    int level_;
    std::shared_ptr<Paragraph> data_;
};
