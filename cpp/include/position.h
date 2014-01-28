#ifndef MUTRADEAPI_POSITION_H
#define MUTRADEAPI_POSITION_H

#include "types.h"
#include <cstring>
#include "instrument.h"
namespace mutrade {

  /**
   *
   * \brief Position class
   *
   * This class is required for \c NetPositions class.
   *
   * \note We need to create an object of type Position before aclling 
   * NetPosition. 
   *
   */
  class Position {

  public:
    /**
     *
     * \brief Position 
     *
     * \param \c Instrument for which we will keep track of postion.
     */ 
    Position(Instrument &instrument);

    /**
     *
     * \brief Position class copy constructor.
     *
     * \param \c Position object to copy.
     */ 
    Position(const Position&);
    
    /**
     *
     * \brief Initialize class members with default values.
     */
    void initialize();
    
    /**
     *
     * \brief Overloaded comparison operator, in order to insetr postions in
     * NetPostion map.
     */
    bool operator <(const Position &rhs) const;
   
    /**
     * \brief Get total quantity for current postion. 
     */ 
    Int32           getQuantity() { return _quantity; }

    /**
     * \brief Get Average Price for current postion. 
     */ 
    Int32           getAveragePrice() { return _averagePrice; }

    /**
     * \brief Get Instrument from current postion. 
     */ 
    Instrument      getInstrument() { return _instrument; }
    
    /**
     * \brief Get Side of current postion. 
     */ 
    Side            getOrderMode() { return _orderMode; }

    /**
     * \brief Set total quantity for current postion. 
     */ 
    void setQuantity(Int32 val) { _quantity = val; }
    
    /**
     * \brief Set Average Price for current postion. 
     */ 
    void setAveragePrice(Int32 val) { _averagePrice = val; }

    /**
     * \brief Set Instrument from current postion. 
     */ 
    void setInstrument(Instrument val) { _instrument = val; }
    
    /**
     * \brief Set Side of current postion. 
     */ 
    void setOrderType(Side val) { _orderMode = val; }
  private: 

    Instrument      _instrument;
    Side            _orderMode;
    Int32           _quantity;
    Int64           _averagePrice;
  };

} // namespace mutrade

#endif
