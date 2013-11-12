public class Test 
{
  public static Data d;
  public static void initData()
  {
    d.firstLegSymbol      = "ESMNSE INFY 20131031";
    d.secondLegSymbol     = "ESMNSE ACC 20131031";
    d.minProfit           = 274000;
    d.avgPriceFirstLeg    = 0;
    d.avgPriceSecondLeg   = 0;
    d.firstLegSide        = Side.Side_SELL;
    d.secondLegSide       = Side.Side_BUY;
    d.bidPlaced           = false;
    d.firstLegMode        = 1;
    d.secondLegMode       = -1;
    d.orderPriceFirstLeg  = 0;
    d.qty                 = 250;
    d.firstLegOrder       = new Order();
    d.secondLegOrder      = new Order();
    d.filledQtyFirstLeg   = 0;
    d.filledQtySecondLeg  = 0;
    d.isOrderPending      = false;
    d.lastBidPrice        = 0;
  }

  public static void main(String args[])
  {
    System.loadLibrary("Context");
    Context instance  = Context.getInstance();
    int usetId        = 14;
    String passwd     = "pass2@123";
    String host       = "10.0.0.31";
    short port        = 8286;
    
    d = new Data();
    
    initData();

    ApplicationImpl app = new ApplicationImpl(d);

    instance.setApplication(app);
    instance.login(usetId, passwd, host, port, true);
    
  }
}
