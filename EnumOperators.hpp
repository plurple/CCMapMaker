#pragma once

#include <type_traits>
#include <stdexcept>

template <typename E>
constexpr typename std::enable_if<std::is_enum<E>::value, E>::type&
operator++(E& e) {
	using Underlying = typename std::underlying_type<E>::type;
	e = static_cast<E>((static_cast<Underlying>(e) + 1) % static_cast<Underlying>(E::COUNT));
	return e;
}

template <typename E>
constexpr typename std::enable_if<std::is_enum<E>::value, E>::type&
operator--(E& e) {
	using Underlying = typename std::underlying_type<E>::type;
	auto val = static_cast<Underlying>(e);
	val = (val == 0) ? static_cast<Underlying>(E::COUNT) - 1 : val - 1;
	e = static_cast<E>(val);
	return e;
}

template <typename E>
constexpr typename std::enable_if<std::is_enum<E>::value, E>::type
operator++(E& e, int) {
	E temp = e;
	++e;
	return temp;
}

template <typename E>
constexpr typename std::enable_if<std::is_enum<E>::value, E>::type
operator--(E& e, int) {
	E temp = e;
	--e;
	return temp;
}