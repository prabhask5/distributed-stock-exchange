#pragma once

#include "OrderCallbackTypes.hpp"
#include "OrderTypes.hpp"

// This is a helper class to manage all the "callback" functions that arise
// during the state management of orders in the order book. A "callback" is
// logic that is executed in reaction to a state change in the order book. The
// reason this callback class is needed is to organize all the callback actions
// into a list that we run at once within one function call during the execution
// of the order book logic. The reason this is needed is because callbacks can
// lead to other callbacks being run, so running each callback synchronously can
// lead to state that is difficult to manage and keep track of. This is
// particularly useful here because of the sheer amount of state changes that
// occur per second in this high frequency system.
class OrderCallback {
public:
  OrderCallback();

  static OrderCallback accept(const OrderPtr &order);

  static OrderCallback accept_stop(const OrderPtr &order);

  static OrderCallback trigger_stop(const OrderPtr &order);

  static OrderCallback reject(const OrderPtr &order, const char *reason);

  static OrderCallback fill(const OrderPtr &inbound_order,
                            const OrderPtr &matched_order,
                            const Quantity &quantity_filled,
                            const Price &fill_price, OrderFillFlags fill_flags);

  static OrderCallback cancel(const OrderPtr &order,
                              const Quantity &quantity_in_market);

  static OrderCallback cancel_stop(const OrderPtr &order);

  static OrderCallback cancel_reject(const OrderPtr &order, const char *reason);

public:
  OrderCallBackTypes m_type;
  OrderPtr m_inbound_order;
  OrderPtr m_matched_order;
  Quantity m_quantity;
  Price m_price;
  uint8_t m_order_fill_flags;
  const char *m_reject_reason;
};