#include "netpositions.h"
#include "ui_netpositions.h"
#include "sgtables.h"
NetPositions::NetPositions(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::NetPositions)
{
    ui->setupUi(this);
    ui->tableView->setModel(SGTables::getInstance()->getNetPositionsModel());
    ui->tableView->resizeColumnsToContents();
}

NetPositions::~NetPositions()
{
    delete ui;
}
