#include "s21_decimal.h"

/* Округление в меньшую сторону */
int s21_floor(s21_decimal value, s21_decimal *result) {
  *result = DEC_NULL;
  int status = 0;
  int scale = s21_zoom(value);
  int sign = s21_get_bit(value.bits[3], 31);
  *result = value;
  if (scale) {
    int zero = 1;
    trim_fraction(result);
    for (int i = 0; i < 3 && zero; i++)
      zero = value.bits[i] == 0;
    if (sign && !zero) {
      s21_negate(*result, result);
      s21_add(*result, DEC_1, result);
      s21_negate(*result, result);
    }
  }
  check_zero_dec(result);
  return status;
}

/* Математическое округление */
int s21_round(s21_decimal value,
              s21_decimal *result) { /* получение остатка от 0,1 */
  *result = DEC_NULL;
  int error = 0;
  s21_decimal save_res = {0};
  error = s21_mod(value, DEC_1, &save_res); /* получаем десятую часть */
  if (!error)
    error = s21_mul(save_res, DEC_TEN, &save_res);
  if (!error)
    error = s21_truncate(save_res, &save_res);
  if (s21_get_bit(save_res.bits[3], 31))
    s21_negate(save_res, &save_res);
  if (s21_is_less(DEC_4, save_res) &&
      !s21_get_bit(value.bits[3], 31)) { /* прибавлять */
    if (!error)
      error = s21_add(value, DEC_1, &value);
    if (!error)
      error = s21_truncate(value, &(*result));
  } else if (s21_is_less(DEC_4, save_res) && s21_get_bit(value.bits[3], 31)) {
    if (!error)
      s21_sub(value, DEC_1, &value);
    if (!error)
      s21_truncate(value, &(*result));
  } else {
    if (!error)
      s21_truncate(value, &(*result));
  }
  check_zero_dec(result);
  return !!error;
}

/* Взятие целой части */
int s21_truncate(s21_decimal value, s21_decimal *result) {
  *result = DEC_NULL;
  int error = 0;
  int zoom = s21_zoom(value); /* берём зум */
  s21_decimal zoom_dec = {0}; /* for mod */
  zoom_dec.bits[0] = 1;
  s21_decimal result_mod = value;                     /* for mod */
  s21_decimal result_mod_end = {0};                   /* for mod */
  error = s21_mod(result_mod, zoom_dec, &result_mod); /* записываем */
  if (!error)
    error = s21_sub(value, result_mod, &(result_mod_end)); /* вычитаем*/
  if (!error) {
    s21_decimal tmp = {0};
    error = s21_div(result_mod_end, zoom_dec, result);
  }
  return !!error;
}

/* Умножение на (-1) */
int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = 0;
  value.bits[3] ^= (unsigned)pow(2, 31);
  *result = value;
  return error;
}
