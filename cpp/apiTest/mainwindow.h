#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialoglogin.h"
#include <QThread>
#include "sgtables.h"
#include "orderbook.h"
#include "tradebook.h"
#include "netpositions.h"
#include "buysell.h"
#include <QStringListModel>
#include "dockwidgetlogger.h"
#include "formtwolegstrategy.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startApp();
    void loginClient();
    static MainWindow* getInstance(){return _instance; }
    BuySell *getBuySell(){return _buySell;}
    QStringListModel* getListModel();
    void logInfo(const mutrade::String &msg);
    void logError(const mutrade::String &msg);
private slots:
    void on_actionLogin_triggered();
    void on_actionLogout_triggered();
    void onLoginSuccess();
    void onLogout(bool loginFail=false);
    void on_actionOrder_Book_triggered();
    void on_actionTrade_Book_triggered();
    void on_actionNet_Positions_triggered();
    void on_actionAdd_Symbols_triggered();
    void on_actionManual_Order_triggered();
    void on_actionLogs_triggered();
    void on_actionTwo_Leg_Arbitrage_triggered();

private:
    static MainWindow *_instance;
    QThread loginThread;
    DialogLogin *login;
    Ui::MainWindow *ui;
    OrderBook *_orderBook;
    NetPositions *_netPositions;
    tradeBook *_tradeBook;
    BuySell *_buySell;
    DockWidgetLogger *_dockLogWindow;
    bool postInitLogin;
    QStringListModel *logListModel;
    FormtwoLegStrategy *_twoLegForm;
    void onLogin();

};

#endif // MAINWINDOW_H
