/*
 * (C) 2013 uTrade Solutions Pvt Ltd. All Rights reserved.
 */

#ifndef MUTRADEAPI_INSTRUMENT_H
#define MUTRADEAPI_INSTRUMENT_H

#include <string>
//#include "detail/instrumentImpl.h"

namespace mutrade
{

  /**
   *
   * \brief Instrument class
   *
   * \description
   * Instrument can be generated from a string identifier
   * which uniquely identifies a particular string. This string
   * takes the following format:
   *
   * \b Equities
   * <tt>"<exchange> <symbol> <series, if any>"</tt> <br>
   * <tt>"<exchange> <security-id>"</tt>
   *
   * \b Futures
   * <tt>"<exchange> <symbol> <maturity-date>"</tt> <br>
   * <tt>"<exchange> <symbol> <year-month, if single contract>"</tt>
   *
   * \b Options
   * <tt>"<exchange> <symbol> <maturity-date> <strike> <call/put>"</tt>
   *
   * For example:
   *   \li \c "NSE SBIN EQ"           equity with symbol and series
   *   \li \c "BSE 500112"            equity with BSE scrip-code (SBI)
   *   \li \c "BSE SBI A"             equity with symbol and series
   *   \li \c "NSE SBIN 20130926"     future listed on NSE with expiry date
   *                                  in format YYYY-MM-DD
   *   \li \c "BSE SBI 26SEP2013"     future on BSE with expiry date
   *                                  in format DDMONYYYY
   *   \li \c "NSE SBIN 26SEP2013 145000 C"  SBIN Call option on NSE with strike
   *                                         price of 1450.00
   *   \li \c "NSE SBIN 20130926 145000 P"   SBIN Put option on NSE with strike
   *                                         price of 1450.00
   *
   */
	namespace detail
	{
		class InstrumentImpl;
	}

  class Instrument
  {
  public:
    
    /**
     * \brief Create an instrument from string identifier
     */
    Instrument();
    
    /**
     * \brief Create an instrument from string identifier
     */
    Instrument(const std::string& identifier);
    
    /**
     * \brief Get Type of instrument (STOCK/FUTURE/OPTION)
     */
    InstrumentType     getInstrumentType() const;
    
    /**
     * \brief Get Expiry Date of the instrument (for FUTURE/OPTION)
     */
    //std::string        getExpiryDate() const;
    
    /**
     * \brief Get Strike Price of the option (for OPTIONs)
     */
    Int64              getStrikePrice() const;
    
    /**
     * \brief Get Type of the option - CALL / PUT (for OPTIONs)
     */
    OptionType         getOptionType() const;
      
	/**
     * \brief Get Series of instrument.
     */
    String              getSeries() const;              
          
    /**
     * \brief Get Lot Size of the instrument (for FUTURE/OPTION)
     */
    Int32               getLotSize() const;
      
    /**
     * \brief Get Tick Size for instrument
     */
    Int32               getTickSize() const;


    bool    operator< (const Instrument &rhs) const;

    bool    operator== (const Instrument &rhs) const;

    /**
     * \brief Get Instrument  name as string.
     */
    String getInstrumentName();
  
    ~Instrument();
  private:
   
    String    _instrumentName;
      
    detail::InstrumentImpl* _impl;
  };

}

#endif /* #ifdef MUTRADE_INSTRUMENT_H */
