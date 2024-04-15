#pragma once
#include <cinttypes>
#include <iostream>
#include <cmath>

const int32_t kMax = 256;
const int32_t kNumberOfElements = 253;


struct int2023_t {
  uint8_t bytes[kNumberOfElements] = {0};
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

bool operator<(const int2023_t& lhs, const int2023_t& rhs);

bool operator<=(const int2023_t& lhs, const int2023_t& rhs);

bool operator>(const int2023_t& lhs, const int2023_t& rhs);

bool operator>=(const int2023_t& lhs, const int2023_t& rhs);

int2023_t MakeItNegative(const int2023_t& lhs);

int2023_t MakeItAbsolute(const int2023_t& value);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);
