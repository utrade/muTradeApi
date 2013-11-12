#ifndef MUTRADEAPI_ORDER_H
#define MUTRADEAPI_ORDER_H

#include "types.h"
#include <cstring>

namespace mutrade {

  class Order {

  public:
    Order() { initialize(); }

    Int32           getClientId()const { return _clientId; }
    TransactionType getTransactionType()const { return _transactionType; }
    Int64           getClOrdId()const { return _clOrdId; }
    Int64           getOrigClOrdId()const { return _origClOrdId; }
    String          getExchangeOrderId()const { return _exchangeOrderId; }
    String          getSymbol()const { return _symbol; }
    Side            getOrderMode()const { return _orderMode; }
    Int32           getQuantity()const { return _quantity; }
    Int32           getDisclosedQuantity()const { return _disclosedQuantity; }
    Int32           getFilledQuantity()const { return _filledQuantity; }
    Int32           getOldQuantity()const { return _oldQuantity; }
    Int32           getPrice()const { return _price; }
    Int32           getStopPrice()const { return _stopPrice; }
    UChar           getSecurityType()const { return _securityType; }
    TimeInForce     getOrderValidity()const { return _orderValidity; }
    OrderType       getOrderType()const { return _orderType; }
    OrderStatus     getOrderStatus()const { return _orderStatus; }
    Int32           getExchangeEntryTime()const { return _exchangeEntryTime; }
    Int32           getExchangeModifyTime()const { return _exchangeModifyTime; }

    void setClientId(Int32 val) { _clientId = val; }
    void setTransactionType(TransactionType val) { _transactionType = val; }
    void setClOrdId(Int64 val) { _clOrdId = val; }
    void setOrigClOrdId(Int64 val) { _origClOrdId = val; }
    void setExchangeOrderId(String val) { _exchangeOrderId = val; }
    void setSymbol(String val) { _symbol = val; }
    void setOrderMode(Side val) { _orderMode = val; }
    void setQuantity(Int32 val) { _quantity = val; }
    void setDisclosedQuantity(Int32 val) { _disclosedQuantity = val; }
    void setFilledQuantity(Int32 val) { _filledQuantity = val; }
    void setOldQuantity(Int32 val) { _oldQuantity = val; }
    void setPrice(Int32 val) { _price = val; }
    void setStopPrice(Int32 val) { _stopPrice = val; }
    void setSecurityType(UChar val) { _securityType = val; }
    void setOrderValidity(TimeInForce val) { _orderValidity = val; }
    void setOrderType(OrderType val) { _orderType = val; }
    void setOrderStatus(OrderStatus val) { _orderStatus = val; }
    void setExchangeEntryTime(Int32 val) { _exchangeEntryTime = val; }
    void setExchangeModifyTime(Int32 val) { _exchangeModifyTime = val; }
    
    void initialize();
    void dumpOrder() const;
  private:
    Int32           _clientId;
    TransactionType _transactionType;
    Int64           _clOrdId;
    Int64           _origClOrdId;
    String          _exchangeOrderId;// set as UNSIGNED_LONG in SingleOrder
    String          _symbol;
    Side            _orderMode;
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
