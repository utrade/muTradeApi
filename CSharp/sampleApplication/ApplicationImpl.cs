using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace sampleApplication
{
    class ApplicationImpl:Application
    {
        Data _data;
        public ApplicationImpl(Data d)
        {
            _data = d;
        }

        public void firstLegBid()
        {
            Console.WriteLine("\n**************** In firstLegBid ***************\n");
            _data.firstLegOrder.setTransactionType(TransactionType.TransactionType_NEW);
            _data.firstLegOrder.setSymbol(_data.firstLegSymbol);
            _data.firstLegOrder.setOrderMode(_data.firstLegSide);
            _data.firstLegOrder.setQuantity(_data.qty);
            _data.firstLegOrder.setPrice(_data.orderPriceFirstLeg);
            _data.firstLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
            _data.firstLegOrder.setDisclosedQuantity(_data.qty);
            _data.firstLegOrder.setOrderValidity(TimeInForce.TimeInForce_DAY);
            _data.firstLegOrder.setOrderType(OrderType.OrderType_LIMIT);
            _data.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
            _data.lastBidPrice = _data.orderPriceFirstLeg;
            Context.getInstance().placeOrder(_data.firstLegOrder);
        }

        public void modifyOrder()
        {
            Console.WriteLine("\n**************** In Modify Order **************\n");
            _data.firstLegOrder.setTransactionType(TransactionType.TransactionType_MODIFY);
            _data.firstLegOrder.setSymbol(_data.firstLegSymbol);
            _data.firstLegOrder.setOrderMode(_data.firstLegSide);
            _data.firstLegOrder.setQuantity(_data.qty);
            _data.firstLegOrder.setPrice(_data.orderPriceFirstLeg);
            _data.firstLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
            _data.firstLegOrder.setDisclosedQuantity(_data.qty);
            _data.firstLegOrder.setOrderValidity(TimeInForce.TimeInForce_DAY);
            _data.firstLegOrder.setOrderType(OrderType.OrderType_LIMIT);
            _data.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
            _data.firstLegOrder.setOrigClOrdId(_data.firstLegOrder.getOrigClOrdId());
            _data.firstLegOrder.setClOrdId(_data.firstLegOrder.getClOrdId());
            _data.lastBidPrice = _data.orderPriceFirstLeg;
            Context.getInstance().placeOrder(_data.firstLegOrder);
        }

        public void cancelFirstLeg()
        {
            Console.WriteLine("\n**************** In Cancel Order **************\n");
            _data.firstLegOrder.setTransactionType(TransactionType.TransactionType_CANCEL);
            _data.firstLegOrder.setSymbol(_data.firstLegSymbol);
            _data.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
            _data.firstLegOrder.setOrigClOrdId(_data.firstLegOrder.getOrigClOrdId());
            _data.firstLegOrder.setClOrdId(_data.firstLegOrder.getClOrdId());
            _data.lastBidPrice = 0;
            Context.getInstance().placeOrder(_data.firstLegOrder);
        }

        public void placeSecondLegMarket()
        {
            Console.WriteLine("\n**************** In Leg-2 Market ***************\n");
            _data.secondLegOrder.setTransactionType(TransactionType.TransactionType_NEW);
            _data.secondLegOrder.setSymbol(_data.secondLegSymbol);
            _data.secondLegOrder.setOrderMode(_data.secondLegSide);
            _data.secondLegOrder.setQuantity(_data.qty);
            _data.secondLegOrder.setPrice(0);
            _data.secondLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
            _data.secondLegOrder.setDisclosedQuantity(_data.qty);
            _data.secondLegOrder.setOrderValidity(TimeInForce.TimeInForce_DAY);
            _data.secondLegOrder.setOrderType(OrderType.OrderType_MARKET);
            _data.secondLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
            Context.getInstance().placeOrder(_data.secondLegOrder);
        }

        public bool getAveragePrice(MarketData md, int qty, Side side, int leg)
        {
            long tempPrice = 0;
            long tempQty = 0;
            for (int i = 0; i < 5; ++i)
            {
                long p = md.getPrice(side, i + 1);
                long q = md.getQty(side, i + 1);

                tempPrice += p * q;
                tempQty += q;

                if (tempQty >= qty)
                    break;
            }

            if (tempQty < qty)
            {
                if (leg == 1)
                    _data.avgPriceFirstLeg = 0;
                else
                    _data.avgPriceSecondLeg = 0;
                return false;
            }
            if (leg == 1)
                _data.avgPriceFirstLeg = (int)(tempPrice / tempQty);
            else
                _data.avgPriceSecondLeg = (int)(tempPrice / tempQty);

            return true;
        }


        public override  void onLogin(bool status)
        {
            Console.WriteLine("================ Got onLogin ===================");
            if(status == false)
            {
                  Console.WriteLine("Logon Error");
                  Environment.Exit(1);
            }
            Console.WriteLine("Calling load instrument for " + _data.firstLegSymbol);
            Context.getInstance().loadInstrument(_data.firstLegSymbol);

            Console.WriteLine("Called load instrument for " + _data.firstLegSymbol);
            Context.getInstance().loadInstrument(_data.secondLegSymbol);
            
            Console.WriteLine("Called load instrument for " + _data.secondLegSymbol);
        }

        public override void onLogout(bool status)
        {
            Console.WriteLine("Got on Logout Event with status.");
        }

        public override void onTick(MarketData md)
        {
            Console.WriteLine(" Got tick for symbol "+md.getInstrument().getInstrumentName());

            if(_data.isOrderPending)
            {
                Console.WriteLine("Orders are pending no need to react on this tick.");
                return ;
            }
            else
            {
                Console.WriteLine("md.getInstrument().getInstrumentName() "+md.getInstrument().getInstrumentName());
                if(md.getInstrument().getInstrumentName() == (_data.firstLegSymbol))
                {
                    getAveragePrice(md, _data.qty, _data.firstLegSide, 1);
                    Console.WriteLine("First leg avg price : %d\n", _data.avgPriceSecondLeg);
                }
                else if( md.getInstrument().getInstrumentName() == (_data.secondLegSymbol))
                {
                    getAveragePrice(md, _data.qty, _data.secondLegSide, 2);
                    Console.WriteLine("Second leg avg price : %d\n", _data.avgPriceFirstLeg);
                }
                else
                {
                    Console.WriteLine("Unknown symbol tick.");
                }
            }

            if(_data.avgPriceSecondLeg == 0)
            {
              // leg-2 is empty if bid is placed cancel our bib.
              if(_data.bidPlaced)
              {
                    cancelFirstLeg();
               } 
           }
           else
           {
              _data.orderPriceFirstLeg = 
                (_data.minProfit * _data.firstLegMode) +
                  _data.avgPriceSecondLeg;

                          
              if(!_data.bidPlaced)
              {
                    firstLegBid();
                    _data.bidPlaced = true;
                    _data.isOrderPending = true;
              }
              else if(_data.bidPlaced && 
                      _data.firstLegOrder.getOrderStatus() == OrderStatus.OrderStatus_PENDING &&
                      _data.lastBidPrice != _data.orderPriceFirstLeg)
              {
                    modifyOrder();
                    _data.isOrderPending = true;
              }
              else
              {
                    Console.WriteLine("Bid update is not needed.");
              }
            }

          }

        public override void onLoadInstrumentEnd(String instrumentName,
                                          bool success)
        {
            Console.WriteLine("Instrument " + instrumentName + " Loaded.");
            Instrument inst = Context.getInstance().getInstrument(instrumentName);
            Context.getInstance().subscribe(inst);
        }

        public override void onExecutionReport(ExecutionReport report)
        {
            Console.WriteLine("Got on onExecutionReport Event.");
            report.dump();
            Console.WriteLine("Status = %d",OrderStatus.OrderStatus_CONFIRMED);
            if(report.getInstrumentName() == (_data.firstLegSymbol))
            {
                if( report.getOrderStatus() == OrderStatus.OrderStatus_CONFIRMED || 
                    report.getOrderStatus() == OrderStatus.OrderStatus_REPLACED )
                {
                    _data.firstLegOrder.setClOrdId(report.getClOrderId());
                    _data.firstLegOrder.setOrigClOrdId(report.getOriginalClOrderId());
                    _data.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_CONFIRMED);
                    _data.isOrderPending = false;
                }
                else if(report.getOrderStatus() == OrderStatus.OrderStatus_FILLED)
                {
                    placeSecondLegMarket();
                    _data.isOrderPending = false;
                }
            }
            else if(report.getInstrumentName() == (_data.secondLegSymbol))
            {
                if(report.getOrderStatus() == OrderStatus.OrderStatus_CONFIRMED)
                {
                    _data.secondLegOrder.setClOrdId(report.getClOrderId());
                    _data.secondLegOrder.setOrigClOrdId(report.getOriginalClOrderId());
                    _data.isOrderPending = false;
                }
                else if(report.getOrderStatus() == OrderStatus.OrderStatus_FILLED)
                {
                    Console.WriteLine("Strategy completed successfully.");
                }
            }
        }




    }
}
