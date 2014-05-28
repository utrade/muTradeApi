#include "futFutStrategy.h"
#include <boost/program_options.hpp>
#include <fstream>

/**
 * @brief FutFutStrategy::FutFutStrategy
 */
FutFutStrategy::FutFutStrategy()
    :AbstractStrategy(AlgoType_FutFut)
{
    _isAlive = true;
    std::cout<<"\n Enter Parameter File Path "<<std::endl;
    std::string filePath;
    //std::cin>>filePath;
    filePath = "futfut.conf";
    readOptionsfromFile(filePath);
    validateParameters();
    std::cout<<getParametersDump()<<std::endl;
    _state = &placeFirstLegOrder;
    _firstLegTradedQty = 0;
    _secondLegTradeQty = 0;
    _totalQty = _lotSize * _totalLots;
    //subscribeMarketDataFeed();

}

/**
 * @brief FutFutStrategy::readOptionsfromFile
 * @param filePath
 */
void FutFutStrategy::readOptionsfromFile(const std::string &filePath)
{
    try
    {
        std::string configFile = filePath;
        int firstLegOrderSide;
        int secondLegOrderSide;
        boost::program_options::options_description config("Configuration");
        config.add_options()

                ( "FUTFUT.SYMBOL",
                  boost::program_options::value<std::string>(&_symbol)->required(),
                  "Symbol")
                ( "FUTFUT.EXCHANGE",
                  boost::program_options::value<std::string>(&_exchange)->required(),
                  "Exchange")
                ( "FUTFUT.MATURITY_FIRST_LEG",
                  boost::program_options::value<std::string>(&_maturityFirstLeg)->required(),
                  "format yyyymmdd")
                ( "FUTFUT.MATURITY_SECOND_LEG",
                  boost::program_options::value<std::string>(&_maturitySecondLeg)->required(),
                  "format yyyymmdd")
                ( "FUTFUT.ORDER_SIDE_FIRST_LEG",
                  boost::program_options::value<int>(&firstLegOrderSide)->required(),
                  "0 BUY 1 SELL")
                ( "FUTFUT.MIN_SPREAD",
                  boost::program_options::value<long>(&_minSpread)->required(),
                  "minSpread")
                ( "FUTFUT.LOT_SIZE",
                  boost::program_options::value<long>(&_lotSize)->required(),
                  "lot Size")
                ( "FUTFUT.TOTAL_LOTS",
                  boost::program_options::value<long>(&_totalLots)->required(),
                  "totalLots")
                ;

        boost::program_options::options_description configFileOptions ;
        configFileOptions.add( config );

        boost::program_options::variables_map vm ;
        std::ifstream ifs( configFile.c_str() ) ;

        if(ifs)
        {
            store( parse_config_file( ifs, configFileOptions, true ), vm ) ;
            boost::program_options::notify ( vm ) ;

            setOrderSide(_firstLegOrderSide,_firstLegWatchSide,firstLegOrderSide);
            setOrderSide(_secondLegWatchSide,_secondLegOrderSide,firstLegOrderSide);

        }
        else
        {
            throw ParamsFileAbsent();
        }
    }
    catch(boost::program_options::error &e)
    {
        std::cout << "Bad Options: " << e.what()
                  << std::endl;
        throw e ;
    }

}
std::string FutFutStrategy::getParametersDump()
{
    std::ostringstream oss;
    oss<<"______________________"<<"\n"
      <<"_symbol "<<_symbol<<"\n"
     <<"_exchange :"<<_exchange<<"\n"
    <<"_maturityFirstLeg :"<<_maturityFirstLeg<<"\n"
    <<"_maturitySecondLeg :"<<_maturitySecondLeg<<"\n"
    <<"_firstLegOrderSide :"<<(char)_firstLegOrderSide<<"\n"
    <<"_secondLegOrderSide :"<<(char)_secondLegOrderSide<<"\n"
    <<"_minSpread :"<<(short)_minSpread<<"\n"
    <<"_lotSize :"<<(short)_lotSize<<"\n";

    return oss.str();
}

/**
 * @brief FutFutStrategy::subscribeMarketDataFeed
 */
void FutFutStrategy::subscribeMarketDataFeed()
{
    subscribeMarketDataFeedLeg(_maturityFirstLeg,_firstLegMarketDataRequest);
    subscribeMarketDataFeedLeg(_maturitySecondLeg,_secondLegMarketDataRequest);
}

/**
 * @brief FutFutStrategy::subscribeMarketDataFeedLeg
 * @param maturity
 * @param saveRequest
 */
void FutFutStrategy::subscribeMarketDataFeedLeg(const std::string &maturity,
                                                FIX42::MarketDataRequest &saveRequest)
{
    FIX::Symbol symbol(_symbol);
    FIX::SecurityExchange exchange(_exchange);
    FIX::SecurityType securityType("FUT");
    FIX::MaturityDay maturityDay(maturity.substr(6,2));
    FIX::MaturityMonthYear maturityMonthYear(maturity.substr(0,6));
    FIX::StrikePrice strikePrice;
    FIX::PutOrCall putCall;
    FIX::SenderCompID senderCompId("CLIENT5");
    FIX::TargetCompID targetCompId("EXECUTOR1");

    saveRequest = Application::queryMarketDataRequest42(symbol,
                                                        exchange,
                                                        securityType,
                                                        maturityDay,
                                                        maturityMonthYear,
                                                        strikePrice,
                                                        putCall,
                                                        senderCompId,
                                                        targetCompId);

    FIX::Session::sendToTarget( saveRequest );
    registerMktData(saveRequest);

}

/**
 * @brief FutFutStrategy::validateParameters
 */
void FutFutStrategy::validateParameters()
{
    if(_maturityFirstLeg.size() != 8)
    {
        throw MaturityFormatWrong();
    }
    if(_maturitySecondLeg.size() != 8)
    {
        throw MaturityFormatWrong();
    }
    if(_lotSize <= 0)
    {
        throw LotSizeWrong();
    }

}

/**
 * @brief FutFutStrategy::onMarketData
 * @param mktDataSnapShot
 */
void FutFutStrategy::onMarketData(const FIX42::MarketDataSnapshotFullRefresh& mktDataSnapShot)
{
    if(Application::getMarketDataUniqueSignature((FIX42::Message*) &mktDataSnapShot) ==
            Application::getMarketDataUniqueSignature(_secondLegMarketDataRequest))
    {
        std::cout<<"\nSecond Leg Market Data recvd";
        getMarketData( mktDataSnapShot,_secondLegMarketData,
                       MARKET_DEPTH_SIZE,_secondLegWatchSide);
        for(int i=0;i<MARKET_DEPTH_SIZE;i++)
        {
            std::cout<<_secondLegMarketData[i]._price
                    <<","
                   <<_secondLegMarketData[i]._qty<<"\n";
        }
        (*_state)(*this);
    }
}

/**
 * @brief FutFutStrategy::setOrderSide
 * @param side
 * @param watchSide
 * @param orderSide
 */
void FutFutStrategy::setOrderSide(FIX::Side &side,
                                  FIX::Side &watchSide,const int &orderSide)
{
    if(orderSide == 0)
    {
        side = FIX::Side_BUY;
        watchSide = FIX::Side_SELL;
    }
    else if(orderSide == 1)
    {
        side = FIX::Side_SELL;
        watchSide = FIX::Side_BUY;
    }
    else
    {
        throw OrderSideFormatWrong();
    }
}

/**
 * @brief FutFutStrategy::getFirstLegPrice
 * @return
 */
double FutFutStrategy::getFirstLegPrice()
{
    double price = 0;

    if(_secondLegMarketData[0]._qty > 0)
    {

        if(_firstLegOrderSide == FIX::Side_BUY)
        {
            price = _secondLegMarketData[0]._price - _minSpread;
        }
        else if(_firstLegOrderSide == FIX::Side_SELL)
        {
            price = -_secondLegMarketData[0]._price + _minSpread;
        }
    }
    if(price <= 0)
        return 0;
    return price;
}

/**
 * @brief FutFutStrategy::closeStrategy
 */
void FutFutStrategy::closeStrategy()
{
    StrategyDriver::getInstance()->unRegisterStrategyForMktData(this,_firstLegMarketDataRequest);
    StrategyDriver::getInstance()->unRegisterStrategyForMktData(this,_secondLegMarketDataRequest);
    _isAlive = false;
}

/**
 * @brief FutFutStrategy::placeFirstLegOrder
 * @param obj
 */
void FutFutStrategy::placeFirstLegOrder(FutFutStrategy &obj)
{
    if(obj._firstLegTradedQty >= obj._totalQty)
    {
        obj.closeStrategy();
        return;
    }

    double price = obj.getFirstLegPrice();
    std::string maturity = obj.getMaturityFirstLeg().substr(0,6);
    if(price >0 )
    {
        std::string clorderId = StrategyDriver::getInstance()->genClOrderId();
        obj.setFirstLegClordId(clorderId);
        FIX42::NewOrderSingle singelOrder = Application::queryNewOrderSingle42(
                    FIX::ClOrdID(clorderId),
                    FIX::Symbol(obj.getSymbol()),
                    obj.getFirstLegOrderSide(),
                    FIX::OrdType(FIX::OrdType_LIMIT),
                    FIX::SecurityExchange(obj.getExchange()),
                    FIX::OrderQty(obj.getLotSize()),
                    FIX::Price(price),
                    FIX::StopPx(0),
                    FIX::TimeInForce(FIX::TimeInForce_DAY),
                    FIX::SecurityType("FUT"),
                    FIX::MaturityMonthYear(maturity),
                    FIX::MaturityDay(obj.getMaturityFirstLeg().substr(6,2)),
                    FIX::StrikePrice(0),
                    FIX::PutOrCall(0),
                    FIX::SenderCompID("CLIENT5"),
                    FIX::TargetCompID("EXECUTOR1"));
        FIX::Session::sendToTarget( singelOrder );
        obj._state = &noAction;
        obj.registerClOrderId(clorderId);
        obj._oldPrice = price;
    }
}


/**
 * @brief FutFutStrategy::onExecutionReport
 * @param report
 */
void FutFutStrategy::onExecutionReport( const FIX42::ExecutionReport& report)
{
    FIX::OrdStatus orderStatus;
    report.get(orderStatus);

    FIX::ClOrdID clorderId;
    report.get(clorderId);

    switch(orderStatus)
    {
    case FIX::OrdStatus_NEW:
    {
        if(clorderId == _firstLegClOrderId)
        {
            std::cout<<"Order Confirmation recvd\n";
            _state = &replaceOrder;
        }
        if(clorderId == _secondLegClOrderId)
        {
            std::cout<<"second leg Order Confirmation recvd\n";
        }
    }
        break;

    case FIX::OrdStatus_REPLACED:
    {
        if(clorderId == _firstLegReplaceClOrderId)
        {
            std::cout<<"Order Replaced recvd\n";
            _firstLegClOrderId = clorderId;
            _state = &replaceOrder;
        }
    }
        break;

    case FIX::OrdStatus_CANCELED:
    {
        if(clorderId == _firstLegReplaceClOrderId)
        {
            std::cout<<"Order Cancelled recvd\n";
            _state = &placeFirstLegOrder;
        }
        StrategyDriver::getInstance()->removeOnCancelFill(clorderId);
    }
        break;

    case FIX::OrdStatus_FILLED:
    {
        
        FIX::LastShares qty;

        report.get(qty);
        if(clorderId == _firstLegReplaceClOrderId ||
                clorderId == _firstLegClOrderId)
        {
            std::cout<<"Order  Filled recvd\n";
            _firstLegTradedQty+= qty;
            placeSecondLegOrder();
            _state = &noAction;
        }
        if(clorderId == _secondLegClOrderId )
        {
            std::cout<<"Order  Filled recvd for second Leg\n";
            _secondLegTradeQty+=qty;
            _state = &placeFirstLegOrder;
        }
        StrategyDriver::getInstance()->removeOnCancelFill(clorderId);
    }
        break;
    }
}


/**
 * @brief FutFutStrategy::noAction
 * @param obj
 */
void FutFutStrategy::noAction(FutFutStrategy &obj)
{

}

/**
 * @brief FutFutStrategy::replaceOrder
 * @param obj
 */
void FutFutStrategy::replaceOrder(FutFutStrategy &obj)
{
    double price = obj.getFirstLegPrice();

    if(price > 0 && abs(obj._oldPrice - price) > 1 &&  obj._firstLegTradedQty < obj._totalQty)
    {
        obj._firstLegReplaceClOrderId = StrategyDriver::getInstance()->genClOrderId();

        FIX42::OrderCancelReplaceRequest replaceOrderRequest = Application::queryCancelReplaceRequest42(
                    FIX::OrdType(FIX::OrdType_LIMIT),
                    obj._firstLegClOrderId,
                    obj._firstLegReplaceClOrderId,
                    obj._symbol,
                    obj.getFirstLegOrderSide(),
                    FIX::Price(price),
                    FIX::OrderQty(obj.getLotSize()),
                    FIX::SenderCompID("CLIENT5"),
                    FIX::TargetCompID("EXECUTOR1"));

        FIX::Session::sendToTarget( replaceOrderRequest );
        obj._state = &noAction;
        obj.registerClOrderId( obj._firstLegReplaceClOrderId);
        obj._oldPrice = price;
    }
    else if(price <= 0 || obj._firstLegTradedQty >= obj._totalQty)
    {
        obj._firstLegReplaceClOrderId = StrategyDriver::getInstance()->genClOrderId();
        FIX42::OrderCancelRequest cancelRequest =
                Application::queryOrderCancelRequest42(
                    FIX::OrigClOrdID(obj._firstLegClOrderId),
                    FIX::ClOrdID(obj._firstLegReplaceClOrderId),
                    FIX::Symbol(obj._symbol),
                    FIX::Side(obj.getFirstLegOrderSide()),
                    FIX::SenderCompID("CLIENT5"),
                    FIX::TargetCompID("EXECUTOR1"));

        FIX::Session::sendToTarget(cancelRequest );
        obj._state = &noAction;
        obj.registerClOrderId( obj._firstLegReplaceClOrderId);
    }
}

/**
 * @brief FutFutStrategy::placeSecondLegOrder
 */
void FutFutStrategy::placeSecondLegOrder()
{
    std::string maturity = getMaturitySecondLeg().substr(0,6);
    _secondLegClOrderId = StrategyDriver::getInstance()->genClOrderId();

    FIX42::NewOrderSingle singelOrder = Application::queryNewOrderSingle42(
                FIX::ClOrdID(_secondLegClOrderId),
                FIX::Symbol(getSymbol()),
                _secondLegOrderSide,
                FIX::OrdType(FIX::OrdType_MARKET),
                FIX::SecurityExchange(getExchange()),
                FIX::OrderQty(getLotSize()),
                FIX::Price(0),
                FIX::StopPx(0),
                FIX::TimeInForce(FIX::TimeInForce_DAY),
                FIX::SecurityType("FUT"),
                FIX::MaturityMonthYear(maturity),
                FIX::MaturityDay(getMaturitySecondLeg().substr(6,2)),
                FIX::StrikePrice(0),
                FIX::PutOrCall(0),
                FIX::SenderCompID("CLIENT5"),
                FIX::TargetCompID("EXECUTOR1"));

    FIX::Session::sendToTarget( singelOrder );
    _state = &noAction;
    registerClOrderId(_secondLegClOrderId);

}
