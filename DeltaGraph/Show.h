
// Show.h : 클래스의 인터페이스
//

#pragma once

typedef enum {
	enUnselectedSubGraph = -1,

	enSubMenu_1 = 1,
	enSubMenu_2,
	enSubMenu_3,
	enSubMenu_4,
	enSubMenu_5,
	enSubMenu_6,


} ENUM_SUB_GRAPH ;


class CShow
{
private:

public:
	CShow();
	virtual ~CShow();

	virtual void ShowGraph( ENUM_SUB_GRAPH enSubGraph )=0;
	
};

