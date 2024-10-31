#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

// Account management
class Account {
 public:
  Account(double initialCash);

  bool buy(double price, int quantity);

  bool sell(double price, int quantity);

  double getCash() const { return cash_; }
  int getStockPosition() const { return stockPosition_; }

 private:
  double cash_;
  int stockPosition_;
};

#endif  // ACCOUNT_H
