#ifndef SRC_S21_SPRINTF_H_
#define SRC_S21_SPRINTF_H_
#include <locale.h>

#include "s21_string.h"

#define is_flag(ch)                                                   \
  ((ch) == '-') || ((ch) == '+') || ((ch) == ' ') || ((ch) == '#') || \
      ((ch) == '0')

#define is_digit(ch) ((ch) >= '0' && (ch) <= '9')

#define is_length(ch) ((ch) == 'h' || (ch) == 'l' || (ch) == 'L')

#define is_specifier(ch)                                                      \
  ((ch) == 'c' || (ch) == 'd' || (ch) == 'i' || (ch) == 'e' || (ch) == 'E' || \
   (ch) == 'f' || (ch) == 'g' || (ch) == 'G' || (ch) == 'o' || (ch) == 's' || \
   (ch) == 'u' || (ch) == 'x' || (ch) == 'X' || (ch) == 'p' || (ch) == 'n' || \
   (ch) == '%')

typedef long unsigned s21_size_t;

typedef struct p_token {
  int flag_minus;  // Выравнивание по левому краю
  int flag_plus;  // Заставляет предварять результат знаком + или -
  int flag_space;  // Если знак не записан, перед значением
                   // вставляется пробел.
  int flag_sharp;  // При o, x или X значению предшествуют 0, 0x или
                   //  0X соответственно, если не нуль. При с e, E и
                   //  f заставляет записанный вывод содержать
                   // десятичную точку. По умолчанию, если цифры не
                   // следуют, десятичная точка не записывается.
  int flag_zero;  // Дополняет число слева нулями (0) вместо
                  // пробелов
  int width;
  int precision;
  char length;
  char specifier;

  int sign;   // plus or minus
  int point;  // point exist flag
} p_token;

int s21_sprintf(char *str, const char *format, ...);

// init
void init_token(p_token *tok);

// parsing

char *parse_token(p_token *tok, const char *format, va_list *input);
char *set_flags(p_token *tok, const char *format);
char *set_width(p_token *tok, const char *format, va_list *input);
char *set_precision(p_token *tok, const char *format, va_list *input);
char *set_length(p_token *tok, const char *format);

void s21_default_precision(p_token *tok);
void s21_check_flag(p_token *tok, const char *format);
void s21_sharp_flag(p_token *tok, char *str);

char *s21_check_specifier(char *str, s21_size_t *str_len, va_list *input,
                          p_token *tok);
void parse_flags_width_precision(p_token *tok, char *str);

// int to [ASCII] and [ASCII] to int parsing

char *s21_str_to_int(int *number, const char *format, va_list *input);
void s21_int_to_str(p_token *tok, char *str, int64_t number);

void s21_8_16_int(char *str, va_list *input, p_token *tok);

void s21_ol_to_str(char *str, unsigned long long number, p_token *tok);
void s21_hl_to_str(char *str, unsigned long long number, p_token *tok);

void s21_float_to_str(char *str, long double number, p_token *tok);
void s21_wch_to_str(char *str, wchar_t *wstr, s21_size_t length);
void s21_eE_to_str(char *str, long double number, p_token *tok);

// helpers

int s21_compare_round(p_token *tok, long double number);
void s21_delete_zero(p_token *tok, char *str);
void s21_move_string(char *str);
void s21_reverse_writing(char *str);

void s21_make_string_flags(p_token *tok, char *str);
void s21_make_string_width(p_token *tok, char *str);
void s21_make_string_precision(p_token *tok, char *str);
void s21_make_mantissa(p_token *tok, char *mantice, int notation);

// spec handlers

void s21_spec_c(char *str, va_list *input, p_token *tok);
void s21_spec_di(char *str, va_list *input, p_token *tok);
void s21_spec_eE(char *str, va_list *input, p_token *tok);
void s21_spec_f(char *str, va_list *input, p_token *tok);
void s21_spec_gG(char *str, va_list *input, p_token *tok);
void s21_spec_s(char *str, va_list *input, p_token *tok);
void s21_spec_p(char *str, va_list *input, p_token *tok);
void s21_spec_n(s21_size_t const *str_len, va_list *input);
void s21_spec_percentage(char *str, p_token *tok);

#endif  //  SRC_S21_SPRINTF_H_