#include "s21_decimal.h"

/* v1 меньше v2 */
int s21_is_less(s21_decimal value1, s21_decimal value2) {
  int ret = 0;
  s21_decimal res = {0};
  if (s21_get_bit(value1.bits[3], 31) == s21_get_bit(value2.bits[3], 31)) {
    s21_sub(value1, value2, &res); // вычитаем первое из второго
    if (s21_get_bit(res.bits[3], 31))
      ret = 1;
  } else if (s21_get_bit(value1.bits[3], 31) >
             s21_get_bit(value2.bits[3], 31)) {
    ret = 1;
  }
  return ret;
}

/* v1 меньше или равeH v2 */
int s21_is_less_or_equal(s21_decimal val1, s21_decimal val2) {
  return s21_is_less(val1, val2) || s21_is_equal(val1, val2);
}

/* v1 больше v2 */
int s21_is_greater(s21_decimal val1, s21_decimal val2) {
  return (!s21_is_equal(val1, val2) && !s21_is_less(val1, val2));
}

/* v1 больше или равно v2 */
int s21_is_greater_or_equal(s21_decimal val1, s21_decimal val2) {
  return (s21_is_equal(val1, val2) || !s21_is_less(val1, val2));
}

/* v1 равно v2 */
int s21_is_equal(s21_decimal val1, s21_decimal val2) {
  int ret = 0;
  char check = 1;
  for (int i = 2; i > -1; i--) {
    if (val1.bits[i] == 0 && val2.bits[i] == 0) {
      ret = 1;
      check = 0;
    } else {
      ret = 0;
      check = 1;
      break;
    }
  }
  if (val1.bits[0] == val2.bits[0] && val1.bits[1] == val2.bits[1] &&
      val1.bits[2] == val2.bits[2] && s21_zoom(val1) != s21_zoom(val2) &&
      check) {
    ret = 0;
    check = 0;
  }
  if (s21_get_bit(val1.bits[3], 31) == s21_get_bit(val2.bits[3], 31) &&
      check) { // проверка на знаки
    ret = 1;
    s21_decimal val1_exp = val1;
    s21_decimal val2_exp = val2;
    unsigned val1_zoom = s21_zoom(val1_exp) << 16;
    unsigned val2_zoom = s21_zoom(val2_exp) << 16;
    val1_exp.bits[3] = val1_zoom;
    val2_exp.bits[3] = val2_zoom;
    if (val1.bits[3] > INT_MAX)
      s21_negate(val1_exp, &val1_exp);
    if (val2.bits[3] > INT_MAX)
      s21_negate(val2_exp, &val2_exp);
    if (val1.bits[3] == val2.bits[3]) {
      val1_exp = val1;
      val2_exp = val2;
    } else {
      int zoom = s21_zoom(val1);
      if (s21_zoom(val1) < s21_zoom(val2)) /* берём максимальный zoom */
        zoom = s21_zoom(val2);
      for (int i = zoom; i > s21_zoom(val1); i--)
        s21_mul(val1_exp, DEC_TEN, &val1_exp);
      for (int i = zoom; i > s21_zoom(val2); i--)
        s21_mul(val2_exp, DEC_TEN, &val2_exp);
    }
    for (int j = 2; j > -1; j--) {
      if (val1_exp.bits[j] != val2_exp.bits[j]) {
        ret = 0;
        break;
      }
    }
  }
  return ret;
}

/* v1 не равен v2 */
int s21_is_not_equal(s21_decimal val1, s21_decimal val2) {
  return !s21_is_equal(val1, val2);
}

int s21_is_equal_big(s21_big_decimal value_1, s21_big_decimal value_2) {
  int equal = 1;
  equalize_scale(&value_1, &value_2);
  for (int i = 5; i >= 0 && equal; i--)
    equal = value_1.bits[i] == value_2.bits[i];
  return equal;
}

int s21_is_greater_or_equal_big(s21_big_decimal value_1,
                                s21_big_decimal value_2) {
  int result = s21_is_greater_big(value_1, value_2);
  if (!result)
    result = s21_is_equal_big(value_1, value_2);
  return result;
}

int s21_is_greater_big(s21_big_decimal value_1, s21_big_decimal value_2) {
  equalize_scale(&value_1, &value_2);
  int idx = 5;
  int greater = 0;
  while (idx >= 0) {
    if ((value_1.bits[idx] || value_2.bits[idx])) {
      if (value_1.bits[idx] > value_2.bits[idx])
        greater = 1;
      if (value_1.bits[idx] != value_2.bits[idx])
        break;
    }
    idx--;
  }
  return greater;
}
