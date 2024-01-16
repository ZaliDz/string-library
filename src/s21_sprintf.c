#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list input = {0};
  va_start(input, format);
  p_token tok = {0};
  s21_size_t i = 0;
  str = s21_strcpy(str, "");
  while (*format) {
    init_token(&tok);
    switch (*format) {
      case '%':
        format++;
        format = parse_token(&tok, format, &input);

        if (is_specifier(*format)) tok.specifier = *(format++);

        s21_default_precision(&tok);

        str = s21_check_specifier(str, &i, &input, &tok);
        break;
      default:
        str[i++] = *(format++);
        str[i] = '\0';
        break;
    }
  }
  va_end(input);
  return (int)s21_strlen(str);
}

/* init */

void init_token(p_token *tok) {
  tok->precision = -1;
  tok->flag_plus = tok->flag_minus = tok->flag_space = tok->flag_zero =
      tok->flag_sharp = tok->width = tok->point = tok->specifier = tok->length =
          0;
}

/* parsing format token */
/////////////////////////////////////////////////////////////

char *parse_token(p_token *tok, const char *format, va_list *input) {
  format = set_flags(tok, format);
  format = set_width(tok, format, input);
  format = set_precision(tok, format, input);
  format = set_length(tok, format);
  return (char *)format;
}

char *set_flags(p_token *tok, const char *format) {
  while (is_flag(*format)) {
    s21_check_flag(tok, format++);
  }
  return (char *)format;
}

char *set_width(p_token *tok, const char *format, va_list *input) {
  if (is_digit(*format) || *format == '*') {
    format = s21_str_to_int(&tok->width, format++, input);
    if (tok->width < 0) {
      tok->flag_minus = 1;
      tok->width *= -1;
    }
  }
  return (char *)format;
}

char *set_precision(p_token *tok, const char *format, va_list *input) {
  if (*format == '.') {
    tok->point = 1;
    tok->precision = 0;
    format++;
    if (is_digit(*format) || *format == '*') {
      format = s21_str_to_int(&tok->precision, format++, input);
    } else {
      tok->precision = -1;
    }
  }
  return (char *)format;
}

char *set_length(p_token *tok, const char *format) {
  if (is_length(*format)) {
    while (is_length(*format)) {
      tok->length = *(format++);
    }
  }
  return (char *)format;
}

void s21_default_precision(p_token *tok) {
  if (tok->point && tok->precision <= 0) {
    tok->precision = 0;
  }

  if (!tok->point && s21_strchr("eEfgG", tok->specifier)) {
    tok->precision = 6;
  }
}

void s21_check_flag(p_token *tok, const char *format) {
  switch (*format) {
    case '-':
      tok->flag_minus = 1;
      break;
    case '+':
      tok->flag_plus = 1;
      break;
    case ' ':
      tok->flag_space = 1;
      break;
    case '#':
      tok->flag_sharp = 1;
      break;
    case '0':
      tok->flag_zero = 1;
      break;
  }
}

char *s21_check_specifier(char *str, s21_size_t *str_len, va_list *input,
                          p_token *tok) {
  char *tmp_str = s21_NULL;
  tmp_str = calloc(1024, sizeof(char));
  if (tmp_str != s21_NULL) {
    switch (tok->specifier) {
      case 'c':
        s21_spec_c(tmp_str, input, tok);
        break;
      case 'd':
      case 'i':
      case 'u':
        s21_spec_di(tmp_str, input, tok);
        break;
      case 'e':
      case 'E':
        s21_spec_eE(tmp_str, input, tok);
        break;
      case 'f':
        s21_spec_f(tmp_str, input, tok);
        break;
      case 'g':
      case 'G':
        s21_spec_gG(tmp_str, input, tok);
        break;
      case 'n':
        s21_spec_n(str_len, input);
        break;
      case 'p':
        s21_spec_p(tmp_str, input, tok);
        break;
      case 's':
        s21_spec_s(tmp_str, input, tok);
        break;
      case 'o':
      case 'x':
      case 'X':
        s21_8_16_int(tmp_str, input, tok);
        break;
      case '%':
        s21_spec_percentage(tmp_str, tok);
        break;
    }
    s21_strcat(str, tmp_str);
    free(tmp_str);
    *str_len = s21_strlen(str);
  }
  return str;
}

/* [ASCII] to int parsing */
////////////////////////////////////////////////

char *s21_str_to_int(int *number, const char *format, va_list *input) {
  if (*format == '*') {
    *number = va_arg(*input, int);
    format++;
  } else {
    char *formstr = (char *)format;

    while (is_digit(*format)) {
      format++;
    }

    *number += parse_number_from_fstr(&formstr);
  }

  return (char *)format;
}

/* int to ASCII parsing */

/* Десятичное целое число в строку */

void s21_int_to_str(p_token *tok, char *str, int64_t number) {
  char *point = s21_NULL;
  int flag = 1;
  tok->sign = number < 0 ? -1 : 1;
  number *= tok->sign;
  point = str;

  while (flag) {
    int digit = 0;
    if (number >= 10) {
      digit = (int)fmod(number, 10);
      number = (number - digit) / 10;
    } else {
      digit = (int)number;
      flag = 0;
    }
    *(point++) = (char)(digit + '0');
  }
  if (s21_strchr("dioxX", tok->specifier) && tok->precision == 0 &&
      number == 0 && tok->point)
    str[0] = '\0';
  *point = '\0';
  s21_reverse_writing(str);
}

/* Беззнаковое восьмеричное число в строку */

void s21_ol_to_str(char *str, unsigned long long number, p_token *tok) {
  if (tok->specifier == 'o') {
    char *point = s21_NULL;
    int flag = 1;

    point = str;

    while (flag) {
      unsigned int digit;
      if (number > 7) {
        digit = number % 8;
        number = (number - digit) / 8;
      } else {
        digit = number;
        flag = 0;
      }
      *(point++) = (char)(digit + '0');
    }

    if (tok->flag_sharp == 1) *(point++) = '0';
    *point = '\0';
    s21_reverse_writing(str);
  }
}

/* Беззнаковое шестнадцатеричное целое число(X - заглавные буквы) в строку */

void s21_hl_to_str(char *str, unsigned long long number, p_token *tok) {
  if (tok->specifier == 'x' || tok->specifier == 'X') {
    char *point = s21_NULL;
    int flag = 1, shift = 0;
    point = str;
    shift = tok->specifier == 'x' ? 32 : 0;

    while (flag) {
      unsigned int digit;
      if (number > 15) {
        digit = number % 16;
        number = (number - digit) / 16;
      } else {
        digit = number;
        flag = 0;
      }
      if (digit < 10)
        *(point++) = (char)(digit + '0');
      else
        *(point++) = (char)(digit + 'A' + shift - 10);
    }
    *point = '\0';
    if (number == 0) tok->flag_sharp = 0;
    s21_reverse_writing(str);
  }
}

/* НЕцелое число в строку */

void s21_float_to_str(char *str, long double number, p_token *tok) {
  tok->sign = number < 0 ? -1 : 1;
  number *= tok->sign;
  int local_sign = tok->sign;

  // цифры после точки не нужны
  if (tok->precision <= 0) {
    s21_int_to_str(tok, str, roundl(number));
  } else {
    long double left = 0, right = 0;
    int pos = 0;
    char result[256] = {0};
    right = modfl(number, &left);
    if (s21_strchr("gG", tok->specifier) && tok->precision == 1 &&
        right >= 0.5) {
      left++;
    }
    s21_int_to_str(tok, str, roundl(left));
    s21_strcat(str, ".");
    int local_precision = tok->precision;
    if (s21_strchr("gG", tok->specifier)) {
      if (left >= 1) {
        local_precision = tok->precision - (int)s21_strlen(str) + 1 +
                          ((int)roundl(number) == 0);
      } else if (right * pow(10, local_precision) < 1) {
        local_precision = 4;
      }
    }
    for (int i = 0; i < local_precision; i++) right *= 10;
    s21_int_to_str(tok, result, roundl(right));
    pos = (int)s21_strlen(result);
    for (int i = 0; pos < local_precision; i++, pos++) {
      s21_move_string(result);
      result[i] = '0';
    }
    if (s21_strchr("gG", tok->specifier) && (int)roundl(number) != 0) {
      result[tok->precision - (int)s21_strlen(str) + 1] = '\0';
    }
    s21_strcat(str, result);
  }
  tok->sign = local_sign;
}

/* Функция обработки ширины форматной строки */
void s21_make_string_width(p_token *tok, char *str) {
  int pos_str = (int)s21_strlen(str);
  if (tok->width > pos_str) {
    str[tok->width + 1] = '\0';
    while (pos_str >= 0) {
      str[tok->width--] = str[pos_str--];
    }
    for (pos_str = 0; pos_str <= tok->width; pos_str++) {
      if (s21_strchr("diopuxX", tok->specifier)) {
        str[pos_str] = (tok->flag_zero && !tok->point) ? '0' : ' ';
      }
      if (s21_strchr("feEgGcs", tok->specifier)) {
        str[pos_str] = (tok->flag_zero) ? '0' : ' ';
      }
    }
  }
}

/* СПЕЦИФИКАТОРЫ */
//////////////////////////////////////////////////////////

void s21_gG_to_str(char *str, long double number, p_token *tok) {
  tok->sign = number < 0 ? -1 : 1;
  number *= tok->sign;
  int local_sign = tok->sign;
  if ((number / pow(10, tok->precision)) > 1 ||
      ((number * (pow(10, 4))) < 1 && (number * (pow(10, 21))) >= 1)) {
    if (number < 1 && tok->precision <= 0) {
      tok->precision = 4;
    } else {
      --tok->precision;
    }
    s21_eE_to_str(str, number, tok);
  } else {
    tok->precision > 0 ? 1 : ++tok->precision;
    s21_float_to_str(str, number, tok);
    s21_delete_zero(tok, str);
  }
  tok->sign = local_sign;
}

void s21_eE_to_str(char *str, long double number, p_token *tok) {
  tok->sign = number < 0 ? -1 : 1;
  int notation = 0, local_sign = tok->sign;
  number *= tok->sign;
  if (number < 1) {
    while (number < 1 && notation-- > -42) {
      number *= 10;
    }
    number < 1 ? (notation = number = 0) : 1;
    if (s21_compare_round(tok, number)) {
      if (tok->precision >= 0) {
        --tok->precision;
      }
      notation++;
    }
    if (s21_strchr("gG", tok->specifier)) s21_delete_zero(tok, str);
    s21_make_mantissa(tok, &str[s21_strlen(str)], notation);
  } else {
    while (number >= 10) {
      notation++;
      number /= 10;
    }
    if (s21_compare_round(tok, number)) {
      if (tok->precision >= 0) {
        --tok->precision;
      }
      notation++;
    }
  }
  number = roundl(number * pow(10, tok->precision));
  s21_int_to_str(tok, str, number);
  if (tok->precision > 0) {
    s21_move_string(&str[1]);
    str[1] = '.';
    for (int i = (int)s21_strlen(str); i < 2 + tok->precision; i++) {
      str[i] = '0';
    }
  }
  if (s21_strchr("gG", tok->specifier)) s21_delete_zero(tok, str);
  s21_make_mantissa(tok, &str[s21_strlen(str)], notation);
  tok->sign = local_sign;
}

/* Функция обработки флага '#' */
void s21_sharp_flag(p_token *tok, char *str) {
  int pos = 0;
  if (s21_strchr("oxX", tok->specifier)) {
    if (tok->specifier == 'x' || tok->specifier == 'X') {
      s21_move_string(str);
    }
    tok->specifier == 'o' ? 1 : s21_move_string(str);
    str[pos++] = '0';
    if (tok->specifier != 'o') str[pos] = tok->specifier == 'x' ? 'x' : 'X';
  }
  if (s21_strchr("eEf", tok->specifier)) {
    if (!s21_strchr(str, '.')) {
      str[s21_strlen(str)] = '.';
    }
  }
}

/* Символ */
void s21_spec_c(char *str, va_list *input, p_token *tok) {
  wchar_t wchar = 0;
  wchar_t wchar_s[10] = {0};
  switch (tok->length) {
    case 'l':
      wchar = va_arg(*input, wchar_t);
      wchar_s[0] = wchar;
      s21_wch_to_str(str, wchar_s, 0);
      break;
    default:
      str[0] = va_arg(*input, int);
      str[1] = '\0';
      break;
  }
  s21_make_string_flags(tok, str);
  s21_make_string_width(tok, str);
}

/* Десятичное целое число */
void s21_spec_di(char *str, va_list *input, p_token *tok) {
  long int ld_number = 0;
  short int sd_number = 0;
  int number = 0;

  switch (tok->length) {
    case 'h':
      sd_number = (short int)va_arg(*input, int);
      s21_int_to_str(tok, str, sd_number);
      break;
    case 'l':
      ld_number = (long int)va_arg(*input, long int);
      s21_int_to_str(tok, str, ld_number);
      break;
    default:
      number = va_arg(*input, int);
      s21_int_to_str(tok, str, number);
      break;
  }
  parse_flags_width_precision(tok, str);
}

/* Научная нотация (мантисса/экспонента) с использованием символа e/E (вывод
 * чисел должен совпадать с точностью до e-6) */
void s21_spec_eE(char *str, va_list *input, p_token *tok) {
  long double l_number = 0;
  double number = 0;
  switch (tok->length) {
    case 'L':
      l_number = va_arg(*input, long double);
      s21_eE_to_str(str, l_number, tok);
      break;
    default:
      number = va_arg(*input, double);
      s21_eE_to_str(str, number, tok);
      break;
  }
  parse_flags_width_precision(tok, str);
}

void s21_spec_f(char *str, va_list *input, p_token *tok) {
  long double l_number = 0;
  double d_number = 0;

  switch (tok->length) {
    case 'L':
      l_number = va_arg(*input, long double);
      s21_float_to_str(str, l_number, tok);
      break;
    case 'l':
    default:
      d_number = va_arg(*input, double);
      s21_float_to_str(str, d_number, tok);
      break;
  }
  s21_make_string_flags(tok, str);
  s21_make_string_width(tok, str);
}

/* Использует кратчайший из представлений десятичного числа */
void s21_spec_gG(char *str, va_list *input, p_token *tok) {
  long double l_number = 0;
  double number = 0;

  switch (tok->length) {
    case 'L':
      l_number = va_arg(*input, long double);
      s21_gG_to_str(str, l_number, tok);
      break;
    case 'l':
    default:
      number = va_arg(*input, double);
      s21_gG_to_str(str, number, tok);
      break;
  }
  parse_flags_width_precision(tok, str);
}

/* Обработка беззнаковых чисел */
void s21_8_16_int(char *str, va_list *input, p_token *tok) {
  unsigned long int ld_number = 0;
  unsigned short int sd_number = 0;
  unsigned int number = 0;

  switch (tok->length) {
    case 'h':
      sd_number = (unsigned short int)va_arg(*input, unsigned int);
      s21_ol_to_str(str, sd_number, tok);
      s21_hl_to_str(str, sd_number, tok);
      break;
    case 'l':
      ld_number = (unsigned long int)va_arg(*input, unsigned long int);

      s21_ol_to_str(str, ld_number, tok);
      s21_hl_to_str(str, ld_number, tok);
      break;
    default:
      number = va_arg(*input, unsigned int);
      s21_ol_to_str(str, number, tok);
      s21_hl_to_str(str, number, tok);
      break;
  }
  parse_flags_width_precision(tok, str);
}

/* Строка символов */
void s21_spec_s(char *str, va_list *input, p_token *tok) {
  char *string = s21_NULL;
  string = va_arg(*input, char *);
  if (tok->precision > -1)
    s21_strncat(str, string, tok->precision);
  else
    s21_strcat(str, string);
  parse_flags_width_precision(tok, str);
}

/* Адрес указателя */
void s21_spec_p(char *str, va_list *input, p_token *tok) {
  int width = tok->width < 15 ? 15 : tok->width;
  char *str_start = s21_NULL;
  str_start = str;
  int *p = va_arg(*input, int *);
  char buf[4] = "x0";
  if (p == s21_NULL) {
    s21_strcpy(buf, "0x0");
  }
  for (int *j = p, k = 0; j && k < width;
       j = (void *)(((s21_size_t)j) >> 4), k++) {
    unsigned int dig = ((s21_size_t)j) % 0x10;
    if (dig < 10)
      *(str_start++) = (char)('0' + dig);
    else
      *(str_start++) = (char)('a' + (dig - 10));
  }
  *str_start = '\0';
  s21_strcat(str, buf);
  s21_reverse_writing(str);
  parse_flags_width_precision(tok, str);
}

/* хелперы */
//////////////////////////////////////////

void s21_delete_zero(p_token *tok, char *str) {
  int pos = (int)s21_strlen(str) - 1;
  if (pos > 0) {
    while (str[pos] == '0') {
      if (!tok->flag_sharp || (tok->flag_sharp && pos > tok->precision)) {
        str[pos] = '\0';
        pos -= 1;
      }
    }
    if (!tok->flag_sharp && str[pos] == '.') {
      str[pos] = '\0';
    }
  }
}

// Если округляется до следующего разряда, необходимо увеличить степень
// и поделить число на 10
// В этом случае возвращаем True
int s21_compare_round(p_token *tok, long double number) {
  char compare_str[128] = {'\0'};
  int length = 0, result = 0;
  number *= pow(10, tok->precision);
  s21_int_to_str(tok, compare_str, number);
  length = s21_strlen(compare_str);
  number = roundl(number);
  s21_int_to_str(tok, compare_str, number);
  if (length < (int)s21_strlen(compare_str)) {
    result = 1;
  }
  return result;
}

void s21_make_mantissa(p_token *tok, char *mantice, int notation) {
  int start_pos = 0;
  if (tok->flag_sharp && tok->precision <= 0) {
    mantice[start_pos++] = '.';
  }
  mantice[start_pos++] = s21_strchr("eg", tok->specifier) ? 'e' : 'E';
  mantice[start_pos++] = (notation >= 0) ? '+' : '-';
  if (abs(notation) < 10) {
    mantice[start_pos++] = '0';
    s21_int_to_str(tok, &mantice[start_pos], abs(notation));
  } else {
    s21_int_to_str(tok, &mantice[start_pos], abs(notation));
  }
}

void s21_wch_to_str(char *str, wchar_t *wstr, s21_size_t length) {
  s21_size_t str_len = s21_strlen(str);
  int cnt = length;
  char *p = str + str_len;

  while (*wstr != '\0' && (cnt-- > 0 || length == 0)) {
    *(p++) = (char)*(wstr++);
  }
  *p = '\0';
}

/* Функция сдвига строки на 1 позицию вправо */
void s21_move_string(char *str) {
  int pos = (int)s21_strlen(str);
  str[pos + 1] = '\0';
  while (pos > 0) {
    str[pos] = str[pos - 1];
    pos--;
  }
}

void s21_make_string_flags(p_token *tok, char *str) {
  int pos = 0;
  if (!s21_strchr("cuo\%", tok->specifier)) {
    if (tok->specifier != 'x' && tok->specifier != 'X') {
      if (tok->sign < 0) {
        s21_move_string(str);
        str[pos++] = '-';
        tok->sign = 0;
      }
      if (tok->flag_plus && tok->sign != 0) {
        s21_move_string(str);
        str[pos] = (tok->sign > 0) ? '+' : '-';
        tok->sign = 0;
      }
      if (tok->flag_space && tok->sign != 0) {
        s21_move_string(str);
        str[pos] = (tok->sign > 0) ? ' ' : '-';
        tok->sign = 0;
      }
    }
    if (tok->flag_sharp) {
      s21_sharp_flag(tok, str);
    }
  }
  if (tok->flag_minus) {
    pos = (int)s21_strlen(str);
    while (pos < tok->width) {
      str[pos++] = ' ';
    }
  }
}

/* Количество символов, напечатанных до появления %n */
void s21_spec_n(s21_size_t const *str_len, va_list *input) {
  int *p = va_arg(*input, int *);
  *p = (int)*str_len;
}

/* Символ % */
void s21_spec_percentage(char *str, p_token *tok) {
  s21_strcat(str, "%");
  parse_flags_width_precision(tok, str);
}

/* Просто объединяющая функция парсинга */
void parse_flags_width_precision(p_token *tok, char *str) {
  s21_make_string_precision(tok, str);
  s21_make_string_flags(tok, str);
  s21_make_string_width(tok, str);
}

/* Функция обработки точности форматной строки */
void s21_make_string_precision(p_token *tok, char *str) {
  char *string_with_precision = s21_NULL;
  string_with_precision = calloc(1024, sizeof(char));
  int pos = 0;
  if (string_with_precision != s21_NULL) {
    if (s21_strchr("diouxX", tok->specifier)) {
      while (pos < tok->precision - (int)s21_strlen(str)) {
        string_with_precision[pos++] = '0';
      }
      for (int i = 0; str[i] != '\0'; i++) {
        string_with_precision[pos++] = str[i];
      }
      s21_strcpy(str, string_with_precision);
    }
    free(string_with_precision);
  }
}

/* Функция обратной записи в строку */
void s21_reverse_writing(char *str) {
  char c = 0;
  s21_size_t length = s21_strlen(str);
  for (s21_size_t i = 0; i < length / 2; i++) {
    c = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = c;
  }
}
