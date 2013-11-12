#ifndef SGTABLES_H
#define SGTABLES_H

#include <QDockWidget>
#include "portfolio.h"
#include <QStandardItemModel>

namespace Ui {
class SGTables;
}
/**
 * @brief The SGTables class
 */
class SGTables : public QDockWidget
{
    Q_OBJECT
    
public:
    ~SGTables();
    /**
     * @brief getInstance
     * @param parent
     * @return
     */
    static SGTables *getInstance(QWidget *parent =0){

        if(!_instance)
        {
            _instance = new SGTables(parent);
        }
        return _instance;
    }
    /**
     * @brief initModels
     * @return
     */
    static void initModels()
    {
        if(_instance)
            _instance = new SGTables();

    }

    /**
     * @brief getOrderBookModel
     * @return
     */
    QStandardItemModel *getOrderBookModel(){return modelOrderBook;}
    /**
     * @brief getNetPositionsModel
     * @return
     */
    QStandardItemModel *getNetPositionsModel(){return modelNetPositions;}
    /**
     * @brief getTradeBookModel
     * @return
     */
    QStandardItemModel *getTradeBookModel(){return modelTradeBook;}

    /**
     * @brief tableColumnOrderBook
     * @param header
     * @return
     */
    int tableColumnOrderBook(const QString& header);
    /**
     * @brief tableColumnTradeBook
     * @param header
     * @return
     */
    int tableColumnTradeBook(const QString& header);
    /**
     * @brief tableColumnNetPosition
     * @param header
     * @return
     */
    int tableColumnNetPosition(const QString& header);

    /**
     * @brief removeRow
     * @param model
     * @param row
     */
    void removeRow(QStandardItemModel *model, int row);

    /**
     * @brief getInstrumentTypeString
     * @param type
     * @return
     */
    QString getInstrumentTypeString(mutrade::InstrumentType type){
        switch(type)
        {
        case mutrade::InstrumentType_FUTURE:
            return "Future";
        case mutrade::InstrumentType_STOCK:
            return "Stock";
        case mutrade::InstrumentType_OPTION:
            return "Option";

        }
        return "Invalid";

    }
    /**
     * @brief getOrderTypeString
     * @param type
     * @return
     */
    QString getOrderTypeString(mutrade::OrderType type)
    {
        switch(type)
        {
        case mutrade::OrderType_LIMIT:
            return "Limit";
        case mutrade::OrderType_MARKET:
            return "Market";
        default:
            return "Invalid";

        }
        return "Invalid";


    }
    /**
     * @brief getOrderStatusString
     * @param status
     * @return
     */
    QString getOrderStatusString(mutrade::OrderStatus status) const;

    /**
     * @brief getOrderModeString
     * @param side
     * @return
     */
    QString getOrderModeString(mutrade::Side side)
    {
        if(side==mutrade::Side_BUY)
            return QString("Buy");
        else
            return QString("Sell");

    }
    /**
     * @brief getOrderValidityString
     * @param validity
     * @return
     */
    QString getOrderValidityString(mutrade::TimeInForce validity)
    {
        switch(validity)
        {
        case mutrade::TimeInForce_DAY:
            return "DAY";
        case mutrade::TimeInForce_IOC:
            return "IOC";
        default:
            return "Invalid";

        }
        return "Invalid";

    }


private:
    /**
     * @brief _instance
     */
    static SGTables *_instance;
    /**
     * @brief ui
     */
    Ui::SGTables *ui;
    /**
     * @brief SGTables
     * @param parent
     */
    explicit SGTables(QWidget *parent = 0);
    /**
     * @brief tableHeadersOrderBook
     */
    QStringList tableHeadersOrderBook;
    /**
     * @brief tableHeadersTradeBook
     */
    QStringList tableHeadersTradeBook;
    /**
     * @brief tableHeadersNetPositions
     */
    QStringList tableHeadersNetPositions;
    /**
     * @brief hashOrderBookColumns
     */
    QHash<QString, int> hashOrderBookColumns;
    /**
     * @brief hashTradeBookColumns
     */
    QHash<QString, int> hashTradeBookColumns;
    /**
     * @brief hashNetPositionsColumns
     */
    QHash<QString, int> hashNetPositionsColumns;
    /**
     * @brief modelOrderBook
     */
    QStandardItemModel *modelOrderBook;
    /**
     * @brief modelTradeBook
     */
    QStandardItemModel *modelTradeBook;
    /**
     * @brief modelNetPositions
     */
    QStandardItemModel *modelNetPositions;
    /**
     * @brief initialize
     */
    void initialize();

    /**
     * @brief removeOrder
     * @param orderId
     */
    void removeOrder(mutrade::Int64 orderId);

    /**
     * @brief updateOrderBook
     * @param report
     */
    void updateOrderBook(
            const mutrade::ExecutionReport &report
            );
    /**
     * @brief updateTradeBook
     * @param clOrderId
     * @param tradeId
     */
    void updateTradeBook(const mutrade::Int64 &clOrderId,
                         const mutrade::Int64 &tradeId);
    /**
     * @brief updateNetPositions
     * @param instrumentName
     * @param side
     */
    void updateNetPositions(const mutrade::String &instrumentName,
                            const mutrade::Side &side);

    /**
     * @brief updateItem
     * @param model
     * @param row
     * @param column
     * @param text
     */
    void updateItem(QStandardItemModel*model, int row, int column, QString text);

    /**
     * @brief insertNewRow
     * @param model
     * @param row
     */
    void insertNewRow(QStandardItemModel *model, int row);

    /**
     * @brief getBookedPnl
     * @param positionA
     * @param positionB
     * @return
     */
    mutrade::Int32 getBookedPnl(mutrade::Position *positionA,
                                mutrade::Position *positionB);

private slots:
    /**
     * @brief slotUpdateOrder
     * @param report
     */
    void slotUpdateOrder(const mutrade::ExecutionReport &report);
    /**
     * @brief reloadNetPosition
     * @param symbol
     */
    void reloadNetPosition(QString symbol);

signals:
    /**
     * @brief hideOrderBookRow
     */
    void hideOrderBookRow(int);
};

#endif // SGTABLES_H
