#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>

#define POSITIVE_SIGN 0
#define NEGATIVE_SIGN 1

typedef struct {
  unsigned bits[4];
} s21_decimal;

#define DEC_NULL                                                               \
  (s21_decimal) {                                                              \
    { 0, 0, 0, 0 }                                                             \
  }

#define DEC_1                                                                  \
  (s21_decimal) {                                                              \
    { 1, 0, 0, 0 }                                                             \
  }

#define DEC_4                                                                  \
  (s21_decimal) {                                                              \
    { 4, 0, 0, 0 }                                                             \
  }

#define DEC_TEN                                                                \
  (s21_decimal) {                                                              \
    { 10, 0, 0, 0 }                                                            \
  }

#define DEC_MAX                                                                \
  (s21_decimal) {                                                              \
    { 4294967295, 4294967295, 4294967295, 0 }                                  \
  }

#define DEC_MIN                                                                \
  (s21_decimal) {                                                              \
    { 4294967295, 4294967295, 4294967295, 2147483648 }                         \
  }

#define DEC_MINT                                                               \
  (s21_decimal) {                                                              \
    { 2147483647, 0, 0, 0 }                                                    \
  }

#define DEC_mINT                                                               \
  (s21_decimal) {                                                              \
    { 2147483648, 0, 0, 2147483648 }                                           \
  }

typedef struct {
  unsigned bits[7];
} s21_big_decimal;

#define big_dec_ten                                                            \
  (s21_big_decimal) {                                                          \
    { 10, 0, 0, 0, 0, 0, 0 }                                                   \
  }

#define big_dec_zero                                                           \
  (s21_big_decimal) { 0 }

#define big_dec_one                                                            \
  (s21_big_decimal) {                                                          \
    { 1, 0, 0, 0, 0, 0, 0 }                                                    \
  }

#define MAX_lim_float 7.9228162514264337593543950335e28 // 0x6f800000
#define MIN_lim_float 1e-28
#define INF                                                                    \
  (s21_decimal) {                                                              \
    { 0, 0, 0, 16711680 }                                                      \
  }
#define N_INF                                                                  \
  (s21_decimal) {                                                              \
    { 0, 0, 0, 2164195328 }                                                    \
  }

/* Arithmetic */
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_shift(s21_big_decimal *value, int step);
int s21_find_nonzero_bit(s21_big_decimal src);
int s21_simple_add(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
int s21_simple_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
int s21_simple_mul(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
int s21_simple_div(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
s21_big_decimal div_int(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result);
int div_fractional(s21_big_decimal decimal_reminder, s21_big_decimal value_2,
                   s21_big_decimal *result);
int s21_simple_mod(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
int trim_fraction(s21_decimal *value);

/* Comparisons */
int s21_is_less(s21_decimal val1, s21_decimal val2);
int s21_is_less_or_equal(s21_decimal val1, s21_decimal val2);
int s21_is_greater(s21_decimal val1, s21_decimal val2);
int s21_is_greater_or_equal(s21_decimal val1, s21_decimal val2);
int s21_is_equal(s21_decimal val1, s21_decimal val2);
int s21_is_not_equal(s21_decimal val1, s21_decimal val2);
int s21_is_greater_big(s21_big_decimal value_1, s21_big_decimal value_2);
int s21_is_greater_or_equal_big(s21_big_decimal value_1,
                                s21_big_decimal value_2);
int s21_is_equal_big(s21_big_decimal value_1, s21_big_decimal value_2);

/* Converters */
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

/* Math funcs */
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

/* Additional funcs*/
int s21_get_bit(int value, int index);
int s21_set_bit(int value, int index);
void s21_set_accur(s21_decimal *val, int acc);
void s21_print_bit(s21_decimal value);
int s21_zoom(s21_decimal value);
int check_zero(s21_big_decimal *value);
void set_bit_dec(s21_decimal *src, int pos);
void set_bit_big_dec(s21_big_decimal *src, int pos);
int check_bit_big_dec(s21_big_decimal src, int pos);
void set_scale_big_dec(s21_big_decimal *src, int scale);
int get_scale_big_dec(s21_big_decimal src);
void equalize_scale(s21_big_decimal *value_1, s21_big_decimal *value_2);
void reset_big_decimal(s21_big_decimal *src);
void reset_decimal(s21_decimal *src);
void set_sign_big_dec(s21_big_decimal *src, int sign);
void set_sign_dec(s21_decimal *src, int sign);
int get_sign_dec(s21_decimal src);
int get_sign_big_dec(s21_big_decimal src);
int from_dec_to_big(s21_decimal src, s21_big_decimal *dst);
int from_big_to_dec(s21_big_decimal src, s21_decimal *dst);
int l_shift(s21_decimal *dec, int step);
int r_shift(s21_decimal *dec, int step);
void check_zero_dec(s21_decimal *src);
int if_dec_inf_or_nan(s21_decimal *src, int *flag_err);

#endif //  SRC_S21_DECIMAL_H_ 
