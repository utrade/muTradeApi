/*
 * (C) 2013 uTrade Solutions Pvt Ltd. All Rights reserved.
 */

#ifndef MUTRADEAPI_CONTEXT_H
#define MUTRADEAPI_CONTEXT_H

#include "types.h"
#include "logger.h"
#include "application.h"
#include "order.h"

namespace mutrade
{
  namespace detail
  {
    class ContextImpl;
  }

  class Application;
  
  /**
   *
   * \brief Context class for the algorithm.
   *
   * This class contains the event engine for the applicaton and does
   * the actual communication with the muTrade server. Application object
   * containing the trading logic is associated with this class.
   * Also, this class is used to tweak various parameters, which are global
   * to the application.
   *
   */

  class Context
  {
  public:

    /**
     *
     * \brief Login to muTrade server with with userId and password
     *
     * \param \c userId to login with
     * \param \c password for user
     * \param \c host ip 
     * \param \c port of host
     * \param \c restore previous trade
     *
     */
    void          login( Int32 userId,
                         const String& password,
                         String host, 
                         Int16 port, 
                         bool restoreState = false );

    /**
     *
     * \brief Logout from the muTrade server
     *
     */
    void          logout();

    /**
     *
     * \brief Send an order to the muTrade server
     *
     */
    bool          placeOrder(const mutrade::Order& order);

    /**
     *
     * \brief Enable logging of various events.
     *
     * \param \c logging level for how much log we want to generate  
     *
     * These logs also go to syslog on Linux/UNIX and to Event Log on Windows
     *
     */
    void          enableLogging(LogLevel level=INFO);

    /**
     *
     * \brief Susbscribe market data for a particular instrument
     *
     * \c loadInstrument must be called for the string before calling 
     * this method.
     *
     */
    void          subscribe(const Instrument& t);
    
    /**
     *
     * \brief Unsusbscribe market data for a previously subscribed instrument
     *
     * \c loadInstrument must be called for the string before calling 
     * this method.
     *
     */
    void          unsubscribe(const Instrument& t);

    /**
     *
     * \brief Load static data for an instrument from the muTrade server
     *
     */
    void          loadInstrument(const String& s);

    /**
     *
     * \brief Get static data for a particular instrument using symbol
     * \c loadInstrument must be called for the string before calling 
     * this method.
     *
     */
    Instrument*   getInstrument(const String& t) const;

    /**
     *
     * \brief Get an Instance of the Context class
     *
     * Context class is a Singleton class, which will have only one instance.
     * This instance can be accessed using the \c getInstance method.
     *
     */
    static Context* getInstance()
    {
      static Context c;
      return &c;
    }

    /**
     *
     * \brief Get the instance of ContextImpl class
     */
    detail::ContextImpl* getContextImpl();

    /**
     *
     * \brief Get the instance of Application class
     */
    Application* getApplication() ;

  private:
    Context();
    Context(const Context& c);

    detail::ContextImpl*  _impl;
    Application*          _app;
  };

}

#endif /* #ifdef MUTRADE_CONTEXT_H */
