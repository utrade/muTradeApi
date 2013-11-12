#include "testStrategy.h"
#include "processapiresponses.h"
#include "marketdata.h"
#include "portfolio.h"
#include "Logger.h"
#include "mainwindow.h"
namespace mutrade{
TestStrategy* TestStrategy::_instance =0;

TestStrategy::TestStrategy()
{
    sd = new SymbolData();
    context = Context::getInstance();
    firstLegInstrumentLoaded = false;
    secondLegInstrumentLoaded = false;
    firstLegInstrument=0;
    secondLegInstrument=0;
    buyPendingFlag =false;
    sellPendingFlag=false;
    connect(mutrade::ProcessApiResponses::getInstance(),
            SIGNAL(signalTick(mutrade::MarketData)),
            this,
            SLOT(onTick(mutrade::MarketData))
            );
    connect(mutrade::ProcessApiResponses::getInstance(),
            SIGNAL(updateOrder(mutrade::ExecutionReport)),
            this,
            SLOT(onExecReport(mutrade::ExecutionReport))
            );
}

void TestStrategy::onTick(MarketData md)
{
    if(sgRunningStatus
            &&
            (
                md.getInstrument().getInstrumentName() == sd->firstLegSymbol
                ||
                md.getInstrument().getInstrumentName() == sd->secondLegSymbol)
            )
    {
        //strategyRunning conitionTest
        MainWindow::getInstance()->logInfo("Got Market data Event for symbol: "+ md.getInstrument().getInstrumentName());
        if(sd->isOrderPending)
        {
            MainWindow::getInstance()->logInfo("Orders are pending, Tick rejected");
        }
        else
        {
            if(md.getInstrument().getInstrumentName() == sd->firstLegSymbol)
            {
                getAveragePrice( static_cast<mutrade::MarketData> (md),
                                 sd->qty,
                                 sd->firstLegSide,
                                 sd->avgPriceFirstLeg
                                 );

                MainWindow::getInstance()->logInfo("First leg avg price : " +QString::number(sd->avgPriceFirstLeg).toStdString());
            }
            else if(md.getInstrument().getInstrumentName() == sd->secondLegSymbol)
            {
                getAveragePrice( static_cast<mutrade::MarketData> (md),
                                 sd->qty,
                                 sd->secondLegSide,
                                 sd->avgPriceSecondLeg
                                 );

                MainWindow::getInstance()->logInfo("Second leg avg price : "+ QString::number(sd->avgPriceSecondLeg).toStdString());
            }
            if(sd->avgPriceSecondLeg>0)
            {
                sd->orderPriceFirstLeg =
                        (sd->minProfit * sd->firstLegMode)
                        + (sd->avgPriceSecondLeg);

                MainWindow::getInstance()->logInfo("Bid Price Leg -1 :" + QString::number(sd->orderPriceFirstLeg).toStdString());
                if( !sd->bidPlaced )
                {
                    firstLegBid();
                    sd->bidPlaced = true;
                    sd->isOrderPending = true;
                }
                else if(  sd->bidPlaced &&
                          sd->firstLegOrder->getOrderStatus()
                          != mutrade::OrderStatus_PENDING )
                {
                    modifyOrder();
                    sd->bidPlaced = true;
                    sd->isOrderPending = true;
                }
                else
                {
                    if(sd->avgPriceFirstLeg != 0)
                        MainWindow::getInstance()->logInfo("Cannot update order as previous order is pending at exchange.");
                }
            }
            else if( sd->avgPriceSecondLeg == 0)
            {
                if(sd->bidPlaced)
                {
                    MainWindow::getInstance()->logInfo("Creating cancel order for first Leg.");
                    cancelFirstLeg();
                }
            }
        }
    }
    else
        return;
}

void TestStrategy::onExecReport(ExecutionReport report)
{
    if( (report.getInstrumentName() != sd->firstLegSymbol ||
         report.getInstrumentName() != sd->secondLegSymbol) && !sgRunningStatus
            )
        return;
    if(report.getInstrumentName() == sd->firstLegSymbol)
    {
        if(report.getOrderStatus() == mutrade::OrderStatus_CONFIRMED)
        {
            sd->firstLegOrder->setClOrdId(report.getClOrderId());
            sd->firstLegOrder->setOrigClOrdId(report.getOriginalClOrderId());
            sd->firstLegOrder->setOrderStatus(mutrade::OrderStatus_CONFIRMED);
            sd->isOrderPending = false;
        }
        else if(report.getOrderStatus() == mutrade::OrderStatus_REPLACED)
        {
            sd->firstLegOrder->setClOrdId(report.getClOrderId());
            sd->firstLegOrder->setOrigClOrdId(report.getOriginalClOrderId());
            sd->firstLegOrder->setOrderStatus(mutrade::OrderStatus_CONFIRMED);
            sd->isOrderPending = false;
        }
        else if(report.getOrderStatus() == mutrade::OrderStatus_FILLED)
        {
            placeSecondLegMarket();
            sd->isOrderPending = true;
        }
    }
    else if(report.getInstrumentName() == sd->secondLegSymbol)
    {
        if(report.getOrderStatus() == mutrade::OrderStatus_CONFIRMED)
        {
            sd->secondLegOrder->setClOrdId(report.getClOrderId());
            sd->secondLegOrder->setOrigClOrdId(report.getOriginalClOrderId());
            sd->isOrderPending = false;
        }
        else if(report.getOrderStatus() == mutrade::OrderStatus_FILLED)
        {
            std::cout << "Strategy completed successfully." << std::endl;
        }
    }

}

void TestStrategy::subscribeMktData(String symbolName)
{
    Instrument *inst =
            mutrade::Context::getInstance()->getInstrument(symbolName);
    mutrade::Context::getInstance()->subscribe(*inst);
    delete inst;
}

bool TestStrategy::startStrategy(SymbolData *symbolData)
{
    if(!ProcessApiResponses::getInstance()->getLoginStatus())
        return false;
    this->sd = symbolData;
    if(!sgRunningStatus){
        subscribeMktData(sd->firstLegSymbol);
        subscribeMktData(sd->secondLegSymbol);
        sgRunningStatus = true;
        return true;
    }
    return false;
}

void TestStrategy::stopStrategy()
{
    sgRunningStatus = false;
    delete sd;
}

TestStrategy *TestStrategy::getInstance()
{
    if(!_instance)
    {
        _instance = new TestStrategy();
    }
    return _instance;
}


Context *TestStrategy::getContext()
{
    return context;
}

bool TestStrategy::getSgRunningStatus()
{
    return sgRunningStatus;
}

Int32 TestStrategy::getNearTickPrice(Int32 price, Int32 tickSize)
{
    Int32 _price = price -price%tickSize;
    return _price;
}
void TestStrategy::firstLegBid()
{
    MainWindow::getInstance()->logInfo("\n**************** In firstLegBid ***************\n");
    sd->firstLegOrder->setTransactionType(mutrade::TransactionType_NEW);
    sd->firstLegOrder->setSymbol(sd->firstLegSymbol);
    sd->firstLegOrder->setOrderMode(sd->firstLegSide);
    sd->firstLegOrder->setQuantity(
                sd->qty
                *
                mutrade::Context::getInstance()->getInstrument(
                    sd->firstLegSymbol
                    )->getLotSize()
                );
    sd->firstLegOrder->setPrice(sd->orderPriceFirstLeg);
    sd->firstLegOrder->setSecurityType(mutrade::InstrumentType_FUTURE);
    sd->firstLegOrder->setDisclosedQuantity(sd->qty
                                            *
                                            mutrade::Context::getInstance()->getInstrument(
                                                sd->firstLegSymbol
                                                )->getLotSize());
    sd->firstLegOrder->setOrderValidity(mutrade::TimeInForce_DAY);
    sd->firstLegOrder->setOrderType(mutrade::OrderType_LIMIT);
    sd->firstLegOrder->setOrderStatus(mutrade::OrderStatus_PENDING);
    context->getInstance()->placeOrder(*(sd->firstLegOrder));

}

void TestStrategy::modifyOrder()
{
    MainWindow::getInstance()->logInfo("**************** In Modify Order *************** ");
    sd->firstLegOrder->setTransactionType(mutrade::TransactionType_MODIFY);
    sd->firstLegOrder->setSymbol(sd->firstLegSymbol);
    sd->firstLegOrder->setOrderMode(sd->firstLegSide);
    sd->firstLegOrder->setQuantity(sd->qty
                                   *
                                   mutrade::Context::getInstance()->getInstrument(
                                       sd->firstLegSymbol
                                       )->getLotSize()
                                   );
    sd->firstLegOrder->setPrice(sd->orderPriceFirstLeg);
    sd->firstLegOrder->setSecurityType(mutrade::InstrumentType_FUTURE);
    sd->firstLegOrder->setDisclosedQuantity(sd->qty
                                            *
                                            mutrade::Context::getInstance()->getInstrument(
                                                sd->firstLegSymbol
                                                )->getLotSize());
    sd->firstLegOrder->setOrderValidity(mutrade::TimeInForce_DAY);
    sd->firstLegOrder->setOrderType(mutrade::OrderType_LIMIT);
    sd->firstLegOrder->setOrderStatus(mutrade::OrderStatus_PENDING);
    sd->firstLegOrder->setOrigClOrdId(
                sd->firstLegOrder->getOrigClOrdId());
    sd->firstLegOrder->setClOrdId(sd->firstLegOrder->getClOrdId());
    sd->firstLegOrder->dumpOrder();
    context->getInstance()->placeOrder(*(sd->firstLegOrder));
}

void TestStrategy::cancelFirstLeg()
{
    MainWindow::getInstance()->logInfo("Cancelling the order................");
    sd->firstLegOrder->setTransactionType(mutrade::TransactionType_CANCEL);
    sd->firstLegOrder->setSymbol(sd->firstLegSymbol);
    sd->firstLegOrder->setOrderStatus(mutrade::OrderStatus_PENDING);
    sd->firstLegOrder->setOrigClOrdId(
                sd->firstLegOrder->getOrigClOrdId() );
    sd->firstLegOrder->setClOrdId(
                sd->firstLegOrder->getClOrdId());
    context->getInstance()->placeOrder(*(sd->firstLegOrder));
}

void TestStrategy::placeSecondLegMarket()
{
    MainWindow::getInstance()->logInfo("**************** In Leg-2 Market *************** ");
    sd->secondLegOrder->setTransactionType(mutrade::TransactionType_NEW);
    sd->secondLegOrder->setSymbol(sd->secondLegSymbol);
    sd->secondLegOrder->setOrderMode(sd->secondLegSide);
    sd->secondLegOrder->setQuantity(sd->qty
                                    *
                                    mutrade::Context::getInstance()->getInstrument(
                                        sd->secondLegSymbol
                                        )->getLotSize());
    sd->secondLegOrder->setPrice(0);
    sd->secondLegOrder->setSecurityType(mutrade::InstrumentType_FUTURE);
    sd->secondLegOrder->setDisclosedQuantity(sd->qty
                                             *
                                             mutrade::Context::getInstance()->getInstrument(
                                                 sd->firstLegSymbol
                                                 )->getLotSize());
    sd->secondLegOrder->setOrderValidity(mutrade::TimeInForce_DAY);
    sd->secondLegOrder->setOrderType(mutrade::OrderType_MARKET);
    sd->secondLegOrder->setOrderStatus(mutrade::OrderStatus_PENDING);
    context->getInstance()->placeOrder(*(sd->secondLegOrder));

}
/*
 *  Function to calculate average price for given qty from
 *  current market depth.
 */
bool TestStrategy::getAveragePrice( mutrade::MarketData md,
                                    int qty,
                                    mutrade::Side side,
                                    int &avgPrice )
{
    mutrade::Int64 tempPrice = 0;
    mutrade::Int64 tempQty = 0;
    for(int i=0; i<5; ++i)
    {
        side = side==mutrade::Side_ASK
                ?
                    mutrade::Side_BID
                  :
                    mutrade::Side_ASK;
        mutrade::Int64 p = md.getPrice(side, i+1);
        mutrade::Int64 q = md.getQty(side, i+1);

        tempPrice += p * q;

        tempQty += q;
        if(tempQty >= qty)
            break;
    }
    if(tempQty < qty)
    {
        avgPrice = 0;
        return false;
    }
    avgPrice = tempPrice / tempQty;
    //    std::cout << "avgPrice " << avgPrice << std::endl;
    return true;
}
}//namespace mutrade

