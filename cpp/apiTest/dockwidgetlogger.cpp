#include "dockwidgetlogger.h"
#include "ui_dockwidgetlogger.h"
#include "mainwindow.h"
DockWidgetLogger::DockWidgetLogger(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockWidgetLogger)
{
    ui->setupUi(this);
    ui->listView->setModel(MainWindow::getInstance()->getListModel());
}

DockWidgetLogger::~DockWidgetLogger()
{
    delete ui;
}
