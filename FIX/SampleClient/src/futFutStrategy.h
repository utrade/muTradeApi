#ifndef FUTFUTSTRATEGY_H
#define FUTFUTSTRATEGY_H
#include"samplestrategy.h"
#include <exception>
struct ParamsFileAbsent : public std::exception 
{
    const char* what() const throw()
    {
      return "ParamsFile does not exist!\n";
    }
};
struct MaturityFormatWrong : public std::exception 
{
    const char* what() const throw()
    {
      return "Maturity Date Format is ddmmyyyy !\n";
    }
};

struct OrderSideFormatWrong : public std::exception 
{
    const char* what() const throw()
    {
      return "0 Buy 1 Sell\n";
    }
};
struct LotSizeWrong : public std::exception 
{
    const char* what() const throw()
    {
      return "Lot Size cannot be zero or negative\n";
    }
};

class FutFutStrategy : public AbstractStrategy
{
  std::string _symbol;
  std::string _exchange;
  std::string _maturityFirstLeg;
  std::string _maturitySecondLeg;
  
  FIX42::MarketDataRequest _firstLegMarketDataRequest;
  FIX42::MarketDataRequest _secondLegMarketDataRequest;


  MarketData _secondLegMarketData[MARKET_DEPTH_SIZE];

  FIX::Side _firstLegOrderSide;
  FIX::Side _secondLegOrderSide;
  
  FIX::Side _firstLegWatchSide;
  FIX::Side _secondLegWatchSide;

  std::string _firstLegClOrderId;
  std::string _firstLegReplaceClOrderId;
  std::string _secondLegClOrderId;
  
  long _minSpread;
  long _lotSize;
  long _totalLots;
  long _totalQty;
  long _firstLegTradedQty;
  long _secondLegTradeQty;
  double _oldPrice;
  void (*_state)(FutFutStrategy &obj);

  void readOptionsfromFile(const std::string &filePath);
  void subscribeMarketDataFeedLeg(const std::string &maturity,
      FIX42::MarketDataRequest &saveRequest);
  void validateParameters();
  void onMarketData(const FIX42::MarketDataSnapshotFullRefresh& mktDataSnapShot);
  void setOrderSide(FIX::Side &side,
      FIX::Side &watchSide,const int &orderSide);


  public :
  void subscribeMarketDataFeed();
    FutFutStrategy();
    ~FutFutStrategy()
    {
      std::cout<<"Strategy Destroyed"<<"\n";
    }
    std::string getParametersDump();
    double getFirstLegPrice();
    
    static void placeFirstLegOrder(FutFutStrategy &obj);
    static void noAction(FutFutStrategy &obj);
    static void replaceOrder(FutFutStrategy &obj);
    
    void onExecutionReport( const FIX42::ExecutionReport& report);
    
    const std::string & getSymbol()
    {
      return _symbol;
    }
    const FIX::Side &getFirstLegOrderSide()
    {
      return _firstLegOrderSide;
    }
    const std::string &getExchange()
    {
      return _exchange;
    }
    const long & getLotSize()
    {
      return _lotSize;
    }
    const std::string & getMaturityFirstLeg()
    {
      return _maturityFirstLeg;
    }
    const std::string & getMaturitySecondLeg()
    {
      return _maturitySecondLeg;
    }

    void setFirstLegClordId(const std::string &clordId)
    {
      _firstLegClOrderId = clordId;
    }
    
    const std::string &getFirstLegClordId()
    {
      return _firstLegClOrderId;
    }
    
    const std::string &getFirstLegReplaceClordId()
    {
      return _firstLegReplaceClOrderId;
    }
    
    void placeSecondLegOrder();
    void cancelFirstLegOrder();
    void closeStrategy();

};
#endif
