/* -*- C++ -*- */

/****************************************************************************
** Copyright (c) quickfixengine.org  All rights reserved.
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifndef TRADECLIENT_APPLICATION_H
#define TRADECLIENT_APPLICATION_H

#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Mutex.h"
#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include "quickfix/fix42/OrderCancelReject.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"
#include "quickfix/fix42/MarketDataRequest.h"
#include "quickfix/fix42/MarketDataIncrementalRefresh.h"

#include "../../dismantleFIX.h"
#include <queue>

class StrategyDriver;

class Application :
      public FIX::Application,
      public FIX::MessageCracker
{
public:
    void run();

private:
  void onCreate( const FIX::SessionID& ) 
  {
    std::cout << std::endl << __LINE__ << " *****************" << std::endl;
  }
  void onLogon( const FIX::SessionID& sessionID );
  void onLogout( const FIX::SessionID& sessionID );
  void toAdmin( FIX::Message& message, const FIX::SessionID& ) 
  {
    std::cout << std::endl << __LINE__ << " *****************" << std::endl;
    
    FIX::MsgType msgType;
    message.getHeader().getField(msgType);

    if (FIELD_GET_REF(message.getHeader(), MsgType) == FIX::MsgType_Logon)
    {
      message.setField(FIX::Username("11"));
      message.setField(FIX::Password("Abhishek"));
      std::cout << "*********************" << std::endl;
    }
    dismantleFIX("From FIX-CLIENT",message.toString());

  }
  void toApp( FIX::Message&, const FIX::SessionID& )
  throw( FIX::DoNotSend );
  void fromAdmin( const FIX::Message& message, const FIX::SessionID& )
  throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) 
  {
      std::cout << std::endl << __LINE__ << " *****************" << std::endl;
      dismantleFIX("From FIX-Server",message.toString());
  }
  void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
  throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

  void onMessage( const FIX42::ExecutionReport&, const FIX::SessionID& );
  void onMessage( const FIX42::OrderCancelReject&, const FIX::SessionID& );
  void onMessage( const FIX42::MarketDataSnapshotFullRefresh&, const FIX::SessionID& );

  void queryEnterOrder();
  void queryCancelOrder();
  void queryReplaceOrder();
  void queryMarketDataRequest();
  static FIX42::NewOrderSingle queryNewOrderSingle42();

  FIX42::OrderCancelRequest queryOrderCancelRequest42();
  FIX42::OrderCancelReplaceRequest queryCancelReplaceRequest42();
  
  static FIX42::MarketDataRequest queryMarketDataRequest42();
  
  static void queryHeader( FIX::Header& header );
  static char queryAction();
  bool queryConfirm( const std::string& query );

  static FIX::SenderCompID querySenderCompID();
  static FIX::TargetCompID queryTargetCompID();
  static FIX::TargetSubID queryTargetSubID();
  static FIX::ClOrdID queryClOrdID();
  static FIX::OrigClOrdID queryOrigClOrdID();
  static FIX::Symbol querySymbol();
  static FIX::Side querySide();
  static FIX::OrderQty queryOrderQty();
  static FIX::OrdType queryOrdType();
  static FIX::Price queryPrice();
  static FIX::StopPx queryStopPx();
  static FIX::TimeInForce queryTimeInForce();
  static FIX::SecurityType querySecurityType();
  static FIX::SecurityExchange querySecurityExchange();
  static FIX::MaturityMonthYear queryMaturityMonthYear();
  static FIX::ExpireDate queryMaturityDate();
  static FIX::MaturityDay queryMaturityDay();
  static FIX::PutOrCall queryOptionMode();
  static FIX::StrikePrice queryStrikePrice();

public :

  static FIX42::MarketDataRequest  
    queryMarketDataRequest42(
      const FIX::Symbol &symbol,
      const FIX::SecurityExchange &exchange,
      const FIX::SecurityType &securityType,
      const FIX::MaturityDay &maturityDay,
      const FIX::MaturityMonthYear &maturityMonthYear,
      const FIX::StrikePrice &strikePrice,
      const FIX::PutOrCall &putCall,
      const FIX::SenderCompID &senderCompId,
      const FIX::TargetCompID &TargetSubID);



  static std::string getMarketDataUniqueSignature(const FIX42::Message *message); 

  static std::string getMarketDataUniqueSignature(const FIX42::MarketDataRequest &message) ;
  
  static FIX42::NewOrderSingle queryNewOrderSingle42(
      const FIX::ClOrdID &clordId,
      const FIX::Symbol &symbol,
      const FIX::Side &side,
      const FIX::OrdType &orderType,
      const FIX::SecurityExchange &securityExchange,
      const FIX::OrderQty &orderQty,
      const FIX::Price &price,
      const FIX::StopPx &stopPrice,
      const FIX::TimeInForce &timeInForce,
      const FIX::SecurityType &securityType,
      const FIX::MaturityMonthYear &maturityMonthYear,
      const FIX::MaturityDay &maturityDay,
      const FIX::StrikePrice &strikePrice,
      const FIX::PutOrCall &putOrCall,
      const FIX::SenderCompID &senderCompId,
      const FIX::TargetCompID &targetCompId);


  static FIX42::OrderCancelReplaceRequest queryCancelReplaceRequest42(
      const FIX::OrdType &orderType,
      const FIX::OrigClOrdID &origClOrdId,
      const FIX::ClOrdID &clOrdId,
      const FIX::Symbol &symbol,
      const FIX::Side &side,
      const FIX::Price &price,
      const FIX::OrderQty &orderQty,
      const FIX::SenderCompID &senderCompID,
      const FIX::TargetCompID &targetCompID);

  
  static FIX42::OrderCancelRequest queryOrderCancelRequest42(
    const FIX::OrigClOrdID &origClOrdID,
    const FIX::ClOrdID &clOrdID,
    const FIX::Symbol &symbol,
    const FIX::Side &side,
    const FIX::SenderCompID &senderCompID,
    const FIX::TargetCompID &targetCompID);
};

#endif
