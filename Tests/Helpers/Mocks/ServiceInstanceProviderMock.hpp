#pragma once

#include <gmock/gmock.h>

#include "SevenBit/DI/IServiceInstanceProvider.hpp"

struct ServiceInstanceProviderMock : public sb::di::IServiceInstanceProvider
{
    MOCK_METHOD((std::unique_ptr<sb::di::IServiceInstanceProvider>), createScope, (), (const override));
    MOCK_METHOD((const sb::di::ServiceProviderOptions &), getOptions, (), (const override));
    MOCK_METHOD((void), init, (sb::di::ServiceProvider &), (override));
    MOCK_METHOD((const sb::di::ServiceInstance *), tryGetInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((const sb::di::ServiceInstance &), getInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((const sb::di::OneOrList<sb::di::ServiceInstance::Ptr> *), tryGetInstances,
                (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::ServiceInstance::Ptr), tryCreateInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::ServiceInstance::Ptr), createInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::ServiceInstance::Ptr), tryCreateInstanceInPlace, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::ServiceInstance::Ptr), createInstanceInPlace, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((std::optional<sb::di::OneOrList<sb::di::ServiceInstance::Ptr>>), tryCreateInstances,
                (sb::di::TypeId serviceTypeId), (override));
};
