#pragma once

#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

namespace sb::di
{
    INLINE ServiceProvider ServiceCollection::buildServiceProvider(ServiceProviderOptions options)
    {
        return ServiceProvider{std::make_unique<details::ServiceInstanceProviderRoot>(begin(), end(), options)};
    }

    INLINE ServiceProvider::Ptr ServiceCollection::buildServiceProviderAsPtr(ServiceProviderOptions options)
    {
        auto instanceProvider = std::make_unique<details::ServiceInstanceProviderRoot>(begin(), end(), options);
        return std::make_unique<ServiceProvider>(std::move(instanceProvider));
    }

    INLINE std::vector<ServiceDescriptor> &ServiceCollection::getInnerVector() { return _serviceDescriptors; }
    INLINE const std::vector<ServiceDescriptor> &ServiceCollection::getInnerVector() const
    {
        return _serviceDescriptors;
    }

    INLINE ServiceDescriptor &ServiceCollection::at(const std::size_t index) { return _serviceDescriptors.at(index); }
    INLINE const ServiceDescriptor &ServiceCollection::at(const std::size_t index) const
    {
        return _serviceDescriptors.at(index);
    }

    INLINE ServiceDescriptor &ServiceCollection::first() { return at(0); }
    INLINE const ServiceDescriptor &ServiceCollection::first() const { return at(0); }

    INLINE ServiceDescriptor &ServiceCollection::last() { return at(size() - 1); }
    INLINE const ServiceDescriptor &ServiceCollection::last() const { return at(size() - 1); }

    INLINE ServiceDescriptor &ServiceCollection::operator[](const std::size_t index) { return at(index); }
    INLINE const ServiceDescriptor &ServiceCollection::operator[](const std::size_t index) const { return at(index); }

    INLINE std::size_t ServiceCollection::maxSize() const { return _serviceDescriptors.max_size(); }

    INLINE std::size_t ServiceCollection::size() const { return _serviceDescriptors.size(); }
    INLINE std::size_t ServiceCollection::count() const { return size(); }

    INLINE bool ServiceCollection::empty() const { return _serviceDescriptors.empty(); }

    INLINE std::size_t ServiceCollection::capacity() const { return _serviceDescriptors.capacity(); }

    INLINE void ServiceCollection::reserve(const std::size_t space) { _serviceDescriptors.reserve(space); }

    INLINE void ServiceCollection::shrinkToFit() { _serviceDescriptors.shrink_to_fit(); }

    INLINE void ServiceCollection::clear() { _serviceDescriptors.clear(); }

    INLINE bool ServiceCollection::contains(const TypeId serviceTypeId) const
    {
        return containsIf(
            [&](const ServiceDescriptor &descriptor) { return descriptor.getServiceTypeId() == serviceTypeId; });
    }

    INLINE bool ServiceCollection::containsKeyed(const TypeId serviceTypeId, const std::string_view serviceKey) const
    {
        return containsIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getServiceTypeId() == serviceTypeId && descriptor.getServiceKey() &&
                   *descriptor.getServiceKey() == serviceKey;
        });
    }

    INLINE bool ServiceCollection::containsExact(const TypeId serviceTypeId, const TypeId implementationTypeId) const
    {
        return containsIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId;
        });
    }

    INLINE bool ServiceCollection::containsKeyedExact(const TypeId serviceTypeId, const TypeId implementationTypeId,
                                                      const std::string_view serviceKey) const
    {
        return containsIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId && descriptor.getServiceKey() &&
                   *descriptor.getServiceKey() == serviceKey;
        });
    }

    INLINE ServiceCollection::Iterator ServiceCollection::insert(const ConstIterator pos, ServiceDescriptor descriptor)
    {
        return _serviceDescriptors.insert(pos, std::move(descriptor));
    }

    INLINE ServiceCollection &ServiceCollection::add(ServiceDescriptor descriptor)
    {
        _serviceDescriptors.push_back(std::move(descriptor));
        return *this;
    }

    INLINE ServiceCollection &ServiceCollection::add(const ServiceCollection &collection)
    {
        reserve(size() + collection.size());
        for (auto &descriptor : collection)
        {
            add(descriptor);
        }
        return *this;
    }

    INLINE ServiceCollection::Iterator ServiceCollection::remove(const Iterator pos)
    {
        return _serviceDescriptors.erase(pos);
    }
    INLINE ServiceCollection::Iterator ServiceCollection::remove(const ConstIterator pos)
    {
        return _serviceDescriptors.erase(pos);
    }

    INLINE ServiceCollection::Iterator ServiceCollection::removeRange(const Iterator begin, const Iterator end)
    {
        return _serviceDescriptors.erase(begin, end);
    }
    INLINE ServiceCollection::Iterator ServiceCollection::removeRange(const ConstIterator begin,
                                                                      const ConstIterator end)
    {
        return _serviceDescriptors.erase(begin, end);
    }

    INLINE std::size_t ServiceCollection::removeAll(const TypeId serviceTypeId)
    {
        return removeIf(
            [&](const ServiceDescriptor &descriptor) { return descriptor.getServiceTypeId() == serviceTypeId; });
    }

    INLINE std::size_t ServiceCollection::removeAllKeyed(const TypeId serviceTypeId, const std::string_view serviceKey)
    {
        return removeIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getServiceTypeId() == serviceTypeId && descriptor.getServiceKey() &&
                   *descriptor.getServiceKey() == serviceKey;
        });
    }

    INLINE std::size_t ServiceCollection::remove(const TypeId serviceTypeId, const TypeId implementationTypeId)
    {
        return removeIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId;
        });
    }

    INLINE std::size_t ServiceCollection::removeKeyed(const TypeId serviceTypeId, const TypeId implementationTypeId,
                                                      const std::string_view serviceKey)
    {
        return removeIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId && descriptor.getServiceKey() &&
                   *descriptor.getServiceKey() == serviceKey;
        });
    }

    INLINE void ServiceCollection::pop() { _serviceDescriptors.pop_back(); }

    INLINE bool operator==(const ServiceCollection &lhs, const ServiceCollection &rhs)
    {
        return lhs._serviceDescriptors == rhs._serviceDescriptors;
    }
    INLINE bool operator!=(const ServiceCollection &lhs, const ServiceCollection &rhs)
    {
        return lhs._serviceDescriptors != rhs._serviceDescriptors;
    }
} // namespace sb::di
