#ifndef MUTRADEAPI_PORTFOLIO_H
#define MUTRADEAPI_PORTFOLIO_H

#include <boost/lexical_cast.hpp>
#include "orderbook.h"
#include "tradebook.h"
#include "netpositions.h"
#include "order.h"
#include "types.h"

#include <map>

#include <stdint.h>
#include <string>

namespace mutrade {

  /**
   * \anchor Portfolio
   * \class Portfolio
   * \brief Portfolio class
   *
   * This class contains the portfolio for the trader/algorithm. Portfolio
   * class provides OrderBook, TradeBook and Net Positions for the trader.
   *
   */
  typedef std::map<UInt32, mutrade::Order*> TokenMap;
  typedef std::map<UInt32, mutrade::Order*>::iterator TokenMapItr;

  class Portfolio
  {

  public:
    /**
     *
     * \brief Get an Instance of the Portfolio class
     *
     * Portfolio class is singleton class, which will have only one instance.
     * This instance can be accessed using the \c getInstance method.
     *
     */ 
    static Portfolio* getInstance()
    {
      if(!_instance)
         _instance = new Portfolio;
      return _instance;
    }

    /**
     * \brief Insterts the order in the Portfolio
     *
     * \note The user of the API does not need to call this
     * method. It is called by the API automatically when an execution is
     * received.
     */
    void insert(mutrade::Order* order);
 
    /**
     * \brief Handle Response from the server
     *
     * \note The user of the API does not need to call this
     * method. It is called by the API automatically when an execution is
     * received.
     */   
    void handleResponse(mutrade::ExecutionResponse* rsp);
    
    /**
     * \brief Handle Confirmations from the server
     *
     * \note The user of the API does not need to call this
     * method. It is called by the API automatically when an execution is
     * received.
     */   
    void handleConfirmations(mutrade::ExecutionReport *conf, 
                             UNSIGNED_SHORT responseCategory);
    
      /**
     * \brief Gets Order From TokenId 
     *
     * \note The user of the API does not need to call this
     * method. It is called by the API automatically when an execution is
     * received.
     */   
    mutrade::Order* getOrderByTokenId(Int32 tokenId);

   /**
     *
     * \brief Get cumulative Net Positions
     *
     */
    NetPositions&  getNetPositions();

    /**
     *
     * \brief Get Order Book (list of all the orders placed)
     *
     */
    OrderBook&     getOrderBook();

    /**
     *
     * \brief Get Trade Book (list of all the trades placed)
     *
     */
    TradeBook&     getTradeBook();

  private:
    static Portfolio* _instance;
    Portfolio();
    Portfolio(const Portfolio& ) { }
    
    //To be filled while Placing Order for first time 
    //and wait for first Order Confirmation
    TokenMap     _tokenMap; 
    OrderBook    _orderBook;
    TradeBook    _tradeBook;
    NetPositions _netPosition;

  };

} // namespace mutrade

#endif
