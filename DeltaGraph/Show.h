
// Show.h : Ŭ������ �������̽�
//

#pragma once

#include "../ShuDeltaGraph/PDW/DataFile.h"


class CShow
{
private:

public:
	CShow();
	virtual ~CShow();

	virtual void ShowGraph( ENUM_SUB_GRAPH enSubGraph=enSubMenu_1, int iFileIndex=0 )=0;
	
};

