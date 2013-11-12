#ifndef MUTRADEAPI_MARKETDATA_H
#define MUTRADEAPI_MARKETDATA_H

#include "quotes.h"
#include "types.h"
#include "instrument.h"

namespace mutrade
{
  /**
   *
   * \brief MarketData Class
   *
   */

  class MarketData
  {
  public:
    MarketData();

    MarketData(const Quote&);

    Instrument getInstrument() const;

    /****************************/
    /* functions related to LTP */
    /****************************/

    /**
     * \brief Last Traded Price of the Instrument
     *
     * Last Traded Price of the Instrument
     */
    Int32    getLastPrice() const;
    /**
     * \brief Last Traded Quantity of the Instrument
     *
     * Last Traded Quantity of the Instrument
     */

    Int32    getLastQty() const;
    /**
     * \brief Time of last trade
     *
     * Time of last trade
     */
    
    Int32    getLastTime() const;
    /**
     * \brief Total Quantity traded in the day
     *
     * Total Quantity traded in the day. This data may not be provided
     * by all the exchanges.
     */
    Int32    getTotalQty() const;

    /******************************/
    /* functions related to DEPTH */
    /******************************/

    /**
     * \brief Depth available on Bid/Ask side
     *
     * Depth available on Bid/Ask side
     */
    Int32    getDepth(Side side) const;
    /**
     * \brief Get Price available at Rank on Bid/Ask side
     *
     * Get Price available at Rank on Bid/Ask side
     */
    Int32    getPrice(Side side, Int32 rank);
    /**
     * \brief Get Quantity available at Rank on Bid/Ask side
     *
     * Get Quantity available at Rank on Bid/Ask side
     */
    Int32    getQty(Side side, Int32 rank);
    /**
     * \brief Get Rank in Market depth for a particular price
     *
     * Get Rank in Market depth for a particular price
     */
    Int32    getRank(Side side, Int32 price) const;
    /**
     * \brief get Order count at Bid/Ask side
     *
     * Get Order count at Bid/Ask side. This data may not be available
     * for all exchanges.
     */
    Boolean   getCount(Side side, Int32 rank) const;

    /*******************************************/
    /* some calculations                       */
    /* - is the quantity available             */
    /* - get average price for given qty       */
    /* - get qty available at an average price */
    /* - get worst price for given qty         */
    /* - get qty available at an worst price   */
    /*******************************************/

    /**
     * \brief Check if a particular qty is available at Bid/Ask side
     *
     * Check if a particular qty is available at Bid/Ask side
     */
    Boolean   hasQty(Side side, Int32 qty) const;
    /**
     * \brief Get Best average price for a particular quantity
     *
     * Get Best average price available for a particular quantity
     */
    Int32    getAvgPrice(Side side, Int32 qty) const;
    /**
     * \brief Get maximum quantity available at Average Price
     *
     * Get Maximum Quantity which is available on Bid/Ask side
     * at specified Average Price or better.
     */
    Int32    getQtyForAvgPrice(Side side, Int32 avgPrice) const;
    /**
     * \brief Get average price for a particular quantity
     *
     * Get Average Price for a particular quantity which is
     * available on Bid/Ask side
     */
    Int32    getAvgPriceForQty(Side side, Int32 qty) const;
    /**
     * \brief Get maximum quantity at worstPrice or better
     *
     * Get Maximum Quantity which is available on Bid/Ask side
     * for Worst Price or better.
     */
    Int32    getQtyForWorstPrice(Side side, Int32 worstPrice) const;
    /**
     * \brief Get Worst price for a particular quantity
     *
     * Get Worst Price which is available on Bid/Ask side
     * for a particular quantity
     */
    Int32    getWorstPriceForQty(Side side, Int32 qty) const;

    /**********************/
    /* functions for OHLC */
    /**********************/

    /**
     * \brief Get Day's Open Price
     *
     * Get Day's Open Price
     */
    Int32    getDayOpen() const;
    /**
     * \brief Get Day's High Price
     *
     * Get Day's High Price
     */
    Int32    getDayHigh() const;
    /**
     * \brief Get Day's Low Price
     *
     * Get Day's Low Price
     */
    Int32    getDayLow() const;
    /**
     * \brief Get Previous Day's Close Price
     *
     * Get Previous Day's Close Price
     */
    Int32    getDayClose() const;
  
  private:
    const Quote    _quote;
    Instrument     _instrument;
  };

}

#endif /* #ifdef MUTRADE_MARKETDATA_H */
