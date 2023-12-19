#include <gtest/gtest.h>

#include "../../Helpers/Classes/Dependencies.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Factories/ServiceFactory.hpp"
#include "SevenBit/DI/Details/Helpers/ServiceFactoryInvoker.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ServiceFactoryInvoker : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceFactoryInvoker() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceFactoryInvoker() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceFactoryInvoker, ShouldInvokeFuncFactory)
{
    ServiceProviderMock mock;
    sb::di::details::services::InPlaceService<TestDependencyClass> test1;

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    auto func = [&](TestDependencyClass *test1) {
        EXPECT_TRUE(test1);
        return 1;
    };

    sb::di::details::helpers::ServiceFactoryInvoker<decltype(func)> invoker{func, mock};

    EXPECT_EQ(invoker.invoke(), 1);
}