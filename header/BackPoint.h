#pragma once
#include"LL1a.h"
class BackPoint
{
	stack<string> VALL;          //���¼��
public:
	vector<SymbolTableNode> FUNSYMBEL;
	vector<QtNode> QT;
	BackPoint(LL1 &a)
	{

	}
	~BackPoint();
};

