#include "formtwolegstrategy.h"
#include "ui_formtwolegstrategy.h"
#include "processapiresponses.h"
FormtwoLegStrategy::FormtwoLegStrategy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormtwoLegStrategy)
{
    ui->setupUi(this);
    initStrategy();
}

FormtwoLegStrategy::~FormtwoLegStrategy()
{
    delete ui;
}

void FormtwoLegStrategy::initStrategy()
{
    sd = new mutrade::SymbolData();
    sd->firstLegSymbol = "";
    sd->secondLegSymbol = "";
    sd->minProfit = 0;
    sd->avgPriceFirstLeg = 0;
    sd->avgPriceSecondLeg = 0;
    sd->firstLegSide = mutrade::Side_SELL;
    sd->secondLegSide = mutrade::Side_BUY;
    sd->bidPlaced = false;
    sd->firstLegMode = 1;
    sd->secondLegMode = -1;
    sd->orderPriceFirstLeg = 0;
    sd->qty = 0;
    sd->firstLegOrder = new mutrade::Order();
    sd->secondLegOrder = new mutrade::Order();
    sd->filledQtyFirstLeg = 0;
    sd->filledQtySecondLeg = 0;
    sd->isOrderPending = false;

}

void FormtwoLegStrategy::on_pushButton_clicked()
{
    ui->comboBox1LSymbol->clear();
    ui->comboBox2LSymbol->clear();
    foreach(QString symbol,mutrade::ProcessApiResponses::getInstance()->getLoadedInstrumentList() )
    {
        ui->comboBox1LSymbol->addItem(symbol);
        ui->comboBox2LSymbol->addItem(symbol);
    }
}

void FormtwoLegStrategy::on_pushButtonStart_clicked()
{
    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
    initStrategy();
    sd->firstLegSymbol = ui->comboBox1LSymbol->currentText().toStdString();
    sd->secondLegSymbol = ui->comboBox2LSymbol->currentText().toStdString();
    sd->minProfit = ui->doubleSpinBoxMinProfit->value()*100;
    sd->avgPriceFirstLeg = 0;
    sd->avgPriceSecondLeg = 0;
    sd->firstLegSide = ui->comboBox1LSide->currentIndex()==1
            ?
                mutrade::Side_SELL
              :
                mutrade::Side_BUY
                ;
    sd->secondLegSide = ui->comboBox2LSide->currentIndex()==1
            ?
                mutrade::Side_SELL
              :
                mutrade::Side_BUY
                ;
    sd->bidPlaced = false;
    sd->firstLegMode = ui->comboBox1LSide->currentIndex()==1
            ?
                1:
                -1;
    sd->secondLegMode = ui->comboBox1LSide->currentIndex()==1
            ?
                1:
                -1;
    sd->orderPriceFirstLeg = 0;
    sd->qty = ui->spinBoxLots->value();
    sd->firstLegOrder = new mutrade::Order();
    sd->secondLegOrder = new mutrade::Order();
    sd->filledQtyFirstLeg = 0;
    sd->filledQtySecondLeg = 0;
    sd->isOrderPending = false;
    mutrade::TestStrategy::getInstance()->startStrategy(sd);
}

void FormtwoLegStrategy::on_pushButtonStop_clicked()
{
    mutrade::TestStrategy::getInstance()->stopStrategy();
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);
}
