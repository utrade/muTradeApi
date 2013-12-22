using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace sampleApplication
{


    class sampleStrategy : Application
    {
        private Data sd;
        public sampleStrategy()
	    {
            logInfo("@@@@@@@@@@@@@@@@@@@@@@@@");
		    Context.getInstance().setApplication(this);
		    //Context.getInstance().login(19, "pass2@123", "10.0.0.31", (short)8286);	
            Context.getInstance().login(19, "pass2@123", "10.0.0.40", (short)9280);
        }

        private void logInfo(String msg)
	    {
		    Console.WriteLine(msg+"----------------->\n");		
	    }

        private void logInfo(String msg, int val)
	    {
		    Console.WriteLine(msg + "---------------------->  "+val);
        }

        private void logInfo(String msg, long val)
	    {
		    Console.WriteLine(msg + "----------------------> " + val);
        }
        
        private void logInfo(String msg, float val)
	    {
		    Console.WriteLine(msg + "----------------------> "+val);
	    }



        private void initStrategy()
        {
            logInfo("Initialising strategy");
            sd = new Data();
            sd.firstLegSymbol = "ESMNSE INFY EQ";
            sd.secondLegSymbol = "ESMNSE RELIANCE EQ";
            sd.minProfit = 800;
            sd.avgPriceFirstLeg = 0;
            sd.avgPriceSecondLeg = 0;
            sd.firstLegSide = Side.Side_SELL;
            sd.secondLegSide = Side.Side_BUY;
            sd.bidPlaced = false;
            sd.firstLegMode = 1;
            sd.secondLegMode = -1;
            sd.orderPriceFirstLeg = 0;
            sd.qty = 1;
            sd.firstLegOrder = new Order();
            sd.secondLegOrder = new Order();
            sd.filledQtyFirstLeg = 0;
            sd.filledQtySecondLeg = 0;
            sd.isOrderPending = false;
        }


        public override void onExecutionReport(ExecutionReport report)
	    {
		    logInfo("onExecutionReport:-------------------->");
    //		report.dump();
		    try{
		    logInfo("Status:"+report.getOrderStatus().ToString());
		    logInfo("Instrument Name: "+report.getInstrumentName());
		
		
		    printOrder(
				    Portfolio.getInstance().getOrderBook().getOrder(
						    report.getClOrderId()
						    )
				    );
		    }
		    catch(Exception e)
		    {
			    logInfo(e.Message);
			
		    }
		    logInfo("Status:"+report.getOrderStatus().ToString());
		    logInfo("Instrument Name: "+report.getInstrumentName());
		
		
		    if(report.getInstrumentName()==(sd.firstLegSymbol))
		    {
			    logInfo("Here1" );
			    if(report.getOrderStatus()==(OrderStatus.OrderStatus_CONFIRMED))
			    {
				    logInfo("Here2" );
				    sd.firstLegOrder.setClOrdId(report.getClOrderId());
				    sd.firstLegOrder.setOrigClOrdId(report.getOriginalClOrderId());
				    sd.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_CONFIRMED);
				    sd.isOrderPending = false;

			    }
			    else if(report.getOrderStatus()==(OrderStatus.OrderStatus_REPLACED))
			    {
				    sd.firstLegOrder.setClOrdId(report.getClOrderId());
				    sd.firstLegOrder.setOrigClOrdId(report.getOriginalClOrderId());
				    sd.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_CONFIRMED);
				    sd.isOrderPending = false;

			    }
			    else if(report.getOrderStatus()==(OrderStatus.OrderStatus_FILLED))
			    {
				    try
				    {
					    TradeQue trades
					     = Portfolio.getInstance().getTradeBook().getTradeQue(report.getClOrderId());
					    logInfo("**************** Dumping Trades ***************** .\n");
					    long size = trades.size();
					    logInfo("size = ",size);
					    for (int i=0; i<size; ++i) {
						    printTrade(trades.getitem(i));
					    }
				    }
				    catch(Exception e)
				    {
					    logInfo(e.Message.ToString());
				    }
				    //				Side side;
				    //				if(report.getOrderMode().)
				    //					
				    //				
				    placeSecondLegMarket();
				    sd.isOrderPending = true;
				    //				Position pos =Portfolio.getInstance().getNetPositions().getPosition(
				    //						Context.getInstance().getInstrument(report.getInstrumentName()),
				    //						report.getOrderMode());
				    //				printPosition(pos);
				    printPortfolio(report.getInstrumentName(),report.getOrderMode());

				    //				List trades = new ArrayList<Trade>();
				    //				trades =ArrayList<Trade>Portfolio.getInstance().getTradeBook().getTrades(
				    //						report.getClOrderId()
				    //						);
				    //				printTrade((Trade)trades.get(trades.size()-1));
			    }
		    }
		    else if(report.getInstrumentName()==(sd.secondLegSymbol))
		    {

			    if(report.getOrderStatus()==(OrderStatus.OrderStatus_CONFIRMED))
			    {
				    sd.secondLegOrder.setClOrdId(report.getClOrderId());
				    sd.secondLegOrder.setOrigClOrdId(report.getOriginalClOrderId());
				    sd.isOrderPending = false;
			    }
			    else if(report.getOrderStatus()==(OrderStatus.OrderStatus_FILLED))
			    {
				    logInfo("Strategy completed successfully.\n");
				    printPortfolio();
				    logInfo("Calling Logout.\n");
				    Context.getInstance().logout();
			    }
		    }


	    }
	    public override void onLoadInstrumentEnd(String instrumentName, bool success)  
        {
		    if(success)
            {
			    logInfo("Loaded Instrument:"+instrumentName);
			    Context.getInstance().subscribe(
					    Context.getInstance().getInstrument(
							    instrumentName
							    )
					    );
		    }
	    }
	
        public override void onLogin(bool status) 
        {
            if (status)
            {
                logInfo("Logged In Successfully");
                initStrategy();
                Context.getInstance().loadInstrument(sd.firstLegSymbol);
                Context.getInstance().loadInstrument(sd.secondLegSymbol);
            }
            else
            {
                logInfo("Error in Login ....");
                Environment.Exit(1);
            }
	    }
		
	    public override void onTick(MarketData md) 
        {
		    //strategyRunning conitionTest
		    logInfo("Tick received for "+md.getInstrument().getInstrumentName());
		    if(sd.isOrderPending)
		    {
			    logInfo("Orders are pending, Tick rejected");
		    }
		    else
		    {
               if(md.getInstrument().getInstrumentName()==(sd.firstLegSymbol))
			    {
				    logInfo("leg 1 Tick");
                    Side side = sd.firstLegSide == Side.Side_ASK ?
                                 Side.Side_BID   :   Side.Side_ASK;
                    for (int i = 0; i < 5; ++i)
                    {
                        logInfo("=========== Price  ",md.getPrice(side, i+1));
                        logInfo("=========== Qty   ", md.getQty(side, i + 1));
                    }
                        sd.avgPriceFirstLeg = getAveragePrice(
                                ref  md,
                                sd.qty,
                                sd.firstLegSide
                                );

				    logInfo("First leg avg price ",sd.avgPriceFirstLeg);
			    }
			    else if(md.getInstrument().getInstrumentName()==(sd.secondLegSymbol))
			    {
				    logInfo("leg 2 Tick");

				    sd.avgPriceSecondLeg = getAveragePrice( 
						    ref md,
						    sd.qty,
						    sd.secondLegSide
						    );

				    logInfo("Second leg avg price ",sd.avgPriceSecondLeg);
			    }
			    else
			    {
				    logInfo("unrecognized Tick");


			    }
			    if(sd.avgPriceSecondLeg>0)
			    {
				    sd.orderPriceFirstLeg =
						    (sd.minProfit * sd.firstLegMode)
						    + (sd.avgPriceSecondLeg);

				    logInfo("Bid Price Leg -1 :",sd.orderPriceFirstLeg);
				    if( !sd.bidPlaced )
				    {
					    firstLegBid();
					    sd.bidPlaced = true;
					    sd.isOrderPending = true;
				    }
				    else if(  sd.bidPlaced &&
						    sd.firstLegOrder.getOrderStatus()
						    != OrderStatus.OrderStatus_PENDING )
				    {
					    modifyOrder();
					    sd.bidPlaced = true;
					    sd.isOrderPending = true;
				    }
				    else
				    {
					    if(sd.avgPriceFirstLeg != 0)
						    logInfo("Cannot update order as previous order is pending at exchange.");
				    }
			    }
			    else if( sd.avgPriceSecondLeg == 0)
			    {
				    if(sd.bidPlaced)
				    {
					    logInfo("Creating cancel order for first Leg.");
					    cancelFirstLeg();
				    }
			    }
		    }

	    }

	    private void printPosition(Position p)
	    {
		    logInfo("Position Dump| "+p.getInstrument().getInstrumentName()+"--------");
		    logInfo("Side-----"+p.getOrderMode().ToString());
		    logInfo("Qty-----",p.getQuantity());
		    logInfo("Avg Price-----",p.getAveragePrice());
	    }

	    private void printOrder(Order o)
	    {
		    logInfo("Order Status Dump| ",o.getClOrdId());
		    logInfo("Order Symbol-----"+o.getSymbol());
		    logInfo("Side-----"+o.getOrderMode().ToString());
		    logInfo("Qty-----",o.getQuantity());
		    logInfo("Price-----",o.getPrice());
		    logInfo("Status-----"+o.getOrderStatus().ToString());
		    logInfo("Filled Qty-----",o.getFilledQuantity());
		    logInfo("Pending Qty-----",o.getQuantity() -o.getFilledQuantity());
		    //		logInfo("Filled Qty-----",o.getFilledQuantity());	
	    }

	    private void printTrade(Trade t)
	    {
		    try
		    {
			    logInfo("Trade Dump| ",t.getTradeId());
			    logInfo("Order ID-----:",t.getClOrdId());
			    logInfo("Instrument-----:"+t.getInstrument().getInstrumentName());
			    logInfo("Side-----"+t.getOrderMode().ToString());
			    logInfo("Qty-----",t.getFilledQuantity());
			    logInfo("Price-----",t.getFilledPrice());
		    }
		    catch(Exception e)
		    {
			    logInfo(e.Message.ToString());
		    }
	    }
	    private void printPortfolio()
	    {
		    Portfolio p = Portfolio.getInstance();
		    try{
			    logInfo("fetching First Leg Position");
			    Position posFirstLeg =p.getNetPositions().getPosition(
					    Context.getInstance().getInstrument(
							    sd.firstLegSymbol				
							    ), sd.firstLegSide);
			    printPosition(posFirstLeg);

		    }
		    catch(Exception e)
		    {
			    logInfo(e.Message.ToString());

		    }
		    try{
			    logInfo("\n\nfetching Second Leg Position");

			    Position posSecondLeg =p.getNetPositions().getPosition(
					    Context.getInstance().getInstrument(
							    sd.secondLegSymbol				
							    ), sd.secondLegSide);
			    printPosition(posSecondLeg);
		    }
		    catch(Exception e)
		    {
			    logInfo(e.Message.ToString());
		    }
	    }

	    private void printPortfolio(String instrumentName,OrderMode o)
	    {
		    Portfolio p = Portfolio.getInstance();
		    Side side = Side.Side_BUY;

		    if(o==(OrderMode.OrderMode_BUY))
			    side= Side.Side_BUY;
		    else if(o==(OrderMode.OrderMode_SELL))
			    side= Side.Side_SELL;
		
		    try{
			    logInfo("fetching First Leg Position");
			    Position posFirstLeg =p.getNetPositions().getPosition(
					    Context.getInstance().getInstrument(instrumentName),
					    side
					    );
					
			    printPosition(posFirstLeg);

		    }
		    catch(Exception e)
		    {
			    logInfo(e.Message.ToString());

		    }
		
	    }


	    public override  void onLogout(bool status)
	    {
		    logInfo("Logged Out/Login Failed");
    //		printPortfolio();
            Environment.Exit(1);
	    }

	    public void firstLegBid()
	    {
		    sd.firstLegOrder.setTransactionType(TransactionType.TransactionType_NEW);
		    sd.firstLegOrder.setSymbol(sd.firstLegSymbol);
		    sd.firstLegOrder.setOrderMode(sd.firstLegSide);
		    sd.firstLegOrder.setQuantity(
				    sd.qty * Context.getInstance().getInstrument(
						    sd.firstLegSymbol ).getLotSize()
				    );
		    sd.firstLegOrder.setPrice(sd.orderPriceFirstLeg);
		    sd.firstLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
		    sd.firstLegOrder.setDisclosedQuantity(
                sd.qty * Context.getInstance().getInstrument(
						    sd.firstLegSymbol).getLotSize());

		    sd.firstLegOrder.setOrderValidity(TimeInForce.TimeInForce_DAY);
		    sd.firstLegOrder.setOrderType(OrderType.OrderType_LIMIT);
		    sd.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
		    Context.getInstance().placeOrder(sd.firstLegOrder);
	    }


	    public void modifyOrder()
	    {
		    sd.firstLegOrder.setTransactionType(TransactionType.TransactionType_MODIFY);
		    sd.firstLegOrder.setSymbol(sd.firstLegSymbol);
		    sd.firstLegOrder.setOrderMode(sd.firstLegSide);
		    sd.firstLegOrder.setQuantity(
                sd.qty * Context.getInstance().getInstrument(
						        sd.firstLegSymbol).getLotSize()
		    );
		    sd.firstLegOrder.setPrice(sd.orderPriceFirstLeg);
		    sd.firstLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
		    sd.firstLegOrder.setDisclosedQuantity(
                sd.qty  * Context.getInstance().getInstrument(
						    sd.firstLegSymbol ).getLotSize());

		    sd.firstLegOrder.setOrderValidity(TimeInForce.TimeInForce_DAY);
		    sd.firstLegOrder.setOrderType(OrderType.OrderType_LIMIT);
		    sd.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
		    sd.firstLegOrder.setOrigClOrdId(
				    sd.firstLegOrder.getOrigClOrdId());
		    sd.firstLegOrder.setClOrdId(sd.firstLegOrder.getClOrdId());
		    sd.firstLegOrder.dumpOrder();
		    Context.getInstance().placeOrder(sd.firstLegOrder);
	    }

	    public void cancelFirstLeg()
	    {
		    sd.firstLegOrder.setTransactionType(TransactionType.TransactionType_CANCEL);
		    sd.firstLegOrder.setSymbol(sd.firstLegSymbol);
		    sd.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
		    sd.firstLegOrder.setOrigClOrdId(
				    sd.firstLegOrder.getOrigClOrdId() );
		    sd.firstLegOrder.setClOrdId(
				    sd.firstLegOrder.getClOrdId());
		    Context.getInstance().placeOrder(sd.firstLegOrder);
	    }

	    public void placeSecondLegMarket()
	    {
		    sd.secondLegOrder.setTransactionType(TransactionType.TransactionType_NEW);
		    sd.secondLegOrder.setSymbol(sd.secondLegSymbol);
		    sd.secondLegOrder.setOrderMode(sd.secondLegSide);
		    sd.secondLegOrder.setQuantity(
                sd.qty * Context.getInstance().getInstrument(
						    sd.secondLegSymbol ).getLotSize());

		    sd.secondLegOrder.setPrice(0);
		    sd.secondLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
		    sd.secondLegOrder.setDisclosedQuantity(
                sd.qty * Context.getInstance().getInstrument(
						    sd.firstLegSymbol ).getLotSize());

		    sd.secondLegOrder.setOrderValidity(TimeInForce.TimeInForce_DAY);
		    sd.secondLegOrder.setOrderType(OrderType.OrderType_MARKET);
		    sd.secondLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
		    Context.getInstance().placeOrder(sd.secondLegOrder);
        }

	    /*
	     *  Function to calculate average price for given qty from
	     *  current market depth.
	     */
	    public int getAveragePrice( ref MarketData md,
			    int qty,
			    Side side
			    )
	    {
		    int tempPrice = 0;
		    int tempQty = 0;
		    for(int i=0; i<5; ++i)
		    {
			    side = side==Side.Side_ASK
					    ?
							    Side.Side_BID
							    :
								    Side.Side_ASK;
			    int p = md.getPrice(side, i+1);
			    int q = md.getQty(side, i+1);
                logInfo (">>>>>>>>>>>>> Price  ",p);
                logInfo (" >>>>>>>>> Qty ", q);
			    tempPrice += p * q;

			    tempQty += q;
			    if(tempQty >= qty)
				    break;
		    }
		    if(tempQty < qty)
		    {
                logInfo("tempQty = ", tempQty); 
                //			 avgPrice = 0;
			    return -1;
		    }
		    logInfo("avgPrice",tempPrice/tempQty);
		    return tempPrice/tempQty;
		    //    std::cout << "avgPrice " << avgPrice << std::endl;
		    //		 return true;
	    }



    }
}
