#include "Core/UUID.h"
#include <gtest/gtest.h>

namespace
{
TEST(UUIDTests, NilCreatesNilUuidWithExpectedSize)
{
  const psych::UUID id = psych::UUID::Nill();

  EXPECT_TRUE(id.is_nil());
  EXPECT_EQ(id.size(), psych::UUID::static_size());
  EXPECT_EQ(id.size(), 16u);
}

TEST(UUIDTests, DefaultUuidIsNonNilAndCopiesCompareEqual)
{
  const psych::UUID id;
  const psych::UUID copy = id;

  EXPECT_FALSE(id.is_nil());
  EXPECT_EQ(copy, id);
  EXPECT_EQ(copy.Value(), id.Value());
}

TEST(UUIDTests, DifferentGeneratedUuidsCompareNotEqual)
{
  const psych::UUID first;
  const psych::UUID second;

  EXPECT_NE(first, second);
}
} // namespace
