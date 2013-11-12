#ifndef NETPOSITIONS_H
#define NETPOSITIONS_H

#include <QDockWidget>

namespace Ui {
class NetPositions;
}

class NetPositions : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit NetPositions(QWidget *parent = 0);
    ~NetPositions();
    
private:
    Ui::NetPositions *ui;
};

#endif // NETPOSITIONS_H
