// MyEchoSocket.cpp : implementation file
//

#include "stdafx.h"
#include "../ShuDeltaGraph.h"
#include "MyEchoSocket.h"
#include "../DlgColList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MyEchoSocket

MyEchoSocket::MyEchoSocket( bool bBigEndian )
{
	m_bConnected = false;
	m_bBigEndian = bBigEndian;

	m_pData = ( char * ) malloc( sizeof(char) * MAX_LAN_BUFFER );
	m_prxBuffer = ( char * ) malloc( sizeof(char) * 100000 );

	//int rcvbuf=128*1024;
	//SetSockOpt( SO_RCVBUF, (char*) & rcvbuf,(int)sizeof(rcvbuf) );

	InitVar();

}

MyEchoSocket::~MyEchoSocket()
{
	free( m_prxBuffer );
	free( m_pData );

	while( ! m_qMsg.empty() ) {
		m_qMsg.pop();
	}

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(MyEchoSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(MyEchoSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// MyEchoSocket member functions

void MyEchoSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode==0)
	{
		((CDlgColList*)m_pDlg)->OnAccept();
		InitVar();

		m_bConnected = true;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}

void MyEchoSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode==0 || nErrorCode == 10053 )
	{
		InitVar();

		((CDlgColList*)m_pDlg)->OnClose();
	}
	CAsyncSocket::OnClose(nErrorCode);
}

void MyEchoSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	((CDlgColList*)m_pDlg)->OnConnect(nErrorCode );
	m_uiErrorCode = CAsyncSocket::GetLastError();
	m_bHeader = false;
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void MyEchoSocket::OnOutOfBandData(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void MyEchoSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode==0)
	{
		char *pRxBuffer;
		STR_MESSAGE *pstRxMessage;
		STR_DATA_CONTENTS *pstRxData;

		UINT uiReceivedData=0, uiDataLength = 0, uiReceivedMessage;

		pRxBuffer = ( char * ) & m_stQueueMsg;

		DWORD dwSize;
		IOCtl( FIONREAD, & dwSize );

		pstRxMessage = ( STR_MESSAGE * ) & m_stQueueMsg.stMsg;
		pstRxData = ( STR_DATA_CONTENTS * ) & m_stQueueMsg.stData;

		TRACE( "\n IOCTL dwSize[%d]" , dwSize );

		if( m_bHeader == false ) {
			uiReceivedMessage = Receive((char *) pstRxMessage, sizeof(STR_MESSAGE) );

			m_bHeader = true;
			m_uiDataLength = pstRxMessage->uiDataLength;
			m_uiReceivedData = 0;

			TRACE( "\n 헤더[%d]" , uiReceivedMessage );
		}
		else {
			m_uiReceivedData = Receive((char *) pstRxData, m_uiDataLength );
			TRACE( "\n 데이터[%d]" , m_uiReceivedData );
		}
		
		if( m_uiDataLength == 0 || m_uiReceivedData > 0 ) {
			m_bHeader = false;
			uiDataLength = 0;

			m_qMsg.push( m_stQueueMsg );
			BOOL bRet = SetEvent( ((CDlgColList*)m_pDlg)->m_hReceveLAN );

			m_uiReceivedData = 0;
		}

	}

	CAsyncSocket::OnReceive(nErrorCode);
}

void MyEchoSocket::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnSend(nErrorCode);
}

void MyEchoSocket::SetParentDlg(CDialog *pDlg)
{
	m_pDlg = pDlg;
}

void MyEchoSocket::InitVar()
{
	m_bConnected = false;
	m_bHeader = false;
	m_uiErrorCode = CAsyncSocket::GetLastError();

	m_uiReceivedData = 0;

	while( ! m_qMsg.empty() ) {
		m_qMsg.pop();
	}
}


/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2019/11/09 20:53:31
 * @warning   
 */
bool MyEchoSocket::Send( void *pData, int iDataLength )
{
	int iSend;
	bool bRet=true;

	if( m_bBigEndian == false ) {
		int iIndex=0, iLength;

		do {
			if( iDataLength-iIndex < MAX_LAN_BUFFER ) {
				iLength = iDataLength - iIndex;
			}
			else {
				iLength = MAX_LAN_BUFFER;
			}

			memcpy( m_pData, (char *) ( pData ) + iIndex, iLength );
			AllSwapData32( m_pData, iLength );
			iSend = CAsyncSocket::Send( m_pData, iLength );

			int nErrorCode = CAsyncSocket::GetLastError();
			if( nErrorCode != 0 ) {
				TRACE( "\n Send[%d] 에러코드:%d" , iLength, nErrorCode );
			}

			if( iSend != iDataLength || iSend == SOCKET_ERROR ) {
				m_uiErrorCode = CAsyncSocket::GetLastError();

				bRet = false;
				break;
			}

			iIndex += iLength;
		} while( iIndex < iDataLength );

	}
	else {
		iSend = CAsyncSocket::Send( pData, iDataLength );

		if( iSend != iDataLength || iSend == SOCKET_ERROR ) {
			m_uiErrorCode = CAsyncSocket::GetLastError();

			bRet = false;
			//Log( enError, "송신 Send() 에러[%d] 입니다." , m_uiErrorCode );
		}

	}

	return bRet;

}

int MyEchoSocket::GetLastError()
{
	int nCode;

	nCode = CAsyncSocket::GetLastError();
	return nCode;
}

/**
 * @brief     
 * @return    int
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2019/11/09 20:54:15
 * @warning   
 */
int MyEchoSocket::Receive( void *pData, int iDataLength, bool bBigEndian )
{
	int nRecvByte;

	nRecvByte = CAsyncSocket::Receive( pData, iDataLength );

	if( m_bBigEndian == false && bBigEndian == true ) {
		if( iDataLength % sizeof(int) != 0 ) {
			//TRACE( "엔디안 데이터가 4바이트 배수가 아닙니다." );
		}
		AllSwapData32( pData, iDataLength );
	}

	return nRecvByte;
}

void swapByteOrder(unsigned int& ui);
void swapByteOrder(unsigned short& us);
void swapByteOrder(unsigned long long& ull);

void swapByteOrder(unsigned short& us)
{
	us = (us >> 8) |
		(us << 8);
}

void swapByteOrder(unsigned int& ui)
{
	ui = (ui >> 24) |
		((ui<<8) & 0x00FF0000) |
		((ui>>8) & 0x0000FF00) |
		(ui << 24);
}

void swapByteOrder(unsigned long long& ull)
{
	ull = (ull >> 56) |
		((ull<<40) & 0x00FF000000000000) |
		((ull<<24) & 0x0000FF0000000000) |
		((ull<<8) & 0x000000FF00000000) |
		((ull>>8) & 0x00000000FF000000) |
		((ull>>24) & 0x0000000000FF0000) |
		((ull>>40) & 0x000000000000FF00) |
		(ull << 56);
}

/**
 * @brief     
 * @return    void
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2019/11/09 20:54:49
 * @warning   
 */
void MyEchoSocket::AllSwapData32( void *pData, int iLength )
{
	int i;
	UINT *pWord;

	pWord = (UINT *) pData;
	for( i=0 ; i < iLength ; i+=sizeof(int) ) {
		swapByteOrder( *pWord );
		++ pWord;
	}

}

queue <STR_QUEUE_MSG> *MyEchoSocket::GetQueueMessage()
{

	return & m_qMsg;
}
// STR_DATA_CONTENTS *MyEchoSocket::GetRxData()
// {
// 	STR_DATA_CONTENTS *pRxData;
// 
// 	pRxData = ;
// 
// 	return pRxData;
// }
// 
// 
// STR_MESSAGE *MyEchoSocket::GetRxMessage()
// {
// 	STR_MESSAGE *pRxMessage;
// 
// 	pRxMessage = ( STR_MESSAGE * ) ( ( char *) m_prxData );
// 
// 	return pRxMessage;
// }