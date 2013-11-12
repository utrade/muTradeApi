class SymbolData
{
	String    firstLegSymbol;
	String    secondLegSymbol;
	int       minProfit;
	int       avgPriceFirstLeg;
	int       avgPriceSecondLeg;
	Side      firstLegSide;
	Side      secondLegSide;
	Boolean   bidPlaced;
	int       firstLegMode;
	int       secondLegMode;
	int       orderPriceFirstLeg;
	int       qty;
	Order     firstLegOrder;
	Order     secondLegOrder;
	int       filledQtyFirstLeg;
	int       filledQtySecondLeg;
	Boolean   isOrderPending;
};

public class SampleStrategy extends Application{

	private SymbolData sd;

	public SampleStrategy()
	{
		Context.getInstance().login(19, "pass2@123", "10.0.0.40", (short)9280);	}
	private void initStrategy()
	{
		logInfo("Initialising strategy");
		sd = new SymbolData();
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
		sd.qty = 10;
		sd.firstLegOrder = new Order();
		sd.secondLegOrder =new Order();
		sd.filledQtyFirstLeg = 0;
		sd.filledQtySecondLeg = 0;
		sd.isOrderPending = false;
	}
	public static void main(String[] args) {
		System.loadLibrary("Context");
		SampleStrategy s = new SampleStrategy();	
		// TODO Auto-generated method stub
	}

	private void logInfo(String msg)
	{
		System.out.print(msg+"----------------->\n");		
	}

	private void logInfo(String msg, int val)
	{
		System.out.format(msg + ":%d---------------------->\n",val);

	}
	private void logInfo(String msg, float val)
	{
		System.out.format(msg + ":%f---------------------->\n",val);
	}

	@Override
	public void onExecutionReport(ExecutionReport report)
	{
		printOrder(
				Portfolio.getInstance().getOrderBook().getOrder(
						report.getClOrderId()
						)
				);
		if(report.getInstrumentName().equals(sd.firstLegSymbol))
		{

			if(report.getOrderStatus() == OrderStatus.OrderStatus_CONFIRMED)
			{
				sd.firstLegOrder.setClOrdId(report.getClOrderId());
				sd.firstLegOrder.setOrigClOrdId(report.getOriginalClOrderId());
				sd.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_CONFIRMED);
				sd.isOrderPending = false;

			}
			else if(report.getOrderStatus() == OrderStatus.OrderStatus_REPLACED)
			{
				sd.firstLegOrder.setClOrdId(report.getClOrderId());
				sd.firstLegOrder.setOrigClOrdId(report.getOriginalClOrderId());
				sd.firstLegOrder.setOrderStatus(OrderStatus.OrderStatus_CONFIRMED);
				sd.isOrderPending = false;

			}
			else if(report.getOrderStatus() == OrderStatus.OrderStatus_FILLED)
			{
				placeSecondLegMarket();
				sd.isOrderPending = true;
				printPortfolio();

				//				List trades = new ArrayList<Trade>();
				//				trades =ArrayList<Trade>Portfolio.getInstance().getTradeBook().getTrades(
				//						report.getClOrderId()
				//						);
				//				printTrade((Trade)trades.get(trades.size()-1));
			}
		}
		else if(report.getInstrumentName().equals(sd.secondLegSymbol))
		{

			if(report.getOrderStatus() == OrderStatus.OrderStatus_CONFIRMED)
			{
				sd.secondLegOrder.setClOrdId(report.getClOrderId());
				sd.secondLegOrder.setOrigClOrdId(report.getOriginalClOrderId());
				sd.isOrderPending = false;
			}
			else if(report.getOrderStatus() == OrderStatus.OrderStatus_FILLED)
			{
				logInfo("Strategy completed successfully.\n");
				printPortfolio();
				Context.getInstance().logout();
			}
		}


	}
	@Override
	public void onLoadInstrumentEnd(String instrumentName, boolean success)  {
		if(success){
			logInfo("Loaded Instrument:"+instrumentName);
			Context.getInstance().subscribe(
					Context.getInstance().getInstrument(
							instrumentName
							)
					);
		}
	}
	@Override
	public void onLogin(boolean status) {
		logInfo("Logged In Successfully");
		initStrategy();
		Context.getInstance().setApplication(this);
		Context.getInstance().loadInstrument(sd.firstLegSymbol);
		Context.getInstance().loadInstrument(sd.secondLegSymbol);
	}
	@Override	
	public void onTick(MarketData md) {
		//strategyRunning conitionTest
		logInfo("Tick received for "+md.getInstrument().getInstrumentName());
		if(sd.isOrderPending)
		{
			logInfo("Orders are pending, Tick rejected");
		}
		else
		{
			if(md.getInstrument().getInstrumentName().equals(sd.firstLegSymbol))
			{
				logInfo("leg 1 Tick");

				sd.avgPriceFirstLeg =getAveragePrice(
						md,
						sd.qty,
						sd.firstLegSide
						);

				logInfo("First leg avg price ",sd.avgPriceFirstLeg);
			}
			else if(md.getInstrument().getInstrumentName().equals(sd.secondLegSymbol))
			{
				logInfo("leg 2 Tick");

				sd.avgPriceSecondLeg = getAveragePrice( 
						md,
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
		logInfo("Side-----"+p.getOrderMode().toString());
		logInfo("Qty-----",p.getQuantity());
		logInfo("Avg Price-----",p.getAveragePrice());
	}

	private void printOrder(Order o)
	{
		logInfo("Order Status Dump| ",o.getClOrdId());
		logInfo("Order Symbol-----"+o.getSymbol());
		logInfo("Side-----"+o.getOrderMode().toString());
		logInfo("Qty-----",o.getQuantity());
		logInfo("Price-----",o.getPrice());
		logInfo("Status-----"+o.getOrderStatus().toString());
		logInfo("Filled Qty-----",o.getFilledQuantity());
		logInfo("Pending Qty-----",o.getQuantity() -o.getFilledQuantity());
		//		logInfo("Filled Qty-----",o.getFilledQuantity());	
	}
	private void printTrade(Trade t)
	{
		logInfo("Order Status Dump| ",t.getTradeId());
		logInfo("Order ID-----:",t.getClOrdId());
		logInfo("Instrument-----:"+t.getInstrument().getInstrumentName());
		logInfo("Side-----"+t.getOrderMode().toString());
		logInfo("Qty-----",t.getFilledQuantity());
		logInfo("Price-----",t.getFilledPrice());
	}
	private void printPortfolio()
	{
		Portfolio p = Portfolio.getInstance();
		Position posFirstLeg =p.getNetPositions().getPosition(
				Context.getInstance().getInstrument(
						sd.firstLegSymbol				
						), Side.swigToEnum(sd.firstLegMode));
		Position posSecondLeg =p.getNetPositions().getPosition(
				Context.getInstance().getInstrument(
						sd.secondLegSymbol				
						), Side.swigToEnum(sd.secondLegMode));
		printPosition(posFirstLeg);
		printPosition(posSecondLeg);
	}

	@Override
	public void onLogout(boolean status)
	{
		logInfo("Logged Out/Login Failed");
		printPortfolio();
		System.exit(1);		
	}
	public void firstLegBid()
	{
		sd.firstLegOrder.setTransactionType(TransactionType.TransactionType_NEW);
		sd.firstLegOrder.setSymbol(sd.firstLegSymbol);
		sd.firstLegOrder.setOrderMode(sd.firstLegSide);
		sd.firstLegOrder.setQuantity(
				sd.qty
				*
				Context.getInstance().getInstrument(
						sd.firstLegSymbol
						).getLotSize()
				);
		sd.firstLegOrder.setPrice(sd.orderPriceFirstLeg);
		sd.firstLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
		sd.firstLegOrder.setDisclosedQuantity(sd.qty
				*
				Context.getInstance().getInstrument(
						sd.firstLegSymbol
						).getLotSize());
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
		sd.firstLegOrder.setQuantity(sd.qty
				*
				Context.getInstance().getInstrument(
						sd.firstLegSymbol
						).getLotSize()
				);
		sd.firstLegOrder.setPrice(sd.orderPriceFirstLeg);
		sd.firstLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
		sd.firstLegOrder.setDisclosedQuantity(sd.qty
				*
				Context.getInstance().getInstrument(
						sd.firstLegSymbol
						).getLotSize());
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
		sd.secondLegOrder.setQuantity(sd.qty
				*
				Context.getInstance().getInstrument(
						sd.secondLegSymbol
						).getLotSize());
		sd.secondLegOrder.setPrice(0);
		sd.secondLegOrder.setSecurityType(InstrumentType.InstrumentType_FUTURE);
		sd.secondLegOrder.setDisclosedQuantity(sd.qty
				*
				Context.getInstance().getInstrument(
						sd.firstLegSymbol
						).getLotSize());
		sd.secondLegOrder.setOrderValidity(TimeInForce.TimeInForce_DAY);
		sd.secondLegOrder.setOrderType(OrderType.OrderType_MARKET);
		sd.secondLegOrder.setOrderStatus(OrderStatus.OrderStatus_PENDING);
		Context.getInstance().placeOrder(sd.secondLegOrder);

	}

	/*
	 *  Function to calculate average price for given qty from
	 *  current market depth.
	 */
	public int getAveragePrice( MarketData md,
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

			tempPrice += p * q;

			tempQty += q;
			if(tempQty >= qty)
				break;
		}
		if(tempQty < qty)
		{
			//			 avgPrice = 0;
			return -1;
		}
		logInfo("avgPrice",tempPrice/tempQty);
		return tempPrice/tempQty;
		//    std::cout << "avgPrice " << avgPrice << std::endl;
		//		 return true;
	}

}
