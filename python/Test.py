#from application import *
import Context

class symbolData(object):
    def __init__( self,
                firstLegSymbol = None,
                secondLegSymbol = None,
                minProfit = 0,
                avgPriceFirstLeg = 0,
                avgPriceSecondLeg = 0,
                firstLegSide = Context.Side_BUY,
                secondLegSide = Context.Side_SELL,
                bidPlaced = False,
                firstLegMode = -1,
                secondLegMode = 1,
                orderPriceFirstLeg = 0,
                qty = 0,
                firstLegOrder = Context.Order(),
                secondLegOrder = Context.Order(),
                filledQtyFirstLeg = 0,
                filledQtySecondLeg =0,
                isOrderPending = False,
                previousBidPrice =0
                ):
        self.firstLegSymbol = firstLegSymbol
        self.secondLegSymbol = secondLegSymbol
        self.minProfit = minProfit
        self.avgPriceFirstLeg = avgPriceFirstLeg
        self.avgPriceSecondLeg = avgPriceSecondLeg
        self.firstLegSide = firstLegSide
        self.secondLegSide = secondLegSide
        self.bidPlaced = bidPlaced
        self.firstLegMode = firstLegMode
        self.secondLegMode = secondLegMode
        self.orderPriceFirstLeg = orderPriceFirstLeg
        self.qty = qty
        self.firstLegOrder = firstLegOrder
        self.secondLegOrder = secondLegOrder
        self.filledQtyFirstLeg = filledQtyFirstLeg
        self.filledQtySecondLeg = filledQtySecondLeg
        self.isOrderPending = isOrderPending
        self.previousBidPrice = previousBidPrice




class Bar(Context.Application):
    def __init__(self, sd):
        super(Bar,self).__init__()
        print "************ Application ********************* "
        print "Symboldata 1st Leg ", sd.firstLegSymbol
        pass
    def onLogin(self, val):
        try:
          print "************ Got onLogin ********************* ", val
          print "Calling Load instrument for ",sd.firstLegSymbol
          if val == False:
            print "Login Error...."
          else:
            print "Calling Load instrument for ",sd.firstLegSymbol
            Context.Context_getInstance().loadInstrument(sd.firstLegSymbol)
            Context.Context_getInstance().loadInstrument(sd.secondLegSymbol)

          print "************** On login End ****************"
        except Exception, e:
          print e

    def onLogout(self, val):
        print "************ Got onLogout ******************** "

    def onTick(self, md):
        try:
          print "Got Market Feed For Symbol ",md.getInstrument().getInstrumentName()
          status = False
          modifyOrder = True

          if sd.isOrderPending :
            print "Orders are pending no need to react on this tick."
          else:
            if md.getInstrument().getInstrumentName() == sd.firstLegSymbol:
              status,sd.avgPriceFirstLeg =  self.getAveragePrice (md, sd.qty, sd.firstLegSide)
              modifyOrder = False
              print "First leg avg price : ",sd.avgPriceFirstLeg
            elif md.getInstrument().getInstrumentName() == sd.secondLegSymbol:
              prveAvg = sd.avgPriceSecondLeg
              status,sd.avgPriceSecondLeg =  self.getAveragePrice (md, sd.qty, sd.secondLegSide)
              if prveAvg == sd.avgPriceSecondLeg:
                modifyOrder = False
              print "Second leg avg price : ",sd.avgPriceSecondLeg
            else:
              print "Unknown symbol tick."

            if sd.avgPriceFirstLeg > 0 and sd.avgPriceSecondLeg > 0:
              sd.orderPriceFirstLeg = (sd.minProfit * sd.firstLegMode) + sd.avgPriceSecondLeg
              print  "Bid Price Leg -1 ",sd.orderPriceFirstLeg

              if not sd.bidPlaced :
                self.firstLegBid()
                sd.bidPlaced = True
                sd.isOrderPending = True
              elif sd.bidPlaced and not sd.isOrderPending and modifyOrder:
                self.modifyOrder()
                sd.bidPlaced = True
                sd.isOrderPending = True
              else:
                print "Cannot upadte order as previous order is pending at exchange."
            elif sd.avgPriceSecondLeg == 0 and sd.bidPlaced and  not sd.isOrderPending :
              print "Creating cancel order for first Leg."
              self.cancelFirstLeg()
        except Exception, e:
            print e


    def onLoadInstrumentEnd(self, instrumentName, success):
        try:
          print "Instrument ", instrumentName, " Loaded."
          inst = Context.Context_getInstance().getInstrument(instrumentName)
          print "subscribing instrument ", inst.getInstrumentName()
          Context.Context_getInstance().subscribe(inst)
        except Exception, e:
          print e

    def onExecutionReport(self, execReport):
        try:
          print "Got on onreportutionReport Event."
          execReport.dump()
          if execReport.getInstrumentName() == sd.firstLegSymbol:

            if execReport.getOrderStatus() == Context.OrderStatus_CONFIRMED:
              sd.firstLegOrder.setClOrdId(execReport.getClOrderId())
              sd.firstLegOrder.setOrigClOrdId(execReport.getOriginalClOrderId())
              sd.firstLegOrder.setOrderStatus(Context.OrderStatus_CONFIRMED)
              sd.isOrderPending = False

            elif execReport.getOrderStatus() == Context.OrderStatus_REPLACED:
              sd.firstLegOrder.setClOrdId(execReport.getClOrderId())
              sd.firstLegOrder.setOrigClOrdId(execReport.getOriginalClOrderId())
              sd.firstLegOrder.setOrderStatus(Context.OrderStatus_CONFIRMED)
              sd.isOrderPending = False
            elif execReport.getOrderStatus() == Context.OrderStatus_FILLED:
              self.placeSecondLegMarket()
              sd.isOrderPending = True

          elif execReport.getInstrumentName() == sd.secondLegSymbol:
            if execReport.getOrderStatus() == Context.OrderStatus_CONFIRMED:
              sd.secondLegOrder.setClOrdId(execReport.getClOrderId());
              sd.secondLegOrder.setOrigClOrdId(execReport.getOriginalClOrderId())
              sd.isOrderPending = False;
            elif execReport.getOrderStatus() == Context.OrderStatus_FILLED:
              print "Strategy completed successfully."
              netPos = Context.Portfolio_getInstance().getNetPositions()
              firstInst = Context.Context_getInstance().getInstrument(sd.firstLegSymbol)
              firstPos = netPos.getPosition(firstInst, sd.firstLegSide)
              print "Qty from net position for first leg : ", firstPos.getQuantity()
              print "Price from net position for first leg : ", firstPos.getAveragePrice()
              tradeBook = Context.Portfolio_getInstance().getTradeBook()
              trdlst = tradeBook.getTrades()
              print "Type ",type(trdlst)
              print trdlst

        except Exception, e:
          print e

    def firstLegBid(self):
        try:
          print "\n**************** In firstLegBid *********************\n"
          sd.firstLegOrder.setTransactionType(Context.TransactionType_NEW)
          sd.firstLegOrder.setSymbol(sd.firstLegSymbol)
          sd.firstLegOrder.setOrderMode(sd.firstLegSide)
          sd.firstLegOrder.setQuantity(sd.qty)
          sd.firstLegOrder.setPrice(sd.orderPriceFirstLeg)
          sd.firstLegOrder.setSecurityType(Context.InstrumentType_FUTURE)
          sd.firstLegOrder.setDisclosedQuantity(sd.qty)
          sd.firstLegOrder.setOrderValidity(Context.TimeInForce_DAY)
          sd.firstLegOrder.setOrderType(Context.OrderType_LIMIT)
          sd.firstLegOrder.setOrderStatus(Context.OrderStatus_PENDING)
          sd.previousBidPrice = sd.orderPriceFirstLeg
          Context.Context_getInstance().placeOrder(sd.firstLegOrder)
        except Exception, e:
          print e

    def modifyOrder(self):
        try:
          print "\n**************** In modifyOrder *********************\n"
          sd.firstLegOrder.setTransactionType(Context.TransactionType_MODIFY)
          sd.firstLegOrder.setSymbol(sd.firstLegSymbol)
          sd.firstLegOrder.setOrderMode(sd.firstLegSide)
          sd.firstLegOrder.setQuantity(sd.qty)
          sd.firstLegOrder.setPrice(sd.orderPriceFirstLeg)
          sd.firstLegOrder.setSecurityType(Context.InstrumentType_FUTURE)
          sd.firstLegOrder.setDisclosedQuantity(sd.qty)
          sd.firstLegOrder.setOrderValidity(Context.TimeInForce_DAY)
          sd.firstLegOrder.setOrderType(Context.OrderType_LIMIT)
          sd.firstLegOrder.setOrderStatus(Context.OrderStatus_PENDING)
          sd.firstLegOrder.setOrigClOrdId(sd.firstLegOrder.getOrigClOrdId())
          sd.firstLegOrder.setClOrdId(sd.firstLegOrder.getClOrderId())
          Context.Context_getInstance().placeOrder(sd.firstLegOrder)
        except Exception, e:
          print e

    def cancelFirstLeg(self):
        try:
          print "\n**************** In cancelFirstLeg *********************\n"
          sd.firstLegOrder.setTransactionType(Context.TransactionType_CANCEL)
          sd.firstLegOrder.setSymbol(sd.firstLegSymbol)
          sd.firstLegOrder.setOrderStatus(Context.OrderStatus_PENDING)
          sd.firstLegOrder.setOrigClOrdId(sd.firstLegOrder.getOrigClOrdId())
          sd.firstLegOrder.setClOrdId(sd.firstLegOrder.getClOrderId())
          Context.Context_getInstance().placeOrder(sd.firstLegOrder)
        except Exception, e:
          print e

    def placeSecondLegMarket(self):
        try:
          print "\n**************** In placeSecondLegMarket ******************\n"
          sd.secondLegOrder.setTransactionType(Context.TransactionType_NEW)
          sd.secondLegOrder.setSymbol(sd.secondLegSymbol)
          sd.secondLegOrder.setOrderMode(sd.secondLegSide)
          sd.secondLegOrder.setQuantity(sd.qty)
          sd.secondLegOrder.setPrice(0)
          sd.secondLegOrder.setSecurityType(Context.InstrumentType_FUTURE)
          sd.secondLegOrder.setDisclosedQuantity(sd.qty)
          sd.secondLegOrder.setOrderValidity(Context.TimeInForce_DAY)
          sd.secondLegOrder.setOrderType(Context.OrderType_MARKET)
          sd.secondLegOrder.setOrderStatus(Context.OrderStatus_PENDING)
          Context.Context_getInstance().placeOrder(sd.secondLegOrder)
        except Exception, e:
          print e

    def getAveragePrice(self, md, qty, side ):
        tempPrice = 0
        tempQty = 0
        for i in range(5):
          p = md.getPrice(side, i+1)
          q = md.getQty(side, i+1)
          tempPrice = tempPrice + p * q
          tempQty = tempQty + q
          if tempQty >= qty :
            break
        if tempQty < qty :
          return False, 0
        avgPrice = tempPrice / tempQty
        print "avgPrice = ", avgPrice
        return True, avgPrice






sd = symbolData("ESMNSE INFY 20131031",
                "ESMNSE ACC 20131031",
                267500,
                0,
                0,
                Context.Side_SELL,
                Context.Side_BUY,
                False,
                1,
                -1,
                0,
                250,
                Context.Order(),
                Context.Order(),
                0,
                0,
                0)
a = Bar(sd)
#a = Application()
contxt = Context.Context_getInstance()
contxt.setApplication(a)
host = "10.0.0.31"
passw = "pass2@123"
user = 14
port  = 8286
try:
  contxt.login(user, passw, host, port, False)
except Exception, e:
  print e
