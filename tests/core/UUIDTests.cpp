#include "Core/UUID.h"
#include <gtest/gtest.h>

namespace
{
TEST(UUIDTests, NilCreatesNilUuidWithExpectedSize)
{
  const ge::UUID id = ge::UUID::Nill();

  EXPECT_TRUE(id.is_nil());
  EXPECT_EQ(id.size(), ge::UUID::static_size());
  EXPECT_EQ(id.size(), 16u);
}

TEST(UUIDTests, DefaultUuidIsNonNilAndCopiesCompareEqual)
{
  const ge::UUID id;
  const ge::UUID copy = id;

  EXPECT_FALSE(id.is_nil());
  EXPECT_EQ(copy, id);
  EXPECT_EQ(copy.Value(), id.Value());
}

TEST(UUIDTests, DifferentGeneratedUuidsCompareNotEqual)
{
  const ge::UUID first;
  const ge::UUID second;

  EXPECT_NE(first, second);
}
} // namespace
