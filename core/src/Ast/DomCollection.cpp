#include "DomCollection.hpp"

DomCollection::DomCollection()
  : data_() {}

DomCollection::DomCollection(const DomCollection& object)
  : data_(object.data_) {}

DomCollection::~DomCollection() {}

DomCollection& DomCollection::operator=(const DomCollection& other)
{
  if (this != &other)
    data_ = other.data_;
  return *this;
}

int DomCollection::encode(Encoder& encoder) const
{
  return encoder.encode(*this);
}

ElementType DomCollection::type() const
{
  return ElementType::kDomCollection;
}

int  DomCollection::add_element(DomCollection::element element)
{
  data_.push_back(element);
  return EXIT_SUCCESS;
}

int  DomCollection::add_elements(const std::vector<DomCollection::element>& elements)
{
  data_.insert(data_.end(), elements.begin(), elements.end());
  return EXIT_SUCCESS;
}

void  DomCollection::set_data(std::vector<DomCollection::element>& data)
{
  data_ = data;
}

const std::vector<DomCollection::element>& DomCollection::data() const
{
  return data_;
}
