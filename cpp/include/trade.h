#ifndef MUTRADEAPI_TRADE_H
#define MUTRADEAPI_TRADE_H

#include "types.h"
#include <cstring>
#include "instrument.h"

namespace mutrade {

  namespace RSP
  {
     class OrderConfirmation;
  }


  class Trade {

  public:
    Trade(Instrument &);
    Trade(const Trade&);
    void initialize();

    Instrument      getInstrument() { return _instrument; }
    String          getTradeId() { return _tradeId; }
    Int64           getClOrdId() { return _clOrdId; }
    Int64           getOrigClOrdId() { return _origClOrdId; }
    String          getExchangeOrderId() { return _exchangeOrderId; }
    Side            getOrderMode() { return _orderMode; }
    Int32           getFilledQuantity() { return _filledQuantity; }
    Int32           getFilledPrice() { return _filledPrice; }
    OrderType       getOrderType() { return _orderType; }
    Int32           getTradeTime() {return _tradeTime; }
    
    void setInstrument(Instrument val) { _instrument = val; }
    void setTradeId(String val) { _tradeId = val; }
    void setClOrdId(Int64 val) { _clOrdId = val; }
    void setOrigClOrdId(Int64 val) { _origClOrdId = val; }
    void setExchangeOrderId(String val) { _exchangeOrderId = val; }
    void setOrderMode(Side val) { 
      _orderMode = val; }
    void setFilledQuantity(Int32 val) { _filledQuantity = val; }
    void setFilledPrice(Int32 val) { _filledPrice = val; }
    void setOrderType(OrderType val) { _orderType = val; }
    void setTradeTime(Int32 val) { _tradeTime = val; }

    void handleConfirmations(RSP::OrderConfirmation* confirmation);

  private:
    Instrument      _instrument;
    String          _tradeId;
    Int64           _clOrdId;
    Int64           _origClOrdId;
    String          _exchangeOrderId;// set as UNSIGNED_LONG in SingleOrder
    Side            _orderMode;
    Int32           _filledQuantity;
    Int32           _filledPrice;
    OrderType       _orderType;
    Int32           _tradeTime;
  };

} // namespace mutrade

#endif
