/*
  clang++ --std=c++11 custom_date.cc -o ../build/custom_date &&
    ../build/custom_date
*/

#include <iostream>

enum class Month { jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };
int month_to_days(Month m);
Month days_to_month(const unsigned short int days, int &leftover);

class CustomDate {
  // Consider days since the epoch (1/1/1970).
  short unsigned int _day;
  Month _month;
  int _year;

  int _days_since_epoch() const;

public:
  CustomDate(int year, Month month, short unsigned int day)
      : _day{day}, _month{month}, _year{year} {}

  int day() const { return _day; }
  Month month() const { return _month; }
  int year() const { return _year; }

  void add_day(const unsigned int n);
};

// Helper functions.
bool operator==(const CustomDate &lhs, const CustomDate &rhs) {
  return lhs.year() == rhs.year() && lhs.month() == rhs.month() &&
         lhs.day() == rhs.day();
}
bool operator!=(const CustomDate &lhs, const CustomDate &rhs) {
  return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, const CustomDate &d);

int main(int argc, char const *argv[]) {

  CustomDate cd0{1970, Month::jan, 30};
  CustomDate cd1{1970, Month::jan, 1};
  CustomDate cd2{1970, Month::mar, 31};
  CustomDate cd3{1970, Month::dec, 31};

  std::cout << cd0 << std::endl;
  cd0.add_day(1);
  std::cout << cd0 << std::endl << std::endl;

  std::cout << cd1 << std::endl;
  cd1.add_day(1);
  std::cout << cd1 << std::endl << std::endl;

  std::cout << cd2 << std::endl;
  cd2.add_day(1);
  std::cout << cd2 << std::endl << std::endl;

  std::cout << cd3 << std::endl;
  cd3.add_day(1);
  std::cout << cd3 << std::endl << std::endl;
}

int CustomDate::_days_since_epoch() const {
  unsigned int d = (year() - 1970) * 365;
  d += month_to_days(month());
  return d + day() - 1;
};

int month_to_days(Month m) {
  // Return the number of days from Jan 1st up to the first day of the month
  // passed as argument.
  switch (m) {
  case Month::jan:
    return 0;
  case Month::feb:
    return 31;
  case Month::mar:
    return 31 + 28;
  case Month::apr:
    return 31 + 28 + 31;
  case Month::may:
    return 31 + 28 + 31 + 30;
  case Month::jun:
    return 31 + 28 + 31 + 30 + 31;
  case Month::jul:
    return 31 + 28 + 31 + 30 + 31 + 30;
  case Month::aug:
    return 31 + 28 + 31 + 30 + 31 + 30 + 31;
  case Month::sep:
    return 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31;
  case Month::oct:
    return 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30;
  case Month::nov:
    return 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31;
  case Month::dec:
    // return 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30;
    return 365 - 31;
  default:
    exit(-1);
  }
}

Month days_to_month(const unsigned short int days, int &leftover) {
  if (days <= 31) {
    leftover = days;
    return Month::jan;
  } else if (days <= 31 + 28) {
    leftover = days % (31);
    return Month::feb;
  } else if (days <= 31 + 28 + 31) {
    leftover = days % (31 + 28);
    return Month::mar;
  } else if (days <= 31 + 28 + 31 + 30) {
    leftover = days % (31 + 28 + 31);
    return Month::apr;
  } else if (days <= 31 + 28 + 31 + 30 + 31) {
    leftover = days % (31 + 28 + 31 + 30);
    return Month::may;
  } else if (days <= 31 + 28 + 31 + 30 + 31 + 30) {
    leftover = days % (31 + 28 + 31 + 30 + 31);
    return Month::jun;
  } else if (days <= 31 + 28 + 31 + 30 + 31 + 30 + 31) {
    leftover = days % (31 + 28 + 31 + 30 + 31 + 30);
    return Month::jul;
  } else if (days <= 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31) {
    leftover = days % (31 + 28 + 31 + 30 + 31 + 30 + 31);
    return Month::aug;
  } else if (days <= 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30) {
    leftover = days % (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31);
    return Month::sep;
  } else if (days <= 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) {
    leftover = days % (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30);
    return Month::oct;
  } else if (days <= 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) {
    leftover = days % (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31);
    return Month::nov;
  } else if (days <= 365) {
    leftover = days % (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30);
    return Month::dec;
  } else {
    exit(-1);
  }
}

void CustomDate::add_day(const unsigned int n) {
  unsigned int d = this->_days_since_epoch();

  // Add the days passed as argument to the days of the current date.
  d += (n + 1);

  _year = (d / 365) + 1970;

  int leftover;
  _month = days_to_month(d % 365, leftover);

  _day = leftover;
}

std::ostream &operator<<(std::ostream &os, const CustomDate &d) {
  return os << d.year() << "/" << ((int)d.month()) + 1 << "/" << d.day();
  // return os;
}
