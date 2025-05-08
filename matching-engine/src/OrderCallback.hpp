#pragma once

#include "Order.hpp"

enum OrderCallBackTypes {
  UNKNOWN,
  ORDER_ACCEPT,
  ORDER_ACCEPT_STOP,
  ORDER_TRIGGER_STOP,
  ORDER_REJECT,
  ORDER_FILL,
  ORDER_CANCEL,
  ORDER_CANCEL_STOP,
  ORDER_CANCEL_REJECT
};

enum OrderFillFlags {
  NEITHER_FILLED,
  INBOUND_FILLED,
  MATCHED_FILLED,
  BOTH_FILLED
};

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