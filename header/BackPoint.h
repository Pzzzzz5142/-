#pragma once
#include"LL1a.h"
class BackPoint
{
	struct ValNode
	{
		string val;
		int len;
	};
	ValNode VAL[200];          //活动记录表

	void add(int loc, string message)
	{
		if (loc < proc.size())
			proc[loc].push_back(message);
		else
		{
			cerr << loc << endl;
			exit(1);
		}

	}

	int top, F_loc, bottom, nowType;
	void init_fun(int func_num)
	{
		VAL[top] = { to_string(bottom),1 };
		bottom = top;
		top++;

		SymbolTableNode tmp = FUNSYMBEL[func_num];

		int len = 0;

		for (auto& x : FUNSYMBEL[func_num].SYNBL)
		{
			len += x.len;
			if (x.cat != FUN)
			{
				x.addr = abs(x.addr) + bottom;
			}
		}

		top += len;

		RUNSYMBEL.push_back(tmp);
	}

	void func_return(int loc, string addr)
	{
		add(loc, string("MOV DX, [") + addr + "]");
	}

	void oper2(int loc, QtNode & a)
	{

	}

	string get_add(SemNode a)
	{
		string name;
		if (a.y[0] == '#')
		{
			name = "#t";
			name += to_string(a.x);
		}
		else
			name = a.y;


		for (int i = RUNSYMBEL.size() - 1; i > -1; i--)
		{
			for (auto x : RUNSYMBEL[i].SYNBL)
			{
				if (name == x.name)
				{
					return to_string(abs(x.addr));
				}
			}
			if (RUNSYMBEL[i].link)
				continue;
		}
		for (auto x : SYNBL)
		{
			if (name == x.name)
				return to_string(abs(x.addr));
		}
		return "-1";
	}

	void run_fun(int bg, int ed)
	{
		for (int loc = bg; loc < ed; loc++)
		{
			auto x = QT[loc];
			if (x.a.y == "+")
			{
				add(loc, "ADD");
			}
			else if (x.a.y == "=")
			{
				add(loc, "MOV AX, [" + get_add(x.d) + "]");
				add(loc, "MOV [" + get_add(x.b) + ", AX");
			}
		}
	}

	int get_type(string name)
	{
		if (F_loc == -1)
		{
			for (int i = 0; i < SYNBL.size(); i++)
				if (name == SYNBL[i].name)
					return SYNBL[i].cat;
		}
		else
		{
			for (int i = RUNSYMBEL.size() - 1; i > -1; i--)
			{
				for (auto x : RUNSYMBEL[i].SYNBL)
				{
					if (name == x.name)
					{
						return x.cat;
					}
				}
				if (RUNSYMBEL[i].link)
					continue;
			}
		}
	}

public:
	vector<SymbolTableNode> FUNSYMBEL;
	vector<SymbolTableNode> RUNSYMBEL;
	vector<QtNode> QT;
	vector<vector<string>> proc;
	map<string, pair<int, int>> fun_ind;
	vector<SynblNode> SYNBL;     //符号总表
	vector<TapelNode> TAPEL;     //类型表
	vector<AinflNode> AINFL;     //数组表
	vector<PfinflNode> PFINFL;   //函数表
	vector<SynblNode> PARAMETER; //参数表
	int MAINLen;
	stack<int> Floc;
	vector<string> res;
	vector<int> LENL;            //长度表
	BackPoint(LL1 & a)
	{
		res.push_back("data segment\n	dw 5 dup(0)\n	dasc dw 80 dup(? )\n data ends\n stk segment stack\n		dw 50 dup(0)\n stk ends\n code segment\n	assume cs : code\n");
		top = 0;
		F_loc = -1;
		bottom = 0;
		proc.resize(a.QT.size() + 1);
		QT = a.QT;
		SYNBL = a.SYNBL;     //符号总表
		TAPEL = a.TAPEL;     //类型表
		AINFL = a.AINFL;     //数组表
		PFINFL = a.PFINFL;   //函数表
		PARAMETER = a.PARAMETER; //参数表
		FUNSYMBEL = a.FUNSYMBEL;
		LENL = a.LENL;            //长度表
		int bg = 0, ed, qtbg;
		for (int i = 0; i < QT.size(); i++)
		{
			if (QT[i].a.y == "main")
			{
				qtbg = i;
				break;
			}
		}
		MAINLen = 0;
		int ll = 0;
		for (auto x : SYNBL)
		{
			if (x.cat == FUN)
			{
				fun_ind[x.name] = { PFINFL[x.addr].entry,PFINFL[x.addr].ed };
			}
			MAINLen += x.len;
		}
		for (int i = 0; i < QT.size(); i++)
		{
			if (QT[i].a.y == "wh")
			{

			}
			else if (QT[i].a.y == "if")
			{
				add(QT[i].d.x, "IMF" + to_string(QT[i].c.x) + ":");
				add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
				add(i, "CMP AX, 0");
				add(i, "JZ IMF" + to_string(QT[i].c.x));
				RUNSYMBEL.push_back(FUNSYMBEL[QT[i].c.x]);
				Floc.push(F_loc);
				F_loc = QT[i].c.x;
			}
			else if (QT[i].a.y == "ef")
			{
				if (QT[i].d.x != i + 1)
				{
					add(QT[i].d.x, "EL" + to_string(QT[i].d.x) + ":");
					add(i, "JMP EL" + to_string(QT[i].c.x));
				}
				RUNSYMBEL.pop_back();
				F_loc = Floc.top();
				Floc.pop();
			}
			else if (QT[i].a.y == "+")
			{
				add(i, "MOV AX, [" + get_add(QT[i].b) + "+SI]");
				add(i, "ADD AX, [" + get_add(QT[i].c) + "+SI]");
				add(i, "MOV [" + get_add(QT[i].d) + "+SI], AX");
			}
			else if (QT[i].a.y == "-")
			{
				add(i, "MOV AX, [" + get_add(QT[i].b) + "+SI]");
				add(i, "SUB AX, [" + get_add(QT[i].c) + "+SI]");
				add(i, "MOV [" + get_add(QT[i].d) + "+SI], AX");
			}
			else if (QT[i].a.y == "*")
			{
				add(i, "MOV AX, [" + get_add(QT[i].b) + "+SI]");
				add(i, "MUL AX, [" + get_add(QT[i].c) + "+SI]");
				add(i, "MOV [" + get_add(QT[i].d) + "+SI], AX");
			}
			else if (QT[i].a.y == "/")
			{
				add(i, "MOV AX, [" + get_add(QT[i].b) + "+SI]");
				add(i, "DIV AX, [" + get_add(QT[i].c) + "+SI]");
				add(i, "MOV [" + get_add(QT[i].d) + "+SI], AX");
			}
			else if (QT[i].a.y == "=")
			{
				add(i, "MOV AX, [" + get_add(QT[i].d) + "+SI]");
				add(i, "MOV [" + get_add(QT[i].b) + "+SI], AX");
			}
			else if (QT[i].a.y == "==")
			{
				add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
				add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
				add(i, "CMP AX, BX");
				add(i, "MOV AX,1");
				add(i, "JZ O" + to_string(ll));
				add(i, "MOV AX,0");
				add(i, "O" + to_string(ll) + ":");
				ll++;
				add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
			}
			else if (QT[i].a.y == "!=")
			{
				add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
				add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
				add(i, "CMP AX, BX");
				add(i, "MOV AX,1");
				add(i, "JNZ O" + to_string(ll));
				add(i, "MOV AX,0");
				add(i, "O" + to_string(ll) + ":");
				ll++;
				add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
			}
			else if (QT[i].a.y == "<")
			{
				add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
				add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
				add(i, "CMP AX, BX");
				add(i, "MOV AX,1");
				add(i, "JB O" + to_string(ll));
				add(i, "MOV AX,0");
				add(i, "O" + to_string(ll) + ":");
				ll++;
				add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
			}
			else if (QT[i].a.y == ">")
			{
				add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
				add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
				add(i, "CMP AX, BX");
				add(i, "MOV AX,1");
				add(i, "JA O" + to_string(ll));
				add(i, "MOV AX,0");
				add(i, "O" + to_string(ll) + ":");
				ll++;
				add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
			}
			else if (QT[i].a.y == ">=")
			{
				add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
				add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
				add(i, "CMP AX, BX");
				add(i, "MOV AX,1");
				add(i, "JAE O" + to_string(ll));
				add(i, "MOV AX,0");
				add(i, "O" + to_string(ll) + ":");
				ll++;
				add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
			}
			else if (QT[i].a.y == "<=")
			{
				add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
				add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
				add(i, "CMP AX, BX");
				add(i, "MOV AX,1");
				add(i, "JBE O" + to_string(ll));
				add(i, "MOV AX,0");
				add(i, "O" + to_string(ll) + ":");
				ll++;
				add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
			}
			else if (QT[i].a.y == "edfun")
			{
				RUNSYMBEL.pop_back();
				F_loc = Floc.top();
				Floc.pop();
			}
			else
			{
				if (get_type(QT[i].a.y) == FUN)
				{
					if (QT[i].a.y == "main")
					{
						add(i, "START:");
						add(i, "MOV AX, DATA");
						add(i, "MOV DS, AX");
						add(i, "MOV SI, 0");
					}
					else
						add(i, "P" + to_string(QT[i].c.x) + ":");
					RUNSYMBEL.push_back(FUNSYMBEL[QT[i].c.x]);
					Floc.push(F_loc);
					F_loc = QT[i].c.x;
				}
			}
		}
		for (int i = 0; i < QT.size(); i++)
		{
			for (auto x : proc[i])
			{
				if (x[x.size() - 1] != ':')
					res.push_back("\t" + x);
				else
					res.push_back(x);
			}
		}
		res.push_back("MOV AH, 4CH");
		res.push_back("INT 21H");
		res.push_back("CODE ENDS");
		res.push_back("END START");
	}
	void show()
	{
		for (auto x : res)
			cout << x << endl;
	}
	~BackPoint();
};

