#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Services/AliasService.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class AliasServiceTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    AliasServiceTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~AliasServiceTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(AliasServiceTest, ShouldCreateExternalService)
{
    TestClass1 test;
    const sb::di::details::services::AliasService service{&test, typeid(TestClass1)};

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_EQ(service.get(), &test);
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(AliasServiceTest, ShouldFailMoveAsUniquePtrExternalService)
{
    TestClass1 test;
    sb::di::details::services::AliasService service{&test, typeid(TestClass1)};

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_THROW(service.moveOutAsUniquePtr<TestClass1>(), sb::di::CannotReleaseServiceException);
}

TEST_F(AliasServiceTest, ShouldFailMoveOutExternalService)
{
    TestClass1 test;
    sb::di::details::services::AliasService service{&test, typeid(TestClass1)};

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_THROW(service.moveOutAs<TestClass1>(), sb::di::CannotMoveOutServiceException);
}

TEST_F(AliasServiceTest, ShouldCreateExternalNullService)
{
    sb::di::details::services::AliasService service{nullptr, typeid(TestClass1)};

    EXPECT_FALSE(service);
    EXPECT_FALSE(service.isValid());
    EXPECT_FALSE(service.get());
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(AliasServiceTest, ShouldFailMoveAsUniquePtrExternalNullService)
{
    sb::di::details::services::AliasService service{nullptr, typeid(TestClass1)};

    EXPECT_FALSE(service);
    EXPECT_FALSE(service.isValid());
    EXPECT_THROW(service.moveOutAsUniquePtr<TestClass1>(), sb::di::CannotReleaseServiceException);
}

TEST_F(AliasServiceTest, ShouldFailMoveOutExternalNullService)
{
    sb::di::details::services::AliasService service{nullptr, typeid(TestClass1)};

    EXPECT_FALSE(service);
    EXPECT_FALSE(service.isValid());
    EXPECT_THROW(service.moveOutAs<TestClass1>(), sb::di::CannotMoveOutServiceException);
}
