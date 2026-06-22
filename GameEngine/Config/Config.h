#pragma once

namespace ge
{
class Config
{
public:
  Config();
  Config(Config&&)                 = default;
  Config(const Config&)            = default;
  Config& operator=(Config&&)      = default;
  Config& operator=(const Config&) = default;
  ~Config();

private:
};
} // namespace ge
