#ifndef I2PSAM_UTILITY_H
#define I2PSAM_UTILITY_H

#ifdef __cplusplus

#include <array>

template <std::size_t... Ns>
constexpr auto concat(const char(&...strs)[Ns]) {
  constexpr std::size_t total = ( ... + (Ns - 1) ) + 1; // суммируем длины без '\0', потом добавляем финальный '\0'
  std::array<char, total> result{};

  char* dst = result.data();
  auto copy = [&](auto const& s) {
    for(std::size_t i = 0; i < sizeof(s) - 1; i++) {
      *dst++ = s[i];
    }
  };

  (copy(strs), ...);
  *dst = '\0';

  return result;
}

#endif // __cplusplus

#endif // I2PSAM_UTILITY_H
