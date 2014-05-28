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

/**
 * @brief The Application class
 */
class Application :
        public FIX::Application,
        public FIX::MessageCracker
{
public:

    /**
     * @brief run
     */
    void run();

private:

    /**
     * @brief onCreate
     */
    void onCreate( const FIX::SessionID& )
    {
        std::cout << std::endl << __LINE__ << " *****************" << std::endl;
    }

    /**
     * @brief onLogon
     * @param sessionID
     */
    void onLogon( const FIX::SessionID& sessionID );

    /**
     * @brief onLogout
     * @param sessionID
     */
    void onLogout( const FIX::SessionID& sessionID );

    /**
     * @brief toAdmin
     * @param message
     */
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

    /**
     * @brief toApp
     */
    void toApp( FIX::Message&, const FIX::SessionID& )
    throw( FIX::DoNotSend );

    /**
     * @brief fromAdmin
     * @param message
     */
    void fromAdmin( const FIX::Message& message, const FIX::SessionID& )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon )
    {
        std::cout << std::endl << __LINE__ << " *****************" << std::endl;
        dismantleFIX("From FIX-Server",message.toString());
    }

    /**
     * @brief fromApp
     * @param message
     * @param sessionID
     */
    void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );


    /**
     * @brief onMessage
     */
    void onMessage( const FIX42::ExecutionReport&, const FIX::SessionID& );

    /**
     * @brief onMessage
     */
    void onMessage( const FIX42::OrderCancelReject&, const FIX::SessionID& );

    /**
     * @brief onMessage
     */
    void onMessage( const FIX42::MarketDataSnapshotFullRefresh&, const FIX::SessionID& );

    /**
     * @brief queryEnterOrder
     */
    void queryEnterOrder();

    /**
     * @brief queryCancelOrder
     */
    void queryCancelOrder();

    /**
     * @brief queryReplaceOrder
     */
    void queryReplaceOrder();

    /**
     * @brief queryMarketDataRequest
     */
    void queryMarketDataRequest();

    /**
     * @brief queryNewOrderSingle42
     * @return
     */
    static FIX42::NewOrderSingle queryNewOrderSingle42();

    /**
     * @brief queryOrderCancelRequest42
     * @return
     */
    FIX42::OrderCancelRequest queryOrderCancelRequest42();

    /**
     * @brief queryCancelReplaceRequest42
     * @return
     */
    FIX42::OrderCancelReplaceRequest queryCancelReplaceRequest42();

    /**
     * @brief queryMarketDataRequest42
     * @return
     */
    static FIX42::MarketDataRequest queryMarketDataRequest42();

    /**
     * @brief queryHeader
     * @param header
     */
    static void queryHeader( FIX::Header& header );

    /**
     * @brief queryAction
     * @return
     */
    static char queryAction();

    /**
     * @brief queryConfirm
     * @param query
     * @return
     */
    bool queryConfirm( const std::string& query );

    /**
     * @brief querySenderCompID
     * @return
     */
    static FIX::SenderCompID querySenderCompID();

    /**
     * @brief queryTargetCompID
     * @return
     */
    static FIX::TargetCompID queryTargetCompID();

    /**
     * @brief queryTargetSubID
     * @return
     */
    static FIX::TargetSubID queryTargetSubID();

    /**
     * @brief queryClOrdID
     * @return
     */
    static FIX::ClOrdID queryClOrdID();

    /**
     * @brief queryOrigClOrdID
     * @return
     */
    static FIX::OrigClOrdID queryOrigClOrdID();

    /**
     * @brief querySymbol
     * @return
     */
    static FIX::Symbol querySymbol();

    /**
     * @brief querySide
     * @return
     */
    static FIX::Side querySide();

    /**
     * @brief queryOrderQty
     * @return
     */
    static FIX::OrderQty queryOrderQty();

    /**
     * @brief queryOrdType
     * @return
     */
    static FIX::OrdType queryOrdType();

    /**
     * @brief queryPrice
     * @return
     */
    static FIX::Price queryPrice();

    /**
     * @brief queryStopPx
     * @return
     */
    static FIX::StopPx queryStopPx();

    /**
     * @brief queryTimeInForce
     * @return
     */
    static FIX::TimeInForce queryTimeInForce();

    /**
     * @brief querySecurityType
     * @return
     */
    static FIX::SecurityType querySecurityType();

    /**
     * @brief querySecurityExchange
     * @return
     */
    static FIX::SecurityExchange querySecurityExchange();

    /**
     * @brief queryMaturityMonthYear
     * @return
     */
    static FIX::MaturityMonthYear queryMaturityMonthYear();

    /**
     * @brief queryMaturityDateh
     * @return
     */
    static FIX::ExpireDate queryMaturityDate();

    /**
     * @brief queryMaturityDay
     * @return
     */
    static FIX::MaturityDay queryMaturityDay();

    /**
     * @brief queryOptionMode
     * @return
     */
    static FIX::PutOrCall queryOptionMode();

    /**
     * @brief queryStrikePrice
     * @return
     */
    static FIX::StrikePrice queryStrikePrice();

public :

    /**
     * @brief queryMarketDataRequest42
     * @param symbol
     * @param exchange
     * @param securityType
     * @param maturityDay
     * @param maturityMonthYear
     * @param strikePrice
     * @param putCall
     * @param senderCompId
     * @param TargetSubID
     * @return
     */
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


    /**
     * @brief getMarketDataUniqueSignature
     * @param message
     * @return
     */
    static std::string getMarketDataUniqueSignature(const FIX42::Message *message);

    /**
     * @brief getMarketDataUniqueSignature
     * @param message
     * @return
     */
    static std::string getMarketDataUniqueSignature(const FIX42::MarketDataRequest &message) ;


    /**
     * @brief queryNewOrderSingle42
     * @param clordId
     * @param symbol
     * @param side
     * @param orderType
     * @param securityExchange
     * @param orderQty
     * @param price
     * @param stopPrice
     * @param timeInForce
     * @param securityType
     * @param maturityMonthYear
     * @param maturityDay
     * @param strikePrice
     * @param putOrCall
     * @param senderCompId
     * @param targetCompId
     * @return
     */
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

    /**
     * @brief queryCancelReplaceRequest42
     * @param orderType
     * @param origClOrdId
     * @param clOrdId
     * @param symbol
     * @param side
     * @param price
     * @param orderQty
     * @param senderCompID
     * @param targetCompID
     * @return
     */
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

    /**
     * @brief queryOrderCancelRequest42
     * @param origClOrdID
     * @param clOrdID
     * @param symbol
     * @param side
     * @param senderCompID
     * @param targetCompID
     * @return
     */
    static FIX42::OrderCancelRequest queryOrderCancelRequest42(
            const FIX::OrigClOrdID &origClOrdID,
            const FIX::ClOrdID &clOrdID,
            const FIX::Symbol &symbol,
            const FIX::Side &side,
            const FIX::SenderCompID &senderCompID,
            const FIX::TargetCompID &targetCompID);
};

#endif
