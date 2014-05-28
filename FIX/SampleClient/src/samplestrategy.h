#ifndef SAMPLESTRATEGY_H
#define SAMPLESTRATEGY_H
#include <map>
#include <set>
#include <string>
#include "application.h"
#include <boost/enable_shared_from_this.hpp>
enum AlgoType{
    AlgoType_SampleBidding =0,
    AlgoType_FutFut,
    AlgoType_MAX
};


const int MARKET_DEPTH_SIZE = 5;
class AbstractStrategy :
public boost::enable_shared_from_this<AbstractStrategy>{
private:
    AlgoType _type;
protected:
    bool _isAlive;
public:

    struct MarketData
    {
       long _price;
       long _qty;
    };
    virtual ~AbstractStrategy()
    {

    }
    AbstractStrategy(const AlgoType &type):
        _type(type),
        _isAlive(true)
    {}
    AlgoType getType()
    {
        return _type;
    }
    virtual void onMarketData(const FIX42::MarketDataSnapshotFullRefresh& mktDataSnapShot){}
    virtual void onExecutionReport(const FIX42::ExecutionReport& executionReport){}
    virtual void subscribeMarketDataFeed(){}
    void registerClOrderId(const std::string &orderId);
    void registerMktData(const FIX42::MarketDataRequest &marketDataRequest);

    void getMarketData(const FIX42::MarketDataSnapshotFullRefresh& fixMarketData,
        MarketData *marketData,const int &size,const FIX::Side &watchSide);

    const bool & isAlive()
    {
      return _isAlive;
    }

    boost::shared_ptr<AbstractStrategy> getSharedptr()
    {
      return shared_from_this();
    }

};
class StrategyDriver{
    typedef std::map<std::string, boost::shared_ptr<AbstractStrategy> > ClOrderIdStrategyMap;
    typedef std::map<std::string,std::set<boost::shared_ptr<AbstractStrategy> > > MktDataSgMap;
    ClOrderIdStrategyMap _clOrderIdStrategyMap;
    MktDataSgMap _mktDataSgMap;
    Application *_appInstance;
    typedef ClOrderIdStrategyMap::const_iterator ClOrderIdStrategyMapIter;
    typedef std::set<boost::shared_ptr<AbstractStrategy> >::const_iterator MktDataSgMapIter;

public:
    static StrategyDriver* getInstance(){
        static StrategyDriver instance;
        return &instance;
    }
    void registerStrategyClOrderId(const std::string &orderId, AbstractStrategy *strategy)
    {
        _clOrderIdStrategyMap[orderId] = strategy->getSharedptr() ;
    }
    void registerStrategyForMktData(AbstractStrategy *strategy,
        const FIX42::MarketDataRequest &marketDataRequest)
    {
      std::string marketDataUniqueSignature = Application::
        getMarketDataUniqueSignature(marketDataRequest);
      _mktDataSgMap[marketDataUniqueSignature].insert(strategy->getSharedptr());
    }
    void unRegisterStrategyForMktData(AbstractStrategy *strategy,
        const FIX42::MarketDataRequest &marketDataRequest)
    {
      std::string marketDataUniqueSignature = Application::
        getMarketDataUniqueSignature(marketDataRequest);
      _mktDataSgMap[marketDataUniqueSignature].erase(strategy->getSharedptr());
    }

    void onExecutionReport( const FIX42::ExecutionReport& report);
    void onMarketDataSnampshot( const FIX42::MarketDataSnapshotFullRefresh& report);

    std::string genClOrderId();


    void removeOnCancelFill(const std::string &clOrdrId)
    {
      _clOrderIdStrategyMap.erase(clOrdrId);
    }

};


class SampleStrategy : public AbstractStrategy
{
public:
    SampleStrategy();
};

#endif // SAMPLESTRATEGY_H
