#include "buysell.h"
#include "ui_buysell.h"
#include "processapiresponses.h"
#include "portfolio.h"
#include "sgtables.h"
#include "mainwindow.h"
BuySell::BuySell(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::BuySell)
{
    ui->setupUi(this);
    on_radioButton_toggled(true);
    clOrderId=0;
}
void BuySell::setEnableItems(bool status)
{
    ui->comboBoxInstrumentName->setEnabled(status);
    ui->comboBoxValidity->setEnabled(status);
    ui->radioButton->setEnabled(status);
    ui->radioButton_2->setEnabled(status);


}
void BuySell::modifyOrder(mutrade::Int64 orderId)
{
    mutrade::Order *order =
            mutrade::Portfolio::getInstance()->getOrderBook().getOrder(orderId);
    ui->comboBoxInstrumentName->setCurrentIndex(
                ui->comboBoxInstrumentName->findText(
                    QString::fromStdString(order->getSymbol())
                    )
                );
    ui->comboBoxValidity->setCurrentIndex(
                ui->comboBoxValidity->findText(
                    SGTables::getInstance()->getOrderValidityString(order->getOrderValidity())
                    )
                );
    ui->spinBoxQty->setValue(order->getQuantity());
    ui->doubleSpinBoxPrice->setValue(order->getPrice()/100);
    clOrderId = orderId;
    if(order->getOrderMode()==mutrade::Side_SELL)
    {
        if(ui->radioButton->isChecked())
            ui->radioButton->toggle();
    }
    ui->comboBoxOrderType->setCurrentIndex(
                ui->comboBoxOrderType->findText(
                    order->getOrderType()==mutrade::OrderType_MARKET?
                        "Market":"Limit"));

    setEnableItems(false);
}

BuySell::~BuySell()
{
    delete ui;
}

void BuySell::on_radioButton_clicked()
{

}

void BuySell::on_radioButton_toggled(bool checked)
{
    if(checked){
        this->setStyleSheet(QString::fromUtf8("QWidget#BuySell\n"
                                              "{\n"
                                              "    background-color: #639632  "
                                              "}\n"
                                              ""));
    }
    else
        this->setStyleSheet(QString::fromUtf8("QWidget#BuySell\n"
                                              "{\n"
                                              "    background-color: #cd5c5c"
                                              "}\n"
                                              ""));
}

void BuySell::on_pushButton_clicked()
{
    if(!mutrade::ProcessApiResponses::getInstance()->getLoginStatus())
        return;

    if(ui->comboBoxInstrumentName->currentIndex()==-1)
        return;
    mutrade::Order orderInstance;
    orderInstance.setClientId(
                mutrade::ProcessApiResponses::getInstance()->getClientId()
                );
    orderInstance.setClOrdId(clOrderId);
    orderInstance.setDisclosedQuantity(ui->spinBoxQty->value());
    orderInstance.setSymbol(ui->comboBoxInstrumentName->currentText().toStdString());
    if(clOrderId==0)
    {
        orderInstance.setTransactionType(mutrade::TransactionType_NEW);;
        orderInstance.setOrderStatus(mutrade::OrderStatus_PENDING);
    }
    else
    {
        orderInstance.setTransactionType(mutrade::TransactionType_MODIFY);
    }
    orderInstance.setQuantity(ui->spinBoxQty->value()
                              *
                              mutrade::Context::getInstance()->getInstrument(
                                  ui->comboBoxInstrumentName->currentText().trimmed().toStdString()
                                  )->getLotSize()
                              );
    mutrade::OrderType type;
    if(ui->comboBoxOrderType->currentText().trimmed()=="Market")
        type=mutrade::OrderType_MARKET;
    else
        type=mutrade::OrderType_LIMIT;
    orderInstance.setOrderType(type);
    mutrade::Side mode;
    if(ui->radioButton->isChecked())
        mode = mutrade::Side_BUY;
    else
        mode= mutrade::Side_SELL;
    orderInstance.setOrderMode(mode);

    mutrade::TimeInForce validity;
    if(ui->comboBoxValidity->currentIndex()==0)
        validity = mutrade::TimeInForce_DAY;
    else
        validity = mutrade::TimeInForce_IOC;
    orderInstance.setOrderValidity(validity);
    orderInstance.setPrice(ui->doubleSpinBoxPrice->value()*100);
    try{
        mutrade::Context::getInstance()->placeOrder(orderInstance);
        if(clOrderId!=0){
            MainWindow::getInstance()->logInfo("Modifying Order with OrderId: "+QString::number(clOrderId).toStdString());
            clOrderId=0;
            on_pushButton_2_clicked();
        }
        else{
            if(ui->radioButton->isChecked())
                MainWindow::getInstance()->logInfo("Placing new Buy Order for : "+ ui->comboBoxInstrumentName->currentText().toStdString());
            else
                MainWindow::getInstance()->logInfo("Placing new Sell Order for : "+ ui->comboBoxInstrumentName->currentText().toStdString());
        }
    }
    catch(std::domain_error &e)
    {
        MainWindow::getInstance()->logError(e.what());
    }
}

void BuySell::populateSymbolList()
{
    ui->comboBoxInstrumentName->clear();
    ui->comboBoxInstrumentName->addItems(
                mutrade::ProcessApiResponses::getInstance()->getLoadedInstrumentList()
                );
}

void BuySell::on_pushButton_3_clicked()
{
    populateSymbolList();
}

void BuySell::on_pushButton_2_clicked()
{
    setEnableItems(true);
    ui->doubleSpinBoxPrice->setValue(0);
    ui->comboBoxOrderType->setCurrentIndex(0);
    ui->comboBoxValidity->setCurrentIndex(0);
    ui->spinBoxQty->setValue(1);
    clOrderId =0;
}
