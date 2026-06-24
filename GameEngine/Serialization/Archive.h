#pragma once

#include <iostream>
#include <string_view>
namespace ge
{
class XmlOutputArchive
{
public:
  XmlOutputArchive();
  XmlOutputArchive(XmlOutputArchive&&)                 = default;
  XmlOutputArchive(const XmlOutputArchive&)            = default;
  XmlOutputArchive& operator=(XmlOutputArchive&&)      = default;
  XmlOutputArchive& operator=(const XmlOutputArchive&) = default;
  ~XmlOutputArchive();

  template <class T>
  void Field(std::string_view name, T& value, bool isObject = false)
  {
    if (isObject) {
      std::cout << "recursing on Object: " << name << std::endl;
      value.TrySerialize(*this);
    } else {
      std::cout << "writing value: " << value << std::endl;
    }
  }

private:
};
} // namespace ge
