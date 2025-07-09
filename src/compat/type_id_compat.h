#pragma once

#if defined(ASIO_NO_TYPEID)
namespace sep {
template<typename T> struct static_type_holder { static const char value; };
template<typename T> const char static_type_holder<T>::value = 0;
template<typename T> constexpr const void* static_type_id() noexcept {
    return &static_type_holder<T>::value;
}
} // namespace sep
#  define SEP_TYPE_ID(T) ::sep::static_type_id<T>()
#else
#  include <typeinfo>
#  define SEP_TYPE_ID(T) &typeid(T)
#endif
