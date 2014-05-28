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

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#else
//#include "config.h"
#endif

#include "application.h"
#include "quickfix/Session.h"
#include "../../dismantleFIX.h"
#include <iostream>
#include "samplestrategy.h"
#include "futFutStrategy.h"
void Application::onLogon( const FIX::SessionID& sessionID )
{
    std::cout << std::endl << "Logon - " << sessionID << std::endl;
}

void Application::onLogout( const FIX::SessionID& sessionID )
{
    std::cout << std::endl << "Logout - " << sessionID << std::endl;
}

void Application::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
    //    std::cout << std::endl << __LINE__ << " *****************" << std::endl;
    //    std::cout << message.toXML() << std::endl;
    //    std::cout << message.toString() << std::endl;
    crack( message, sessionID );
    dismantleFIX("From Fix String",message.toString());
    //    std::cout << std::endl << "IN: " << message << std::endl;
}

void Application::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::DoNotSend )
{
    std::cout << std::endl << __LINE__ << " *****************" << std::endl;
    try
    {
        FIX::PossDupFlag possDupFlag;
        message.getHeader().getField( possDupFlag );
        if ( possDupFlag ) throw FIX::DoNotSend();
    }
    catch ( FIX::FieldNotFound& ) {}

    std::cout << std::endl
              << "OUT: " << message << std::endl;
}

//void Application::onMessage
//( const FIX40::ExecutionReport&, const FIX::SessionID& ) {}
//void Application::onMessage
//( const FIX40::OrderCancelReject&, const FIX::SessionID& ) {}
//void Application::onMessage
//( const FIX41::ExecutionReport&, const FIX::SessionID& ) {}
//void Application::onMessage
//( const FIX41::OrderCancelReject&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX42::ExecutionReport& report, const FIX::SessionID& ) {

    StrategyDriver::getInstance()->onExecutionReport(report);

}
void Application::onMessage
( const FIX42::OrderCancelReject&, const FIX::SessionID& ) {}
//void Application::onMessage
//( const FIX43::ExecutionReport&, const FIX::SessionID& ) {}
//void Application::onMessage
//( const FIX43::OrderCancelReject&, const FIX::SessionID& ) {}
//void Application::onMessage
//( const FIX44::ExecutionReport&, const FIX::SessionID& ) {}
//void Application::onMessage
//( const FIX44::OrderCancelReject&, const FIX::SessionID& ) {}
//void Application::onMessage
//( const FIX50::ExecutionReport&, const FIX::SessionID& ) {}
//void Application::onMessage
//( const FIX50::OrderCancelReject&, const FIX::SessionID& ) {}

void Application::onMessage
( const FIX42::MarketDataSnapshotFullRefresh& report, const FIX::SessionID& )
{
    std::cout << "========= Got Feed ========= " << std::endl;
    StrategyDriver::getInstance()->onMarketDataSnampshot(report);

}

void Application::run()
{
  std::vector< boost::shared_ptr<AbstractStrategy> > strategyList;
  //int strategyId = 1;
  while ( true )
  {
    try
    {
      char action = queryAction();

      if ( action == '1' )
        queryEnterOrder();
      else if ( action == '2' )
        queryCancelOrder();
      else if ( action == '3' )
        queryReplaceOrder();
      else if ( action == '4' )
        queryMarketDataRequest();
      else if ( action == '5' )
      {
        strategyList.push_back(
            boost::shared_ptr<AbstractStrategy>(new FutFutStrategy()));
        strategyList[strategyList.size()-1]->subscribeMarketDataFeed();
      }
      else if ( action == '6' )
        break;

      std::vector< boost::shared_ptr<AbstractStrategy> >::iterator it = strategyList.begin();
      sleep(1);
      std::cout<<strategyList.size()<<std::endl;
      while(it!= strategyList.end())
      {
        if((**it).isAlive() == false)
        {
          it = strategyList.erase(it);
          std::cout<<"deleting strategy\n";
        }
        else
        {
          it++;
        }
      }
    }
    catch ( std::exception & e )
    {
      std::cout << "Message Not Sent: " << e.what();
    }
  }
}

void Application::queryEnterOrder()
{
    //    int version = queryVersion();
    std::cout << "\nNewOrderSingle\n";
    FIX::Message order;

    //    switch ( version ) {
    //    case 40:
    //        order = queryNewOrderSingle40();
    //        break;
    //    case 41:
    //        order = queryNewOrderSingle41();
    //        break;
    //    case 42:
    order = queryNewOrderSingle42();
    //        break;
    //    case 43:
    //        order = queryNewOrderSingle43();
    //        break;
    //    case 44:
    //        order = queryNewOrderSingle44();
    //        break;
    //    case 50:
    //        order = queryNewOrderSingle50();
    //        break;
    //    default:
    //        std::cerr << "No test for version " << version << std::endl;
    //        break;
    //    }
    if ( queryConfirm( "Send order" ) )
        FIX::Session::sendToTarget( order );
}

void Application::queryCancelOrder()
{
    //    int version = queryVersion();
    std::cout << "\nOrderCancelRequest\n";
    FIX::Message cancel;

    //    switch ( version ) {
    //    case 40:
    //        cancel = queryOrderCancelRequest40();
    //        break;
    //    case 41:
    //        cancel = queryOrderCancelRequest41();
    //        break;
    //    case 42:
    cancel = queryOrderCancelRequest42();
    //        break;
    //    case 43:
    //        cancel = queryOrderCancelRequest43();
    //        break;
    //    case 44:
    //        cancel = queryOrderCancelRequest44();
    //        break;
    //    case 50:
    //        cancel = queryOrderCancelRequest50();
    //        break;
    //    default:
    //        std::cerr << "No test for version " << version << std::endl;
    //        break;
    //    }

    if ( queryConfirm( "Send cancel" ) )
        FIX::Session::sendToTarget( cancel );
}

void Application::queryReplaceOrder()
{
    //    int version = queryVersion();
    std::cout << "\nCancelReplaceRequest\n";
    FIX::Message replace;

    //    switch ( version ) {
    //    case 40:
    //        replace = queryCancelReplaceRequest40();
    //        break;
    //    case 41:
    //        replace = queryCancelReplaceRequest41();
    //        break;
    //    case 42:
    replace = queryCancelReplaceRequest42();
    //        break;
    //    case 43:
    //        replace = queryCancelReplaceRequest43();
    //        break;
    //    case 44:
    //        replace = queryCancelReplaceRequest44();
    //        break;
    //    case 50:
    //        replace = queryCancelReplaceRequest50();
    //        break;
    //    default:
    //        std::cerr << "No test for version " << version << std::endl;
    //        break;
    //    }

    if ( queryConfirm( "Send replace" ) )
        FIX::Session::sendToTarget( replace );
}

void Application::queryMarketDataRequest()
{
    //    int version = queryVersion();
    std::cout << "\nMarketDataRequest\n";
    FIX::Message md;

    //    switch (version) {
    //    case 42:
    md = queryMarketDataRequest42();
    //        break;
    //    case 43:
    //        md = queryMarketDataRequest43();
    //        break;
    //    case 44:
    //        md = queryMarketDataRequest44();
    //        break;
    //    case 50:
    //        md = queryMarketDataRequest50();
    //        break;
    //    default:
    //        std::cerr << "No test for version " << version << std::endl;
    //        break;
    //    }

    FIX::Session::sendToTarget( md );
}
/*
FIX40::NewOrderSingle Application::queryNewOrderSingle40()
{
    FIX::OrdType ordType;

    FIX40::NewOrderSingle newOrderSingle(
                queryClOrdID(), FIX::HandlInst( '1' ), querySymbol(), querySide(),
                queryOrderQty(), ordType = queryOrdType() );

    newOrderSingle.set( queryTimeInForce() );
    if ( ordType == FIX::OrdType_LIMIT || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryPrice() );
    if ( ordType == FIX::OrdType_STOP || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryStopPx() );

    queryHeader( newOrderSingle.getHeader() );
    return newOrderSingle;
}

FIX41::NewOrderSingle Application::queryNewOrderSingle41()
{
    FIX::OrdType ordType;

    FIX41::NewOrderSingle newOrderSingle(
                queryClOrdID(), FIX::HandlInst( '1' ), querySymbol(), querySide(),
                ordType = queryOrdType() );

    newOrderSingle.set( queryOrderQty() );
    newOrderSingle.set( queryTimeInForce() );
    if ( ordType == FIX::OrdType_LIMIT || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryPrice() );
    if ( ordType == FIX::OrdType_STOP || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryStopPx() );

    queryHeader( newOrderSingle.getHeader() );
    return newOrderSingle;
}
*/
FIX42::NewOrderSingle Application::queryNewOrderSingle42()
{

    std::cout << __FUNCTION__ <<std::endl;
    FIX::OrdType ordType;

    FIX42::NewOrderSingle newOrderSingle(
                queryClOrdID(), FIX::HandlInst( '1' ), querySymbol(), querySide(),
                FIX::TransactTime(), ordType = queryOrdType() );

    newOrderSingle.set( querySecurityExchange());
    newOrderSingle.set( queryOrderQty() );
    newOrderSingle.set( queryTimeInForce() );
    FIX::SecurityType securityType = querySecurityType();
    newOrderSingle.set( securityType);

    std::string secType = FIX::StringField(securityType);
    if(secType == "FUT" || secType == "OPT")
    {
        newOrderSingle.set( queryMaturityMonthYear() );
        newOrderSingle.set( queryMaturityDay() );
    }
    if(secType == "OPT")
    {
        newOrderSingle.set( queryStrikePrice() );
        newOrderSingle.set( queryOptionMode() );
    }



    //    newOrderSingle.set( FIX::MaturityMonthYear(FIX::MONTHYEAR("012014")));
    //FIX::LOCALMKTDATE date("30");
    //newOrderSingle.set( FIX::MaturityDate(date));
    //newOrderSingle.set( FIX::MaturityDay(FIX::("30")));


    if ( ordType == FIX::OrdType_LIMIT || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryPrice() );
    if ( ordType == FIX::OrdType_STOP || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryStopPx() );

    queryHeader( newOrderSingle.getHeader() );
    return newOrderSingle;
}

FIX42::NewOrderSingle Application::queryNewOrderSingle42
(const FIX::ClOrdID &clordId,
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
 const FIX::TargetCompID &targetCompId)
{

    std::cout << __FUNCTION__ <<std::endl;
    FIX42::NewOrderSingle newOrderSingle(
                clordId, FIX::HandlInst( '1' ), symbol,side,
                FIX::TransactTime(),orderType);

    newOrderSingle.set( securityExchange);
    newOrderSingle.set( orderQty );
    newOrderSingle.set( timeInForce );
    newOrderSingle.set( securityType);

    std::string secType = FIX::StringField(securityType);
    if(secType == "FUT" || secType == "OPT")
    {
        newOrderSingle.set( maturityMonthYear );
        newOrderSingle.set( maturityDay );
    }
    if(secType == "OPT")
    {
        newOrderSingle.set( strikePrice );
        newOrderSingle.set( putOrCall );
    }



    //    newOrderSingle.set( FIX::MaturityMonthYear(FIX::MONTHYEAR("012014")));
    //FIX::LOCALMKTDATE date("30");
    //newOrderSingle.set( FIX::MaturityDate(date));
    //newOrderSingle.set( FIX::MaturityDay(FIX::("30")));


    if ( orderType == FIX::OrdType_LIMIT || orderType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( price );
    if ( orderType == FIX::OrdType_STOP || orderType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( stopPrice );

    newOrderSingle.getHeader().setField( senderCompId );
    newOrderSingle.getHeader().setField( targetCompId );
    return newOrderSingle;
}
/*
FIX43::NewOrderSingle Application::queryNewOrderSingle43()
{
    FIX::OrdType ordType;

    FIX43::NewOrderSingle newOrderSingle(
                queryClOrdID(), FIX::HandlInst( '1' ), querySide(),
                FIX::TransactTime(), ordType = queryOrdType() );

    newOrderSingle.set( querySymbol() );
    newOrderSingle.set( queryOrderQty() );
    newOrderSingle.set( queryTimeInForce() );
    if ( ordType == FIX::OrdType_LIMIT || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryPrice() );
    if ( ordType == FIX::OrdType_STOP || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryStopPx() );

    queryHeader( newOrderSingle.getHeader() );
    return newOrderSingle;
}

FIX44::NewOrderSingle Application::queryNewOrderSingle44()
{
    FIX::OrdType ordType;

    FIX44::NewOrderSingle newOrderSingle(
                queryClOrdID(), querySide(),
                FIX::TransactTime(), ordType = queryOrdType() );

    newOrderSingle.set( FIX::HandlInst('1') );
    newOrderSingle.set( querySymbol() );
    newOrderSingle.set( queryOrderQty() );
    newOrderSingle.set( queryTimeInForce() );
    if ( ordType == FIX::OrdType_LIMIT || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryPrice() );
    if ( ordType == FIX::OrdType_STOP || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryStopPx() );

    queryHeader( newOrderSingle.getHeader() );
    return newOrderSingle;
}

FIX50::NewOrderSingle Application::queryNewOrderSingle50()
{
    FIX::OrdType ordType;

    FIX50::NewOrderSingle newOrderSingle(
                queryClOrdID(), querySide(),
                FIX::TransactTime(), ordType = queryOrdType() );

    newOrderSingle.set( FIX::HandlInst('1') );
    newOrderSingle.set( querySymbol() );
    newOrderSingle.set( queryOrderQty() );
    newOrderSingle.set( queryTimeInForce() );
    if ( ordType == FIX::OrdType_LIMIT || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryPrice() );
    if ( ordType == FIX::OrdType_STOP || ordType == FIX::OrdType_STOP_LIMIT )
        newOrderSingle.set( queryStopPx() );

    queryHeader( newOrderSingle.getHeader() );
    return newOrderSingle;
}

FIX40::OrderCancelRequest Application::queryOrderCancelRequest40()
{
    FIX40::OrderCancelRequest orderCancelRequest(
                queryOrigClOrdID(), queryClOrdID(), FIX::CxlType( 'F' ),
                querySymbol(), querySide(), queryOrderQty() );

    queryHeader( orderCancelRequest.getHeader() );
    return orderCancelRequest;
}

FIX41::OrderCancelRequest Application::queryOrderCancelRequest41()
{
    FIX41::OrderCancelRequest orderCancelRequest(
                queryOrigClOrdID(), queryClOrdID(), querySymbol(), querySide() );

    orderCancelRequest.set( queryOrderQty() );
    queryHeader( orderCancelRequest.getHeader() );
    return orderCancelRequest;
}

*/
FIX42::OrderCancelRequest Application::queryOrderCancelRequest42()
{
    FIX42::OrderCancelRequest orderCancelRequest( queryOrigClOrdID(),
                                                  queryClOrdID(), querySymbol(), querySide(), FIX::TransactTime() );

    //    FIX42::OrderCancelRequest orderCancelRequest;

    //    orderCancelRequest.set( queryOrigClOrdID() );
    //    orderCancelRequest.set( queryClOrdID() );
    //    orderCancelRequest.set( querySide() );
    //    orderCancelRequest.set( querySymbol() );

    queryHeader( orderCancelRequest.getHeader() );
    return orderCancelRequest;
}

FIX42::OrderCancelRequest Application::queryOrderCancelRequest42(
    const FIX::OrigClOrdID &origClOrdID,
    const FIX::ClOrdID &clOrdID,
    const FIX::Symbol &symbol,
    const FIX::Side &side,
    const FIX::SenderCompID &senderCompID,
    const FIX::TargetCompID &targetCompID)
{
    FIX42::OrderCancelRequest orderCancelRequest( origClOrdID,
                                                  clOrdID, 
                                                  symbol, 
                                                  side, 
                                                  FIX::TransactTime() );

    //    FIX42::OrderCancelRequest orderCancelRequest;

    //    orderCancelRequest.set( queryOrigClOrdID() );
    //    orderCancelRequest.set( queryClOrdID() );
    //    orderCancelRequest.set( querySide() );
    //    orderCancelRequest.set( querySymbol() );

    orderCancelRequest.getHeader().setField( senderCompID );
    orderCancelRequest.getHeader().setField( targetCompID );
    
    return orderCancelRequest;
}

/*
FIX43::OrderCancelRequest Application::queryOrderCancelRequest43()
{
    FIX43::OrderCancelRequest orderCancelRequest( queryOrigClOrdID(),
                                                  queryClOrdID(), querySide(), FIX::TransactTime() );

    orderCancelRequest.set( querySymbol() );
    orderCancelRequest.set( queryOrderQty() );
    queryHeader( orderCancelRequest.getHeader() );
    return orderCancelRequest;
}

FIX44::OrderCancelRequest Application::queryOrderCancelRequest44()
{
    FIX44::OrderCancelRequest orderCancelRequest( queryOrigClOrdID(),
                                                  queryClOrdID(), querySide(), FIX::TransactTime() );

    orderCancelRequest.set( querySymbol() );
    orderCancelRequest.set( queryOrderQty() );
    queryHeader( orderCancelRequest.getHeader() );
    return orderCancelRequest;
}

FIX50::OrderCancelRequest Application::queryOrderCancelRequest50()
{
    FIX50::OrderCancelRequest orderCancelRequest( queryOrigClOrdID(),
                                                  queryClOrdID(), querySide(), FIX::TransactTime() );

    orderCancelRequest.set( querySymbol() );
    orderCancelRequest.set( queryOrderQty() );
    queryHeader( orderCancelRequest.getHeader() );
    return orderCancelRequest;
}

FIX40::OrderCancelReplaceRequest Application::queryCancelReplaceRequest40()
{
    FIX40::OrderCancelReplaceRequest cancelReplaceRequest(
                queryOrigClOrdID(), queryClOrdID(), FIX::HandlInst( '1' ),
                querySymbol(), querySide(), queryOrderQty(), queryOrdType() );

    if ( queryConfirm( "New price" ) )
        cancelReplaceRequest.set( queryPrice() );
    if ( queryConfirm( "New quantity" ) )
        cancelReplaceRequest.set( queryOrderQty() );

    queryHeader( cancelReplaceRequest.getHeader() );
    return cancelReplaceRequest;
}

FIX41::OrderCancelReplaceRequest Application::queryCancelReplaceRequest41()
{
    FIX41::OrderCancelReplaceRequest cancelReplaceRequest(
                queryOrigClOrdID(), queryClOrdID(), FIX::HandlInst( '1' ),
                querySymbol(), querySide(), queryOrdType() );

    if ( queryConfirm( "New price" ) )
        cancelReplaceRequest.set( queryPrice() );
    if ( queryConfirm( "New quantity" ) )
        cancelReplaceRequest.set( queryOrderQty() );

    queryHeader( cancelReplaceRequest.getHeader() );
    return cancelReplaceRequest;
}

*/
FIX42::OrderCancelReplaceRequest Application::queryCancelReplaceRequest42()
{

    FIX::OrdType oOrderType = queryOrdType();

    FIX42::OrderCancelReplaceRequest cancelReplaceRequest(
                queryOrigClOrdID(), queryClOrdID(), FIX::HandlInst( '1' ),
                querySymbol(), querySide(), FIX::TransactTime(), oOrderType );


    if(oOrderType == FIX::OrdType_LIMIT){
        if ( queryConfirm( "New price" ) )
            cancelReplaceRequest.set( queryPrice() );
    }
    if ( queryConfirm( "New quantity" ) )
    {
        std::cout << "============== Setting Qty ============ "<<std::endl;

        cancelReplaceRequest.set( queryOrderQty() );
    }

    queryHeader( cancelReplaceRequest.getHeader() );
    return cancelReplaceRequest;
}

FIX42::OrderCancelReplaceRequest Application::queryCancelReplaceRequest42(
    const FIX::OrdType &orderType,
    const FIX::OrigClOrdID &origClOrdId,
    const FIX::ClOrdID &clOrdId,
    const FIX::Symbol &symbol,
    const FIX::Side &side,
    const FIX::Price &price,
    const FIX::OrderQty &orderQty,
    const FIX::SenderCompID &senderCompID,
    const FIX::TargetCompID &targetCompID)
{


    FIX42::OrderCancelReplaceRequest cancelReplaceRequest(
                origClOrdId, clOrdId, FIX::HandlInst( '1' ),
                symbol, side, FIX::TransactTime(), orderType );


    if(orderType == FIX::OrdType_LIMIT){
            cancelReplaceRequest.set( price );
    }
    cancelReplaceRequest.set( orderQty );
    
    cancelReplaceRequest.getHeader().setField( senderCompID );
    cancelReplaceRequest.getHeader().setField( targetCompID );

    return cancelReplaceRequest;
}
/*
FIX43::OrderCancelReplaceRequest Application::queryCancelReplaceRequest43()
{
    FIX43::OrderCancelReplaceRequest cancelReplaceRequest(
                queryOrigClOrdID(), queryClOrdID(), FIX::HandlInst( '1' ),
                querySide(), FIX::TransactTime(), queryOrdType() );

    cancelReplaceRequest.set( querySymbol() );
    if ( queryConfirm( "New price" ) )
        cancelReplaceRequest.set( queryPrice() );
    if ( queryConfirm( "New quantity" ) )
        cancelReplaceRequest.set( queryOrderQty() );

    queryHeader( cancelReplaceRequest.getHeader() );
    return cancelReplaceRequest;
}

FIX44::OrderCancelReplaceRequest Application::queryCancelReplaceRequest44()
{
    FIX44::OrderCancelReplaceRequest cancelReplaceRequest(
                queryOrigClOrdID(), queryClOrdID(),
                querySide(), FIX::TransactTime(), queryOrdType() );

    cancelReplaceRequest.set( FIX::HandlInst('1') );
    cancelReplaceRequest.set( querySymbol() );
    if ( queryConfirm( "New price" ) )
        cancelReplaceRequest.set( queryPrice() );
    if ( queryConfirm( "New quantity" ) )
        cancelReplaceRequest.set( queryOrderQty() );

    queryHeader( cancelReplaceRequest.getHeader() );
    return cancelReplaceRequest;
}

FIX50::OrderCancelReplaceRequest Application::queryCancelReplaceRequest50()
{
    FIX50::OrderCancelReplaceRequest cancelReplaceRequest(
                queryOrigClOrdID(), queryClOrdID(),
                querySide(), FIX::TransactTime(), queryOrdType() );

    cancelReplaceRequest.set( FIX::HandlInst('1') );
    cancelReplaceRequest.set( querySymbol() );
    if ( queryConfirm( "New price" ) )
        cancelReplaceRequest.set( queryPrice() );
    if ( queryConfirm( "New quantity" ) )
        cancelReplaceRequest.set( queryOrderQty() );

    queryHeader( cancelReplaceRequest.getHeader() );
    return cancelReplaceRequest;
}

*/
FIX42::MarketDataRequest Application::queryMarketDataRequest42()
{
    FIX::MDReqID mdReqID( "MARKETDATAID" );
    FIX::SubscriptionRequestType subType( FIX::SubscriptionRequestType_SNAPSHOT );
    FIX::MarketDepth marketDepth( 5 );

    FIX42::MarketDataRequest::NoMDEntryTypes marketDataEntryGroup;
    FIX::MDEntryType mdEntryType( FIX::MDEntryType_BID );
    marketDataEntryGroup.set( mdEntryType );

    FIX42::MarketDataRequest::NoRelatedSym symbolGroup;
    //    FIX::Symbol symbol( querySymbol() );

    symbolGroup.set( querySymbol() );
    symbolGroup.set( querySecurityExchange());

    FIX::SecurityType securityType = querySecurityType();
    symbolGroup.set ( securityType );
    std::string secType = FIX::StringField(securityType);
    if(secType == "FUT" || secType == "OPT")
    {
        symbolGroup.set( queryMaturityDay() );
        symbolGroup.set( queryMaturityMonthYear() );
    }
    if(secType == "OPT")
    {
        symbolGroup.set( queryStrikePrice() );
        symbolGroup.set( queryOptionMode() );
    }

    FIX42::MarketDataRequest message( mdReqID, subType, marketDepth );
    message.addGroup( marketDataEntryGroup );
    message.addGroup( symbolGroup );

    queryHeader( message.getHeader() );

    std::cout << message.toXML() << std::endl;
    std::cout << message.toString() << std::endl;

    return message;
}

FIX42::MarketDataRequest Application::queryMarketDataRequest42(
    const FIX::Symbol &symbol,
    const FIX::SecurityExchange &exchange,
    const FIX::SecurityType &securityType,
    const FIX::MaturityDay &maturityDay,
    const FIX::MaturityMonthYear &maturityMonthYear,
    const FIX::StrikePrice &strikePrice,
    const FIX::PutOrCall &putCall,
    const FIX::SenderCompID &senderCompId,
    const FIX::TargetCompID &targetCompId)
{
  FIX::MDReqID mdReqID( "MARKETDATAID" );
  FIX::SubscriptionRequestType subType( FIX::SubscriptionRequestType_SNAPSHOT );
  FIX::MarketDepth marketDepth( 5 );

  FIX42::MarketDataRequest::NoMDEntryTypes marketDataEntryGroup;
  FIX::MDEntryType mdEntryType( FIX::MDEntryType_BID );
  marketDataEntryGroup.set( mdEntryType );

  FIX42::MarketDataRequest::NoRelatedSym symbolGroup;
  //    FIX::Symbol symbol( querySymbol() );

  symbolGroup.set( symbol );
  symbolGroup.set( exchange);

  symbolGroup.set ( securityType );
  std::string secType = FIX::StringField(securityType);
  if(secType == "FUT" || secType == "OPT")
  {
    symbolGroup.set( maturityDay );
    symbolGroup.set( maturityMonthYear );
  }
  if(secType == "OPT")
  {
    symbolGroup.set( strikePrice );
    symbolGroup.set( putCall );
  }

  FIX42::MarketDataRequest message( mdReqID, subType, marketDepth );
  message.addGroup( marketDataEntryGroup );
  message.addGroup( symbolGroup );
  message.getHeader().setField(senderCompId);
  message.getHeader().setField(targetCompId);
  

  std::cout << message.toXML() << std::endl;
  std::cout << message.toString() << std::endl;
  return message;
}

std::string Application::getMarketDataUniqueSignature(const FIX42::Message *message) 
{
  std::string signature;
  std::ostringstream ss;

  FIX::Symbol symbol;
  FIX::SecurityExchange exchange;
  FIX::SecurityType securityType;

  message->getField(exchange);
  message->getField(symbol);
  message->getField(securityType);

  ss<<exchange<<","
    <<symbol<<","
    <<securityType;

  if(securityType == "FUT" || securityType == "OPT")
  {
    FIX::MaturityDay maturityDay;
    FIX::MaturityMonthYear maturityMonthYear;

    message->getField(maturityDay);
    message->getField(maturityMonthYear);

    ss<<","
      <<maturityDay<<","
      <<maturityMonthYear;

  }
  if(securityType == "OPT")
  {
    FIX::StrikePrice strikePrice;
    FIX::PutOrCall putOrCall;

    message->getField(strikePrice);
    message->getField(putOrCall);

    ss<<","
      <<putOrCall<<","
      <<strikePrice;
  }

  signature = ss.str();
  std::cout<<signature<<std::endl;
  return signature ;

}

std::string Application::getMarketDataUniqueSignature(const FIX42::MarketDataRequest &marketDataRequest)
{
  FIX42::MarketDataRequest::NoRelatedSym symbolGroup;
  marketDataRequest.getGroup(1,symbolGroup);
  return getMarketDataUniqueSignature((const FIX42::Message*)&symbolGroup) ;
}

/*
FIX43::MarketDataRequest Application::queryMarketDataRequest43()
{
    FIX::MDReqID mdReqID( "MARKETDATAID" );
    FIX::SubscriptionRequestType subType( FIX::SubscriptionRequestType_SNAPSHOT );
    FIX::MarketDepth marketDepth( 0 );

    FIX43::MarketDataRequest::NoMDEntryTypes marketDataEntryGroup;
    FIX::MDEntryType mdEntryType( FIX::MDEntryType_BID );
    marketDataEntryGroup.set( mdEntryType );

    FIX43::MarketDataRequest::NoRelatedSym symbolGroup;
    FIX::Symbol symbol( "LNUX" );
    symbolGroup.set( symbol );

    FIX43::MarketDataRequest message( mdReqID, subType, marketDepth );
    message.addGroup( marketDataEntryGroup );
    message.addGroup( symbolGroup );


    queryHeader( message.getHeader() );

    std::cout << message.toXML() << std::endl;
    std::cout << message.toString() << std::endl;

    return message;
}

FIX44::MarketDataRequest Application::queryMarketDataRequest44()
{
    FIX::MDReqID mdReqID( "MARKETDATAID" );
    FIX::SubscriptionRequestType subType( FIX::SubscriptionRequestType_SNAPSHOT );
    FIX::MarketDepth marketDepth( 0 );

    FIX44::MarketDataRequest::NoMDEntryTypes marketDataEntryGroup;
    FIX::MDEntryType mdEntryType( FIX::MDEntryType_BID );
    marketDataEntryGroup.set( mdEntryType );

    FIX44::MarketDataRequest::NoRelatedSym symbolGroup;
    FIX::Symbol symbol( "LNUX" );
    symbolGroup.set( symbol );

    FIX44::MarketDataRequest message( mdReqID, subType, marketDepth );
    message.addGroup( marketDataEntryGroup );
    message.addGroup( symbolGroup );

    queryHeader( message.getHeader() );

    std::cout << message.toXML() << std::endl;
    std::cout << message.toString() << std::endl;

    return message;
}

FIX50::MarketDataRequest Application::queryMarketDataRequest50()
{
    FIX::MDReqID mdReqID( "MARKETDATAID" );
    FIX::SubscriptionRequestType subType( FIX::SubscriptionRequestType_SNAPSHOT );
    FIX::MarketDepth marketDepth( 0 );

    FIX50::MarketDataRequest::NoMDEntryTypes marketDataEntryGroup;
    FIX::MDEntryType mdEntryType( FIX::MDEntryType_BID );
    marketDataEntryGroup.set( mdEntryType );

    FIX50::MarketDataRequest::NoRelatedSym symbolGroup;
    FIX::Symbol symbol( "LNUX" );
    symbolGroup.set( symbol );

    FIX50::MarketDataRequest message( mdReqID, subType, marketDepth );
    message.addGroup( marketDataEntryGroup );
    message.addGroup( symbolGroup );

    queryHeader( message.getHeader() );

    std::cout << message.toXML() << std::endl;
    std::cout << message.toString() << std::endl;

    return message;
}

*/
void Application::queryHeader( FIX::Header& header )
{
    header.setField( querySenderCompID() );
    header.setField( queryTargetCompID() );

    //if ( queryConfirm( "Use a TargetSubID" ) )
    //  header.setField( queryTargetSubID() );
}

char Application::queryAction()
{
    char value;
    std::cout << std::endl
              << "1) Enter Order" << std::endl
              << "2) Cancel Order" << std::endl
              << "3) Replace Order" << std::endl
              << "4) Market data test" << std::endl
              << "5) Fut Fut Strategy" << std::endl
              << "6) Quit" << std::endl
              << "Action: ";
    std::cin >> value;
    switch ( value )
    {
      case '1': case '2': case '3': case '4': case '5': case '6': break;
    default: throw std::exception();
    }
    return value;
}
/*
int Application::queryVersion()
{
    return 42;
    char value;
    std::cout << std::endl
              << "1) FIX.4.0" << std::endl
              << "2) FIX.4.1" << std::endl
              << "3) FIX.4.2" << std::endl
              << "4) FIX.4.3" << std::endl
              << "5) FIX.4.4" << std::endl
              << "6) FIXT.1.1 (FIX.5.0)" << std::endl
              << "BeginString: ";
    //std::cin >> value;
    value = '3';
    switch ( value )
    {
    case '1': return 40;
    case '2': return 41;
    case '3': return 42;
    case '4': return 43;
    case '5': return 44;
    case '6': return 50;
    default: throw std::exception();
    }
}
*/
bool Application::queryConfirm( const std::string& query )
{
    std::string value;
    std::cout << std::endl << query << "?: ";
    std::cin >> value;
    //value = 'Y';
    return (toupper( *value.c_str() ) == 'Y') || (*value.c_str() == '1');
}

FIX::SenderCompID Application::querySenderCompID()
{
    std::string value;
    std::cout << std::endl << "SenderCompID: ";
    //std::cin >> value;
    value = "CLIENT5";
    return FIX::SenderCompID( value );
}

FIX::TargetCompID Application::queryTargetCompID()
{
    std::string value;
    std::cout << std::endl << "TargetCompID: ";
    //std::cin >> value;
    value = "EXECUTOR1";
    return FIX::TargetCompID( value );
}

FIX::TargetSubID Application::queryTargetSubID()
{
    std::string value;
    std::cout << std::endl << "TargetSubID: ";
    //std::cin >> value;
    value = "1";
    return FIX::TargetSubID( value );
}

FIX::ClOrdID Application::queryClOrdID()
{
    std::string value;
    std::cout << std::endl << "ClOrdID: ";
    std::cin >> value;
    //    value = "0";
    return FIX::ClOrdID( value );
}

FIX::OrigClOrdID Application::queryOrigClOrdID()
{
    std::string value;
    std::cout << std::endl << "OrigClOrdID: ";
    std::cin >> value;
    return FIX::OrigClOrdID( value );
}

FIX::Symbol Application::querySymbol()
{
    std::string value;
    std::cout << std::endl << "Symbol: ";
    //value = "30044263";
    value = "INFY";
    //        std::cin >> value;
    return FIX::Symbol( value );
}

FIX::Side Application::querySide()
{
    char value;
    std::cout << std::endl
              << "1) Buy" << std::endl
              << "2) Sell" << std::endl
                 //              << "3) Sell Short" << std::endl
                 //              << "4) Sell Short Exempt" << std::endl
                 //              << "5) Cross" << std::endl
                 //              << "6) Cross Short" << std::endl
                 //              << "7) Cross Short Exempt" << std::endl
              << "Side: ";

    std::cin >> value;
    //    value = '1';
    switch ( value )
    {
    case '1': return FIX::Side( FIX::Side_BUY );
    case '2': return FIX::Side( FIX::Side_SELL );
        //    case '3': return FIX::Side( FIX::Side_SELL_SHORT );
        //    case '4': return FIX::Side( FIX::Side_SELL_SHORT_EXEMPT );
        //    case '5': return FIX::Side( FIX::Side_CROSS );
        //    case '6': return FIX::Side( FIX::Side_CROSS_SHORT );
        //    case '7': return FIX::Side( 'A' );
    default: throw std::exception();
    }
}

FIX::OrderQty Application::queryOrderQty()
{
    long value;
    std::cout << std::endl << "OrderQty: ";
    std::cin >> value;
    //value = 500;
    return FIX::OrderQty( value );
}

FIX::OrdType Application::queryOrdType()
{
    char value;
    std::cout << std::endl
              << "1) Market" << std::endl
              << "2) Limit" << std::endl
                 //              << "3) Stop" << std::endl
                 //              << "4) Stop Limit" << std::endl
              << "OrdType: ";

    std::cin >> value;
    //    value ='2';
    switch ( value )
    {
    case '1': return FIX::OrdType( FIX::OrdType_MARKET );
    case '2': return FIX::OrdType( FIX::OrdType_LIMIT );
        //    case '3': return FIX::OrdType( FIX::OrdType_STOP );
        //    case '4': return FIX::OrdType( FIX::OrdType_STOP_LIMIT );
    default: throw std::exception();
    }
}

FIX::Price Application::queryPrice()
{
    double value;
    std::cout << std::endl << "Price: ";
    std::cin >> value;
    //value = 25100;
    return FIX::Price( value );
}

FIX::StopPx Application::queryStopPx()
{
    double value;
    std::cout << std::endl << "StopPx: ";
    std::cin >> value;
    return FIX::StopPx( value );
}

FIX::TimeInForce Application::queryTimeInForce()
{
    char value;
    std::cout << std::endl
              << "1) Day" << std::endl
              << "2) IOC" << std::endl
              << "3) OPG" << std::endl
              << "4) GTC" << std::endl
              << "5) GTX" << std::endl
              << "TimeInForce: ";

    std::cin >> value;
    //    value = '1';
    switch ( value )
    {
    case '1': return FIX::TimeInForce( FIX::TimeInForce_DAY );
    case '2': return FIX::TimeInForce( FIX::TimeInForce_IMMEDIATE_OR_CANCEL );
    case '3': return FIX::TimeInForce( FIX::TimeInForce_AT_THE_OPENING );
    case '4': return FIX::TimeInForce( FIX::TimeInForce_GOOD_TILL_CANCEL );
    case '5': return FIX::TimeInForce( FIX::TimeInForce_GOOD_TILL_CROSSING );
    default: throw std::exception();
    }
}

FIX::SecurityType Application::querySecurityType()
{
    char value;
    std::cout << std::endl
              << "1) Common Stock" << std::endl
              << "2) Future" << std::endl
              << "3) Option" << std::endl
                 ;
    std::cin >> value;
    //    value = '1';
    switch ( value )
    {
    case '1': return FIX::SecurityType( FIX::SecurityType("CS") );
    case '2': return FIX::SecurityType( FIX::SecurityType("FUT") );
    case '3': return FIX::SecurityType( FIX::SecurityType("OPT") );
    default: throw std::exception();
    }

}

FIX::SecurityExchange Application::querySecurityExchange()
{
    char value;
    std::cout << std::endl
              << "1) ESMNSE" << std::endl
              << "2) BSE" << std::endl
              << "3) NSE" << std::endl
                 ;
    std::cin >> value;
    //    value = '1';
    switch ( value )
    {
    case '1': return FIX::SecurityExchange( "ESMNSE" );
    case '2': return FIX::SecurityExchange( "BSE" );
    case '3': return FIX::SecurityExchange( "NSE" );
    default: throw std::exception();
    }
}

FIX::MaturityMonthYear Application::queryMaturityMonthYear()
{
    std::string value;
    std::cout << std::endl
              << "Enter maturity Date in " << std::endl
              << " format mmyyyy";

    value = "052014";
    //    std::cin >> value;
    return FIX::MaturityMonthYear(FIX::MONTHYEAR(value));
}

FIX::ExpireDate Application::queryMaturityDate()
{
    std::string value;
    std::cout << std::endl
              << "Enter maturity Date " << std::endl;

    value = "29";
    //    std::cin >> value;
    return FIX::ExpireDate(FIX::LOCALMKTDATE(value));
}

FIX::MaturityDay Application::queryMaturityDay()
{
    std::string value;
    std::cout << std::endl
              << "Enter maturity Date " << std::endl;

    value = "29";
    //    std::cin >> value;
    return FIX::MaturityDay(FIX::DAYOFMONTH(value));
}


FIX::PutOrCall Application::queryOptionMode()
{
    char value;
    std::cout << std::endl
              << "1) Put" << std::endl
              << "2) Call" << std::endl
                 ;
    //    value = '2';
    std::cin >> value;
    switch ( value )
    {
    case '1': return FIX::PutOrCall( 0 );
    case '2': return FIX::PutOrCall( 1 );
    default: throw std::exception();
    }
}

FIX::StrikePrice Application::queryStrikePrice()
{
    long value;
    std::cout << std::endl
              << "Enter strike price " << std::endl;

    //    value = "350000";
    std::cin >> value;
    return FIX::StrikePrice(value);

}

