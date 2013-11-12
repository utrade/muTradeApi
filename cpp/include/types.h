/*
 * (C) 2013 uTrade Solutions Pvt Ltd. All Rights reserved.
 */

#ifndef MUTRADEAPI_INCLUDE_TYPES_H
#define MUTRADEAPI_INCLUDE_TYPES_H

#include <iostream>
#include <string>
#include <stdint.h>
#include <sstream>

namespace mutrade
{

  namespace RSP
  {
    class OrderConfirmation;
  }

  /* typedef unsigned long    SymbolId; */
  typedef int16_t          Int16;
  typedef int32_t          Int32;
  typedef uint32_t         UInt32;
  typedef int64_t          Int64;
  typedef uint64_t        UInt64;
  typedef unsigned char    UChar;
  typedef std::string      String;
  typedef bool             Boolean;

  /**
   * \brief Side of the trade (Buy/Sell)
   *
   * You can use Side_BUY, Side_BID interchangeably.
   */
  enum Side
  {
    Side_BUY = 0,
    Side_SELL = 1,
    Side_BID = 0,
    Side_ASK = 1
  };

   /**
   * \brief Instrument Type of the trade (Buy/Sell)
   *
   * You can use Side_BUY, Side_BID interchangeably.
   */
  enum InstrumentType
  {
    InstrumentType_STOCK,
    InstrumentType_FUTURE,
    InstrumentType_OPTION
  };

  /**
   * \brief Option Type  (Call/Put)
   *
   * You can use OptionType_PUT, OptionType_CALL.
   */
  enum OptionType
  {
    OptionType_PUT,
    OptionType_CALL
  };

    /**
   * \brief Time In Force  (DAY/IOC)
   *
   * You can use TimeInForce_DAY, TimeInForce_IOC  interchangeably.
   */
  enum TimeInForce
  {
    TimeInForce_DAY,
    TimeInForce_IOC,
    TimeInForce_MAX
  };

   /**
   * \brief Order Type 
   *
   * You can use OrderType_LIMIT, OrderType_MARKET, OrderType_STOP_LIMIT 
   * and OrderType_STOP, while placing oredr.
   */
  enum OrderType {
    OrderType_LIMIT,
    OrderType_MARKET,
    OrderType_STOP_LIMIT,
    OrderType_STOP,
    OrderType_MAX
  };

   /**
   * \brief Transaction Type 
   *
   * You can use TransactionType_NEW, TransactionType_MODIFY
   * and TransactionType_CANCEL while placing oredr.
   */
  enum TransactionType
  {
    TransactionType_NEW,
    TransactionType_MODIFY,
    TransactionType_CANCEL,
    TransactionType_MAX
  };

   /**
   * \brief Order Status 
   */
  enum OrderStatus
  {
    OrderStatus_PENDING,
    OrderStatus_CONFIRMED,
    OrderStatus_FILLED, // Same for partial fill and fill
    OrderStatus_CANCELED,
    OrderStatus_REPLACED,
    OrderStatus_NEW_REJECTED,
    OrderStatus_CANCEL_REJECTED,
    OrderStatus_REPLACE_REJECTED,
    OrderStatus_FROZEN,
    OrderStatus_MARKET_TO_LIMIT,
    OrderStatus_TRIGGERED,
    OrderStatus_PARTIALLY_FILLED,
    OrderStatus_CANCELED_OF_IOC,
    OrderStatus_RMS_REJECT, // = 13
    OrderStatus_MAX
  };
  
   /**
   * \brief Order Mode
   */
  enum OrderMode
  {
    OrderMode_BUY = 0,
    OrderMode_SELL = 1,
    OrderMode_MAX
  };

   /**
   * \brief Response Type
   * Internally used by API.
   */
  enum ResponseType
  {
    ResponseType_SUCCESS,
    ResponseType_FAILURE,
    ResponseType_TERMINATE_SUCCESS,
    ResponseType_TERMINATE_FAILURE,
    ResponseType_TERMINATE_SQUAREOFF_SUCCESS,
    ResponseType_TERMINATE_SQUAREOFF_FAILURE,
    ResponseType_PAUSE_STRATEGY_SUCCESS,
    ResponseType_PAUSE_STRATEGY_FAILURE,
    ResponseType_STRATEGY_RUNNING, // = 8
    ResponseType_TERMINATE_SPREAD_BREACHED,
    ResponseType_ALGO_NOT_ALLOWED,
    ResponseType_MAX
  };

  String longToString(Int64 num);

  class ExecutionReport
  {
  public:
    ExecutionReport();
    ExecutionReport(const char* buf);
    ExecutionReport(RSP::OrderConfirmation& confirmation);
    void initialize();

    Int64 getClOrderId() const { return _clOrderId; }
    Int64 getExchangeOrderId() const { return _exchangeOrderId; }
    Int64 getSymbolId() const { return _symbolId; }
    Int32 getLastFillQuantity() const { return _lastFillQuantity; }
    Int32 getLastFillPrice() const { return _lastFillPrice; }
    Int32 getExchangeEntryTime() const { return _exchangeEntryTime; }
    Int32 getExchangeModifyTime() const { return _exchangeModifyTime; }
    Int32 getStrategyId() const { return _strategyId; }
    Int32 getClientId() const { return _clientId; }
    Int32 getLimitPrice() const { return _limitPrice; }
    UChar getOrderStatus() const { return _orderStatus; }
    UChar getOrderMode() const { return _orderMode; }
    Int32 getOrderQuantity() const { return _orderQuantity; }
    Int32 getOrderPrice() const { return _orderPrice; }
    Int32 getIOCCanceledQuantity() const { return _iocCanceledQuantity; }
    Int64 getOriginalClOrderId() const { return _originalClOrderId; }
    Int64 getConfirmationTimeSeconds() const
      { return _confirmationTimeSeconds; }
    Int64 getConfirmationTimeMicroSeconds() const
      { return _confirmationTimeMicroSeconds; }
    UChar getIsOffline() const {return _isOffline; }
    Int64 getSequenceNumber() const {return _sequenceNumber; }
    Int64 getTradeId() const {return _tradeId; }
    Int32 getErrorCode() const {return _errorCode; }
    Int32 getReasonText() const {return _reasonText; }
    UChar getUnknownOrder() const {return _unknownOrder; }
    String getInstrumentName() const { return _instrumentName; }

    void setClOrderId(Int64 clOrderId)
      { _clOrderId = clOrderId; }
    void setExchangeOrderId(Int64 exchangeOrderId)
      { _exchangeOrderId = exchangeOrderId; }
    void setSymbolId(Int64 symbolId)
      { _symbolId = symbolId; }
    void setLastFillQuantity(Int32 qty)
      { _lastFillQuantity = qty; }
    void setLastFillPrice(Int32 price)
      { _lastFillPrice = price; }
    void setExchangeEntryTime(Int32 exchangeEntryTime)
      { _exchangeEntryTime = exchangeEntryTime; }
    void setExchangeModifyTime(Int32 exchangeModifyTime)
      { _exchangeModifyTime = exchangeModifyTime; }
    void setStrategyId(Int32 strategyId)
      { _strategyId = strategyId; }
    void setClientId(Int32 clientId)
      { _clientId = clientId; }
    void setLimitPrice(Int32 limitPrice)
      { _limitPrice = limitPrice; }
    void setOrderStatus(UChar orderStatus)
      { _orderStatus = orderStatus; }
    void setOrderMode(UChar orderMode)
      { _orderMode = orderMode; }
    void setOrderQuantity(Int32 quantity)
      { _orderQuantity = quantity; }
    void setOrderPrice(Int32 price)
      { _orderPrice = price; }
    void setIOCCanceledQuantity(Int32 quantity)
      { _iocCanceledQuantity = quantity; }
    void setOriginalClOrderId(Int64 originalClOrderId)
      { _originalClOrderId = originalClOrderId; }
    void setConfirmationTimeSeconds(Int64 seconds)
      { _confirmationTimeSeconds = seconds; }
    void setConfirmationTimeMicroSeconds(Int64 microSeconds)
      { _confirmationTimeMicroSeconds = microSeconds; }
    void setIsOffline(UChar isOffline)
      { _isOffline = isOffline; }
    void setSequenceNumber(Int64 sequenceNumber)
      { _sequenceNumber = sequenceNumber; }
    void setTradeId(Int64 tradeId)
      { _tradeId = tradeId; }
    void setErrorCode(Int32 errorCode)
      { _errorCode = errorCode; }
    void setReasonText(Int32 reasonText)
      { _reasonText = reasonText; }
    void setUnknownOrder(UChar unknownOrder)
      { _unknownOrder = unknownOrder; }
    void setInstrumentName(String instrumentName)
      { _instrumentName = instrumentName; }

    void dump();
    void dumpCSV(std::ofstream &csvFile);
    int serialize(char* buf);

  private:
    Int64 _clOrderId;
    Int64 _exchangeOrderId;
    Int64 _symbolId;
    Int32 _lastFillQuantity;
    Int32 _lastFillPrice;
    Int32 _exchangeEntryTime;
    Int32 _exchangeModifyTime;
    Int32 _strategyId;
    Int32 _clientId;
    // Limit price will come in Market to Limit order conversion
    Int32 _limitPrice;
    UChar _orderStatus;
    UChar _orderMode;
    // Adding following two fields for market making
    Int32 _orderQuantity;
    Int32 _orderPrice;
    Int32 _iocCanceledQuantity;
    Int64 _originalClOrderId;
    Int64 _confirmationTimeSeconds;
    Int64 _confirmationTimeMicroSeconds;
    UChar _isOffline;
    Int64 _sequenceNumber;
    Int64 _tradeId;
    Int32 _errorCode;
    Int32 _reasonText;
    UChar _unknownOrder;
    String _instrumentName;

  };

  class ExecutionResponse
  {
    public :
      ExecutionResponse();
      ExecutionResponse(const char* buf); 
      void dump();

      UInt64 getClOrderId() const { return _clOrderId; }
      UChar getTransactionType() const { return _transactionType; }
      UChar getResponseType() const { return _responseType; }
      UInt32 getTokenId() const { return _tokenId; }

      void setClOrderId(UInt64 clOrderId) { _clOrderId = clOrderId; }
      void setTransactionType(UChar transactionType) { _transactionType = transactionType; }
      void setResponseType(UChar responseType) { _responseType = responseType; }
      void setTokenId(UInt32 val) { _tokenId = val; }

      
    private:
      UInt64  _clOrderId;
      UChar  _transactionType;
      UChar  _responseType;
      UInt32 _tokenId;
  };

}
#endif
