#pragma once
#include"LL1a.h"
class BackPoint
{
	stack<string> VALL;          //活动记录表
public:
	vector<SymbolTableNode> FUNSYMBEL;
	vector<QtNode> QT;
	BackPoint(LL1 &a)
	{

	}
	~BackPoint();
};

