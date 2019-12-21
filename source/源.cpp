#include <bits/stdc++.h>
#include "BackPoint.h"
#include "Optimize.h"
#include"Tokenlize.h"
using namespace std;

int main()
{
	LL1 a;
	Tokenlize::solve();
	a.solve("./res/demo.txt");
	Optimizer c(a);
	c.solve();
	c.show();
	BackPoint b(a);
	//a.printingLL1();
	a.printingtable1();
	a.printingtable2();
	b.output("C:\\Users\\Pzzzzz\\OneDrive\\MASM");
	return 0;
}