// MyEchoSocket.cpp : implementation file
//

#include "stdafx.h"
#include "RWRJAM-SIM.h"
#include "MyEchoSocket.h"
#include "RWRJAM-SIMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MyEchoSocket

MyEchoSocket::MyEchoSocket( enUnitID id, bool bBigEndian )
{
	m_id = id;
	m_bConnected = false;
	m_bBigEndian = bBigEndian;

	m_pData = ( char * ) malloc( sizeof(char) * MAX_LAN_BUFFER );

	m_uiErrorCode = 0;

}

MyEchoSocket::~MyEchoSocket()
{
	m_id = enUnknown;

	free( m_pData );
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
		((CRWRJAMSIMDlg*)m_pDlg)->OnAccept( m_id );
		//m_bConnected = true;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}

void MyEchoSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode==0 || nErrorCode == 10053 )
	{
		m_uiErrorCode = CAsyncSocket::GetLastError();

		((CRWRJAMSIMDlg*)m_pDlg)->OnClose(m_id );
		TRACE( "·£ÀÌ ²÷°å½À´Ï´Ù !!!" );
	}
	CAsyncSocket::OnClose(nErrorCode);
}

void MyEchoSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	((CRWRJAMSIMDlg*)m_pDlg)->OnConnect(nErrorCode, m_id );
	m_uiErrorCode = CAsyncSocket::GetLastError();
	
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
		((CRWRJAMSIMDlg*)m_pDlg)->OnReceive(m_id );
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

/**
 * @brief     
 * @return    void
 * @author    Á¶Ã¶Èñ (churlhee.jo@lignex1.com)
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
			Log( enError, "¼Û½Å Send() ¿¡·¯[%d] ÀÔ´Ï´Ù." , m_uiErrorCode );
		}

	}

	return bRet;

}

int MyEchoSocket::GetLastError()
{
	int nCode;

	nCode = CAsyncSocket::GetLastError();
	return m_uiErrorCode;
}

/**
 * @brief     
 * @return    int
 * @author    Á¶Ã¶Èñ (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2019/11/09 20:54:15
 * @warning   
 */
int MyEchoSocket::Receive( void *pData, int iDataLength )
{
	int nRecvByte;

	nRecvByte = CAsyncSocket::Receive( pData, iDataLength );

	if( m_bBigEndian == false ) {
		if( iDataLength % sizeof(int) != 0 ) {
			AfxMessageBox( "¿£µð¾È µ¥ÀÌÅÍ°¡ 4¹ÙÀÌÆ® ¹è¼ö°¡ ¾Æ´Õ´Ï´Ù." );
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
 * @author    Á¶Ã¶Èñ (churlhee.jo@lignex1.com)
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

