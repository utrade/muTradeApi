#ifndef MUTRADEAPI_APPLICATION_H
#define MUTRADEAPI_APPLICATION_H

#include "marketdata.h"
#include "context.h"

namespace mutrade
{

  /**
   *
   * \mainpage muTrade API documentation
   *
   * \section Introduction
   * This is an early release of the muTrade API, which exposes the core trading
   * functionalities and allows the developer to write an event driven
   * trading algorithm.
   *
   * \note This version of API is still experimental and the functionality/interface
   * may break in the future versions of API.
   *
   * \b Code \b Flow
   * @n 1) Application developer has to override the virtual methods of Application class.
   * @n 2) Register your overridden Application class to API using \ref setApplication function. 
   * @n 3) Call \ref login function. Once user is logged in, application developer has to 
   *         control its flow from the overridden Application class. 
   * @n 4) In \ref onLogin user has to call \ref loadInstrument.[User must load the instrument 
   *         before using it.]
   * @n 5) In \ref onLoadInstrumentEnd user should call \ref subscribe function in order to get 
   *         live ticks/quotes from the server. 
   * @n 6) For every subscribed symbol user will get an event \ref onTick.
   * @n 7) Based on the ticks user can place their order using \ref placeOrder.
   * @n 8) For every placed order user will get an event \ref onExecutionReport.
   * 
   * @n \ref OrderBook, \ref TradeBook and \ref NetPositions can be accessed from the \ref Portfolio 
   * class.
   *
   */


  /**
   * \class Application
   * \brief Abstract Application class, to be overridden by the developer.
   *
   * Application is the base abstract class. An application developer, using
   * muTrade API needs to inherit from this class and override the virtual
   * methods of this class.
   */


  class Context;

  class Application
  {
  public:


    virtual ~Application(){};

    /**
     * \anchor onTick
     * \brief Event called when a tick is received
     *
     * \param 
     */
    virtual void onTick(const MarketData &) = 0;


    /**
     * \anchor onLogin
     * \brief Event called when Login message is returned
     *
     * \param
     */
    virtual void onLogin(bool status) = 0;

    /**
     * \anchor onLogout
     * \brief Event called when Logout message is returned
     *
     * \param
     */
    virtual void onLogout(bool status) = 0;

    /**
     * \anchor onExecutionReport
     * \brief Event called when an execution is received from Server
     *
     * \param
     */
    virtual void onExecutionReport(ExecutionReport& report);
    
    /**
     * \anchor onLoadInstrumentEnd
     * \brief Event called when instrument is loaded from the back-end
     *
     * \param
     */
    virtual void onLoadInstrumentEnd(const String instrumentName, bool success)=0;

  };

}

#endif
