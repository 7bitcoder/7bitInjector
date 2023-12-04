#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Core/IServiceProviderData.hpp"
#include "SevenBit/DI/Details/Helpers/CircularDependencyGuard.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::core
{
    class EXPORT DefaultServiceProvider : public ServiceProvider
    {
      public:
        using Ptr = std::unique_ptr<DefaultServiceProvider>;

      private:
        IServiceProviderData::SPtr _sharedData;
        containers::ServiceInstancesMap _scoped;
        helpers::CircularDependencyGuard _guard;

        DefaultServiceProvider(const DefaultServiceProvider &);

      public:
        explicit DefaultServiceProvider(IServiceProviderData::Ptr core);

        DefaultServiceProvider(DefaultServiceProvider &&) noexcept = default;

        DefaultServiceProvider &operator=(const DefaultServiceProvider &) = delete;
        DefaultServiceProvider &operator=(DefaultServiceProvider &&) = default;

        void clear();

        ServiceProvider::Ptr createScope() override;

      protected:
        const IServiceInstance &getInstance(TypeId serviceTypeId) override;
        const IServiceInstance *tryGetInstance(TypeId serviceTypeId) override;
        const OneOrList<IServiceInstance::Ptr> *tryGetInstances(TypeId serviceTypeId) override;

        IServiceInstance::Ptr createInstance(TypeId serviceTypeId) override;
        IServiceInstance::Ptr tryCreateInstance(TypeId serviceTypeId) override;
        std::optional<OneOrList<IServiceInstance::Ptr>> tryCreateInstances(TypeId serviceTypeId) override;

        IServiceInstance::Ptr createInstanceInPlace(TypeId serviceTypeId) override;
        IServiceInstance::Ptr tryCreateInstanceInPlace(TypeId serviceTypeId) override;

      private:
        const IServiceInstance *tryCreateAndRegister(const containers::ServiceDescriptorList &descriptors);
        const OneOrList<IServiceInstance::Ptr> *tryCreateAndRegisterAll(
            const containers::ServiceDescriptorList &descriptors);

        IServiceInstance::Ptr tryCreate(const ServiceDescriptor &descriptor, bool inPlaceRequest);
        std::optional<OneOrList<IServiceInstance::Ptr>> tryCreateAll(
            const containers::ServiceDescriptorList &descriptors);

        OneOrList<IServiceInstance::Ptr> &createRestInstances(const containers::ServiceDescriptorList &descriptors,
                                                              containers::ServiceInstanceList &instances,
                                                              bool inPlaceRequest);

        IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor, bool inPlaceRequest);

        containers::ServiceInstancesMap *tryGetInstancesMap(const ServiceLifeTime &lifeTime);

        containers::ServiceInstanceList *findRegisteredInstances(TypeId serviceTypeId);

        [[nodiscard]] const containers::ServiceDescriptorList *findDescriptors(TypeId serviceTypeId) const;

        const ServiceProviderOptions &getOptions();

        void prebuildSingletons();
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/DefaultServiceProvider.hpp"
#endif
