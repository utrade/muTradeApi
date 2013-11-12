#include "tradebook.h"
#include "ui_tradebook.h"
#include "sgtables.h"
tradeBook::tradeBook(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::tradeBook)
{
    ui->setupUi(this);
    ui->tableView->setModel(SGTables::getInstance()->getTradeBookModel());
    ui->tableView->resizeColumnsToContents();
}

tradeBook::~tradeBook()
{
    delete ui;
}
