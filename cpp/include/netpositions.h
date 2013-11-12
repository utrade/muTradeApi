#ifndef MUTRADEAPI_NET_POSITIONS_H
#define MUTRADEAPI_NET_POSITIONS_H

/* #include "./shared/common.h" */
/* #include "./shared/commands.h" */
/* #include "./shared/response.h" */

#include <map>

#include "types.h"
#include "position.h"
#include "instrument.h"
namespace mutrade {

  /**
   *
   * \brief NetPositions class
   *
   * This class stores the list of all the positions which the client has
   * accumulated through the trading day.
   *
   * \note The trades which happened before the connection was
   * made can be replayed back from the server and this class will then be
   * able to provide the net positions for the day.
   *
   */

  class NetPositions
  {
    /**
     * Positions are stored per Instrument and Side
     */
    typedef std::pair<Instrument, Side> PositionKey;
    typedef std::map<PositionKey, Position*> NetPositionsMap;
    typedef std::map<PositionKey, Position*>::iterator NetPositionsMapIter;
    typedef std::map<PositionKey, Position*>::iterator NetPositionsMapCIter;

    public:

    /**
     * \brief Get Net Positions for an Instrument and Side
     *
     * \param instrument
     * \param side ( BUY/SELL )
     */

    Position* getPosition(Instrument instrument, Side orderMode)
      throw (std::domain_error);

    /**
     * \brief Updates the position in the NetPositions
     *
     * This method updates the positions which are receieved as executions
     * from the exchange.
     *
     * \note The user of the API does not need to call this
     * method. It is called by the API automatically when an execution is
     * received.
     */

    int update(ExecutionReport& report);

    //void insert(Position* position);
    
    private:
    NetPositionsMap _netPositions;

  };

} // namespace mutrade
#endif
