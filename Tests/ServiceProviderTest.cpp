#include <gtest/gtest.h>
#include <memory>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "Classes/CirularDependency.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class ServiceProviderTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ServiceProviderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceProviderTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(ServiceProviderTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass2>();

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(ServiceProviderTest, ShouldFailGetServiceDueToAlreadyRegisteredInheritedService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestInheritClass1, TestInheritClass2>();
    collection.addSingleton<TestInheritClass1, TestInheritClass2>();
    collection.addSingleton<TestInheritClass1, TestInheritClass4>();

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(ServiceProviderTest, ShouldFailGetServiceDueToLifetimeMissmatchInheritedService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestInheritClass1, TestInheritClass3>();
    collection.addSingleton<TestInheritClass1, TestInheritClass5>();
    collection.addTransient<TestInheritClass1, TestInheritClass4>();

    EXPECT_THROW(collection.buildServiceProvider(), sb::di::ServiceLifeTimeMismatchException);
}

// tryGetInstance Tests

TEST_F(ServiceProviderTest, ShouldTryGetInstance)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addScoped<TestClass2>()
                              .addTransient<TestClass3>()
                              .buildServiceProvider();

    auto &instanceProvider = provider->asInstanceProvider();
    EXPECT_TRUE(instanceProvider.tryGetInstance(typeid(TestClass1)));
    EXPECT_TRUE(instanceProvider.tryGetInstance(typeid(TestClass2)));
    EXPECT_FALSE(instanceProvider.tryGetInstance(typeid(TestClass3)));
    EXPECT_FALSE(instanceProvider.tryGetInstance(typeid(TestClass4)));
}

TEST_F(ServiceProviderTest, ShouldTryGetInheritedInstance)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass2, TestInheritClass3>()
                              .addTransient<TestInheritClass3, TestInheritClass4>()
                              .buildServiceProvider();

    auto &instanceProvider = provider->asInstanceProvider();
    EXPECT_TRUE(instanceProvider.tryGetInstance(typeid(TestInheritClass1)));
    EXPECT_TRUE(instanceProvider.tryGetInstance(typeid(TestInheritClass2)));
    EXPECT_FALSE(instanceProvider.tryGetInstance(typeid(TestInheritClass3)));
    EXPECT_FALSE(instanceProvider.tryGetInstance(typeid(TestInheritClass4)));
}

// tryGetService Tests

TEST_F(ServiceProviderTest, ShouldTryGetService)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addScoped<TestClass2>()
                              .addTransient<TestClass3>()
                              .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetService<TestClass1>());
    EXPECT_TRUE(provider->tryGetService<TestClass2>());
    EXPECT_FALSE(provider->tryGetService<TestClass3>());
    EXPECT_FALSE(provider->tryGetService<TestClass4>());
}

TEST_F(ServiceProviderTest, ShouldTryGetInheritedService)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass2, TestInheritClass3>()
                              .addTransient<TestInheritClass3, TestInheritClass4>()
                              .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetService<TestInheritClass1>());
    EXPECT_TRUE(provider->tryGetService<TestInheritClass2>());
    EXPECT_FALSE(provider->tryGetService<TestInheritClass3>());
    EXPECT_FALSE(provider->tryGetService<TestInheritClass4>());
}

TEST_F(ServiceProviderTest, ShouldTryGetOneLastServiceSingleton)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addSingleton<TestInheritClass1, TestInheritClass3>()
                              .addSingleton<TestInheritClass1, TestInheritClass5>()
                              .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetService<TestInheritClass1>());
    EXPECT_EQ(provider->tryGetService<TestInheritClass1>()->number(), 5);
}

TEST_F(ServiceProviderTest, ShouldTryGetOneLastServiceScope)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addScoped<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass1, TestInheritClass3>()
                              .addScoped<TestInheritClass1, TestInheritClass5>()
                              .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetService<TestInheritClass1>());
    EXPECT_EQ(provider->tryGetService<TestInheritClass1>()->number(), 5);
}

TEST_F(ServiceProviderTest, ShouldFailTryGetServiceDueToCircularDependency)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<CircularDependencyA>()
                              .addScoped<CircularDependencyB>()
                              .buildServiceProvider();

    EXPECT_THROW(provider->tryGetService<CircularDependencyA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceProviderTest, ShouldTryGetSelf)
{
    const auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto self = provider->tryGetService<sb::di::ServiceProvider>();
    EXPECT_TRUE(self);
    EXPECT_EQ(self, provider.get());
}

// getInstance Tests

TEST_F(ServiceProviderTest, ShouldGetInstance)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addScoped<TestClass2>()
                              .addTransient<TestClass3>()
                              .buildServiceProvider();

    auto &instanceProvider = provider->asInstanceProvider();
    EXPECT_NO_THROW(instanceProvider.getInstance(typeid(TestClass1)));
    EXPECT_NO_THROW(instanceProvider.getInstance(typeid(TestClass2)));
    EXPECT_TRUE(instanceProvider.getInstance(typeid(TestClass1)));
    EXPECT_TRUE(instanceProvider.getInstance(typeid(TestClass2)));
    EXPECT_THROW(instanceProvider.getInstance(typeid(TestClass3)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(instanceProvider.getInstance(typeid(TestClass4)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceProviderTest, ShouldGetInheritedInstance)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass2, TestInheritClass3>()
                              .addTransient<TestInheritClass3, TestInheritClass4>()
                              .buildServiceProvider();

    auto &instanceProvider = provider->asInstanceProvider();
    EXPECT_NO_THROW(instanceProvider.getInstance(typeid(TestInheritClass1)));
    EXPECT_NO_THROW(instanceProvider.getInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(instanceProvider.getInstance(typeid(TestInheritClass1)));
    EXPECT_TRUE(instanceProvider.getInstance(typeid(TestInheritClass2)));
    EXPECT_THROW(instanceProvider.getInstance(typeid(TestInheritClass3)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(instanceProvider.getInstance(typeid(TestInheritClass4)), sb::di::ServiceNotFoundException);
}

// getService Tests

TEST_F(ServiceProviderTest, ShouldGetService)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addScoped<TestClass2>()
                              .addTransient<TestClass3>()
                              .buildServiceProvider();

    EXPECT_NO_THROW(provider->getService<TestClass1>());
    EXPECT_NO_THROW(provider->getService<TestClass2>());
    EXPECT_THROW(provider->getService<TestClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider->getService<TestClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceProviderTest, ShouldGetInheritedService)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass2, TestInheritClass3>()
                              .addTransient<TestInheritClass3, TestInheritClass4>()
                              .buildServiceProvider();

    EXPECT_NO_THROW(provider->getService<TestInheritClass1>());
    EXPECT_NO_THROW(provider->getService<TestInheritClass2>());
    EXPECT_THROW(provider->getService<TestInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider->getService<TestInheritClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceProviderTest, ShouldGetOneLastServiceSingleton)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addSingleton<TestInheritClass1, TestInheritClass3>()
                              .addSingleton<TestInheritClass1, TestInheritClass5>()
                              .buildServiceProvider();

    EXPECT_EQ(provider->getService<TestInheritClass1>().number(), 5);
}

TEST_F(ServiceProviderTest, ShouldGetOneLastServiceScope)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addScoped<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass1, TestInheritClass3>()
                              .addScoped<TestInheritClass1, TestInheritClass5>()
                              .buildServiceProvider();

    EXPECT_EQ(provider->getService<TestInheritClass1>().number(), 5);
}

TEST_F(ServiceProviderTest, ShouldFailGetServiceDueToCircularDependency)
{

    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<CircularDependencyA>()
                              .addScoped<CircularDependencyB>()
                              .buildServiceProvider();

    EXPECT_THROW(provider->getService<CircularDependencyA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceProviderTest, ShouldGetSelf)
{
    const auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    auto &self = provider->getService<sb::di::ServiceProvider>();
    EXPECT_EQ(&self, provider.get());
}

// getInstances Tests

TEST_F(ServiceProviderTest, ShouldGetInstances)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addScoped<TestClass2>()
                              .addTransient<TestClass3>()
                              .buildServiceProvider();

    auto &instanceProvider = provider->asInstanceProvider();
    EXPECT_EQ(instanceProvider.tryGetInstances(typeid(TestClass1))->size(), 1);
    EXPECT_EQ(instanceProvider.tryGetInstances(typeid(TestClass2))->size(), 1);
    EXPECT_FALSE(instanceProvider.tryGetInstances(typeid(TestClass3)));
    EXPECT_FALSE(instanceProvider.tryGetInstances(typeid(TestClass4)));
}

TEST_F(ServiceProviderTest, ShouldGetInstancesInOrder)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addScoped<TestInheritClass1>()
                              .addScoped<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass1, TestInheritClass3>()
                              .addScoped<TestInheritClass1, TestInheritClass4>()
                              .addScoped<TestInheritClass1, TestInheritClass5>()
                              .buildServiceProvider();

    auto &services = *provider->asInstanceProvider().tryGetInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0]->get())->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1]->get())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2]->get())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3]->get())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4]->get())->number(), 5);
}

// getServices Tests

TEST_F(ServiceProviderTest, ShouldGetInheritedServices)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass2, TestInheritClass3>()
                              .addTransient<TestInheritClass3, TestInheritClass4>()
                              .buildServiceProvider();

    EXPECT_EQ(provider->getServices<TestInheritClass1>().size(), 1);
    EXPECT_EQ(provider->getServices<TestInheritClass2>().size(), 1);
    EXPECT_TRUE(provider->getServices<TestInheritClass3>().empty());
    EXPECT_TRUE(provider->getServices<TestInheritClass4>().empty());
}

TEST_F(ServiceProviderTest, ShouldGetCastedServicesInOrder)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addScoped<TestInheritClass1>()
                              .addScoped<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass1, TestInheritClass3>()
                              .addScoped<TestInheritClass1, TestInheritClass4>()
                              .addScoped<TestInheritClass1, TestInheritClass5>()
                              .buildServiceProvider();

    const auto services = provider->getServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}

TEST_F(ServiceProviderTest, ShouldGetServicesInOrderAfterNormalGet)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addScoped<TestInheritClass1>()
                              .addScoped<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass1, TestInheritClass3>()
                              .addScoped<TestInheritClass1, TestInheritClass4>()
                              .addScoped<TestInheritClass1, TestInheritClass5>()
                              .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetService<TestInheritClass1>());
    const auto services = provider->getServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
    EXPECT_TRUE(provider->tryGetService<TestInheritClass1>());
}

TEST_F(ServiceProviderTest, ShouldGetEmptyServicesForNotExisting)
{
    const auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto all = provider->getServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(ServiceProviderTest, ShouldFailGetServicesDueToCircularDependency)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<CircularDependencyA>()
                              .addScoped<CircularDependencyB>()
                              .buildServiceProvider();

    EXPECT_THROW(provider->getServices<CircularDependencyA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceProviderTest, ShouldGetSelfServices)
{
    const auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto services = provider->getServices<sb::di::ServiceProvider>();
    EXPECT_EQ(services.size(), 1);
    EXPECT_EQ(services[0], provider.get());
}

// tryCreateInstance Tests

TEST_F(ServiceProviderTest, ShouldTryCreateInstance)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addScoped<TestClass2>()
                              .addTransient<TestClass3>()
                              .buildServiceProvider();

    auto &instanceProvider = provider->asInstanceProvider();
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestClass1)));
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestClass2)));
    EXPECT_TRUE(instanceProvider.tryCreateInstance(typeid(TestClass3)));
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestClass4)));
}

TEST_F(ServiceProviderTest, ShouldTryCreateInheritedInstance)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass2, TestInheritClass3>()
                              .addTransient<TestInheritClass3, TestInheritClass4>()
                              .buildServiceProvider();

    auto &instanceProvider = provider->asInstanceProvider();
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestInheritClass1)));
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(instanceProvider.tryCreateInstance(typeid(TestInheritClass3)));
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestInheritClass4)));
}

// tryCreateService Tests

TEST_F(ServiceProviderTest, ShouldTryCreateService)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addScoped<TestClass2>()
                              .addTransient<TestClass3>()
                              .buildServiceProvider();

    EXPECT_FALSE(provider->tryCreateService<TestClass1>());
    EXPECT_FALSE(provider->tryCreateService<TestClass2>());
    EXPECT_TRUE(provider->tryCreateService<TestClass3>());
    EXPECT_FALSE(provider->tryCreateService<TestClass4>());
}

TEST_F(ServiceProviderTest, ShouldTryCreateInheritedService)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass2, TestInheritClass3>()
                              .addTransient<TestInheritClass3, TestInheritClass4>()
                              .buildServiceProvider();

    EXPECT_FALSE(provider->tryCreateService<TestInheritClass1>());
    EXPECT_FALSE(provider->tryCreateService<TestInheritClass2>());
    EXPECT_TRUE(provider->tryCreateService<TestInheritClass3>());
    EXPECT_FALSE(provider->tryCreateService<TestInheritClass4>());
}

TEST_F(ServiceProviderTest, ShouldFailTryCreateServiceDueToCircularDependency)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addTransient<CircularDependencyUniqueA>()
                              .addTransient<CircularDependencyUniqueB>()
                              .buildServiceProvider();

    EXPECT_THROW(provider->tryCreateService<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceProviderTest, ShouldNotTryCreateSelf)
{
    const auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    EXPECT_FALSE(provider->tryCreateService<sb::di::ServiceProvider>());
}

// createInstance Tests

TEST_F(ServiceProviderTest, ShouldCreateInstance)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addScoped<TestClass2>()
                              .addTransient<TestClass3>()
                              .buildServiceProvider();

    auto &instanceProvider = provider->asInstanceProvider();
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestClass1)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestClass2)), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(instanceProvider.createInstance(typeid(TestClass3)));
    EXPECT_TRUE(instanceProvider.createInstance(typeid(TestClass3)));
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestClass4)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceProviderTest, ShouldCreateInheritedInstance)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass2, TestInheritClass3>()
                              .addTransient<TestInheritClass3, TestInheritClass4>()
                              .buildServiceProvider();

    auto &instanceProvider = provider->asInstanceProvider();
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestInheritClass1)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestInheritClass2)), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(instanceProvider.createInstance(typeid(TestInheritClass3)));
    EXPECT_TRUE(instanceProvider.createInstance(typeid(TestInheritClass3)));
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestInheritClass4)), sb::di::ServiceNotFoundException);
}

// createInstance Tests

TEST_F(ServiceProviderTest, ShouldCreateService)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addScoped<TestClass2>()
                              .addTransient<TestClass3>()
                              .buildServiceProvider();

    EXPECT_THROW(provider->createService<TestClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider->createService<TestClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider->createService<TestClass3>());
    EXPECT_THROW(provider->createService<TestClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceProviderTest, ShouldCreateInheritedService)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass2, TestInheritClass3>()
                              .addTransient<TestInheritClass3, TestInheritClass4>()
                              .buildServiceProvider();

    EXPECT_THROW(provider->createService<TestInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider->createService<TestInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider->createService<TestInheritClass3>());
    EXPECT_THROW(provider->createService<TestInheritClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceProviderTest, ShouldFailCreateServiceDueToCircularDependency)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addTransient<CircularDependencyUniqueA>()
                              .addTransient<CircularDependencyUniqueB>()
                              .buildServiceProvider();

    EXPECT_THROW(provider->createService<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceProviderTest, ShouldNotCreateSelfService)
{
    const auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    EXPECT_THROW(provider->createService<sb::di::ServiceProvider>(), sb::di::ServiceNotFoundException);
}

// createInstances Tests

TEST_F(ServiceProviderTest, ShouldCreateInstances)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addScoped<TestClass2>()
                              .addTransient<TestClass3>()
                              .buildServiceProvider();

    auto &instanceProvider = provider->asInstanceProvider();
    EXPECT_FALSE(instanceProvider.tryCreateInstances(typeid(TestClass1)));
    EXPECT_FALSE(instanceProvider.tryCreateInstances(typeid(TestClass2)));
    EXPECT_EQ(instanceProvider.tryCreateInstances(typeid(TestClass3))->size(), 1);
    EXPECT_FALSE(instanceProvider.tryCreateInstances(typeid(TestClass4)));
}

TEST_F(ServiceProviderTest, ShouldCreateInstancesInOrder)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addTransient<TestInheritClass1>()
                              .addTransient<TestInheritClass1, TestInheritClass2>()
                              .addTransient<TestInheritClass1, TestInheritClass3>()
                              .addTransient<TestInheritClass1, TestInheritClass4>()
                              .addTransient<TestInheritClass1, TestInheritClass5>()
                              .buildServiceProvider();

    auto services = *provider->asInstanceProvider().tryCreateInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0]->get())->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1]->get())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2]->get())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3]->get())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4]->get())->number(), 5);
}

// createServices Tests

TEST_F(ServiceProviderTest, ShouldCreateInheritedServices)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestInheritClass1, TestInheritClass2>()
                              .addScoped<TestInheritClass2, TestInheritClass3>()
                              .addTransient<TestInheritClass3, TestInheritClass4>()
                              .buildServiceProvider();

    EXPECT_TRUE(provider->createServices<TestInheritClass1>().empty());
    EXPECT_TRUE(provider->createServices<TestInheritClass2>().empty());
    EXPECT_EQ(provider->createServices<TestInheritClass3>().size(), 1);
    EXPECT_TRUE(provider->createServices<TestInheritClass4>().empty());
}

TEST_F(ServiceProviderTest, ShouldCreateServicesInOrder)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addTransient<TestInheritClass1>()
                              .addTransient<TestInheritClass1, TestInheritClass2>()
                              .addTransient<TestInheritClass1, TestInheritClass3>()
                              .addTransient<TestInheritClass1, TestInheritClass4>()
                              .addTransient<TestInheritClass1, TestInheritClass5>()
                              .buildServiceProvider();

    const auto services = provider->createServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}

TEST_F(ServiceProviderTest, ShouldCreateEmptyServicesForNotExisting)
{
    const auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto all = provider->createServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(ServiceProviderTest, ShouldFaildCreateServicesDueToCircularDependency)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addTransient<CircularDependencyUniqueA>()
                              .addTransient<CircularDependencyUniqueB>()
                              .buildServiceProvider();

    EXPECT_THROW(provider->createServices<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceProviderTest, ShouldFailCreateSelfServices)
{
    const auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto services = provider->createServices<sb::di::ServiceProvider>();
    EXPECT_TRUE(services.empty());
}