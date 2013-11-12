#ifndef PROCESSAPIRESPONSES_H
#define PROCESSAPIRESPONSES_H
#include "application.h"
#include "types.h"
#include <QObject>
//#include "mainwindow.h"
#include <QSet>
namespace mutrade{


class ProcessApiResponses : public QObject, public mutrade::Application
{
    /**
     * \class ProcessApiResponses
     * \brief Class derived from mutrade::Application,
     * overriding the virtual methods
     */

    Q_OBJECT
private:
    /**
     * @brief logonStatus
     */
    bool logonStatus;
    /**
     * @brief clientId
     */
    Int32 clientId;
    /**
     * @brief _instance
     */
    static ProcessApiResponses *_instance;
    /**
     *
     *\brief Constructor,
     *It registers its object to the mutrade::Context instance,
     *to recieve event callbacks
     *
    */
    ProcessApiResponses();
    /**
     * @brief loadedInstrumentList
     */
    QSet<QString> loadedInstrumentList;
    /**
     * @brief instrumentLoadReqList
     */
    QSet<QString> instrumentLoadReqList;

public:
    ~ProcessApiResponses();
    /**
     * @brief getClientId
     * @return
     */
    Int32 getClientId();
    /**
     * @brief setClientId
     * @param clientId
     */
    void setClientId(Int32 clientId){this->clientId = clientId;}
    /**
     * @brief getLoginStatus
     * @return
     */
    bool getLoginStatus();
    /**
     * @brief getInstance
     * @return
     */
    static ProcessApiResponses *getInstance();
    /**
     * @brief onTick
     * @param mktData
     */
    void onTick(const MarketData &mktData);
    /**
     * @brief onLogin
     * @param status
     */
    void onLogin(bool status);
    /**
     * @brief onLogout
     * @param status
     */
    void onLogout(bool status);
    /**
     * @brief onExecutionReport
     * @param report
     */
    void onExecutionReport(ExecutionReport &report);
    /**
     * @brief onLoadInstrumentEnd
     * @param instrumentName
     * @param success
     */
    void onLoadInstrumentEnd(const String instrumentName, bool success);
    /**
     * @brief getLoadedInstrumentList
     * @return
     */
    QList<QString> getLoadedInstrumentList(){
        return loadedInstrumentList.toList();
    }
    /**
     * @brief loadInstruments
     * @param instruments
     */
    void loadInstruments(QStringList instruments);

signals:
    /**
     * @brief onLoginSuccess
     */
    void onLoginSuccess();
    /**
     * @brief onLogoutResponse
     * @param loginFailure
     */
    void onLogoutResponse(bool loginFailure);
    /**
     * @brief updateOrder
     */
    void updateOrder(mutrade::ExecutionReport);
    /**
     * @brief replayNetPosition
     * @param instrumentName
     */
    void replayNetPosition(QString instrumentName);
    /**
     * @brief signalTick
     */
    void signalTick(mutrade::MarketData);
};
}//namespace muTrade
#endif // PROCESSAPIRESPONSES_H
