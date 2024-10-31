#include "account.h"
#include <iostream>
#include <string>

Account::Account(double initialCash) : cash_(initialCash), stockPosition_(0) {}

bool Account::buy(double price, int quantity) {
  double cost = price * quantity;
  if (cash_ >= cost) {
    cash_ -= cost;
    stockPosition_ += quantity;
    std::cout << "Bought " << quantity << " shares at " << price
              << ", remaining cash: " << cash_
              << ", total stock: " << stockPosition_ << std::endl;
    return true;
  } else {
    std::cout << "Insufficient funds to buy " << quantity << " shares at "
              << price << std::endl;
    return false;
  }
}

bool Account::sell(double price, int quantity) {
  if (stockPosition_ >= quantity) {
    double revenue = price * quantity;
    cash_ += revenue;
    stockPosition_ -= quantity;
    std::cout << "Sold " << quantity << " shares at " << price
              << ", new cash: " << cash_
              << ", remaining stock: " << stockPosition_ << std::endl;
    return true;
  } else {
    std::cout << "Insufficient stock to sell " << quantity << " shares at "
              << price << std::endl;
    return false;
  }
}
