#include <type_traits>

template<typename EnumClass>
typename std::underlying_type<EnumClass>::type operator +(EnumClass EnumValue)
{
    return static_cast<typename std::underlying_type<EnumClass>::type>(EnumValue);
}