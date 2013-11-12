#include "dockwidgetstrategy.h"
#include "ui_dockwidgetstrategy.h"
#include "testStrategy.h"
#include <QtGui>
#include <QMessageBox>
DockWidgetStrategy* DockWidgetStrategy::_instance=0;
DockWidgetStrategy::DockWidgetStrategy(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockWidgetStrategy)
{
    ui->setupUi(this);
    apiResponse = mutrade::ProcessApiResponses::getInstance();
    strategyContext = mutrade::TestStrategy::getInstance();

    connect(mutrade::TestStrategy::getInstance(),
            SIGNAL(updateIV(double,double,double)),
            this,
            SLOT(slotUpdateIVVal(double,double,double))
            );

}

DockWidgetStrategy::~DockWidgetStrategy()
{


    delete ui;
}

DockWidgetStrategy *DockWidgetStrategy::getInstance(QWidget *parent)
{
    if(!_instance)
    {
        _instance = new DockWidgetStrategy(parent);

    }
    return _instance;
}

bool DockWidgetStrategy::validateForm()
{

    return true;

}

mutrade::StrategyParams DockWidgetStrategy::getParams()
{
    mutrade::StrategyParams params;
    params.buyIV = ui->doubleSpinBoxbuySpread->value();
    params.sellIV = ui->doubleSpinBoxSellSpread->value();
    params.orderLots = ui->spinBoxOrderLots->value();
    params.refSymbolName = ui->tradeSymbolUnderlying->currentText().toStdString();
    params.tradeSymbolName = ui->comboBoxSymbol->currentText().toStdString();
    return params;
}


void DockWidgetStrategy::on_pushButtonLogin_clicked()
{
    //    mutrade::TestStrategy *sg= new mutrade::TestStrategy::getInstance();
}

void DockWidgetStrategy::on_pushButtonStartSG_clicked()
{
    if(strategyContext->startStrategy(getParams()))
    {
        QMessageBox::information(this,
                                 "muTrade ApiTest",
                                 "Strategy Launched"
                                 );
    }
    else
        QMessageBox::information(this,
                                 "muTrade ApiTest",
                                 "Strategy Launch failed"
                                 );

}

void DockWidgetStrategy::on_comboBox_activated(const QString &arg1)
{

}

//void DockWidgetStrategy::on_pushButtonLoadSymbol_clicked()
//{
//    if(!apiResponse->getLoginStatus())
//        return;
//    mutrade::String tradeSymbolString =(mutrade::String)
//            ui->comboBoxSymbol->currentText().toStdString();
//    mutrade::String refSymbolString =(mutrade::String)
//            ui->tradeSymbolUnderlying->currentText().toStdString();

//    strategyContext->loadTradeInstrument(tradeSymbolString);
//    strategyContext->loadRefInstrument(refSymbolString);
//}

void DockWidgetStrategy::slotUpdateIVVal(double bidIvVal, double askIvVal,double lastIvVal)
{
    ui->ivVal->setText(QString::number(lastIvVal*100,'f',4));
    ui->labelAskIv->setText(QString::number(askIvVal*100,'f',4));
    ui->labelBidIv->setText(QString::number(bidIvVal*100,'f',4));
}

void DockWidgetStrategy::on_pushButtonReload_clicked()
{
    ui->comboBoxSymbol->clear();
    ui->comboBoxSymbol->addItems(
                mutrade::ProcessApiResponses::getInstance()->
                getLoadedInstrumentList()
                );
    ui->tradeSymbolUnderlying->clear();
    ui->tradeSymbolUnderlying->addItems(
                mutrade::ProcessApiResponses::getInstance()->
                getLoadedInstrumentList()
                );
}

void DockWidgetStrategy::on_pushButtonStopSG_clicked()
{
    mutrade::TestStrategy::getInstance()->stopStrategy();
}
