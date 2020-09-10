#pragma once

namespace MpiWrapper::Utils {

#ifdef DEBUG
constexpr void runtime_assert(bool condition, const char* msg) {
    if (!condition) throw std::runtime_error(msg);
}
#else
constexpr void runtime_assert([[maybe_unused]] bool condition, [[maybe_unused]] const char* msg) {}
#endif

} // namespace Utils