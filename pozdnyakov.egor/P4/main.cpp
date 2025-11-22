#include <iostream>
#include <cstdlib>

namespace pozdnyakov {

  size_t string_length(const char* str) {
    const char* p = str;
    while (*p) {
      ++p;
    }
    return p - str;
  }

  void string_copy(const char* source, char* destination, size_t dest_size) {
    if (source == nullptr || destination == nullptr || dest_size == 0) {
      return;
    }

    size_t i = 0;
    while (source[i] && i < dest_size - 1) {
      destination[i] = source[i];
      ++i;
    }
    destination[i] = '\0';
  }

  int is_alpha_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
  }

  void replace_chars(const char* input, char* output, size_t output_size, char old_char, char new_char) {
    if (input == nullptr || output == nullptr || output_size == 0) {
      return;
    }

    size_t i = 0;
    while (input[i] && i < output_size - 1) {
      output[i] = (input[i] == old_char) ? new_char : input[i];
      ++i;
    }
    output[i] = '\0';
  }

  void merge_latin_letters(const char* str1, const char* str2, char* output, size_t output_size) {
    if (str1 == nullptr || str2 == nullptr || output == nullptr || output_size == 0) {
      return;
    }

    char temp_buffer[1024];
    size_t temp_index = 0;

    for (size_t i = 0; str1[i] && temp_index < 1023; ++i) {
      if (is_alpha_char(str1[i])) {
        temp_buffer[temp_index++] = str1[i];
      }
    }

    for (size_t i = 0; str2[i] && temp_index < 1023; ++i) {
      if (is_alpha_char(str2[i])) {
        temp_buffer[temp_index++] = str2[i];
      }
    }
    temp_buffer[temp_index] = '\0';

    for (size_t i = 0; i < temp_index; ++i) {
      for (size_t j = i + 1; j < temp_index; ++j) {
        if (temp_buffer[i] > temp_buffer[j]) {
          char temp = temp_buffer[i];
          temp_buffer[i] = temp_buffer[j];
          temp_buffer[j] = temp;
        }
      }
    }

    string_copy(temp_buffer, output, output_size);
  }

  char* read_string() {
    size_t capacity = 256;
    size_t size = 0;
    char* str = new(std::nothrow) char[capacity];

    if (str == nullptr) {
      std::cerr << "Memory allocaton error.";
      std::exit(1);
    }

    int ch;
    while ((ch = std::cin.get()) != '\n' && ch != EOF) {
      if (size + 1 >= capacity) {
        capacity *= 2;
        char* new_str = new(std::nothrow) char[capacity];
        if (new_str == nullptr) {
          std::cerr << "Memory allocaton error.";
          delete[] str;
          std::exit(1);
        }

        for (size_t i = 0; i < size; ++i) {
          new_str[i] = str[i];
        }
        delete[] str;
        str = new_str;
      }
      str[size++] = static_cast<char>(ch);
    }
    str[size] = '\0';

    return str;
  }
}

int main() {
  using namespace pozdnyakov;

  std::cout << "String for var 10";
  char* input_str1 = read_string();

  if (input_str1 == nullptr) {
    std::cerr << "String reading error.";
    return 1;
  }

  size_t len1 = string_length(input_str1);
  char* result1 = new(std::nothrow) char[len1 + 1];

  if (result1 == nullptr) {
    std::cerr << "Memory allocation error.";
    delete[] input_str1;
    return 1;
  }

  replace_chars(input_str1, result1, len1 + 1, 'c', 'b');
  std::cout << "Variant 10 result: " << result1 << std::endl;

  delete[] input_str1;
  delete[] result1;

  std::cout << "String for var 20";
  char* input_str2 = read_string();

  if (input_str2 == nullptr) {
    std::cerr << "String reading error.";
    return 1;
  }

  const char* second_string = "def_ghk";

  size_t len2 = string_length(input_str2);
  size_t len3 = string_length(second_string);
  char* result2 = new(std::nothrow) char[len2 + len3 + 1];

  if (result2 == nullptr) {
    std::cerr << "Memory allocation error.";
    delete[] input_str2;
    return 1;
  }

  merge_latin_letters(input_str2, second_string, result2, len2 + len3 + 1);
  std::cout << "Variant 20 result: " << result2 << std::endl;

  delete[] input_str2;
  delete[] result2;

  return 0;
}
