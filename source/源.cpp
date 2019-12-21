#include <bits/stdc++.h>
#include "BackPoint.h"
#include "Optimize.h"
using namespace std;

int main()
{
	LL1 a;
	a.solve("./res/demo.txt");
	a.show();
	Optimizer c(a);
	c.solve();
	c.show();
	BackPoint b(a);
	b.show();
	return 0;
}