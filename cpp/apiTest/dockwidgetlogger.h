#ifndef DOCKWIDGETLOGGER_H
#define DOCKWIDGETLOGGER_H

#include <QDockWidget>

namespace Ui {
class DockWidgetLogger;
}

/**
 * @brief The DockWidgetLogger class
 */
class DockWidgetLogger : public QDockWidget
{
    Q_OBJECT
    
public:
    /**
     * @brief DockWidgetLogger
     * @param parent
     */
    explicit DockWidgetLogger(QWidget *parent = 0);
    ~DockWidgetLogger();
    
private:
    /**
     * @brief ui
     */
    Ui::DockWidgetLogger *ui;
};

#endif // DOCKWIDGETLOGGER_H
