using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace sampleApplication
{
    class Data
    {
        public   String firstLegSymbol;
        public String secondLegSymbol;
        public int minProfit;
        public int avgPriceFirstLeg;
        public int avgPriceSecondLeg;
        public Side firstLegSide;
        public Side secondLegSide;
        public bool bidPlaced;
        public int firstLegMode;
        public int secondLegMode;
        public int orderPriceFirstLeg;
        public int qty;
        public Order firstLegOrder;
        public Order secondLegOrder;
        public int filledQtyFirstLeg;
        public int filledQtySecondLeg;
        public bool isOrderPending;
        public int lastBidPrice;
    }
}
