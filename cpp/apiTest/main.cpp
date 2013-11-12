#include "mainwindow.h"
#include <QApplication>
#include <Logger.h>
#include <FileAppender.h>
//#include <ConsoleAppender.h>
#include <stringlistmodelappender.h>
#include <QDir>
#include <QDateTime>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<mutrade::ExecutionReport>("mutrade::ExecutionReport");
    qRegisterMetaType<mutrade::MarketData>("mutrade::MarketData");
    QString logFile = "logs/"+QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss");
    FileAppender* fileAppender = new FileAppender(logFile);
    Logger::registerAppender(fileAppender);
    fileAppender->setFormat("[%t] %m\n");
    fileAppender->setDetailsLevel(Logger::Info);
    MainWindow w;
    QStringListModel *logModel = w.getListModel();
    StringListModelAppender *stringListModelAppender = new StringListModelAppender(logModel);
    stringListModelAppender->setFormat("[%t] %m\n");
    Logger::registerAppender(stringListModelAppender);
    LOGGER_INFO("Starting Application");
    w.show();
    
    return a.exec();
}
