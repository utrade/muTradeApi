#ifndef BUYSELL_H
#define BUYSELL_H
#include <types.h>
#include <QDockWidget>

namespace Ui {
class BuySell;
}

/**
 * @brief The BuySell class
 */
class BuySell : public QDockWidget
{
    Q_OBJECT
    
public:
    /**
     * @brief BuySell
     * @param parent
     */
    explicit BuySell(QWidget *parent = 0);
    /**
     * @brief modifyOrder
     * @param orderId
     */
    void modifyOrder(mutrade::Int64 orderId);
    ~BuySell();
    
    /**
     * @brief setEnableItems
     * @param status
     */
    void setEnableItems(bool status);
private slots:
    /**
     * @brief on_radioButton_clicked
     */
    void on_radioButton_clicked();

    /**
     * @brief on_radioButton_toggled
     * @param checked
     */
    void on_radioButton_toggled(bool checked);

    /**
     * @brief on_pushButton_clicked
     */
    void on_pushButton_clicked();

    /**
     * @brief populateSymbolList
     */
    void populateSymbolList();
    /**
     * @brief on_pushButton_3_clicked
     */
    void on_pushButton_3_clicked();
    /**
     * @brief on_pushButton_2_clicked
     */
    void on_pushButton_2_clicked();

private:
    /**
     * @brief clOrderId
     */
    mutrade::Int64 clOrderId;
    /**
     * @brief ui
     */
    Ui::BuySell *ui;

};

#endif // BUYSELL_H
