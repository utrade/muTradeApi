#ifndef MUTRADEAPI_MARKETDATA_SUBSCRIPTION_H
#define MUTRADEAPI_MARKETDATA_SUBSCRIPTION_H

#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>
#include <common/mclBroadcastMessages.h>

namespace mutrade {

  class MarketDataSubscription
  {
    typedef std::set<int> UserSet;
    /**
     * Map of
     * Symbol -> Users Set
     */
    typedef boost::unordered_map<long,
                                 UserSet>
    SymbolUserSetMap;

  public:

    void subscribe(int userId,
                   long symbolId);

    void unsubscribe(int userId,
                     long symbolId);

    bool isSubscribed(int userId,
                      long symbolId);

    static MarketDataSubscription* getInstance()
    {
      static MarketDataSubscription instance;
      return &instance;
    }

  private:

    MarketDataSubscription()
    { }

    MarketDataSubscription(const MarketDataSubscription& )
    { }

    SymbolUserSetMap _symbolUserMap;
    boost::mutex _mutex;
  };
#if 0
  class MDStorage
  {
    typedef boost::unordered_map< long,
                                  Quote* >
    SymbolQuoteMap;

  public:
    void save( const Quote& );
    Quote get( long symbolId );

    static MDStorage* getInstance()
    {
      static MDStorage instance;
      return &instance;
    }

  private:
    MDStorage()
    { }
    MDStorage( const MDStorage& );

    SymbolQuoteMap _symbolQuoteMap;
  };
#endif
}

#endif
