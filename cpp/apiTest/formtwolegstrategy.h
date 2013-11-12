#ifndef FORMTWOLEGSTRATEGY_H
#define FORMTWOLEGSTRATEGY_H

#include <QWidget>
#include "testStrategy.h"
namespace Ui {
class FormtwoLegStrategy;
}

/**
 * @brief The FormtwoLegStrategy class
 */
class FormtwoLegStrategy : public QWidget
{
    Q_OBJECT
    
public:
    /**
     * @brief FormtwoLegStrategy
     * @param parent
     */
    explicit FormtwoLegStrategy(QWidget *parent = 0);
    ~FormtwoLegStrategy();

private slots:
    /**
     * @brief on_pushButton_clicked
     */
    void on_pushButton_clicked();
    /**
     * @brief on_pushButtonStart_clicked
     */
    void on_pushButtonStart_clicked();
    /**
     * @brief on_pushButtonStop_clicked
     */
    void on_pushButtonStop_clicked();

private:
    /**
     * @brief ui
     */
    Ui::FormtwoLegStrategy *ui;
    /**
     * @brief sd
     */
    mutrade::SymbolData *sd;
    /**
     * @brief initStrategy
     */
    void initStrategy();

};

#endif // FORMTWOLEGSTRATEGY_H
