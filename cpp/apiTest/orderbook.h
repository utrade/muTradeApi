#ifndef ORDERBOOK_H
#define ORDERBOOK_H
#include "types.h"
#include <QDockWidget>

namespace Ui {
class OrderBook;
}
/**
 * @brief The OrderBook class
 */
class OrderBook : public QDockWidget
{
    Q_OBJECT
    
public:
    /**
     * @brief OrderBook
     * @param parent
     */
    explicit OrderBook(QWidget *parent = 0);
    ~OrderBook();
    
private slots:
    /**
     * @brief on_tableView_customContextMenuRequested
     * @param pos
     */
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    /**
     * @brief resizeToContents
     */
    void resizeToContents();

    /**
     * @brief slotCancel
     */
    void slotCancel();
    /**
     * @brief slotModify
     */
    void slotModify();
    /**
     * @brief removeRow
     * @param row
     */
    void removeRow(int row);
private:
    /**
     * @brief ui
     */
    Ui::OrderBook *ui;
    /**
     * @brief selectedOrderId
     */
    mutrade::Int64 selectedOrderId;
};

#endif // ORDERBOOK_H
