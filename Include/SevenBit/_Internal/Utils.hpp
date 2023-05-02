#pragma once

#include <algorithm>
#include <charconv>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <vector>

namespace sb::utils
{
    template <class... T> inline constexpr bool notSupportedType = false;

    template <size_t N> struct StringLiteral
    {
        constexpr StringLiteral(const char (&str)[N])
        {
            for (size_t i = 0; i < N; ++i)
            {
                value[i] = str[i];
            }
        }

        char value[N] = {'\0'};
    };

    template <typename T, typename Enable = void> struct IsUniquePtr : std::false_type
    {
        using Type = T;
    };

    template <typename T> struct IsUniquePtr<std::unique_ptr<T>> : std::true_type
    {
        using Type = T;
    };

    template <typename T> inline constexpr bool IsUniquePtrV = IsUniquePtr<T>::value;

} // namespace sb::utils