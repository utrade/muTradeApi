#ifndef MUTRADEAPI_ORDER_H
#define MUTRADEAPI_ORDER_H

#include "types.h"
#include <cstring>

namespace mutrade {
  /**
   * \anchor Order
   * \brief Order Class
   * 
   * User has to set the fields of this class while placing 
   * oder.(New/Modify/Cancel)
   */
  class Order {

  public:
    /**
     * @brief Client order Id.
     *
     * @return Client order Id.
     */
    Int64           getClOrdId() { return _clOrdId; }
    /**
     * @brief Trasnsaction Type.
     *
     * @return \ref TransactionType [New/Modify/Cancel]
     */
    TransactionType getTransactionType() { return _transactionType; }
    Int64           getOrigClOrdId() { return _origClOrdId; }
    /**
     * @brief Exchange Order Id.
     *
     * @return Exchange order Id.
     */
    String          getExchangeOrderId() { return _exchangeOrderId; }
    /**
     * @brief Instrument name.
     *
     * @return Instrument name.
     */
    String          getSymbol() { return _symbol; }
    /**
     * @brief Order Mode.
     *
     * @return \ref OrderMode 
     */
    OrderMode       getOrderMode() { return _orderMode; }
    /**
     * @brief Order Quantity.
     *
     * @return Order quantity.
     */
    Int32           getQuantity() { return _quantity; }
    Int32           getDisclosedQuantity() { return _disclosedQuantity; }
    /**
     * @brief Filled quantity.
     *
     * @return Filled qty.
     */
    Int32           getFilledQuantity() { return _filledQuantity; }
    Int32           getOldQuantity() { return _oldQuantity; }
    /**
     * @brief Order Price.
     *
     * @return Order Price.
     */
    Int32           getPrice() { return _price; }
    /**
     * @brief Stop Price.
     *
     * @return Stop Price.
     */
    Int32           getStopPrice() { return _stopPrice; }
    /**
     * @brief Instrument Type.
     *
     * @return InstrumentType_STOCK/InstrumentType_FUTURE/
     *          InstrumentType_OPTION.
     */
    UChar           getSecurityType() { return _securityType; }

    /**
     * @brief Time in force.
     *
     * @return TimeInForce_DAY/TimeInForce_IOC.
     */
    TimeInForce     getOrderValidity() { return _orderValidity; }
    /**
     * @brief Order Type.
     *
     * @return OrderType_LIMIT/OrderType_MARKET/OrderType_STOP_LIMIT.
     */
    OrderType       getOrderType() { return _orderType; }
    /**
     * @brief Order Status.
     *
     * @return \ref OrderStatus
     */
    OrderStatus     getOrderStatus() { return _orderStatus; }
    Int64           getExchangeEntryTime() { return _exchangeEntryTime; }
    Int64           getExchangeModifyTime() { return _exchangeModifyTime; }

    /**
     * @brief Set Client Order Id.
     *
     * @param val
     *
     */
    void setClOrdId(Int64 val) { _clOrdId = val; }
    /**
     * @brief Set Transaction Type.
     *
     * @param val 
     * User must update this field accordingly.
     * [New/Modify/Cancel]
     */
    void setTransactionType(TransactionType val) { _transactionType = val; }
    void setOrigClOrdId(Int64 val) { _origClOrdId = val; }
    void setExchangeOrderId(String val) { _exchangeOrderId = val; }
    /**
     * @brief Set Symbol.
     *
     * @param val Instrument Name.
     * User must set this field in case of New Order type.
     */
    void setSymbol(String val) { _symbol = val; }
    /**
     * @brief Set Order Mode.
     *
     * @param val \ref OrderMode
     * User must set this field while placing New Order. 
     */
    void setOrderMode(OrderMode val) { _orderMode = val; }
    /**
     * @brief Set Order Quantity. 
     *
     * @param val 
     * User must set this field for transaction type New/Modify. 
     */
    void setQuantity(Int32 val) { _quantity = val; }
    void setDisclosedQuantity(Int32 val) { _disclosedQuantity = val; }
    void setFilledQuantity(Int32 val) { _filledQuantity = val; }
    void setOldQuantity(Int32 val) { _oldQuantity = val; }
    /**
     * @brief Set Order Price. 
     *
     * @param val 
     * User must set this field for transaction type New/Modify. 
     */
    void setPrice(Int32 val) { _price = val; }
    void setStopPrice(Int32 val) { _stopPrice = val; }
    void setSecurityType(UChar val) { _securityType = val; }
    void setOrderValidity(TimeInForce val) { _orderValidity = val; }
    /**
     * @brief Set Order Type. 
     *
     * @param val 
     * User must set this field for transaction type New/Modify. 
     */
    void setOrderType(OrderType val) { _orderType = val; }
    /**
     * @brief Set Order Status. 
     *
     * @param val 
     * Interanally updated by API. 
     */
    void setOrderStatus(OrderStatus val) { _orderStatus = val; }
    void setExchangeEntryTime(Int32 val) { _exchangeEntryTime = val; }
    void setExchangeModifyTime(Int32 val) { _exchangeModifyTime = val; }

  private:
    Int64           _clOrdId;
    TransactionType _transactionType;
    Int64           _origClOrdId;
    String          _exchangeOrderId;
    String          _symbol;
    OrderMode       _orderMode;
    Int32           _quantity;
    Int32           _disclosedQuantity;
    Int32           _filledQuantity;
    Int32           _oldQuantity;
    Int32           _price;
    Int32           _stopPrice;
    UChar           _securityType;
    TimeInForce     _orderValidity;
    OrderType       _orderType;
    OrderStatus     _orderStatus;
    Int32           _exchangeEntryTime;
    Int32           _exchangeModifyTime;
  };

} // namespace mutrade

#endif
