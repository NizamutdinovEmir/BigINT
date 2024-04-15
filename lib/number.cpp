#include "number.h"

int2023_t MakeItNegative(const int2023_t& lhs) {
  int2023_t ans;
  ans = lhs;
  for (int32_t i = 0; i < kNumberOfElements; i++) {
    ans.bytes[i] = ~ans.bytes[i];
  }
  return ans;
}

int2023_t MakeItAbsolute(const int2023_t& value) {
  if (value.bytes[252] > 127) {
    return MakeItNegative(value);
  }
  return value;
}

int2023_t from_int(int32_t i) {
  int2023_t ans;
  bool is_Negative = (i < 0);

  if (is_Negative) {
    i = -i;
  }

  for(int j = 0; j < 4; j++) {
    ans.bytes[j] = i & 255;
    i >>= 8;
  }

  if(is_Negative == true) {
    ans = MakeItNegative(ans);
  }

  return ans;
}

int2023_t from_string(const char* buff) {
  int2023_t ans;
  bool is_Negative (buff[0] == '-');
  int StartIndex = is_Negative ? 1 : 0;

  for (int i = StartIndex; buff[i] != '\0'; ++i) {
    ans = ans * from_int(10);
    ans = ans + from_int((buff[i] - '0'));
  }
  if (is_Negative) {
    ans = MakeItNegative(ans);
  }
  return ans;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t ans;
  int32_t carry = 0;

  for (int32_t i = 0; i < kNumberOfElements; ++i) {
    int32_t sum = carry + lhs.bytes[i] + rhs.bytes[i];
    ans.bytes[i] = sum % kMax;
    carry = sum / kMax;
  }

  if (carry > 0) {
    ans = ans + from_int(1);
  }

  if (MakeItAbsolute(ans) == from_int(0)) {
    return from_int(0);
  }

  return ans;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
  return (lhs + MakeItNegative(rhs));
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t ans;
  int2023_t first = lhs;
  int2023_t second = rhs;
  bool is_MakeItNegative = (first.bytes[252] >= 128) ^ (second.bytes[252] >= 128);
  first = MakeItAbsolute(first);
  second = MakeItAbsolute(second);

  int32_t carry = 0;
  for (int32_t i = 0; i < kNumberOfElements; i++) {
    int2023_t help;
    for (int32_t j = 0; j < kNumberOfElements-i; j++) {
      int32_t temp = first.bytes[i] * second.bytes[j] + carry;
      carry = temp / kMax;
      help.bytes[i + j] = temp % kMax;
    }
    ans = ans + help;
  }

  if (is_MakeItNegative) {
    ans = MakeItNegative(ans);
  }

  return ans;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t dividend = lhs;
  int2023_t divisor = rhs;
  int2023_t Quotient_of_division;
  int2023_t remainder;

  if (divisor == int2023_t()) {
    throw std::runtime_error("Division by zero");
  }

  if (dividend == int2023_t()) {
    return int2023_t();
  }

  if (divisor == from_int(1)) {
    return dividend;
  }

  bool isNegativeResult = (dividend.bytes[kNumberOfElements - 1] >= 128) ^ (divisor.bytes[kNumberOfElements - 1] >= 128);

  dividend = MakeItAbsolute(dividend);
  divisor = MakeItAbsolute(divisor);

  int32_t StartIndex = kNumberOfElements - 1;
  while (StartIndex >= 0) {
    remainder = remainder * from_int(256);
    remainder.bytes[0] += dividend.bytes[StartIndex];

    uint8_t MaxMultiplier = 0;
    while (MaxMultiplier < kMax && (divisor * from_int(MaxMultiplier)) <= remainder) {
      MaxMultiplier++;
    }
    MaxMultiplier--;

    Quotient_of_division.bytes[StartIndex] = MaxMultiplier;
    remainder = remainder - (divisor * from_int(MaxMultiplier));
    StartIndex--;
  }

  if (isNegativeResult && (Quotient_of_division != from_int(0))) {
    Quotient_of_division = MakeItNegative(Quotient_of_division);
  }

  return Quotient_of_division;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
  for (int32_t i = 0; i < kNumberOfElements; ++i) {
    if (lhs.bytes[i] != rhs.bytes[i]) {
      return false;
    }
  }
  return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
  bool lhsMakeItNegative = lhs.bytes[kNumberOfElements - 1] >= 128;
  bool rhsMakeItNegative = rhs.bytes[kNumberOfElements - 1] >= 128;
  if(lhsMakeItNegative != rhsMakeItNegative) {
    return true;
  }
  if (MakeItAbsolute((lhs)) == MakeItAbsolute(rhs)) {
    return false;
  } else {
    return true;
  }
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
  bool lhsIsNegative = lhs.bytes[kNumberOfElements - 1] >= 128;
  bool rhsIsNegative = rhs.bytes[kNumberOfElements - 1] >= 128;

  if (lhsIsNegative != rhsIsNegative) {
    return lhsIsNegative;
  }

  for (int i = kNumberOfElements - 2; i >= 0; --i) {
    if (lhs.bytes[i] != rhs.bytes[i]) {
      return lhs.bytes[i] < rhs.bytes[i];
    }
  }

  return false;
}


bool operator<=(const int2023_t& lhs, const int2023_t& rhs) {
  return lhs < rhs || lhs == rhs;
}

bool operator>(const int2023_t& lhs, const int2023_t& rhs) {
  bool lhsIsNegative = lhs.bytes[kNumberOfElements - 1] >= 128;
  bool rhsIsNegative = rhs.bytes[kNumberOfElements - 1] >= 128;

  if (lhsIsNegative != rhsIsNegative) {
    return rhsIsNegative;
  }

  for (int i = kNumberOfElements - 2; i >= 0; --i) {
    if (lhs.bytes[i] != rhs.bytes[i]) {
      return lhs.bytes[i] > rhs.bytes[i];
    }
  }

  return false;
}


bool operator>=(const int2023_t& lhs, const int2023_t& rhs) {
  return lhs > rhs || lhs == rhs;
}


std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
  bool isNegative = value.bytes[kNumberOfElements - 1] >= 128;

  if (isNegative) {
    stream << "-";
  }

  for (int32_t i = kNumberOfElements - 1; i >= 0; --i) {
    stream << int(value.bytes[i]);
  }

  return stream;
}
