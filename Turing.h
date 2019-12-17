#pragma once
#include <bits/stdc++.h>
using namespace std;
#define F first
#define S second
#define ll long long
#define mp make_pair
typedef std::pair<int, char> P;
struct node
{
	int to;
	char c, chg;
	node* nx;
	node* ed;
	node(int to_ = -1, char c_ = '\0', char chg_ = '\0', node* nx_ = nullptr, node* ed_ = nullptr) :to(to_), c(c_), chg(chg_), nx(nx_), ed(ed_)
	{
	}
};
struct ID
{
	int loc,cd;
	string tp;
	ID(int loc_ = 0, int cd_ = 0, string tp_ = string()) :loc(loc_), tp(tp_), cd(cd_)
	{
	}

};

ostream& operator<<(ostream& out, ID& x);

class Turing
{
private:
	std::string tp;
	int loc;
	int num;
	std::map<std::pair<int, int>, int> ind;
	std::vector<node> sig;
	int bg;
	set<int> ed;
public:
	Turing();
	void Run();
	void Read();
	void Read_fl(const string F);
	void insert_sig(P fr, node tt);
	void input();
	void del();
	void del_cell(node*x);
	~Turing();
};