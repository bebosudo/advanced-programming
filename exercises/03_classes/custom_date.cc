/*
  clang++ --std=c++11 custom_date.cc -o ../build/custom_date &&
    ../build/custom_date
*/

#include <iostream>

enum class Month { jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };
int month_to_days(Month m);

class CustomDate {
  // Consider days since the epoch (1/1/1970).
  short unsigned int _day;
  Month _month;
  int _year;

public:
  int _days_since_epoch(CustomDate date);
  CustomDate(int year, Month month, short unsigned int day);
  int day() const;
  Month month() const;
  int year() const;
};

int main(int argc, char const *argv[]) {
  unsigned int i = 0;
  std::cout << i - 1 << std::endl;
  Month m = Month::jul;
  // std::cout << m << std::endl;
  CustomDate cd{1970, Month::jan, 2};
  std::cout << CustomDate::_days_since_epoch(cd) << std::endl;
}

CustomDate::CustomDate(int year, Month month, short unsigned int day)
    : _day{day}, _month{month}, _year{year} {}
int CustomDate::day() const { return _day; };
Month CustomDate::month() const { return _month; };
int CustomDate::year() const { return _year; };
int CustomDate::_days_since_epoch(CustomDate date) {
  unsigned int d = (date.year() - 1970) * 365;
  d += month_to_days(date.month());
  return d + date.day();
};

int month_to_days(Month m) {
  switch (m) {
  case Month::jan:
  case Month::mar:
  case Month::may:
  case Month::jul:
  case Month::aug:
  case Month::oct:
  case Month::dec:
    return 31;

  case Month::apr:
  case Month::jun:
  case Month::sep:
  case Month::nov:
    return 30;

  case Month::feb:
    return 28;
  }
}
