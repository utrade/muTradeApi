/*
 * (C) 2013 uTrade Solutions Pvt Ltd. All Rights reserved.
 */

/*
 * Sample stragey based on mutrade API.
 */


#include <application.h>
#include <cstdlib>

using namespace mutrade;

/*
 * Structure to store strategy state and legs info.
 */
struct SymbolData
{
  String      firstLegSymbol;
  String      secondLegSymbol;
  Int32       minProfit;
  Int32       avgPriceFirstLeg;
  Int32       avgPriceSecondLeg;
  OrderMode   firstLegSide;
  OrderMode   secondLegSide;
  Boolean     bidPlaced;
  Int16       firstLegMode;
  Int16       secondLegMode;
  Int32       orderPriceFirstLeg;
  Int32       qty;
  Order       *firstLegOrder;
  Order       *secondLegOrder;
  Int32       filledQtyFirstLeg;
  Int32       filledQtySecondLeg;
  Boolean     isOrderPending;
};


SymbolData *sd;
mutrade::Context* context;


void firstLegBid()
{
  std::cout << "\n**************** In firstLegBid ***************\n" << std::endl;
  sd->firstLegOrder->setTransactionType(mutrade::TransactionType_NEW);  
  sd->firstLegOrder->setSymbol(sd->firstLegSymbol);
  sd->firstLegOrder->setOrderMode(sd->firstLegSide);
  sd->firstLegOrder->setQuantity(sd->qty);
  sd->firstLegOrder->setPrice(sd->orderPriceFirstLeg);
  sd->firstLegOrder->setSecurityType(mutrade::InstrumentType_FUTURE);
  sd->firstLegOrder->setDisclosedQuantity(sd->qty);
  sd->firstLegOrder->setOrderValidity(mutrade::TimeInForce_DAY);
  sd->firstLegOrder->setOrderType(mutrade::OrderType_LIMIT);
  sd->firstLegOrder->setOrderStatus(mutrade::OrderStatus_PENDING);
  context->getInstance()->placeOrder(*(sd->firstLegOrder));

}

void modifyOrder()
{
  std::cout << "**************** In Modify Order *************** " << std::endl;
  sd->firstLegOrder->setTransactionType(mutrade::TransactionType_MODIFY);  
  sd->firstLegOrder->setSymbol(sd->firstLegSymbol);
  sd->firstLegOrder->setOrderMode(sd->firstLegSide);
  sd->firstLegOrder->setQuantity(sd->qty);
  sd->firstLegOrder->setPrice(sd->orderPriceFirstLeg);
  sd->firstLegOrder->setSecurityType(mutrade::InstrumentType_FUTURE);
  sd->firstLegOrder->setDisclosedQuantity(sd->qty);
  sd->firstLegOrder->setOrderValidity(mutrade::TimeInForce_DAY);
  sd->firstLegOrder->setOrderType(mutrade::OrderType_LIMIT);
  sd->firstLegOrder->setOrderStatus(mutrade::OrderStatus_PENDING);
  sd->firstLegOrder->setOrigClOrdId(
          sd->firstLegOrder->getOrigClOrdId());
  sd->firstLegOrder->setClOrdId(sd->firstLegOrder->getClOrdId());
  //sd->firstLegOrder->dumpOrder();
  context->getInstance()->placeOrder(*(sd->firstLegOrder));
}

void cancelFirstLeg()
{
  std::cout << "Cancelling the order................" << std::endl;
  sd->firstLegOrder->setTransactionType(mutrade::TransactionType_CANCEL);  
  sd->firstLegOrder->setSymbol(sd->firstLegSymbol);
  sd->firstLegOrder->setOrderStatus(mutrade::OrderStatus_PENDING);
  sd->firstLegOrder->setOrigClOrdId(
          sd->firstLegOrder->getOrigClOrdId() );
  sd->firstLegOrder->setClOrdId(
          sd->firstLegOrder->getClOrdId());
  context->getInstance()->placeOrder(*(sd->firstLegOrder));
}


void placeSecondLegMarket()
{
  std::cout << "**************** In Leg-2 Market *************** " << std::endl;
  sd->secondLegOrder->setTransactionType(mutrade::TransactionType_NEW);  
  sd->secondLegOrder->setSymbol(sd->secondLegSymbol);
  sd->secondLegOrder->setOrderMode(sd->secondLegSide);
  sd->secondLegOrder->setQuantity(sd->qty);
  sd->secondLegOrder->setPrice(0);
  sd->secondLegOrder->setSecurityType(mutrade::InstrumentType_FUTURE);
  sd->secondLegOrder->setDisclosedQuantity(sd->qty);
  sd->secondLegOrder->setOrderValidity(mutrade::TimeInForce_DAY);
  sd->secondLegOrder->setOrderType(mutrade::OrderType_MARKET);
  sd->secondLegOrder->setOrderStatus(mutrade::OrderStatus_PENDING);
  context->getInstance()->placeOrder(*(sd->secondLegOrder));

}


/*
 *  Function to calculate average price for given qty from 
 *  current market depth.
 */
bool getAveragePrice( mutrade::MarketData md, 
                      int qty, 
                      mutrade::Side side,
                      int &avgPrice ) 
{
  mutrade::Int64 tempPrice = 0;
  mutrade::Int64 tempQty = 0;
  for(int i=0; i<5; ++i)
  {
    mutrade::Int64 p = md.getPrice(side, i+1);
    mutrade::Int64 q = md.getQty(side, i+1);
    
    tempPrice += p * q; 
    
    tempQty += q;
    if(tempQty >= qty)
      break;
  }
  if(tempQty < qty)
  {
    avgPrice = 0;
    return false;
  }
  avgPrice = tempPrice / tempQty;
  std::cout << "avgPrice " << avgPrice << std::endl;
  return true;
}

/*
 *  Overridding onTick method of Application class.
 */
void Application::onTick(const MarketData& md)
{
  std::cout << "Got Market data Event " << std::endl;
	
  std::cout << " Symbol name is " 
					  << md.getInstrument().getInstrumentName() 
					  << std::endl;
  
  if(sd->isOrderPending)
  {
    std::cout << "Orders are pending no need to react on this tick."
              << std::endl;
  }
  else
  {
    if(md.getInstrument().getInstrumentName() == sd->firstLegSymbol)
    {
      getAveragePrice( static_cast<mutrade::MarketData> (md), 
                       sd->qty, 
                       (mutrade::Side)sd->firstLegSide, 
                       sd->avgPriceFirstLeg
                      );
      
      std::cout <<  "First leg avg price : " 
                << sd->avgPriceFirstLeg << std::endl;
    }
    else if(md.getInstrument().getInstrumentName() == sd->secondLegSymbol)
    {
      getAveragePrice( static_cast<mutrade::MarketData> (md), 
                       sd->qty, 
                       (mutrade::Side)sd->secondLegSide, 
                       sd->avgPriceSecondLeg
                     );

      std::cout <<  "Second leg avg price : " 
                << sd->avgPriceSecondLeg 
                << std::endl;
    }
    else
    {
      std::cout << "Unknown symbol tick." << std::endl;
    }

    if(sd->avgPriceSecondLeg>0)
    {
      sd->orderPriceFirstLeg = 
            (sd->minProfit * sd->firstLegMode) 
            + (sd->avgPriceSecondLeg);

      std::cout << "Bid Price Leg -1 " 
                << sd->orderPriceFirstLeg
                << std::endl;
      
      if( !sd->bidPlaced )
      {
        firstLegBid();
        sd->bidPlaced = true;
        sd->isOrderPending = true;
      }
      else if(  sd->bidPlaced && 
                sd->firstLegOrder->getOrderStatus() 
                            != mutrade::OrderStatus_PENDING )
      {
        modifyOrder();
        sd->bidPlaced = true;
        sd->isOrderPending = true;
      }
      else
      {
        if(sd->avgPriceFirstLeg != 0)
          std::cout << "Cannot upadte order as previous order is pending at exchange."
                    << std::endl;
      }
    }
    else if( sd->avgPriceSecondLeg == 0)
    {
      if(sd->bidPlaced)
      {
        std::cout << "Creating cancel order for first Leg." 
                  << std::endl;
        cancelFirstLeg();
      }
    }
  }

}

void Application::onLogin(const Boolean status)
{
  std::cout << "Got on Logon Event with status "
            << int (status) << std::endl;

  if(status == 0)
  {
    std::cout << "Fatal Error!!!" << std::endl;
    exit(1);
  }

  mutrade::Context::getInstance()->loadInstrument(sd->firstLegSymbol);
  mutrade::Context::getInstance()->loadInstrument(sd->secondLegSymbol);


}


void Application::onLogout(const Boolean status)
{
  std::cout << "Got on Logout Event with status "
            << int (status) << std::endl;
}

/******************************************************
 * User need to keep track of clOrdId bcz             *
 * in case of modification and cancel backend         *
 * retrives info based on this id.                    *
 *****************************************************/
void Application::onExecutionReport(ExecutionReport &report)
{
  std::cout << "Got on onreportutionReport Event."<< std::endl;
	report.dump();
  if(report.getInstrumentName() == sd->firstLegSymbol)
  {
    if(report.getOrderStatus() == mutrade::OrderStatus_CONFIRMED)
    {
      sd->firstLegOrder->setClOrdId(report.getClOrderId());
      sd->firstLegOrder->setOrigClOrdId(report.getOriginalClOrderId());
      sd->firstLegOrder->setOrderStatus(mutrade::OrderStatus_CONFIRMED);
      sd->isOrderPending = false;
    }
    else if(report.getOrderStatus() == mutrade::OrderStatus_REPLACED)
    {
      sd->firstLegOrder->setClOrdId(report.getClOrderId());
      sd->firstLegOrder->setOrigClOrdId(report.getOriginalClOrderId());
      sd->firstLegOrder->setOrderStatus(mutrade::OrderStatus_CONFIRMED);
      sd->isOrderPending = false;
    }
    else if(report.getOrderStatus() == mutrade::OrderStatus_FILLED)
    {
      placeSecondLegMarket();  
      sd->isOrderPending = true;
    }
  }
  else if(report.getInstrumentName() == sd->secondLegSymbol)
  {
    if(report.getOrderStatus() == mutrade::OrderStatus_CONFIRMED)
    {
      sd->secondLegOrder->setClOrdId(report.getClOrderId());
      sd->secondLegOrder->setOrigClOrdId(report.getOriginalClOrderId());
      sd->isOrderPending = false;  
    }
    else if(report.getOrderStatus() == mutrade::OrderStatus_FILLED)
    {
      std::cout << "Strategy completed successfully." << std::endl;
    }
  }


}

/****************************************************************
 * Instrument must be loaded before subscription for data feed. *
 ***************************************************************/
void Application::onLoadInstrumentEnd( 
          const String  instrumentName,
          Boolean       success 
      )
{
  std::cout << "Instrument " << instrumentName 
            << " Loaded."    << std::endl;

  Instrument *inst =  
    mutrade::Context::getInstance()->getInstrument(instrumentName);

  mutrade::Context::getInstance()->subscribe(*inst);
  delete inst;

}


void init()
{
  sd->firstLegSymbol = "NSE INFY 20131031";
  sd->secondLegSymbol = "NSE ACC 20131031";
  sd->minProfit = 274000;
  sd->avgPriceFirstLeg = 0;
  sd->avgPriceSecondLeg = 0;
  sd->firstLegSide = mutrade::OrderMode_SELL;
  sd->secondLegSide = mutrade::OrderMode_BUY;
  sd->bidPlaced = false;
  sd->firstLegMode = 1;
  sd->secondLegMode = -1;
  sd->orderPriceFirstLeg = 0;
  sd->qty = 250;
  sd->firstLegOrder = new mutrade::Order();
  sd->secondLegOrder = new mutrade::Order();
  sd->filledQtyFirstLeg = 0;
  sd->filledQtySecondLeg = 0;
  sd->isOrderPending = false;
}





int main()
{
  mutrade::String host = "10.0.0.31", pass = "pass2@123";
	mutrade::Int32 user = 14;
	mutrade::Int16 port  = 8286;
  
  sd = new SymbolData();
  init();

	context = mutrade::Context::getInstance();
	context->login(user, pass, host, port, true);
	std::cout << "Login Error " << std::endl;

  return 0;
}

