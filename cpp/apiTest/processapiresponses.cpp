#include "processapiresponses.h"
#include "testStrategy.h"
#include "mainwindow.h"

namespace mutrade{
ProcessApiResponses* ProcessApiResponses::_instance =0;
ProcessApiResponses::ProcessApiResponses()
{
    clientId =0;
    Context::getInstance()->setApplication(this);
    SGTables::initModels();

}

ProcessApiResponses::~ProcessApiResponses()
{
    delete _instance;
}

Int32 ProcessApiResponses::getClientId()
{
    return clientId;
}

bool ProcessApiResponses::getLoginStatus()
{
    return logonStatus;
}

ProcessApiResponses* ProcessApiResponses::getInstance()
{
    if(!_instance)
    {
        _instance = new ProcessApiResponses();
    }
    return _instance;
}

void ProcessApiResponses::onTick(const MarketData & mktData)
{
    emit signalTick(mktData);
}


void ProcessApiResponses::onLogin(bool status)
{
    logonStatus = status;
    if(status)
        emit onLoginSuccess();
    //        MainWindow::getInstance()->onLoginSuccess();
    else
        emit onLogoutResponse(true);
    //    mainwindow->startApp();
}

void ProcessApiResponses::onLogout(bool status)
{
    logonStatus = !status;
    if(status)
        emit onLogoutResponse(false);
}

void ProcessApiResponses::onExecutionReport(ExecutionReport &report)
{
    emit updateOrder(report);
}

void ProcessApiResponses::onLoadInstrumentEnd(const String instrumentName, bool success)
{
    if(success){
        instrumentLoadReqList.remove(QString::fromStdString(instrumentName));
        loadedInstrumentList.insert(QString::fromStdString(instrumentName));
        mutrade::Context::getInstance()->subscribe(
                    *(mutrade::Context::getInstance()->getInstrument(instrumentName))
                    );
        emit replayNetPosition(QString::fromStdString(instrumentName));
    }
}

void ProcessApiResponses::loadInstruments(QStringList instruments)
{
    foreach(QString instrument, instruments){
        if(!loadedInstrumentList.contains(instrument)){
            instrumentLoadReqList.insert(instrument);
            Context::getInstance()->loadInstrument(instrument.toStdString());
        }
    }
}
}//mutrade namespace
