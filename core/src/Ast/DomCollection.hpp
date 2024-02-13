#pragma once

#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "Ast/Type.hpp"
#include "Ast/Element.hpp"

class DomCollection : public MarkdownElement
{
  public:
    using element = std::shared_ptr<MarkdownElement>;
    DomCollection();
    DomCollection(const DomCollection& object);
    virtual ~DomCollection();

    DomCollection& operator=(const DomCollection& other);

    virtual int encode(Encoder& encoder) const override;
    virtual ElementType  type() const override;

    virtual int  add_element(element element);
    virtual int  add_elements(const std::vector<element>& elements);

    void  set_data(std::vector<element>& data);
    const std::vector<element>& data() const;

  protected:
    std::vector<element> data_;
};
