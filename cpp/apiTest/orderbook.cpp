#include "orderbook.h"
#include "ui_orderbook.h"
#include "sgtables.h"
#include <context.h>
#include <QMenu>
#include "mainwindow.h"
OrderBook::OrderBook(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::OrderBook)
{
    ui->setupUi(this);
    ui->tableView->setModel(SGTables::getInstance()->getOrderBookModel());
    ui->tableView->resizeColumnsToContents();
    connect(SGTables::getInstance(),
            SIGNAL(hideOrderBookRow(int)),
            this,
            SLOT(removeRow(int))
            );
}

OrderBook::~OrderBook()
{
    delete ui;
}

void OrderBook::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    QPoint globalPos = ui->tableView->viewport()->mapToGlobal(pos);
    menu.addAction("Resize To Contents", this, SLOT(resizeToContents()));
    int row = ui->tableView->indexAt(pos).row();
    if(SGTables::getInstance()->getOrderBookModel()->rowCount()>0
            &&
            row < SGTables::getInstance()->getOrderBookModel()->rowCount()
            && row!=-1
            )
    {
        selectedOrderId = SGTables::getInstance()->getOrderBookModel()->item(
                    row,
                    SGTables::getInstance()->tableColumnOrderBook("Order Id")
                    )->text().toInt();
        mutrade::Order* orderSelected= mutrade::Portfolio::getInstance()->getOrderBook().getOrder(
                    selectedOrderId);
        switch(orderSelected->getOrderStatus())
        {
        case mutrade::OrderStatus_CONFIRMED:
        case mutrade::OrderStatus_PARTIALLY_FILLED:
            menu.addAction("Cancel Order",this, SLOT(slotCancel()));
            menu.addAction("Modify Order",this, SLOT(slotModify()));
            break;
        default:
            break;
        }
    }
    menu.exec(globalPos);
}

void OrderBook::resizeToContents()
{
    ui->tableView->resizeColumnsToContents();
}

void OrderBook::slotCancel()
{
    mutrade::Order *order =mutrade::Portfolio::getInstance()->getOrderBook().getOrder(selectedOrderId);
    order->setTransactionType(mutrade::TransactionType_CANCEL);
    mutrade::Context::getInstance()->placeOrder(*order);
}

void OrderBook::slotModify()
{
    mutrade::Order *order =mutrade::Portfolio::getInstance()->getOrderBook().getOrder(selectedOrderId);
    MainWindow::getInstance()->getBuySell()->modifyOrder(order->getClOrdId());
    MainWindow::getInstance()->getBuySell()->show();
    MainWindow::getInstance()->getBuySell()->raise();
}

void OrderBook::removeRow(int row)
{
    ui->tableView->setRowHidden(row,true);
}
