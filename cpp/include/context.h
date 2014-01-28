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
     * \anchor login
     *
     * \brief Login to muTrade server with with userId and password
     *
     * \param  userId to login with
     * \param  password for user
     * \param  host ip 
     * \param  port of host
     * \param  restore previous trade
     *
     */
    void          login( Int32 userId,
                         const String& password,
                         String host, 
                         Int16 port, 
                         bool restoreState = false );

    /**
     * \anchor logout
     * \brief Logout from the muTrade server
     *
     */
    void          logout();

    /**
     * \anchor placeOrder
     * \brief Send an order to the muTrade server.
     * 
     * \param  order \ref Order  
     * 
     * Before placing the oder user need to set the order 
     * with these informations.
     *
     * For \b New \b Order 
     * \n \ref TransactionType to TransactionType_NEW.
     * \n Symbol Name with Instrument name.
     * \n Order Mode with \ref OrderMode.
     * \n Order Quantity. It must be muliple of lot size.
     * \n Order Price. Try to set it in multiple of tick size.
     * \n Orfer Validity to \ref TimeInForce.
     * \n Disclosed Quantity as order qty.
     * \n Order Type with \ref OrderType.
     * \n Order Status to OrderStatus_PENDING.
     * \n Security Type to \ref InstrumentType.
     *
     *\n\n
     * For \b Modify \b Order
     * \n \ref TransactionType to TransactionType_MODIFY.
     * \n Order Quantity.
     * \n Order Price. Try to set it in multiple of tick size.
     * \n Orfer Validity to \ref TimeInForce.
     * \n Order Type with \ref OrderType.
     * \n Order Status to OrderStatus_PENDING.
     * \n setClOrderId to ClOrdId of previous order.
     *
     * \n\n
     * For \b Cancel \b Order
     * \n \ref TransactionType to TransactionType_CANCEL.
     * \n Order Status to OrderStatus_PENDING.
     * \n Symbol Name with Instrument name.
     * \n setClOrderId to ClOrdId of previous order.
     *
     */
    bool          placeOrder(const mutrade::Order& order);

    /**
     * \anchor enableLogging
     * \brief Enable logging of various events.
     *
     * \param \c logging level for how much log we want to generate  
     *
     * These logs also go to syslog on Linux/UNIX and to Event Log on Windows
     *
     */
    void          enableLogging(LogLevel level=INFO);

    /**
     * \anchor subscribe
     * \brief Susbscribe market data for a particular instrument
     *
     * \ref loadInstrument must be called for the string before calling 
     * this method.
     *
     */
    void          subscribe(const Instrument& t);
    
    /**
     * \anchor unsubscribe
     * \brief Unsusbscribe market data for a previously subscribed instrument
     *
     * \ref loadInstrument must be called for the string before calling 
     * this method.
     *
     */
    void          unsubscribe(const Instrument& t);

    /**
     * \anchor loadInstrument
     * \brief Load static data for an instrument from the muTrade server
     *
     */
    void          loadInstrument(const String& s);

    /**
     * \anchor getInstrument
     * \brief Get static data for a particular instrument using symbol
     * \ref loadInstrument must be called for the string before calling 
     * this method.
     *
     */
    Instrument*   getInstrument(const String& t) const;

    /**
     * \anchor getInstance
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

    /**
     * \anchor setApplication
     * \brief Set the instance of Application class. 
     * User need to resigster it's derived  application 
     * class to context. User must call this function before
     * login.
     */
    void setApplication(Application *) ;

  private:
    Context();
    Context(const Context& c);

    detail::ContextImpl*  _impl;
    Application*          _app;
  };

}

#endif /* #ifdef MUTRADE_CONTEXT_H */
