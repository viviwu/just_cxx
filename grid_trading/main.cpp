// #include <iostream>
#include <string>

#include "account.h"
#include "grid_strategy.h"
#include "market_simulator.h"

// using namespace std;

int main() {
  // User-defined parameters
  auto lowerLimit               = 90.0;
  auto upperLimit               = 110.0;
  auto buyGridSpacing           = 2.0;
  auto sellGridSpacing          = 2.5;
  auto quantityPerGrid          = 10;
  const std::string stockSymbol = "AAPL";
  auto initialCash              = 10000.0;

  // Initialize account
  Account account(initialCash);

  // Initialize strategy
  GridStrategy gridStrategy(lowerLimit, upperLimit, buyGridSpacing,
                            sellGridSpacing, quantityPerGrid, stockSymbol,
                            account);

  // Initialize market simulator
  MarketSimulator market;

  // Simulate market updates
  auto simulatedPrices = {95.0, 92.0, 89.0, 91.0, 93.0, 108.0, 112.0};
  for (auto price : simulatedPrices) {
    market.updatePrice(price);
    gridStrategy.run(market);
  }

  return 0;
}
