#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processapiresponses.h"
#include <QtGui>
#include <QMessageBox>
#include <testStrategy.h>
#include <QtConcurrent/QtConcurrentRun>
#include <QFileDialog>
#include <Logger.h>

MainWindow *MainWindow::_instance=0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    logListModel = 0;
    postInitLogin =false;
    _instance = this;
    _twoLegForm =0; _orderBook=0; _tradeBook =0; _netPositions=0;_buySell=0;
    connect(mutrade::ProcessApiResponses::getInstance(),
            SIGNAL(onLoginSuccess()),
            this,
            SLOT(onLoginSuccess())
            );
    connect(mutrade::ProcessApiResponses::getInstance(),
            SIGNAL(onLogoutResponse(bool)),
            this,
            SLOT(onLogout(bool))
            );
    mutrade::ProcessApiResponses::getInstance();
    login = new DialogLogin(this);
    onLogin();
}
void MainWindow::startApp(){
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete _orderBook;
    delete _tradeBook;
    delete _netPositions;
    delete login;
    delete _buySell;
    delete ui;
    delete _instance;
    delete _twoLegForm;
}
void MainWindow::onLoginSuccess()
{
    if(!postInitLogin){
        ui->setupUi(this);
        postInitLogin=true;
        this->showMaximized();
    }
    LOGGER_INFO("Logged In");
    QMessageBox::information(
                this,
                "muTrade API Test",
                "Login succesful"
                );
    LOGGER_INFO("Setting up widgets");
    on_actionTwo_Leg_Arbitrage_triggered();
    on_actionOrder_Book_triggered();
    on_actionTrade_Book_triggered();
    on_actionNet_Positions_triggered();
    on_actionLogs_triggered();
    tabifyDockWidget(_orderBook,_tradeBook);
    tabifyDockWidget(_tradeBook,_netPositions);
    tabifyDockWidget(_netPositions,_dockLogWindow);
}

void MainWindow::onLogout(bool loginFail)
{
    LOGGER_INFO(loginFail?"Login Failed":"Logged out");
    QMessageBox::information(
                this,
                "muTrade API Test",
                loginFail?"Login Failed":"Logged out"
                          );
    exit(1);

}

void MainWindow::on_actionLogin_triggered()
{
    onLogin();
}



void MainWindow::on_actionLogout_triggered()
{
    login->reqLogout();

}

void MainWindow::loginClient()
{
    login->reqLogin(login->getUser(),
                    login->getPswd(),true);

}

QStringListModel *MainWindow::getListModel()
{
    if(!logListModel)
    {
        logListModel = new QStringListModel();

    }
    return logListModel;
}


void MainWindow::logInfo(const mutrade::String &msg)
{
    LOGGER_INFO(QString::fromStdString(msg));

}
void MainWindow::logError(const mutrade::String &msg)
{
    LOGGER_ERROR(QString::fromStdString(msg));
}


void MainWindow::on_actionOrder_Book_triggered()
{
    if(!_orderBook)
    {
        _orderBook = new OrderBook(this);
        setDockNestingEnabled(true);
        _orderBook->setAllowedAreas(Qt::AllDockWidgetAreas);
        addDockWidget(Qt::BottomDockWidgetArea,_orderBook);
    }
    _orderBook->show();
    _orderBook->raise();
}

void MainWindow::on_actionTrade_Book_triggered()
{
    if(!_tradeBook)
    {
        _tradeBook = new tradeBook(this);
        _tradeBook->setAllowedAreas(Qt::AllDockWidgetAreas);
        addDockWidget(Qt::BottomDockWidgetArea,_tradeBook);
    }
    _tradeBook->show();
    _tradeBook->raise();
}

void MainWindow::on_actionNet_Positions_triggered()
{
    if(!_netPositions)
    {
        _netPositions = new NetPositions(this);
        _netPositions->setAllowedAreas(Qt::AllDockWidgetAreas);
        addDockWidget(Qt::BottomDockWidgetArea,_netPositions);
    }
    _netPositions->show();
    _netPositions->raise();
}


void MainWindow::onLogin()
{

    if(login->exec()){
        QtConcurrent::run(this,&MainWindow::loginClient);
        mutrade::ProcessApiResponses::getInstance()->setClientId(login->getUser());
    }
    else
        exit(1);
}


void MainWindow::on_actionAdd_Symbols_triggered()
{
    QFileDialog openFileDialog(this);
    openFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    openFileDialog.setFileMode(QFileDialog::AnyFile);
    openFileDialog.setNameFilter(tr("CSV (*.csv)"));
    openFileDialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (QDialog::Accepted == openFileDialog.exec())
    {
        fileNames = openFileDialog.selectedFiles();
        QFile* file = new QFile(fileNames.at(0));
        if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
#ifdef DEBUG
            qDebug()<<"error reading file";
#endif
            return;
        }
        QTextStream in(file);
        QStringList instrumentList;
        while(!in.atEnd())
        {
            instrumentList.append(in.readLine());
        }
        file->close();
        delete file;
        mutrade::ProcessApiResponses::getInstance()->loadInstruments(instrumentList);
    }
}

void MainWindow::on_actionManual_Order_triggered()
{
    if(!_buySell)
    {
        _buySell = new BuySell();
        _buySell->setAllowedAreas(Qt::NoDockWidgetArea);
    }
    _buySell->show();
    _buySell->raise();

}

void MainWindow::on_actionLogs_triggered()
{
    if(!_dockLogWindow)
    {
        _dockLogWindow = new DockWidgetLogger();
        _dockLogWindow->setAllowedAreas(Qt::AllDockWidgetAreas);
        addDockWidget(Qt::BottomDockWidgetArea,_dockLogWindow);
    }
    _dockLogWindow->show();
    _dockLogWindow->raise();
}

void MainWindow::on_actionTwo_Leg_Arbitrage_triggered()
{
    if(!_twoLegForm)
    {
        _twoLegForm = new FormtwoLegStrategy();
    }
    _twoLegForm->show();
    _twoLegForm->raise();
}
