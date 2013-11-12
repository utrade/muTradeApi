
public class ApplicationImpl extends Application 
{
  Data data;
  public ApplicationImpl(Data d)
  {
    data = d;
  }

  public void firstLegBid()
  {
    System.out.println("\n**************** In firstLegBid ***************\n");
    data.firstLegOrder.setTransactionType(TransactionType.TransactionType_NEW);
    data.firstLegOrder.setSymbol(data.firstLegSymbol);
    data.firstLegOrder.setOrderMode(data.firstLegSide);
    data.firstLegOrder.setQuantity(data.qty);
    data.firstLegOrder.setPrice(data.orderPriceFirstLeg);
    data.firstLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
    data.firstLegOrder.setDisclosedQuantity(data.qty);
    data.firstLegOrder.setOrderValidity(TimeInForce.TimeInForce_DAY);
    data.firstLegOrder.setOrderType(OrderType.OrderType_LIMIT);
    data.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
    data.lastBidPrice = data.orderPriceFirstLeg;
    Context.getInstance().placeOrder(data.firstLegOrder);
  }

  public void modifyOrder()
  {
    System.out.println("\n**************** In Modify Order **************\n");
    data.firstLegOrder.setTransactionType(TransactionType.TransactionType_MODIFY);
    data.firstLegOrder.setSymbol(data.firstLegSymbol);
    data.firstLegOrder.setOrderMode(data.firstLegSide);
    data.firstLegOrder.setQuantity(data.qty);
    data.firstLegOrder.setPrice(data.orderPriceFirstLeg);
    data.firstLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
    data.firstLegOrder.setDisclosedQuantity(data.qty);
    data.firstLegOrder.setOrderValidity(TimeInForce.TimeInForce_DAY);
    data.firstLegOrder.setOrderType(OrderType.OrderType_LIMIT);
    data.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
    data.firstLegOrder.setOrigClOrdId(data.firstLegOrder.getOrigClOrdId());
    data.firstLegOrder.setClOrdId(data.firstLegOrder.getClOrdId());
    data.lastBidPrice = data.orderPriceFirstLeg;
    Context.getInstance().placeOrder(data.firstLegOrder);
  }

  public void cancelFirstLeg()
  {
    System.out.println("\n**************** In Cancel Order **************\n");
    data.firstLegOrder.setTransactionType(TransactionType.TransactionType_CANCEL);
    data.firstLegOrder.setSymbol(data.firstLegSymbol);
    data.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
    data.firstLegOrder.setOrigClOrdId(data.firstLegOrder.getOrigClOrdId());
    data.firstLegOrder.setClOrdId(data.firstLegOrder.getClOrdId());
    data.lastBidPrice = 0;
    Context.getInstance().placeOrder(data.firstLegOrder);
  }

  public void placeSecondLegMarket()
  {
    System.out.println("\n**************** In Leg-2 Market ***************\n");
    data.secondLegOrder.setTransactionType(TransactionType.TransactionType_NEW);
    data.secondLegOrder.setSymbol(data.secondLegSymbol);
    data.secondLegOrder.setOrderMode(data.secondLegSide);
    data.secondLegOrder.setQuantity(data.qty);
    data.secondLegOrder.setPrice(0);
    data.secondLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
    data.secondLegOrder.setDisclosedQuantity(data.qty);
    data.secondLegOrder.setOrderValidity(TimeInForce.TimeInForce_DAY);
    data.secondLegOrder.setOrderType(OrderType.OrderType_MARKET);
    data.secondLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
    Context.getInstance().placeOrder(data.secondLegOrder);
  }

  public boolean getAveragePrice( MarketData md, int qty, Side side, int leg)
  {
    long tempPrice = 0;
    long tempQty = 0;
    for(int i=0; i<5; ++i)
    {
      long p = md.getPrice(side, i+1);
      long q = md.getQty(side, i+1);

      tempPrice += p*q;
      tempQty   += q;

      if(tempQty >= qty)
        break;
    }

    if(tempQty < qty)
    {
      if(leg == 1)
        data.avgPriceFirstLeg = 0;
      else
        data.avgPriceSecondLeg = 0;
      return false;
    }
    if(leg == 1)
      data.avgPriceFirstLeg = (int)(tempPrice / tempQty);
    else
      data.avgPriceSecondLeg = (int)(tempPrice / tempQty); 

    return true;
  }

  public void onLogin(boolean status)
  {
    System.out.println("\nGot  onLogon Event\n");
    if(status == false)
    {
      System.out.println("Logon Error");
      System.exit(1);
    }
    System.out.println("Calling load instrument for "+data.firstLegSymbol);
    Context.getInstance().loadInstrument(data.firstLegSymbol);
    System.out.println("Called load instrument for "+data.firstLegSymbol);
    Context.getInstance().loadInstrument(data.secondLegSymbol);
    System.out.println("Called load instrument for "+data.secondLegSymbol);
  }
 
  public void onLogout( boolean status)
  {
    System.out.println("Got on Logout Event with status.");
  }

  public void onTick(MarketData md)
  {
    System.out.println(" Got tick for symbol "+md.getInstrument().getInstrumentName());

    if(data.isOrderPending)
    {
      System.out.println("Orders are pending no need to react on this tick.");
      return ;
    }
    else
    {
      System.out.println("md.getInstrument().getInstrumentName() "+md.getInstrument().getInstrumentName());
      if(md.getInstrument().getInstrumentName().equals(data.firstLegSymbol))
      {
        getAveragePrice(md, data.qty, data.firstLegSide, 1);
        System.out.format("First leg avg price : %d\n",data.avgPriceSecondLeg);
      }
      else if( md.getInstrument().getInstrumentName().equals(data.secondLegSymbol))
      {
        getAveragePrice(md, data.qty, data.secondLegSide, 2);
        System.out.format("Second leg avg price : %d\n",data.avgPriceFirstLeg);
      }
      else
      {
        System.out.format("Unknown symbol tick.");
      }
    }

    if(data.avgPriceSecondLeg == 0)
    {
      // leg-2 is empty if bid is placed cancel our bib.
      if(data.bidPlaced)
      {
        cancelFirstLeg();

      } 
    }
    else
    {
      data.orderPriceFirstLeg = 
        (data.minProfit * data.firstLegMode) +
          data.avgPriceSecondLeg;

                          
      if(!data.bidPlaced)
      {
        firstLegBid();
        data.bidPlaced = true;
        data.isOrderPending = true;
      }
      else if(data.bidPlaced && 
              data.firstLegOrder.getOrderStatus() == OrderStatus.OrderStatus_PENDING &&
              data.lastBidPrice != data.orderPriceFirstLeg)
      {
        modifyOrder();
        data.isOrderPending = true;
      }
      else
      {
        System.out.println("Bid update is not needed.");
      }
    }

  }

  public void onLoadInstrumentEnd(  String  instrumentName, 
                                    boolean success )
  {
    System.out.println("Instrument " + instrumentName + " Loaded.");
    Instrument inst = Context.getInstance().getInstrument(instrumentName);
    Context.getInstance().subscribe(inst);
  }

  public void onExecutionReport(ExecutionReport report)
  {
    System.out.println("Got on onExecutionReport Event.");
    report.dump();
    System.out.format("Status = %d",OrderStatus.OrderStatus_CONFIRMED);
    if(report.getInstrumentName().equals(data.firstLegSymbol))
    {
      if( report.getOrderStatus() == OrderStatus.OrderStatus_CONFIRMED || 
          report.getOrderStatus() == OrderStatus.OrderStatus_REPLACED )
      {
        data.firstLegOrder.setClOrdId(report.getClOrderId());
        data.firstLegOrder.setOrigClOrdId(report.getOriginalClOrderId());
        data.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_CONFIRMED);
        data.isOrderPending = false;
      }
      else if(report.getOrderStatus() == OrderStatus.OrderStatus_FILLED)
      {
        placeSecondLegMarket();
        data.isOrderPending = false;
      }
    }
    else if(report.getInstrumentName().equals(data.secondLegSymbol))
    {
      if(report.getOrderStatus() == OrderStatus.OrderStatus_CONFIRMED)
      {
        data.secondLegOrder.setClOrdId(report.getClOrderId());
        data.secondLegOrder.setOrigClOrdId(report.getOriginalClOrderId());
        data.isOrderPending = false;
      }
      else if(report.getOrderStatus() == OrderStatus.OrderStatus_FILLED)
      {
        System.out.println("Strategy completed successfully.");
      }
    }
  }

}
