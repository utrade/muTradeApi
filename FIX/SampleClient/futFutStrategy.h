#ifndef FUTFUTSTRATEGY_H
#define FUTFUTSTRATEGY_H
#include"samplestrategy.h"
#include <exception>

/**
 * @brief The ParamsFileAbsent struct
 */
struct ParamsFileAbsent : public std::exception 
{
    const char* what() const throw()
    {
        return "ParamsFile does not exist!\n";
    }
};

/**
 * @brief The MaturityFormatWrong struct
 */
struct MaturityFormatWrong : public std::exception 
{
    const char* what() const throw()
    {
        return "Maturity Date Format is ddmmyyyy !\n";
    }
};

/**
 * @brief The OrderSideFormatWrong struct
 */
struct OrderSideFormatWrong : public std::exception 
{
    const char* what() const throw()
    {
        return "0 Buy 1 Sell\n";
    }
};

/**
 * @brief The LotSizeWrong struct
 */
struct LotSizeWrong : public std::exception 
{
    const char* what() const throw()
    {
        return "Lot Size cannot be zero or negative\n";
    }
};

/**
 * @brief The FutFutStrategy class
 */
class FutFutStrategy : public AbstractStrategy
{

    /**
     * @brief _symbol
     */
    std::string _symbol;

    /**
     * @brief _exchange
     */
    std::string _exchange;

    /**
     * @brief _maturityFirstLeg
     */
    std::string _maturityFirstLeg;

    /**
     * @brief _maturitySecondLeg
     */
    std::string _maturitySecondLeg;

    /**
     * @brief _firstLegMarketDataRequest
     */
    FIX42::MarketDataRequest _firstLegMarketDataRequest;

    /**
     * @brief _secondLegMarketDataRequest
     */
    FIX42::MarketDataRequest _secondLegMarketDataRequest;

    /**
     * @brief _secondLegMarketData
     */
    MarketData _secondLegMarketData[MARKET_DEPTH_SIZE];

    /**
     * @brief _firstLegOrderSide
     */
    FIX::Side _firstLegOrderSide;

    /**
     * @brief _secondLegOrderSide
     */
    FIX::Side _secondLegOrderSide;

    /**
     * @brief _firstLegWatchSide
     */
    FIX::Side _firstLegWatchSide;

    /**
     * @brief _secondLegWatchSide
     */
    FIX::Side _secondLegWatchSide;

    /**
     * @brief _firstLegClOrderId
     */
    std::string _firstLegClOrderId;

    /**
     * @brief _firstLegReplaceClOrderId
     */
    std::string _firstLegReplaceClOrderId;

    /**
     * @brief _secondLegClOrderId
     */
    std::string _secondLegClOrderId;

    /**
     * @brief _minSpread
     */
    long _minSpread;

    /**
     * @brief _lotSize
     */
    long _lotSize;

    /**
     * @brief _totalLots
     */
    long _totalLots;

    /**
     * @brief _totalQty
     */
    long _totalQty;

    /**
     * @brief _firstLegTradedQty
     */
    long _firstLegTradedQty;

    /**
     * @brief _secondLegTradeQty
     */
    long _secondLegTradeQty;

    /**
     * @brief _oldPrice
     */
    double _oldPrice;

    /**
     *
     */
    void (*_state)(FutFutStrategy &obj);

    /**
     * @brief readOptionsfromFile
     * @param filePath
     */
    void readOptionsfromFile(const std::string &filePath);

    /**
     * @brief subscribeMarketDataFeedLeg
     * @param maturity
     * @param saveRequest
     */
    void subscribeMarketDataFeedLeg(const std::string &maturity,
                                    FIX42::MarketDataRequest &saveRequest);

    /**
     * @brief validateParameters
     */
    void validateParameters();

    /**
     * @brief onMarketData
     * @param mktDataSnapShot
     */
    void onMarketData(const FIX42::MarketDataSnapshotFullRefresh& mktDataSnapShot);

    /**
     * @brief setOrderSide
     * @param side
     * @param watchSide
     * @param orderSide
     */
    void setOrderSide(FIX::Side &side,
                      FIX::Side &watchSide,const int &orderSide);

public :

    /**
     * @brief subscribeMarketDataFeed
     */
    void subscribeMarketDataFeed();

    /**
     * @brief FutFutStrategy
     */
    FutFutStrategy();


    ~FutFutStrategy()
    {
        std::cout<<"Strategy Destroyed"<<"\n";
    }

    /**
     * @brief getParametersDump
     * @return
     */
    std::string getParametersDump();

    /**
     * @brief getFirstLegPrice
     * @return
     */
    double getFirstLegPrice();
    
    /**
     * @brief placeFirstLegOrder
     * @param obj
     */
    static void placeFirstLegOrder(FutFutStrategy &obj);

    /**
     * @brief noAction
     * @param obj
     */
    static void noAction(FutFutStrategy &obj);

    /**
     * @brief replaceOrder
     * @param obj
     */
    static void replaceOrder(FutFutStrategy &obj);
    
    /**
     * @brief onExecutionReport
     * @param report
     */
    void onExecutionReport( const FIX42::ExecutionReport& report);
    
    /**
     * @brief getSymbol
     * @return
     */
    const std::string & getSymbol()
    {
        return _symbol;
    }

    /**
     * @brief getFirstLegOrderSide
     * @return
     */
    const FIX::Side &getFirstLegOrderSide()
    {
        return _firstLegOrderSide;
    }

    /**
     * @brief getExchange
     * @return
     */
    const std::string &getExchange()
    {
        return _exchange;
    }

    /**
     * @brief getLotSize
     * @return
     */
    const long & getLotSize()
    {
        return _lotSize;
    }

    /**
     * @brief getMaturityFirstLeg
     * @return
     */
    const std::string & getMaturityFirstLeg()
    {
        return _maturityFirstLeg;
    }

    /**
     * @brief getMaturitySecondLeg
     * @return
     */
    const std::string & getMaturitySecondLeg()
    {
        return _maturitySecondLeg;
    }

    /**
     * @brief setFirstLegClordId
     * @param clordId
     */
    void setFirstLegClordId(const std::string &clordId)
    {
        _firstLegClOrderId = clordId;
    }
    
    /**
     * @brief getFirstLegClordId
     * @return
     */
    const std::string &getFirstLegClordId()
    {
        return _firstLegClOrderId;
    }
    
    /**
     * @brief getFirstLegReplaceClordId
     * @return
     */
    const std::string &getFirstLegReplaceClordId()
    {
        return _firstLegReplaceClOrderId;
    }
    
    /**
     * @brief placeSecondLegOrder
     */
    void placeSecondLegOrder();

    /**
     * @brief cancelFirstLegOrder
     */
    void cancelFirstLegOrder();

    /**
     * @brief closeStrategy
     */
    void closeStrategy();

};
#endif
