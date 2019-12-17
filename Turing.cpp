#include "Turing.h"
#include <cstdio>
#include <conio.h>
using namespace std;
ostream& operator<<(ostream& out, ID& x)
{
	for (int i = 0; i < x.tp.size(); i++)
	{
		if (i == x.loc)
		{
			cout << '|' << x.tp[x.loc];
		}
		else if (x.tp[i] != 'B')
			out << x.tp[i];
	}
	return out;
}

Turing::Turing()
{
	num = 0;
	bg = 0;
	loc = 9;
}

void Turing::Run()
{
	queue<pair<ID,int>> que;	
	que.push(mp(ID(loc, bg, tp), 0));
	int now = 0, cnt = 1;
	vector<ID> ans;
	while (!que.empty())
	{
		auto pp = que.front();
		que.pop();
		if (now != pp.second)
		{
			cout << "�� "<<now <<" �ε����ѽ������� e �������������������\n";
			char aaa;
			now = pp.second;
			aaa=_getch();
			if (aaa == 'e' || aaa == 'E')
				break;
			system("cls");
			cout << "��ǰִ�е� " << now << " �ε���" << endl;
			cnt = 1;
		}
		cout << "Case " << cnt++ << ": " << endl;
		cout << "��ǰֽ��״̬��" << endl;
		cout << "\t" << pp.first << endl;
		auto it = ind.find(mp(pp.first.cd, pp.first.tp[pp.first.loc]));
		if (it == ind.end())
		{
			if (ed.find(pp.first.cd) != ed.end())
			{
				cout << "------------------" << endl;
				cout << "Language Accepted!" << endl;
				cout << "------------------" << endl;
				ans.push_back(pp.first);
				cout << "��ͣ�����Ƿ������[Y\\N]";
				char aaa;
				cin >> aaa;
				if (aaa == 'N')
					break;
			}
			else
			{
				cout << "------------------" << endl;
				cout << "No such function!" << endl;
				cout << "------------------" << endl;
			}
			continue;
		}
		cout << "λ�ڵ� " << pp.first.cd << " ״̬" << endl;
		cout << endl;

		node tloc;
		tloc = sig[it->second];
		while(1)
		{
			auto tmp = pp;
			tmp.first.tp[pp.first.loc] = tloc.chg;
			tmp.first.cd = tloc.to;
			tmp.second = pp.second + 1;
			switch (tloc.c)
			{
			case'L':
				--tmp.first.loc;
				if (tmp.first.loc == -1)
				{
					tmp.first.loc = 0;
					tmp.first.tp.insert(0, "B");
				}
				break;
			case'R':
				++tmp.first.loc;
				break;
			default:
				break;
			}
			que.push(tmp);
			if (tloc.nx)
				tloc = *tloc.nx;
			else
				break;
		}
	}
	cout << "���ս����" << endl;
	for (auto i : ans)
		cout << '\t' << i << endl;
}

void Turing::Read()
{
	int n,cd1,cd2;
	char c1, c2, cd;
	cout << "������ת�ƺ���������";
	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		cout << "��ǰ����� " << i << " ��ת�ƺ�����";
		cin >> cd1 >> c1 >> cd2 >> c2 >> cd;
		insert_sig(mp(cd1, c1), node(cd2, cd, c2));
	}
	int tmp;
	cout << "��������ʼ״̬��";
	cin >> bg;
	cout << "��������ֹ״̬������";
	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		cout << "��ǰ����� " << i << " ����ֹ״̬��";
		cin >> tmp;
		ed.insert(tmp);
	}
}

void Turing::Read_fl(const string F)
{
	ifstream fl(F);
	int n,cd1,cd2;
	char c1, c2, cd;
	fl >> n;
	for (int i = 0; i < n; i++)
	{
		fl >> cd1 >> c1 >> cd2 >> c2 >> cd;
		insert_sig(mp(cd1, c1), node(cd2, cd, c2));
	}
	int tmp;
	fl >> bg;
	fl >> n;
	for (int i = 0; i < n; i++)
	{
		fl >> tmp;
		ed.insert(tmp);
	}
}

void Turing::insert_sig(P fr, node tt)
{
	auto it = ind.find(fr);
	if (it == ind.end())
	{
		ind.insert(mp(fr, num++));
		sig.push_back(tt);
	}
	else
	{
		int x = it->second;
		if (sig[x].nx)
		{
			sig[x].ed->nx = new node;
			*(sig[x].ed->nx) = tt;
			sig[x].ed = sig[x].ed->nx;
		}
		else
		{
			sig[x].nx = new node;
			*(sig[x].nx) = tt;
			sig[x].ed = sig[x].nx;
		}
	}
}

void Turing::input()
{
	cout << "������һ�������Կ�ʼ�жϣ�";
	cin >> tp;
	tp.insert(0, "BBBBBBBBB");
	tp.append(10,'B');
}

void Turing::del()
{
	for (int i = 0; i < num; i++)
	{
		del_cell(sig[i].nx);
	}
	num = 0;
	loc = 0;
	ind.clear();
	sig.clear();
	ed.clear();
}

void Turing::del_cell(node*x)
{
	if (!x)
		return;
	del_cell(x->nx);
	delete x;
}

Turing::~Turing()
{
	del();
}
