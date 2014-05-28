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


/**
 * @brief MARKET_DEPTH_SIZE
 */
const int MARKET_DEPTH_SIZE = 5;

/**
 * @brief The AbstractStrategy class
 */
class AbstractStrategy :
        public boost::enable_shared_from_this<AbstractStrategy>{
private:

    /**
     * @brief _type
     */
    AlgoType _type;
protected:

    /**
     * @brief _isAlive
     */
    bool _isAlive;
public:

    /**
     * @brief The MarketData struct
     */
    struct MarketData
    {
        long _price;
        long _qty;
    };

    /**
     * @brief ~AbstractStrategy
     */
    virtual ~AbstractStrategy()
    {

    }

    /**
     * @brief AbstractStrategy
     * @param type
     */
    AbstractStrategy(const AlgoType &type):
        _type(type),
        _isAlive(true)
    {}

    /**
     * @brief getType
     * @return
     */
    AlgoType getType()
    {
        return _type;
    }

    /**
     * @brief onMarketData
     * @param mktDataSnapShot
     */
    virtual void onMarketData(const FIX42::MarketDataSnapshotFullRefresh& mktDataSnapShot){}

    /**
     * @brief onExecutionReport
     * @param executionReport
     */
    virtual void onExecutionReport(const FIX42::ExecutionReport& executionReport){}

    /**
     * @brief subscribeMarketDataFeed
     */
    virtual void subscribeMarketDataFeed(){}

    /**
     * @brief registerClOrderId
     * @param orderId
     */
    void registerClOrderId(const std::string &orderId);

    /**
     * @brief registerMktData
     * @param marketDataRequest
     */
    void registerMktData(const FIX42::MarketDataRequest &marketDataRequest);

    /**
     * @brief getMarketData
     * @param fixMarketData
     * @param marketData
     * @param size
     * @param watchSide
     */
    void getMarketData(const FIX42::MarketDataSnapshotFullRefresh& fixMarketData,
                       MarketData *marketData,const int &size,const FIX::Side &watchSide);

    /**
     * @brief isAlive
     * @return
     */
    const bool & isAlive()
    {
        return _isAlive;
    }

    /**
     * @brief getSharedptr
     * @return
     */
    boost::shared_ptr<AbstractStrategy> getSharedptr()
    {
        return shared_from_this();
    }

};

/**
 * @brief The StrategyDriver class
 */
class StrategyDriver{
    /**
     * @brief ClOrderIdStrategyMap
     */
    typedef std::map<std::string, boost::shared_ptr<AbstractStrategy> > ClOrderIdStrategyMap;

    /**
     * @brief MktDataSgMap
     */
    typedef std::map<std::string,std::set<boost::shared_ptr<AbstractStrategy> > > MktDataSgMap;

    /**
     * @brief _clOrderIdStrategyMap
     */
    ClOrderIdStrategyMap _clOrderIdStrategyMap;

    /**
     * @brief _mktDataSgMap
     */
    MktDataSgMap _mktDataSgMap;

    /**
     * @brief _appInstance
     */
    Application *_appInstance;

    /**
     * @brief ClOrderIdStrategyMapIter
     */
    typedef ClOrderIdStrategyMap::const_iterator ClOrderIdStrategyMapIter;

    /**
     * @brief MktDataSgMapIter
     */
    typedef std::set<boost::shared_ptr<AbstractStrategy> >::const_iterator MktDataSgMapIter;

public:

    /**
     * @brief getInstance
     * @return
     */
    static StrategyDriver* getInstance(){
        static StrategyDriver instance;
        return &instance;
    }

    /**
     * @brief registerStrategyClOrderId
     * @param orderId
     * @param strategy
     */
    void registerStrategyClOrderId(const std::string &orderId, AbstractStrategy *strategy)
    {
        _clOrderIdStrategyMap[orderId] = strategy->getSharedptr() ;
    }

    /**
     * @brief registerStrategyForMktData
     * @param strategy
     * @param marketDataRequest
     */
    void registerStrategyForMktData(AbstractStrategy *strategy,
                                    const FIX42::MarketDataRequest &marketDataRequest)
    {
        std::string marketDataUniqueSignature = Application::
                getMarketDataUniqueSignature(marketDataRequest);
        _mktDataSgMap[marketDataUniqueSignature].insert(strategy->getSharedptr());
    }

    /**
     * @brief unRegisterStrategyForMktData
     * @param strategy
     * @param marketDataRequest
     */
    void unRegisterStrategyForMktData(AbstractStrategy *strategy,
                                      const FIX42::MarketDataRequest &marketDataRequest)
    {
        std::string marketDataUniqueSignature = Application::
                getMarketDataUniqueSignature(marketDataRequest);
        _mktDataSgMap[marketDataUniqueSignature].erase(strategy->getSharedptr());
    }

    /**
     * @brief onExecutionReport
     * @param report
     */
    void onExecutionReport( const FIX42::ExecutionReport& report);

    /**
     * @brief onMarketDataSnampshot
     * @param report
     */
    void onMarketDataSnampshot( const FIX42::MarketDataSnapshotFullRefresh& report);

    /**
     * @brief genClOrderId
     * @return
     */
    std::string genClOrderId();


    /**
     * @brief removeOnCancelFill
     * @param clOrdrId
     */
    void removeOnCancelFill(const std::string &clOrdrId)
    {
        _clOrderIdStrategyMap.erase(clOrdrId);
    }

};

/**
 * @brief The SampleStrategy class
 */
class SampleStrategy : public AbstractStrategy
{
public:
    /**
     * @brief SampleStrategy
     */
    SampleStrategy();
};

#endif // SAMPLESTRATEGY_H
