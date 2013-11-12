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
    virtual ~Application();

    /**
     * \brief Event called when a tick is received
     *
     * \param
     */
    //virtual void onTick(const MarketData &) = 0;
    virtual void onTick(const MarketData &);


    /**
     * \brief Event called when Login message is returned
     *
     * \param
     */
    //virtual void onLogin(bool status) = 0;
    virtual void onLogin(bool status);

    /**
     * \brief Event called when Logout message is returned
     *
     * \param
     */
    //virtual void onLogout(bool status) = 0;
    virtual void onLogout(bool status);

    /**
     * \brief Event called when an execution is received from Server
     *
     * \param
     */
    //virtual void onExecutionReport(ExecutionReport& report) = 0;
    virtual void onExecutionReport(ExecutionReport& report);
    /**
     * \brief Event called when instrument is loaded from the backend
     *
     * \param
     */
    //virtual void onLoadInstrumentEnd(bool success) = 0;
    virtual void onLoadInstrumentEnd(const String instrumentName, bool success);

  };

}

#endif
