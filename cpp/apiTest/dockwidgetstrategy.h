#ifndef DOCKWIDGETSTRATEGY_H
#define DOCKWIDGETSTRATEGY_H

#include <QDockWidget>
#include "processapiresponses.h"
#include "testStrategy.h"

namespace Ui {
class DockWidgetStrategy;
}

class DockWidgetStrategy : public QDockWidget
{
    Q_OBJECT
    
public:
    ~DockWidgetStrategy();
    static DockWidgetStrategy *getInstance(QWidget *parent = 0);
    
private slots:

    void on_pushButtonLogin_clicked();

    void on_pushButtonStartSG_clicked();


    void on_comboBox_activated(const QString &arg1);

    //    void on_pushButtonLoadSymbol_clicked();

    void slotUpdateIVVal(double bidIvVal, double askIvVal, double lastIvVal);


    void on_pushButtonReload_clicked();

    void on_pushButtonStopSG_clicked();

private:
    static DockWidgetStrategy *_instance;
    mutrade::ProcessApiResponses *apiResponse;
    explicit DockWidgetStrategy(QWidget *parent = 0);
    Ui::DockWidgetStrategy *ui;
    bool validateForm();
    mutrade::TestStrategy *strategyContext;
    //    mutrade::StrategyParams getParams();

};

#endif // DOCKWIDGETSTRATEGY_H
