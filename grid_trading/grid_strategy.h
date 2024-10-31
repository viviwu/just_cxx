#ifndef GRIDSTRATEGY_H
#define GRIDSTRATEGY_H
#include <string>
#include <vector>
#include "account.h"
#include "market_simulator.h"

struct Strategy {
  int id;
  int account_id;
  std::string params;
};

// Grid strategy
class GridStrategy {
 public:
  GridStrategy(double lowerLimit, double upperLimit, double buyGridSpacing,
               double sellGridSpacing, int quantityPerGrid,
               const std::string& stockSymbol, Account& account);

  void initializeGrids();

  void run(MarketSimulator& market);

 private:
  struct Grid {
    double price;
    int quantity;
    bool executed;
  };

  std::vector<Grid> buyGrids_;
  std::vector<Grid> sellGrids_;
  double lowerLimit_;
  double upperLimit_;
  double buyGridSpacing_;
  double sellGridSpacing_;
  int quantityPerGrid_;
  std::string stockSymbol_;
  Account& account_;
};

#endif  // GRIDSTRATEGY_H
