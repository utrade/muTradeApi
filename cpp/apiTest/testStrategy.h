#ifndef SGSONTEXT_H
#define SGSONTEXT_H
#include "context.h"
#include "instrument.h"
#include <QSharedDataPointer>
#include <QMap>
#include <QObject>
namespace mutrade{
/**
 * @brief The SymbolData struct to store
 * strategy state and legs info.
 */
struct SymbolData
{
    String      firstLegSymbol;
    String      secondLegSymbol;
    Int32       minProfit;
    Int32       avgPriceFirstLeg;
    Int32       avgPriceSecondLeg;
    Side        firstLegSide;
    Side        secondLegSide;
    Boolean     bidPlaced;
    Int16       firstLegMode;
    Int16       secondLegMode;
    Int32       orderPriceFirstLeg;
    Int32       qty;
    Order       *firstLegOrder;
    Order       *secondLegOrder;
    Int32       filledQtyFirstLeg;
    Int32       filledQtySecondLeg;
    Boolean     isOrderPending;
};

/**
 * @brief The TestStrategy class
 */
class TestStrategy :public QObject
{
    Q_OBJECT

private:
    /**
     * @brief context
     */
    Context *context;
    /**
     * @brief TestStrategy
     */
    TestStrategy();
    /**
     * @brief _instance
     */
    static TestStrategy *_instance;
    /**
     * @brief firstLegInstrument
     */
    mutrade::Instrument *firstLegInstrument;
    /**
     * @brief secondLegInstrument
     */
    mutrade::Instrument *secondLegInstrument;
    /**
     * @brief tradeSymbolName
     */
    mutrade::String tradeSymbolName;
    /**
     * @brief refSymbolName
     */
    mutrade::String refSymbolName;
    /**
     * @brief firstLegInstrumentLoaded
     */
    bool firstLegInstrumentLoaded;
    /**
     * @brief secondLegInstrumentLoaded
     */
    bool secondLegInstrumentLoaded;
    /**
     * @brief buyOrderId
     */
    Int64 buyOrderId;
    /**
     * @brief sellOrderId
     */
    Int64 sellOrderId;
    /**
     * @brief sgRunningStatus
     */
    bool sgRunningStatus;
    /**
     * @brief orderInstance
     */
    Order orderInstance;
    /**
     * @brief buyPendingFlag
     */
    bool buyPendingFlag;
    /**
     * @brief sellPendingFlag
     */
    bool sellPendingFlag;
    /**
     * @brief bidPrice
     */
    Int64 bidPrice;
    /**
     * @brief askPrice
     */
    Int64 askPrice;
    /**
     * @brief firstLegBid
     */
    void firstLegBid();
    /**
     * @brief modifyOrder
     */
    void modifyOrder();
    /**
     * @brief cancelFirstLeg
     */
    void cancelFirstLeg();
    /**
     * @brief getAveragePrice
     * @param md
     * @param qty
     * @param side
     * @param avgPrice
     * @return
     */
    bool getAveragePrice(mutrade::MarketData md, int qty, mutrade::Side side, int &avgPrice);
    /**
     * @brief placeSecondLegMarket
     */
    void placeSecondLegMarket();
    /**
     * @brief subscribeMktData
     * @param symbolName
     */
    void subscribeMktData(String symbolName);


public:
    /**
     * @brief onRefSymbolTick
     * @param mktData
     */
    void onRefSymbolTick(MarketData mktData);
    /**
     * @brief onTradeSymbolTick
     * @param mktData
     */
    void onTradeSymbolTick(MarketData mktData);
    /**
     * @brief getRefInstrument
     * @return
     */
    Instrument *getRefInstrument(){return secondLegInstrument;}
    /**
     * @brief getTradeInstrument
     * @return
     */
    Instrument *getTradeInstrument(){return firstLegInstrument;}
    /**
     * @brief setBuyOrderId
     * @param orderId
     */
    void setBuyOrderId(Int64 orderId){ buyOrderId = orderId;}
    /**
     * @brief setSellOrderId
     * @param orderId
     */
    void setSellOrderId(Int64 orderId){ sellOrderId = orderId;}
    /**
     * @brief getBuyOrderId
     * @return
     */
    Int64 getBuyOrderId(){return buyOrderId;}
    /**
     * @brief getSellOrderId
     * @return
     */
    Int64 getSellOrderId(){return sellOrderId;}
    /**
     * @brief startStrategy
     * @param sd
     * @return
     */
    bool startStrategy(SymbolData *sd);
    /**
     * @brief stopStrategy
     */
    void stopStrategy();
    /**
     * @brief getInstance
     * @return
     */
    static TestStrategy *getInstance();
    /**
     * @brief getContext
     * @return
     */
    Context *getContext();
    /**
     * @brief loadTradeInstrument
     * @param symbol
     */
    void loadTradeInstrument(const String &symbol);
    /**
     * @brief loadRefInstrument
     * @param symbol
     */
    void loadRefInstrument(const String &symbol);
    /**
     * @brief getIntrumentStatus
     * @return
     */
    bool getIntrumentStatus();
    /**
     * @brief getSgRunningStatus
     * @return
     */
    bool getSgRunningStatus();
    /**
     * @brief getNearTickPrice
     * @param price
     * @param tickSize
     * @return
     */
    Int32 getNearTickPrice(Int32 price, Int32 tickSize);
    /**
     * @brief sendOrder
     * @param side
     * @param price
     * @param transType
     * @param clOrderId
     */
    void sendOrder(const Side &side, const Int32 price, const TransactionType &transType, const Int64 clOrderId);
    /**
     * @brief sendTrade
     */
    void sendTrade();
    /**
     * @brief sd
     */
    SymbolData *sd;

private slots:
    /**
     * @brief onTick
     * @param md
     */
    void onTick(mutrade::MarketData md);
    /**
     * @brief onExecReport
     * @param report
     */
    void onExecReport(mutrade::ExecutionReport report);
signals:
    /**
     * @brief updateIV
     * @param bidIvVal
     * @param askIvVal
     * @param lastIvVal
     */
    void updateIV(double bidIvVal,double askIvVal,double lastIvVal);
};

}//namespace mutrade

#endif // SGSONTEXT_H
