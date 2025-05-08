#include "OrderCallback.hpp"

OrderCallback::OrderCallback()
    : m_type(OrderCallBackTypes::UNKNOWN), m_inbound_order(nullptr),
      m_matched_order(nullptr), m_quantity(0), m_price(0),
      m_order_fill_flags(0), m_reject_reason(nullptr) {}

OrderCallback OrderCallback::accept(const OrderPtr &order) {
  OrderCallback callback;

  callback.m_type = OrderCallBackTypes::ORDER_ACCEPT;
  callback.m_inbound_order = order;

  return callback;
}

OrderCallback OrderCallback::accept_stop(const OrderPtr &order) {
  OrderCallback callback;

  callback.m_type = OrderCallBackTypes::ORDER_ACCEPT_STOP;
  callback.m_inbound_order = order;

  return callback;
}

OrderCallback OrderCallback::trigger_stop(const OrderPtr &order) {
  OrderCallback callback;

  callback.m_type = OrderCallBackTypes::ORDER_TRIGGER_STOP;
  callback.m_inbound_order = order;

  return callback;
}

OrderCallback OrderCallback::reject(const OrderPtr &order, const char *reason) {
  OrderCallback callback;

  callback.m_type = OrderCallBackTypes::ORDER_REJECT;
  callback.m_inbound_order = order;
  callback.m_reject_reason = reason;

  return callback;
}

OrderCallback OrderCallback::fill(const OrderPtr &inbound_order,
                                  const OrderPtr &matched_order,
                                  const Quantity &quantity_filled,
                                  const Price &fill_price,
                                  OrderFillFlags fill_flags) {
  OrderCallback callback;

  callback.m_type = OrderCallBackTypes::ORDER_FILL;
  callback.m_inbound_order = inbound_order;
  callback.m_matched_order = matched_order;
  callback.m_quantity = quantity_filled;
  callback.m_price = fill_price;
  callback.m_order_fill_flags = fill_flags;

  return callback;
}

OrderCallback OrderCallback::cancel(const OrderPtr &order,
                                    const Quantity &quantity_in_market) {
  OrderCallback callback;

  callback.m_type = OrderCallBackTypes::ORDER_CANCEL;
  callback.m_inbound_order = order;
  callback.m_quantity = quantity_in_market;

  return callback;
}

OrderCallback OrderCallback::cancel_stop(const OrderPtr &order) {
  OrderCallback callback;

  callback.m_type = OrderCallBackTypes::ORDER_CANCEL_STOP;
  callback.m_inbound_order = order;

  return callback;
}

OrderCallback OrderCallback::cancel_reject(const OrderPtr &order,
                                           const char *reason) {
  OrderCallback callback;

  callback.m_type = OrderCallBackTypes::ORDER_CANCEL_REJECT;
  callback.m_inbound_order = order;
  callback.m_reject_reason = reason;

  return callback;
}