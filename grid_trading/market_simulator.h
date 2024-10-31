#ifndef MARKETSIMULATOR_H
#define MARKETSIMULATOR_H

// Market simulation
class MarketSimulator {
 public:
  MarketSimulator();
  double getCurrentPrice() { return currentPrice_; }

  void updatePrice(double newPrice) { currentPrice_ = newPrice; }

 private:
  double currentPrice_ = 100.0;  // Starting price
};

#endif  // MARKETSIMULATOR_H
