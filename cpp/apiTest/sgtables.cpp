#include "sgtables.h"
#include "ui_sgtables.h"
#include "processapiresponses.h"
#include <Logger.h>
SGTables* SGTables::_instance=0;

SGTables::SGTables(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SGTables)
{
    ui->setupUi(this);
    initialize();

}

SGTables::~SGTables()
{
    delete ui;
    delete modelNetPositions;
    delete modelTradeBook;
    delete modelOrderBook;

}

void SGTables::initialize()
{
    LOGGER_INFO("Initialising Portfolio Widgets");
    tableHeadersOrderBook <<
                             "Order Id"<<
                             "Mode"<<
                             "Instrument Name"<<
                             "Instrument Type"<<
                             "Quantity"<<
                             "Price"<<
                             "Status"<<
                             "Exch Order ID"<<
                             "Mkt-To-Limit"<<
                             "Orig Order Id"<<
                             "Disclosed Order Qty"<<
                             "Last Fill Qty"<<
                             "Last Fill Price"<<
                             "Total Fill qty"<<
                             "Pending Qty"<<
                             "Stop Price"<<
                             "Product Type"<<
                             "Series"<<
                             "Order Type"<<
                             "Validity"<<
                             "Is Triggered"<<
                             "Date/Time";
    int i =0;
    foreach (QString header, tableHeadersOrderBook) {
        hashOrderBookColumns.insert(header,i++);
    }

    tableHeadersTradeBook <<
                             "Order Id"<<
                             "Mode"<<
                             "Instrument Name"<<
                             "Instrument Type"<<
                             "Filled Quantity"<<
                             "Filled Price"<<
                             "Exch Order ID"<<
                             "Trade Id"<<
                             "Amount";
    i =0;
    foreach (QString header, tableHeadersTradeBook) {
        hashTradeBookColumns.insert(header,i++);
    }

    tableHeadersNetPositions <<
                                "Instrument Name"<<
                                "Instrument Type"<<
                                "Buy Qty"<<
                                "Avg Buy Price"<<
                                "Sell Qty"<<
                                "Avg Sell Price"<<
                                "Buy Amount"<<
                                "Sell Amount"<<
                                "LTP"<<
                                "M2M PnL"<<
                                "Booked PnL"<<
                                "NET Qty";
    //                                "NET Value"
    //                                ;

    i =0;
    foreach (QString header, tableHeadersNetPositions) {
        hashNetPositionsColumns.insert(header,i++);
    }

    modelNetPositions = new QStandardItemModel();
    modelNetPositions->setColumnCount(tableHeadersNetPositions.count());
    modelNetPositions->setHorizontalHeaderLabels(tableHeadersNetPositions);

    modelOrderBook = new QStandardItemModel();
    modelOrderBook->setColumnCount(tableHeadersOrderBook.count());
    modelOrderBook->setHorizontalHeaderLabels(tableHeadersOrderBook);

    modelTradeBook = new QStandardItemModel();
    modelTradeBook->setColumnCount(tableHeadersTradeBook.count());
    modelTradeBook->setHorizontalHeaderLabels(tableHeadersTradeBook);

    connect(mutrade::ProcessApiResponses::getInstance(),
            SIGNAL(updateOrder(mutrade::ExecutionReport)),
            this,
            SLOT(slotUpdateOrder(mutrade::ExecutionReport))
            );

    connect(mutrade::ProcessApiResponses::getInstance(),
            SIGNAL(replayNetPosition(QString)),
            this,
            SLOT(reloadNetPosition(QString)));

}

void SGTables::removeOrder(mutrade::Int64 orderId)
{
    QList<QStandardItem *> listRows = modelOrderBook->findItems(
                QString::number(orderId)
                );
    int row=0;
    if(listRows.count()>0)
    {
        foreach (QStandardItem *itemRow, listRows) {
            row=itemRow->row();
            removeRow(getOrderBookModel(),row);
        }

    }
}
void SGTables::removeRow(QStandardItemModel *model, int row)
{
    model->item(row,0)->setData("0",Qt::DisplayRole);
    emit hideOrderBookRow(row);

}
void SGTables::updateOrderBook(const mutrade::ExecutionReport &report)
{

    LOGGER_INFO("Updating OrderBook for Order-Id:"+QString::number(report.getClOrderId()));
    mutrade::Order *order =
            mutrade::Portfolio::getInstance()->
            getOrderBook().getOrder(report.getClOrderId());

    if(order->getOrigClOrdId()!=0)
    {
        removeOrder(order->getOrigClOrdId());
    }
    //    switch(order->getOrderStatus())
    //    {
    //    case mutrade::OrderStatus_CANCELED:
    //    case mutrade::OrderStatus_CANCELED_OF_IOC:
    //    case mutrade::OrderStatus_FILLED:
    //    case mutrade::OrderStatus_FROZEN:
    //    case mutrade::OrderStatus_NEW_REJECTED:
    //    case mutrade::OrderStatus_RMS_REJECT:
    //        removeOrder(order->getClOrdId());
    //        return;
    //    default:
    //        break;
    //    }
    QList<QStandardItem *> listRows = modelOrderBook->findItems(
                QString::number(order->getClOrdId()));

    bool flagNew =false;
    int row = 0;
    if(listRows.count()>1)
    {
        std::cout<<"SOME ERROR STATE";
    }
    else if(listRows.count()==1)
    {
        row = listRows[0]->row();
    }
    else
    {
        insertNewRow(modelOrderBook,row);
        flagNew =true;
    }
    if(flagNew)
    {
        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Order Id"),
                   QString::number(report.getClOrderId())
                   );


        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Mode"),
                   getOrderModeString(
                       (mutrade::Side)order->getOrderMode()
                       )
                   );


        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Instrument Name"),
                   QString::fromStdString(order->getSymbol())
                   );


        QString instrumentType =  getInstrumentTypeString(
                    (mutrade::InstrumentType)
                    mutrade::Context::getInstance()->getInstrument(
                        order->getSymbol()
                        )->getInstrumentType()
                    );
        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Instrument Type"),
                   instrumentType);

        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Quantity"),
                   QString::number(order->getQuantity())
                   );
        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Price"),
                   QString::number((double)order->getPrice()/100,'f',2)
                   );
        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Exch Order ID"),
                   QString::fromStdString(order->getExchangeOrderId())
                   );
        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Orig Order Id"),
                   QString::number(order->getOrigClOrdId())
                   );

        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Disclosed Order Qty"),
                   QString::number(order->getDisclosedQuantity())
                   );


        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Order Type"),
                   getOrderTypeString(order->getOrderType())
                   );

        updateItem(modelOrderBook,
                   row,
                   tableColumnOrderBook("Validity"),
                   QString::number(order->getOrderValidity())
                   );
    }
    updateItem(modelOrderBook,
               row,
               tableColumnOrderBook("Status"),
               getOrderStatusString(order->getOrderStatus())
               );

    updateItem(modelOrderBook,
               row,
               tableColumnOrderBook("Last Fill Qty"),
               QString::number(report.getLastFillQuantity())
               );
    updateItem(modelOrderBook,
               row,
               tableColumnOrderBook("Last Fill Price"),
               QString::number((double)report.getLastFillPrice()/100,'f',2)
               );
    updateItem(modelOrderBook,
               row,
               tableColumnOrderBook("Total Fill qty"),
               QString::number(order->getFilledQuantity())
               );
    updateItem(modelOrderBook,
               row,
               tableColumnOrderBook("Pending Qty"),
               QString::number(
                   order->getQuantity()
                   -
                   order->getFilledQuantity()
                   )
               );

    updateItem(modelOrderBook,
               row,
               tableColumnOrderBook("Stop Price"),
               QString::number(order->getStopPrice())
               );
}

void SGTables::reloadNetPosition(QString symbol)
{
    LOGGER_INFO("Loading Netpositions for added Symbols");
    updateNetPositions(
                symbol.toStdString(),mutrade::Side_BUY
                );
    updateNetPositions(
                symbol.toStdString(),mutrade::Side_SELL
                );
}

void SGTables::updateTradeBook(const mutrade::Int64 &clOrderId,
                               const mutrade::Int64 &tradeId
                               )
{
    LOGGER_INFO("Updating Trade for Trade-Id:"+QString::number(tradeId));
    std::list<mutrade::Trade*> *tradeList =
            mutrade::Portfolio::getInstance()->getTradeBook().getTrades(clOrderId);
    mutrade::Trade *trade=0;
    bool flagTradeFound = false;
    for(std::list<mutrade::Trade *>::const_reverse_iterator i=tradeList->rbegin();
        i!=tradeList->rend();++i
        )
    {
        trade= *i;
        if(trade->getTradeId()==tradeId)
        {
            flagTradeFound = true;
            break;
        }
    }
    int row =0;
    insertNewRow(modelTradeBook,row);


    updateItem(modelTradeBook,
               row,
               tableColumnTradeBook("Order Id"),
               QString::number(clOrderId)
               );


    updateItem(modelTradeBook,
               row,
               tableColumnTradeBook("Mode"),
               (mutrade::Side)trade->getOrderMode()==mutrade::Side_BUY?
                   "Buy":"Sell"
                   );

    updateItem(modelTradeBook,
               row,
               tableColumnTradeBook("Instrument Name"),
               QString::fromStdString(trade->getInstrument().getInstrumentName())
               );

    updateItem(modelTradeBook,
               row,
               tableColumnTradeBook("Instrument Type"),
               getInstrumentTypeString(trade->getInstrument().getInstrumentType())
               );

    updateItem(modelTradeBook,
               row,
               tableColumnTradeBook("Filled Quantity"),
               QString::number(trade->getFilledQuantity())
               );

    updateItem(modelTradeBook,
               row,
               tableColumnTradeBook("Filled Price"),
               QString::number((double)trade->getFilledPrice()/100,'f',2)
               );

    updateItem(modelTradeBook,
               row,
               tableColumnTradeBook("Exch Order ID"),
               QString::number(trade->getExchangeOrderId())
               );


    updateItem(modelTradeBook,
               row,
               tableColumnTradeBook("Trade Id"),
               QString::number(trade->getTradeId())
               );


    updateItem(modelTradeBook,
               row,
               tableColumnTradeBook("Amount"),
               QString::number(trade->getFilledPrice()*trade->getFilledQuantity())
               );



}

void SGTables::updateNetPositions(const mutrade::String &instrumentName,
                                  const mutrade::Side &side)
{
    LOGGER_INFO("Updating NetPosition for instrument:"+QString::fromStdString(instrumentName));
    mutrade::Instrument in(instrumentName);
    mutrade::Position* _positionA=0;
    try{
        _positionA=
                mutrade::Portfolio::getInstance()->
                getNetPositions().getPosition(in,side);
    }
    catch(std::domain_error &e)
    {
        mutrade::Logger::getInstance()->log(
                    mutrade::WARN,e.what()
                    );
        return;

    }
    mutrade::Position* _positionB=0;
    bool sideBNull=false;
    try{
        _positionB=
                mutrade::Portfolio::getInstance()->
                getNetPositions().getPosition(
                    in,
                    side==mutrade::Side_BUY
                    ?
                        mutrade::Side_SELL
                      :
                        mutrade::Side_BUY
                        );
    }
    catch(std::domain_error &e)
    {
        mutrade::Logger::getInstance()->log(
                    mutrade::WARN,e.what()
                    );
        sideBNull=true;
    }
    QList<QStandardItem *> listRows = modelNetPositions->findItems(
                QString::fromStdString(instrumentName));
    bool flagNew =false;
    int row = 0;
    if(listRows.count()>1)
    {
        std::cout<<"SOME ERROR STATE";
    }
    else if(listRows.count()==1)
    {
        row = listRows[0]->row();
    }
    else
    {
        insertNewRow(modelNetPositions,row);
        flagNew =true;
    }
    if(flagNew)
    {
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Instrument Name"),
                   QString::fromStdString(instrumentName)
                   );

        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Instrument Type"),
                   getInstrumentTypeString(
                       _positionA->getInstrument().getInstrumentType()
                       )
                   );
    }
    else
    {
        if(!sideBNull){
            mutrade::Position* buyPosition = _positionA->getOrderMode()==mutrade::Side_BUY
                    ?
                        _positionA
                      :
                        _positionB;

            mutrade::Position* sellPosition = _positionA->getOrderMode()==mutrade::Side_SELL
                    ?
                        _positionA
                      :
                        _positionB;
            updateItem(modelNetPositions,
                       row,
                       tableColumnNetPosition("Booked PnL"),
                       QString::number(
                           (double)getBookedPnl(
                               buyPosition,
                               sellPosition)/100,
                           'f',2
                           )
                       );
            updateItem(modelNetPositions,
                       row,
                       tableColumnNetPosition("NET Qty"),
                       QString::number(
                           buyPosition->getQuantity()
                           -
                           sellPosition->getQuantity()
                           )
                       );

            //            updateItem(modelNetPositions,
            //                       row,
            //                       tableColumnNetPosition("NET Value"),
            //                       QString::number(
            //                           (
            //                               (mutrade::Int64)sellPosition->getQuantity()*
            //                               (mutrade::Int64)sellPosition->getAveragePrice()
            //                               -
            //                               (mutrade::Int64)buyPosition->getQuantity()*
            //                               (mutrade::Int64)buyPosition->getAveragePrice()
            //                               )/100,'f',2
            //                           )
            //                       );
        }
        else
        {
            updateItem(modelNetPositions,
                       row,
                       tableColumnNetPosition("NET Qty"),
                       QString::number(
                           _positionA->getOrderMode()==mutrade::Side_BUY
                           ?
                               _positionA->getQuantity()
                             :
                               _positionA->getQuantity()*(-1)
                               )
                       );

            //            double netValue=
            //                    (_positionA->getQuantity()*
            //                     _positionA->getAveragePrice())/100;
            //            if(_positionA->getOrderMode()==mutrade::Side_BUY)
            //                netValue=0-netValue;
            //            updateItem(modelNetPositions,
            //                       row,
            //                       tableColumnNetPosition("NET Value"),
            //                       QString::number(netValue,'f',2));

        }

    }
    if(side== mutrade::Side_BUY){
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Buy Qty"),
                   QString::number(_positionA->getQuantity())
                   );
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Avg Buy Price"),
                   QString::number((double)_positionA->getAveragePrice()/100,'f',2)
                   );
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Buy Amount"),
                   QString::number(
                       (double)_positionA->getQuantity()*
                       (double)_positionA->getAveragePrice()/100,
                       'f',
                       2)
                   );

    }
    else{
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Sell Qty"),
                   QString::number(_positionA->getQuantity())
                   );

        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Avg Sell Price"),
                   QString::number((double)_positionA->getAveragePrice()/100,'f',2)
                   );
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Sell Amount"),
                   QString::number(
                       (double)_positionA->getQuantity()*
                       (double)_positionA->getAveragePrice()/100,
                       'f',
                       2)
                   );
    }

    if(side== mutrade::Side_BUY){
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Buy Qty"),
                   QString::number(_positionA->getQuantity())
                   );
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Avg Buy Price"),
                   QString::number((double)_positionA->getAveragePrice()/100,'f',2)
                   );
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Buy Amount"),
                   QString::number(
                       (double)_positionA->getQuantity()*
                       (double)_positionA->getAveragePrice()/100,
                       'f',
                       2)
                   );

    }
    else{
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Sell Qty"),
                   QString::number(_positionA->getQuantity())
                   );

        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Avg Sell Price"),
                   QString::number((double)_positionA->getAveragePrice()/100,'f',2)
                   );
        updateItem(modelNetPositions,
                   row,
                   tableColumnNetPosition("Sell Amount"),
                   QString::number(
                       (double)_positionA->getQuantity()*
                       (double)_positionA->getAveragePrice()/100,
                       'f',
                       2)
                   );
    }



}

void SGTables::updateItem(QStandardItemModel *model, int row, int column, QString text)
{
    QStandardItem *item = model->item(row,column);
    //    if(!item)
    //        item = new QStandardItem();
    item->setData(text,Qt::DisplayRole);
}

void SGTables::slotUpdateOrder(
        const mutrade::ExecutionReport &report)
{
    if(report.getOrderStatus()== mutrade::OrderStatus_PARTIALLY_FILLED
            ||
            report.getOrderStatus()== mutrade::OrderStatus_FILLED
            )
    {
        updateTradeBook(
                    report.getClOrderId(),
                    report.getTradeId()
                    );
        updateNetPositions(
                    report.getInstrumentName(),
                    (mutrade::Side)report.getOrderMode()
                    );
    }
    updateOrderBook(report);
}


int SGTables::tableColumnOrderBook(const QString &header)
{
    return hashOrderBookColumns.value(header);
}

int SGTables::tableColumnNetPosition(const QString &header)
{
    return hashNetPositionsColumns.value(header);
}

void SGTables::insertNewRow(QStandardItemModel *model, int row)
{
    QStandardItem *item;
    model->insertRow(row);
    for(int i=0; i< model->columnCount();i++)
    {
        item = new QStandardItem("");
        model->setItem(row,i,item);
    }
}

QString SGTables::getOrderStatusString(mutrade::OrderStatus status) const
{
    switch(status)
    {
    case mutrade::OrderStatus_PENDING:
        return QString("PENDING");
    case mutrade::OrderStatus_CONFIRMED:
        return QString("CONFIRMED");
    case mutrade::OrderStatus_FILLED: // Same for partial fill and fill
        return QString("FILLED");
    case mutrade::OrderStatus_CANCELED:
        return QString("CANCELED");
    case mutrade::OrderStatus_REPLACED:
        return QString("REPLACED");
    case mutrade::OrderStatus_NEW_REJECTED:
        return QString("NEW_REJECTED");
    case mutrade::OrderStatus_CANCEL_REJECTED:
        return QString("CANCEL_REJECTED");
    case mutrade::OrderStatus_REPLACE_REJECTED:
        return QString("REPLACE_REJECTED");
    case mutrade::OrderStatus_FROZEN:
        return QString("FROZEN");
    case mutrade::OrderStatus_MARKET_TO_LIMIT:
        return QString("MARKET_TO_LIMIT");
    case mutrade::OrderStatus_TRIGGERED:
        return QString("TRIGGERED");
    case mutrade::OrderStatus_PARTIALLY_FILLED:
        return QString("PARTIALLY_FILLED");
    case mutrade::OrderStatus_CANCELED_OF_IOC:
        return QString("CANCELED_OF_IOC");
    case mutrade::OrderStatus_RMS_REJECT: // = 13
        return QString("RMS_REJECT");
    case mutrade::OrderStatus_MAX:
        return QString("INVALID");
    }
    return QString("INVALID");

}


mutrade::Int32 SGTables::getBookedPnl(mutrade::Position *positionA,
                                      mutrade::Position *positionB
                                      )
{
    mutrade::Int64 openQty = positionB->getQuantity()-positionB->getQuantity();
    return openQty*(positionB->getAveragePrice()-positionA->getAveragePrice());
}

int SGTables::tableColumnTradeBook(const QString &header)
{
    return hashTradeBookColumns.value(header);
}
