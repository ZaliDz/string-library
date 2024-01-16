#include "test_all.h"

#include <check.h>

#include "s21_string.h"

#define COLOR "\e[0;105m"

#define LEN 256

START_TEST(s21_memchr_test) {
  char *str = "It's a good day for review!";
  ck_assert_ptr_eq(s21_memchr(str, 'a', 3), memchr(str, 'a', 3));
  ck_assert_ptr_eq(s21_memchr(str, 'f', 15), memchr(str, 'f', 15));
  ck_assert_ptr_eq(s21_memchr(str, ' ', 7), memchr(str, ' ', 7));
  ck_assert_ptr_eq(s21_memchr(str, 's', 4), memchr(str, 's', 4));
  ck_assert_ptr_eq(s21_memchr(str, 'p', 9), memchr(str, 'p', 9));
}
END_TEST

START_TEST(s21_memcmp_test) {
  char str1[] = "When writing code it is necessary to follow the Google style";
  char str2[] = "When writing code it is necessary to follow the google style";
  char str3[] = "1234";
  char str4[] = "12";
  ck_assert_int_eq(s21_memcmp(str1, str2, 30), memcmp(str1, str2, 30));
  ck_assert_int_eq(s21_memcmp(str1, str2, 5), memcmp(str1, str2, 5));
  ck_assert_int_eq(s21_memcmp(str3, str4, 4), memcmp(str3, str4, 4));
  ck_assert_int_eq(s21_memcmp(str4, str3, 2), memcmp(str4, str3, 2));
}
END_TEST

START_TEST(s21_memcpy_test) {
  char input_desc[][LEN] = {"ababab", "12345", "\0"};
  const char *input_str[] = {"0001000", "bababa", ""};
  s21_size_t n[] = {4, 2, 0};
  s21_size_t len = sizeof(n) / sizeof(s21_size_t);

  char copy[LEN] = {0};
  for (s21_size_t i = 0; i < len; i++) {
    strcpy(copy, input_desc[i]);
    char *result = s21_memcpy(input_desc[i], input_str[i], n[i]);
    char *expected_result = memcpy(copy, input_str[i], n[i]);
    ck_assert_pstr_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_memset_test) {
  char *dest1 = calloc(100, sizeof(char));
  char *dest2 = calloc(100, sizeof(char));
  for (s21_size_t i = 0; i < 100; i++) {
    char c = (char)((i * 7) % 127);
    dest1 = memset(dest1, c, i);
    dest2 = s21_memset(dest2, c, i);
    ck_assert_str_eq(dest1, dest2);
  }
  dest1 = memset(dest1, '\0', 15);
  dest2 = s21_memset(dest2, '\0', 15);
  ck_assert_str_eq(dest1, dest2);
  free(dest1);
  free(dest2);
}
END_TEST

START_TEST(s21_strncat_test) {
  struct input_data {
    char str1[LEN];
    const char *str2;
    int n;
  };
  struct input_data testcases[] = {
      {"This is strncat test,", " we add till this! and more", 18},
      {"This is strncat test,", " we add till this! and more", 18},
      {"This is strncat test, add nothing", "", 10},
      {"This is strncat test, add nothing", "", 10},
      {"", "+cat to empty line", 10},
      {"", "+cat to empty line", 10},
      {"Try to n-cat 0 symbols", "aboba", 0},
      {"Try to n-cat 0 symbols", "aboba", 0},
      {"Cat several symbols from empty line", "", 5},
      {"Cat several symbols from empty line", "", 5},
      {"     ", "      ", 3},
      {"     ", "      ", 3}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i += 2) {
    char *result =
        s21_strncat(testcases[i].str1, testcases[i].str2, testcases[i].n);
    char *expected_result =
        strncat(testcases[i].str1, testcases[i + 1].str2, testcases[i + 1].n);
    ck_assert_pstr_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_strchr_test) {
  char *str1[] = {"TEST", "test", "school", "salam"};
  char str2[] = {'E', 'a', 'i', ' '};
  s21_size_t n = 4;
  for (s21_size_t i = 0; i < n; i += 2) {
    char *result = s21_strchr(str1[i], str2[i]);
    char *expected_result = strchr(str1[i], str2[i]);
    ck_assert_pstr_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_strncmp_test) {
  struct input_data {
    const char *str1;
    const char *str2;
    s21_size_t n;
  };
  struct input_data testcases[] = {
      {"Hello world!", "Hello", 13},
      {"Hello world!", "Hello world!", 13},
      {"Hello world!", "Hello world!!!", 13},
      {"\0string", "", 1},
  };
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    int result =
        s21_strncmp(testcases[i].str1, testcases[i].str2, testcases[i].n);
    int expected_result =
        strncmp(testcases[i].str1, testcases[i].str2, testcases[i].n);
    ck_assert_int_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_strncpy_test) {
  char input_desc[][LEN] = {"ababab", "12345", "\0"};
  const char *input_str[] = {"0001000", "bababa", ""};
  s21_size_t n[] = {4, 2, 0};
  s21_size_t len = sizeof(n) / sizeof(s21_size_t);
  char copy[LEN];

  for (s21_size_t i = 0; i < len; i++) {
    strcpy(copy, input_desc[i]);
    char *result = s21_strncpy(input_desc[i], input_str[i], n[i]);
    char *expected_result = strncpy(copy, input_str[i], n[i]);
    ck_assert_pstr_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_strcspn_test) {
  const char *testcases[] = {"Hello world!", "Hi world!", "Hi everybody!", "",
                             "\0string"};
  const char *str2[] = {"lo", "i", "everybody", "lala", "t"};

  for (s21_size_t i = 0; i < 5; i++) {
    s21_size_t result = s21_strcspn(testcases[i], str2[i]);
    s21_size_t expected_result = strcspn(testcases[i], str2[i]);
    ck_assert_int_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_strerror_test) {
  for (int i = -10; i < MAX_ERR; i++) {
    char *result = s21_strerror(i);
    char *expected_result = strerror(i);
    ck_assert_pstr_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_strlen_test) {
  char *testcases[] = {"Hello", "Hello world!", "Hello world!!!", "",
                       "\0string"};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    s21_size_t result = s21_strlen(testcases[i]);
    s21_size_t expected_result = strlen(testcases[i]);
    ck_assert_uint_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_strpbrk_test) {
  struct input_data {
    const char *str1;
    const char *str2;
  };
  struct input_data testcases[] = {{"Hello world!", "w!"},
                                   {"Hello world!", " d"},
                                   {"lalala", "opask"},
                                   {"", "\0"},
                                   {"", ""}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char *result = s21_strpbrk(testcases[i].str1, testcases[i].str2);
    char *expected_result = strpbrk(testcases[i].str1, testcases[i].str2);
    ck_assert_pstr_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_strrchr_test) {
  const char *testcases[] = {"lalalala", "abababab", "zZzZzZ", "\0string"};
  char symb[] = {'b', 'a', ' ', 'l', 'z', 't'};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char *result = s21_strchr(testcases[i], symb[i]);
    char *expected_result = strchr(testcases[i], symb[i]);
    ck_assert_pstr_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_strstr_test) {
  struct input_data {
    const char *haystack;
    const char *needle;
  };
  struct input_data testcases[] = {{"Hello world!", "world"},
                                   {"Hello world!", "Goodbye"},
                                   {"Hello world!", ""},
                                   {"", ""},
                                   {"\0HEY", "HEY"}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char *result = s21_strstr(testcases[i].haystack, testcases[i].needle);
    char *expected_result = strstr(testcases[i].haystack, testcases[i].needle);
    ck_assert_pstr_eq(result, expected_result);
  }
}
END_TEST

START_TEST(s21_strtok_test) {
  char str1[] = "lal+_mdmd++++++dldld";
  char str2[] = "lal+_mdmd++++++dldld";
  char symb[] = "+_";
  char *result = s21_strtok(str1, symb);
  char *expected_result = strtok(str2, symb);
  ck_assert_pstr_eq(result, expected_result);
}
END_TEST

START_TEST(s21_to_upper_test) {
  const char str1[] = "the capital of great britain is london";
  char *result = (char *)s21_to_upper(str1);
  char *expected_result = "THE CAPITAL OF GREAT BRITAIN IS LONDON";
  ck_assert_pstr_eq(result, expected_result);
  if (result) {
    free(result);
  }
}
END_TEST

START_TEST(s21_to_lower_test) {
  const char str1[] = "THE CAPITAL OF GREAT BRITAIN IS LONDON";
  char *result = (char *)s21_to_lower(str1);
  char *expected_result = "the capital of great britain is london";
  ck_assert_pstr_eq(result, expected_result);
  if (result) {
    free(result);
  }
}
END_TEST

START_TEST(s21_insert_test) {
  struct input_data {
    const char *str1;
    const char *str2;
    s21_size_t index;
  };
  struct input_data cases[] = {{"12345", "0", 3},    {"do it", "Let's ", 0},
                               {"I need no", "", 0}, {"", "", 0},
                               {"", " ", 0},         {" ", "", 0},
                               {" ", " ", 0}};
  char *expected_result[] = {"123045", "Let's do it", "I need no", "",
                             " ",      " ",           "  "};
  s21_size_t n = sizeof(cases) / sizeof(cases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    char *result =
        (char *)s21_insert(cases[i].str1, cases[i].str2, cases[i].index);
    ck_assert_pstr_eq(result, expected_result[i]);
    if (result) {
      free(result);
    }
  }
}
END_TEST

START_TEST(s21_trim_test) {
  struct input_data {
    const char *str1;
    const char *str2;
  };
  struct input_data cases[] = {
      {"Floor! Let the bodies hit the Floor!", "Floor !"},
      {"p p p Poker face", "p "},
      {"Oh no no no", "no "},
      {"Oh no no no", "yes "},
      {"aaa", "a"},
      {" ", ""},
      {"", " "},
      {"      ", "       "}};
  char *expected_result[] = {"Let the bodies hit the",
                             "Poker face",
                             "Oh",
                             "Oh no no no",
                             "",
                             " ",
                             "",
                             ""};
  s21_size_t n = sizeof(cases) / sizeof(cases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char *result = (char *)s21_trim(cases[i].str1, cases[i].str2);
    ck_assert_pstr_eq(result, expected_result[i]);
    if (result) {
      free(result);
    }
  }
}
END_TEST

Suite *s21_string_tests_create() {
  Suite *s21_string = suite_create("s21_string");
  TCase *s21_string_tests = tcase_create("S21_STRING");
  tcase_add_test(s21_string_tests, s21_memchr_test);
  tcase_add_test(s21_string_tests, s21_memcmp_test);
  tcase_add_test(s21_string_tests, s21_memcpy_test);
  tcase_add_test(s21_string_tests, s21_memset_test);
  tcase_add_test(s21_string_tests, s21_strncat_test);
  tcase_add_test(s21_string_tests, s21_strchr_test);
  tcase_add_test(s21_string_tests, s21_strncmp_test);
  tcase_add_test(s21_string_tests, s21_strncpy_test);
  tcase_add_test(s21_string_tests, s21_strcspn_test);
  tcase_add_test(s21_string_tests, s21_strerror_test);
  tcase_add_test(s21_string_tests, s21_strlen_test);
  tcase_add_test(s21_string_tests, s21_strpbrk_test);
  tcase_add_test(s21_string_tests, s21_strrchr_test);
  tcase_add_test(s21_string_tests, s21_strstr_test);
  tcase_add_test(s21_string_tests, s21_strtok_test);
  tcase_add_test(s21_string_tests, s21_to_upper_test);
  tcase_add_test(s21_string_tests, s21_to_lower_test);
  tcase_add_test(s21_string_tests, s21_insert_test);
  tcase_add_test(s21_string_tests, s21_trim_test);
  suite_add_tcase(s21_string, s21_string_tests);
  return s21_string;
}

START_TEST(atoi_test1) {
  char src[] = "  +123123123";
  ck_assert_int_eq(atoi(src), s21_atoi(src));
}
END_TEST

Suite *suite_atoi(void) {
  Suite *s = suite_create("suite_atoi");
  TCase *tc = tcase_create("atoi_tc");

  tcase_add_test(tc, atoi_test1);

  suite_add_tcase(s, tc);
  return s;
}

START_TEST(insert_test1) {
  char str[] = "Shlepa";
  char src[] = "I love my . He is very kind!";
  s21_size_t index = 10;
  char expected[] = "I love my Shlepa. He is very kind!";
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_str_eq(got, expected);
  if (got) free(got);
}
END_TEST

START_TEST(insert_test2) {
  char str[] = "Hello, ";
  char src[] = "Aboba!";
  s21_size_t index = 0;
  char expected[] = "Hello, Aboba!";
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_str_eq(got, expected);
  if (got) free(got);
}
END_TEST

START_TEST(insert_test3) {
  char str[] = "";
  char src[] = "";
  s21_size_t index = 100;
  char *expected = "";
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_str_eq(got, expected);
  if (got) free(got);
}
END_TEST

START_TEST(insert_test4) {
  char *src = NULL;
  char *str = NULL;
  s21_size_t index = 100;
  char *expected = NULL;
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_ptr_eq(got, expected);
  if (got) free(got);
}
END_TEST

START_TEST(insert_test5) {
  char str[] = "Monkey";
  char src[] = "Space  ";
  s21_size_t index = 6;
  char expected[] = "Space Monkey ";
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_str_eq(got, expected);
  if (got) free(got);
}
END_TEST

START_TEST(insert_str_null) {
  char *str = NULL;
  char src[] = "Space  ";
  s21_size_t index = 6;
  char expected[] = "Space  ";
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_str_eq(got, expected);
  if (got) free(got);
}
END_TEST

Suite *suite_insert(void) {
  Suite *s = suite_create("suite_insert");
  TCase *tc = tcase_create("insert_tc");

  tcase_add_test(tc, insert_test1);
  tcase_add_test(tc, insert_test2);
  tcase_add_test(tc, insert_test3);
  tcase_add_test(tc, insert_test4);
  tcase_add_test(tc, insert_test5);
  tcase_add_test(tc, insert_str_null);

  suite_add_tcase(s, tc);
  return s;
}

START_TEST(empty_str) {
  s21_size_t n_byte = 0;
  char str[10] = "";
  int find_byte = '\0';

  ck_assert_ptr_eq(s21_memchr(str, find_byte, n_byte),
                   memchr(str, find_byte, n_byte));
}
END_TEST

START_TEST(find_zero_in_string) {
  s21_size_t n_byte = 7;
  char str[10] = "string";
  int find_byte = '\0';

  ck_assert_ptr_eq(s21_memchr(str, find_byte, n_byte),
                   memchr(str, find_byte, n_byte));
}
END_TEST

START_TEST(find_simbol_in_string_in_start) {
  s21_size_t n_byte = 6;
  char str[10] = "Atring";
  int find_byte = 'A';

  ck_assert_ptr_eq(s21_memchr(str, find_byte, n_byte),
                   memchr(str, find_byte, n_byte));
}
END_TEST

START_TEST(find_simbol_in_string_in_end) {
  s21_size_t n_byte = 7;
  char str[10] = "StringA";
  int find_byte = 'A';

  ck_assert_ptr_eq(s21_memchr(str, find_byte, n_byte),
                   memchr(str, find_byte, n_byte));
}
END_TEST

START_TEST(find_num_in_string_in_mid) {
  s21_size_t n_byte = 15;
  char str[16] = "Str55333222ingA";
  int find_byte = '3';

  ck_assert_ptr_eq(s21_memchr(str, find_byte, n_byte),
                   memchr(str, find_byte, n_byte));
}
END_TEST

START_TEST(find_num_in_array_num) {
  int array[] = {1, 2, 3, 666, 5, 99, 100};
  s21_size_t n_byte = sizeof(int) * 7;
  int find_byte = 666;

  ck_assert_ptr_eq(s21_memchr(array, find_byte, n_byte),
                   memchr(array, find_byte, n_byte));
}
END_TEST

START_TEST(find_float_in_array) {
  float array[] = {1, 2, 3, 666, 5, 99, 100};
  s21_size_t n_byte = sizeof(float) * 7;
  int find_byte = 666;

  ck_assert_ptr_eq(s21_memchr(array, find_byte, n_byte),
                   memchr(array, find_byte, n_byte));
}
END_TEST

START_TEST(easy_test_string) {
  s21_size_t n_byte = 5;
  char str[10] = "aboba";
  int find_byte = 'c';

  ck_assert_ptr_eq(s21_memchr(str, find_byte, n_byte),
                   memchr(str, find_byte, n_byte));
}
END_TEST

Suite *suite_memchr(void) {
  Suite *s = suite_create("suite_memchr");
  TCase *tc = tcase_create("memchr_tc");

  tcase_add_test(tc, empty_str);
  tcase_add_test(tc, find_zero_in_string);
  tcase_add_test(tc, find_simbol_in_string_in_start);
  tcase_add_test(tc, find_simbol_in_string_in_end);
  tcase_add_test(tc, find_num_in_string_in_mid);
  tcase_add_test(tc, find_num_in_array_num);
  tcase_add_test(tc, find_float_in_array);
  tcase_add_test(tc, easy_test_string);

  suite_add_tcase(s, tc);
  return s;
}

START_TEST(zero_byte) {
  char str1[] = "aboba";
  char str2[] = "aboba";
  int n = 0;

  ck_assert_int_eq(s21_memcmp(str1, str2, n), memcmp(str1, str2, n));
}
END_TEST

START_TEST(first_byte_in_string) {
  char str1[] = "aboba";
  char str2[] = "aboba";
  int n = 3;

  ck_assert_int_eq(s21_memcmp(str1, str2, n), memcmp(str1, str2, n));
}
END_TEST

START_TEST(register_test) {
  char str1[] = "aboba";
  char str2[] = "Aboba";
  int n = 1;

  ck_assert_int_eq(s21_memcmp(str1, str2, n), memcmp(str1, str2, n));
}
END_TEST

START_TEST(zero_byte_in_num) {
  char str1[] = "1";
  char str2[] = "1";
  int n = 0;

  ck_assert_int_eq(s21_memcmp(str1, str2, n), memcmp(str1, str2, n));
}
END_TEST

START_TEST(first_byte_in_num) {
  char str1[] = "1";
  char str2[] = "1234";
  int n = 1;

  ck_assert_int_eq(s21_memcmp(str1, str2, n), memcmp(str1, str2, n));
}
END_TEST

START_TEST(two_byte_in_num) {
  char str1[] = "1234";
  char str2[] = "1234";
  int n = 2;

  ck_assert_int_eq(s21_memcmp(str1, str2, n), memcmp(str1, str2, n));
}
END_TEST

START_TEST(two_byte_long_two_num) {
  char str1[] = "13";
  char str2[] = "1234";
  int n = 2;

  ck_assert_int_eq(s21_memcmp(str1, str2, n), memcmp(str1, str2, n));
}
END_TEST

START_TEST(two_byte_long_first_num) {
  char str1[] = "134567";
  char str2[] = "1234";
  int n = 2;

  ck_assert_int_eq(s21_memcmp(str1, str2, n), memcmp(str1, str2, n));
}
END_TEST

START_TEST(eq_long_string) {
  char str1[] =
      "111111111111111111112312312312312312434524563567adsfe 4rafa ewfseASDASD";
  char str2[] =
      "111111111111111111112312312312312312434524563567adsfe 4rafa ewfseASDASD";
  int n = 71;

  ck_assert_int_eq(s21_memcmp(str1, str2, n), memcmp(str1, str2, n));
}
END_TEST

START_TEST(not_eq_long_string) {
  char str1[] =
      "111111111111111111112312312312312312434524563567adsfe 4rafa ewfseASDASD";
  char str2[] =
      "111111111111111111112312312312312312434524563567adsfe 4rafa ewfseASDASd";
  int n = 71;

  ck_assert_int_eq(s21_memcmp(str1, str2, n), memcmp(str1, str2, n));
}
END_TEST

Suite *suite_memcmp(void) {
  Suite *s = suite_create("suite_memcmp");
  TCase *tc = tcase_create("memcmp_tc");

  tcase_add_test(tc, zero_byte);
  tcase_add_test(tc, first_byte_in_string);
  tcase_add_test(tc, register_test);
  tcase_add_test(tc, zero_byte_in_num);
  tcase_add_test(tc, first_byte_in_num);
  tcase_add_test(tc, two_byte_in_num);
  tcase_add_test(tc, two_byte_long_two_num);
  tcase_add_test(tc, two_byte_long_first_num);
  tcase_add_test(tc, eq_long_string);
  tcase_add_test(tc, not_eq_long_string);

  suite_add_tcase(s, tc);
  return s;
}

void run_testcase(Suite *testcase) {
  setlocale(LC_ALL, "");
  static int counter_testcase = 1;

  if (counter_testcase > 1) putchar('\n');
  printf("%s%d%s", "CURRENT TEST: ", counter_testcase, COLOR);
  counter_testcase++;

  SRunner *sr = srunner_create(testcase);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  srunner_free(sr);
}

void run_tests(void) {
  Suite *list_cases[] = {suite_sprintf(),
                         suite_sscanf(),
                         s21_string_tests_create(),
                         suite_memcmp(),
                         suite_memchr(),
                         suite_insert(),
                         suite_atoi(),
                         NULL};

  for (Suite **current_testcase = list_cases; *current_testcase != NULL;
       current_testcase++) {
    run_testcase(*current_testcase);
  }
}

int main() {
  run_tests();

  return EXIT_SUCCESS;
}
