#ifndef MUTRADEAPI_QUOTES_H
#define MUTRADEAPI_QUOTES_H

#include "types.h"
//#include "../shared/commands.h"
#include <cstring>


namespace mutrade {

  
  class Quote
  {
    static const UChar MAX_LOOKUP_LEVEL = 10;
  
    public:
      Quote();
      //Quote(CMD::MDQuote&);
      Quote(const Quote&);
      Quote& operator= (const Quote &q);

      void setSymbolId (Int64 val) { _symbolId = val; }
      void setNummberOfTrades (Int64 val) { _numberOfTrades = val; }
      void setVolume (Int64 val) { _volume = val; }
      void setValue (Int64 val) { _value = val; }
      void setLastTradePrice (Int64 val) { _lastTradePrice = val; }
      void setLastTradeQty (Int64 val) { _lastTradeQty = val; }
      void setOpenPrice (Int64 val) { _openPrice = val; }
      void setClosePrice (Int64 val) { _closePrice = val; }
      void setHighPrice (Int64 val) { _highPrice = val; }
      void setLowPrice (Int64 val) { _lowPrice = val; }
      void setTotalBidQty (Int64 val) { _totalBidQty = val; }
      void setTotalAskQty (Int64 val) { _totalAskQty = val; }
      void setLowerCktLimit (Int64 val) { _lowerCktLimit = val; }
      void setUpperCktLimit (Int64 val) { _upperCktLimit = val; }
      void setDepth (UChar val) { _depth = val; }
      void setBidPrice ( Int64 val[]) { memcpy(_bidPrice, val, _depth * sizeof(Int64)); }
      void setBidQty (Int64 val[]) { memcpy(_bidQty, val, _depth * sizeof(Int64)); }
      void setAskPrice ( Int64 val[]) { memcpy(_askPrice, val, _depth * sizeof(Int64)); }
      void setAskQty (Int64 val[]) { memcpy(_askQty, val, _depth * sizeof(Int64)); }
      
      Int64 getSymbolId () const { return _symbolId; }
      Int64 getNumberOfTrades ()  const { return _numberOfTrades; }
      Int64 getVolume ()  const { return _volume; }
      Int64 getValue ()  const { return _value; }
      Int64 getLastTradePrice ()  const { return _lastTradePrice; }
      Int64 getLastTradeQty ()  const { return _lastTradeQty; }
      Int64 getOpenPrice ()  const { return _openPrice; }
      Int64 getClosePrice ()  const { return _closePrice; }
      Int64 getHighPrice ()  const { return _highPrice; }
      Int64 getLowPrice ()  const { return _lowPrice; }
      Int64 getTotalBidQty ()  const { return _totalBidQty; }
      Int64 getTotalAskQty ()  const { return _totalAskQty; }
      Int64 getLowerCktLimit () const  { return _lowerCktLimit; }
      Int64 getUpperCktLimit () const  { return _upperCktLimit; }
      UChar getDepth () const  { return _depth; }
      Int64* getBidPrice ( ) { return _bidPrice; }
      Int64* getBidQty () { return _bidQty; }
      Int64* getAskPrice ( ) { return _askPrice; }
      Int64* getAskQty ( ) { return _askQty; } 

    private:
      Int64         _symbolId;
      Int64         _numberOfTrades;
      Int64         _volume;
      Int64         _value;
      Int64         _lastTradePrice;
      Int64         _lastTradeQty;
      Int64         _openPrice;
      Int64         _closePrice;
      Int64         _highPrice;
      Int64         _lowPrice;
      Int64         _totalBidQty;
      Int64         _totalAskQty;
      Int64         _lowerCktLimit;
      Int64         _upperCktLimit;
      UChar         _depth;
      Int64         _bidPrice[MAX_LOOKUP_LEVEL];
      Int64         _bidQty[MAX_LOOKUP_LEVEL];
      Int64         _askPrice[MAX_LOOKUP_LEVEL];
      Int64         _askQty[MAX_LOOKUP_LEVEL];
      
  };

}

#endif
