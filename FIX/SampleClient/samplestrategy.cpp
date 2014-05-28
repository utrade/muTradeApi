#include "samplestrategy.h"
#include <string>
#include <quickfix/fix42/MarketDataSnapshotFullRefresh.h>

SampleStrategy::SampleStrategy()
    :AbstractStrategy(AlgoType_SampleBidding)
{
}

void AbstractStrategy::registerClOrderId(const std::string &orderId)
{
    StrategyDriver::getInstance()->registerStrategyClOrderId(orderId, this);
}

void AbstractStrategy::registerMktData(const FIX42::MarketDataRequest &marketDataRequest)
{
    StrategyDriver::getInstance()->registerStrategyForMktData(this,marketDataRequest);
}

void StrategyDriver::onExecutionReport(const FIX42::ExecutionReport &report)
{
    try{
      std::string fixId = report.getField(FIX::FIELD::ClOrdID);

        ClOrderIdStrategyMapIter iter = _clOrderIdStrategyMap.find(fixId);
        if(iter!= _clOrderIdStrategyMap.end()){
          boost::shared_ptr<AbstractStrategy> sg = iter->second;
            sg->onExecutionReport(report);
        }
    }
    catch(...)
    {
        std::cerr<< "Exception on fwding ExecutionReport to strategy"<<std::endl;
    }
}

void StrategyDriver::onMarketDataSnampshot(const FIX42::MarketDataSnapshotFullRefresh &report)
{
    std::cout<<"MarketDataRecvd"
      <<std::endl;
    
    std::string marketDataSignature = 
      Application::getMarketDataUniqueSignature((const FIX42::Message *)&report);

    std::cout<<"signature:"<<marketDataSignature<<std::endl;
    for(MktDataSgMapIter iter = _mktDataSgMap[marketDataSignature].begin(); 
        iter != _mktDataSgMap[marketDataSignature].end();
        iter++)
    {
      std::cout<<"sending\n";
      boost::shared_ptr<AbstractStrategy> sg = *iter;
        sg->onMarketData(report);
    }

}

void AbstractStrategy::getMarketData(const FIX42::MarketDataSnapshotFullRefresh &fixMarketData ,
    MarketData *marketData,
    const int &size,
    const FIX::Side &watchSide)
{
  FIX::NoMDEntries noMdEntries;
  fixMarketData.get(noMdEntries);
  
  int index = 0;
  for(int i = 1;i<= noMdEntries;i++)
  {
    FIX42::MarketDataSnapshotFullRefresh::NoMDEntries group;
    FIX::MDEntryType MDEntryType;
    FIX::MDEntryPx MDEntryPx;
    FIX::MDEntrySize MDEntrySize;

    fixMarketData.getGroup(i,group);

    group.get(MDEntryType);
    group.get(MDEntryPx);
    group.get(MDEntrySize);

    if(
        (MDEntryType == '0'/*BID*/ && watchSide ==  FIX::Side_BUY) ||
        (MDEntryType == '1'/*OFFER*/ && watchSide ==  FIX::Side_SELL)
      )
    {
      marketData[index]._price = MDEntryPx;
      marketData[index++]._qty = MDEntrySize;

      if(index >= size)
        break;
    }

  }


}

std::string StrategyDriver::genClOrderId()
{
  std::ostringstream oss;
  
  static int orderIdCounter = 1;
  const int maxOrdersPerSecond = 10000;
  const int digitsForSeconds = 100000;
  struct timeval timestamp;
  // Take lock
  //   pthread_mutex_lock( &uniqueOrderIdMutex );
  gettimeofday(&timestamp, NULL);
  long orderId = timestamp.tv_sec % digitsForSeconds +
    orderIdCounter * digitsForSeconds;
  orderIdCounter++;
  if (orderIdCounter == maxOrdersPerSecond)
    orderIdCounter = 0;
  oss<<orderId;

  return oss.str();

}
