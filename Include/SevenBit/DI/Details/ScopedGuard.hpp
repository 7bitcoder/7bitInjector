#pragma once
#include <unordered_set>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class ScopedGuard
    {
      private:
        std::unordered_set<TypeId> &_typeIdsUnderConstruction;
        TypeId _typeIdUnderConstruction;

      public:
        ScopedGuard(TypeId typeIdUnderConstruction, std::unordered_set<TypeId> &typeIdsUnderConstruction);

        ~ScopedGuard();
    };

} // namespace sb::di::details

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ScopedGuard.hpp"
#endif