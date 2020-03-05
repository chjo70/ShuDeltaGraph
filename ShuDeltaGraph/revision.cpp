
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
	static TCHAR st_szRev[100] = { _T("437247b08d2571eb6dc6e2cc3283feaf22e1fd86") };
	return st_szRev;
}

const TCHAR *GetBuildDate()
{
	static TCHAR st_szDate[100] = { _T("2020/03/05 23:48:44") };
	return st_szDate;
}