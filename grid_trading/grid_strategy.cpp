#include "grid_strategy.h"

GridStrategy::GridStrategy(double lowerLimit, double upperLimit,
                           double buyGridSpacing, double sellGridSpacing,
                           int quantityPerGrid, const std::string& stockSymbol,
                           Account& account)
    : lowerLimit_(lowerLimit),
      upperLimit_(upperLimit),
      buyGridSpacing_(buyGridSpacing),
      sellGridSpacing_(sellGridSpacing),
      quantityPerGrid_(quantityPerGrid),
      stockSymbol_(stockSymbol),
      account_(account)  //
{
  initializeGrids();
}

void GridStrategy::initializeGrids()  //
{
  for (double buyPrice = lowerLimit_; buyPrice <= upperLimit_;
       buyPrice += buyGridSpacing_) {
    buyGrids_.emplace_back(Grid{buyPrice, quantityPerGrid_, false});
  }

  for (double sellPrice = lowerLimit_; sellPrice <= upperLimit_;
       sellPrice += sellGridSpacing_) {
    sellGrids_.emplace_back(Grid{sellPrice, quantityPerGrid_, false});
  }
}

void GridStrategy::run(MarketSimulator& market)  //
{
  double currentPrice = market.getCurrentPrice();

  // Execute buy orders
  for (auto& grid : buyGrids_) {
    if (!grid.executed && currentPrice <= grid.price) {
      if (account_.buy(grid.price, grid.quantity)) {
        grid.executed = true;
      }
    }
  }

  // Execute sell orders
  for (auto& grid : sellGrids_) {
    if (!grid.executed && currentPrice >= grid.price) {
      if (account_.sell(grid.price, grid.quantity)) {
        grid.executed = true;
      }
    }
  }
  //
}
