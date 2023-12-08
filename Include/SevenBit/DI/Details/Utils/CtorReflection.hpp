#pragma once

#include <utility>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
#if defined(__GNUC__) && (__GNUC__ >= 7)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-template-friend"
#endif

    template <class, class> struct is_copy_ctor__ : std::false_type
    {
    };

    template <class T> struct is_copy_ctor__<T, T> : std::true_type
    {
    };

    template <class T> struct is_copy_ctor__<T, const T> : std::true_type
    {
    };

    template <typename T, int N> struct Tag
    {
        friend auto loophole(Tag<T, N>);
        constexpr friend int cloophole(Tag<T, N>);
    };

    template <typename T, typename U, int N, bool B, typename = typename std::enable_if_t<!is_copy_ctor__<T, U>::value>>
    struct FnDef
    {
        friend auto loophole(Tag<T, N>)
        {
            return helpers::ServiceParamProvider<U>{}.getParam(*(static_cast<ServiceProvider *>(nullptr)));
        }
        constexpr friend int cloophole(Tag<T, N>) { return 0; }
    };

    template <typename T, typename U, int N> struct FnDef<T, U, N, true>
    {
    };

    template <typename T, int N> struct Cop
    {
        template <typename U, int M> static auto ins(...) -> int;
        template <typename U, int M, int = cloophole(Tag<T, M>{})> static auto ins(int) -> char;

        template <typename U, int = sizeof(FnDef<T, U, N, sizeof(ins<U, N>(0)) == sizeof(char)>)> operator U();
    };

    template <typename T, int... Ns> constexpr int fieldsNumber(...) { return sizeof...(Ns) - 1; }

    template <typename T, int... Ns> constexpr auto fieldsNumber(int) -> decltype(T{Cop<T, Ns>{}...}, 0)
    {
        return fieldsNumber<T, Ns..., sizeof...(Ns)>(0);
    }

    template <typename T, int... Ns> constexpr auto fieldsNumberCtor(int) -> decltype(T{Cop<T, Ns>{}...}, 0)
    {
        return sizeof...(Ns);
    }

    template <typename T, int... Ns> constexpr int fieldsNumberCtor(...)
    {
        return fieldsNumberCtor<T, Ns..., sizeof...(Ns)>(0);
    }

    template <typename T, typename U> struct LoopholeTuple;

    template <typename T, int... Ns> struct LoopholeTuple<T, std::integer_sequence<int, Ns...>>
    {
        using Type = std::tuple<decltype(loophole(Tag<T, Ns>{}))...>;
    };

    template <typename T>
    using AsTuple = typename LoopholeTuple<T, std::make_integer_sequence<int, fieldsNumber<T>(0)>>::Type;

    template <class T> struct ConstructorTraits
    {
        using Type = T;
        using TupleArgs = AsTuple<Type>;

        static constexpr size_t ArgsSize = std::tuple_size_v<TupleArgs>;

        template <size_t I> struct Arg
        {
            using Type = typename std::tuple_element<I, TupleArgs>::type;
        };
    };
#if defined(__GNUC__) && (__GNUC__ >= 7)
#pragma GCC diagnostic pop
#endif
} // namespace sb::di::details::utils
