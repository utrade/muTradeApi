#ifndef TRADEBOOK_H
#define TRADEBOOK_H

#include <QDockWidget>

namespace Ui {
class tradeBook;
}

/**
 * @brief The tradeBook class
 */
class tradeBook : public QDockWidget
{
    Q_OBJECT
    
public:
    /**
     * @brief tradeBook
     * @param parent
     */
    explicit tradeBook(QWidget *parent = 0);
    ~tradeBook();
    
private:
    /**
     * @brief ui
     */
    Ui::tradeBook *ui;
};

#endif // TRADEBOOK_H
