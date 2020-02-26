
#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "ShuDeltaGraph.h"


//////////////////////////////////////////////////////////////////////
// »ý¼º/¼Ò¸ê
//////////////////////////////////////////////////////////////////////

const TCHAR *GetRevision()
{
	static TCHAR st_szRev[100] = { _T("c200a6965b5519c112730df72e032917164845c3") };
	return st_szRev;
}

const TCHAR *GetBuildDate()
{
	static TCHAR st_szDate[100] = { _T("2020/02/27 01:09:35") };
	return st_szDate;
}