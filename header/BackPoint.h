#pragma once
#include "LL1a.h"
class BackPoint
{
    struct ValNode
    {
        string val;
        int len;
    };
    ValNode VAL[200]; //活动记录表

    void add(int loc, string message)
    {
        if (loc < proc.size())
        {
            if (message[message.size() - 1] == ':' && message[0] != 'O')
                proc[loc].insert(proc[loc].begin(), message);
            else
                proc[loc].push_back(message);
        }
        else
        {
            cerr << loc << endl;
            exit(1);
        }
    }

    int top, F_loc, bottom, nowType;
    void init_fun(int func_num)
    {
        VAL[top] = { to_string(bottom), 1 };
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

    void oper2(int loc, QtNode& a)
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
                    return "SI+" + to_string(abs(x.addr) * 2);
                }
            }
            if (RUNSYMBEL[i].link)
                continue;
        }
        for (auto x : SYNBL)
        {
            if (name == x.name)
                return to_string(abs(x.addr * 2));
        }
        return "-1";
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
            for (int i = 0; i < SYNBL.size(); i++)
                if (name == SYNBL[i].name)
                    return SYNBL[i].cat;
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

    void movax(int loc, SemNode a, string to = "AX", string op = "MOV")
    {
        if (!a.y.empty() && a.y[0] == '@')
            add(loc, op + " " + to + ", " + to_string(a.x));
        else if (a.y == "&DX")
            add(loc, op + " " + to + ", DX");
        else
            add(loc, op + " " + to + ", [" + get_add(a) + "]");
    }

    void movd(int loc, SemNode a, string to = "AX")
    {
        if (!a.y.empty() && a.y[0] == '@')
            add(loc, "MOV " + to_string(a.x) + ", " + to);
        else if (a.y == "&DX")
            add(loc, "MOV DX, " + to);
        else
            add(loc, "MOV [" + get_add(a) + "], " + to);
    }

    void better()
    {
        if (res.empty())
            return;
        string pre;
        for (vector<string>::iterator i = res.begin() + 1; i != res.end();)
        {
            pre = *(i - 1);
            stringstream ss(pre);
            string t1, t2;
            ss >> t1;
            ss >> t1;
            stringstream tt(*i);
            tt >> t2;
            tt >> t2;
            tt >> t2;
            if (t1[t1.size() - 1] == ',')
                t1.pop_back();
            if (t1 == t2&&t1[0]=='[')
            {
                i=res.erase(i);
            }
            else
                ++i;
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
    int tmp;
    int par_num;
	int fun_tmp;
    int fun_addr;
    stack<int> Floc;
    vector<string> res;
    string pre;
    vector<int> LENL; //长度表
    BackPoint(LL1& a)
    {
        res.push_back("data segment\n\tdw 100 dup(0)\n\tPARA dw 100 dup(? )\ndata ends\nstk segment stack\n\tdw 50 dup(0)\nstk ends\ncode segment\n\tassume cs : code\n");
        top = 0;
        F_loc = -1;
        bottom = 0;
        par_num = 0;
        proc.resize(a.QT.size() + 1);
        QT = a.QT;
        SYNBL = a.SYNBL;         //符号总表
        TAPEL = a.TAPEL;         //类型表
        AINFL = a.AINFL;         //数组表
        PFINFL = a.PFINFL;       //函数表
        PARAMETER = a.PARAMETER; //参数表
        FUNSYMBEL = a.FUNSYMBEL;
        LENL = a.LENL; //长度表
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
                fun_ind[x.name] = { PFINFL[x.addr].entry, PFINFL[x.addr].ed };
            }
            MAINLen += x.len;
        }
        for (int i = 0; i < QT.size(); i++)
        {
            if (QT[i].a.y == "wh")
            {
                tmp = QT[i].c.x;
            }
			else if (QT[i].a.y == "puts")
			{
				movax(i, QT[i].b);
				add(i, "CALL dsp");
			}
            else if (QT[i].a.y == "INIP")
            {
                par_num = 0;
                fun_addr = stoi(get_add(QT[i].c));
				fun_tmp = PFINFL[fun_addr].entry;
                add(i, "LEA DI, PARA");
            }
            else if (QT[i].a.y == "SVP")
            {
				movax(i, QT[i].b);
                add(i, "MOV [DI+" + to_string(par_num * 2) + "],AX");
                par_num++;
            }
			else if (QT[i].a.y == "call")
			{
                add(i, "ADD SI, " + to_string(FUNSYMBEL[F_loc].len));
				for (int i = 0; i < PFINFL[fun_addr].param.size(); i++)
				{
                    add(i, "MOV AX, [DI+" + to_string(i * 2) + "]");
                    add(i, "MOV [SI+" + to_string(PFINFL[fun_addr].param[i].addr * 2) + "], AX");
				}
				add(i, "CALL " + QT[i].b.y);
                add(i, "SUB SI, " + to_string(FUNSYMBEL[F_loc].len));
			}
			else if (QT[i].a.y == "ret")
			{
				add(i, "RET");
			}
            else if (QT[i].a.y == "for")
            {
                tmp = QT[i].c.x;
            }
            else if (QT[i].a.y == "judge")
            {
                add(QT[i].c.x, "FORM" + to_string(QT[i].c.x) + ":");
                add(QT[i].d.x, "FORE" + to_string(QT[i].d.x) + ":");
                movax(i, QT[i].b);
                add(i, "CMP AX, 0");
                add(i, "JNZ FORM" + to_string(QT[i].c.x));
                add(i, "JZ FORE" + to_string(QT[i].d.x));
                RUNSYMBEL.push_back(FUNSYMBEL[tmp]);
                Floc.push(F_loc);
                F_loc = tmp;
            }
            else if (QT[i].a.y == "fordone")
            {
                add(QT[i].d.x, "FORB" + to_string(QT[i].d.x) + ":");
                add(i, "JMP FORB" + to_string(QT[i].d.x));
            }
            else if (QT[i].a.y == "fe")
            {
                add(QT[i].d.x, "FORRT" + to_string(QT[i].d.x) + ":");
                add(i, "JMP FORRT" + to_string(QT[i].d.x));
                RUNSYMBEL.pop_back();
                F_loc = Floc.top();
                Floc.pop();
            }
            else if (QT[i].a.y == "do")
            {
                add(QT[i].d.x, "WHOUT" + to_string(QT[i].d.x) + ":");
                //add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
                movax(i, QT[i].b);
                add(i, "CMP AX, 0");
                add(i, "JZ WHOUT" + to_string(QT[i].d.x));
                RUNSYMBEL.push_back(FUNSYMBEL[tmp]);
                Floc.push(F_loc);
                F_loc = tmp;
            }
            else if (QT[i].a.y == "we")
            {
                add(QT[i].d.x, "WHIN" + to_string(QT[i].d.x) + ":");
                add(i, "JMP WHIN" + to_string(QT[i].d.x));
                RUNSYMBEL.pop_back();
                F_loc = Floc.top();
                Floc.pop();
            }
            else if (QT[i].a.y == "if")
            {
                add(QT[i].d.x, "IMF" + to_string(QT[i].c.x) + ":");
                //add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
                movax(i, QT[i].b);
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
                    add(i, "JMP EL" + to_string(QT[i].d.x));
                }
                RUNSYMBEL.pop_back();
                F_loc = Floc.top();
                Floc.pop();
            }
            else if (QT[i].a.y == "else")
            {
                RUNSYMBEL.push_back(FUNSYMBEL[QT[i].c.x]);
                Floc.push(F_loc);
                F_loc = QT[i].c.x;
            }
            else if (QT[i].a.y == "ee")
            {
                RUNSYMBEL.pop_back();
                F_loc = Floc.top();
                Floc.pop();
            }
            else if (QT[i].a.y == "+")
            {
                //add(i, "MOV AX, [" + get_add(QT[i].b) + "+SI]");
                movax(i, QT[i].b);
                //add(i, "ADD AX, [" + get_add(QT[i].c) + "+SI]");
                movax(i, QT[i].c, "AX", "ADD");
                //add(i, "MOV [" + get_add(QT[i].d) + "+SI], AX");
                movd(i, QT[i].d);
            }
            else if (QT[i].a.y == "-")
            {
                //add(i, "MOV AX, [" + get_add(QT[i].b) + "+SI]");
                movax(i, QT[i].b);
                //add(i, "ADD AX, [" + get_add(QT[i].c) + "+SI]");
                movax(i, QT[i].c, "AX", "SUB");
                //add(i, "MOV [" + get_add(QT[i].d) + "+SI], AX");
                movd(i, QT[i].d);
            }
            else if (QT[i].a.y == "*")
            {
                //add(i, "MOV AX, [" + get_add(QT[i].b) + "+SI]");
                add(i, "XOR DX, DX");
                movax(i, QT[i].b);
                movax(i, QT[i].c, "BX");
                add(i, "MUL BX");
                //add(i, "MOV [" + get_add(QT[i].d) + "+SI], AX");
                movd(i, QT[i].d);
            }
            else if (QT[i].a.y == "/")
            {
                //add(i, "MOV AX, [" + get_add(QT[i].b) + "+SI]");
                add(i, "XOR DX, DX");
                movax(i, QT[i].b);
                movax(i, QT[i].c, "BX");
                add(i, "DIV BX");
                movax(i, QT[i].c, "AX", "DIV");
                //add(i, "MOV [" + get_add(QT[i].d) + "+SI], AX");
                movd(i, QT[i].d);
            }
            else if (QT[i].a.y == "=")
            {
                //add(i, "MOV AX, [" + get_add(QT[i].d) + "+SI]");
                movax(i, QT[i].d);
                //add(i, "MOV [" + get_add(QT[i].b) + "+SI], AX");
                movd(i, QT[i].b);
            }
            else if (QT[i].a.y == "==")
            {
                //add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
                movax(i, QT[i].b);
                //add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
                movax(i, QT[i].c, "BX");
                add(i, "CMP AX, BX");
                add(i, "MOV AX,1");
                add(i, "JZ O" + to_string(ll));
                add(i, "MOV AX,0");
                add(i, "O" + to_string(ll) + ":");
                ll++;
                //add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
                movd(i, QT[i].d);
            }
            else if (QT[i].a.y == "!=")
            {
                //add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
                movax(i, QT[i].b);
                //add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
                movax(i, QT[i].c, "BX");
                add(i, "CMP AX, BX");
                add(i, "MOV AX,1");
                add(i, "JNZ O" + to_string(ll));
                add(i, "MOV AX,0");
                add(i, "O" + to_string(ll) + ":");
                ll++;
                //add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
                movd(i, QT[i].d);
            }
            else if (QT[i].a.y == "<")
            {
                //add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
                movax(i, QT[i].b);
                //add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
                movax(i, QT[i].c, "BX");
                add(i, "CMP AX, BX");
                add(i, "MOV AX,1");
                add(i, "JB O" + to_string(ll));
                add(i, "MOV AX,0");
                add(i, "O" + to_string(ll) + ":");
                ll++;
                //add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
                movd(i, QT[i].d);
            }
            else if (QT[i].a.y == ">")
            {
                //add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
                movax(i, QT[i].b);
                //add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
                movax(i, QT[i].c, "BX");
                add(i, "CMP AX, BX");
                add(i, "MOV AX,1");
                add(i, "JA O" + to_string(ll));
                add(i, "MOV AX,0");
                add(i, "O" + to_string(ll) + ":");
                ll++;
                //add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
                movd(i, QT[i].d);
            }
            else if (QT[i].a.y == ">=")
            {
                //add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
                movax(i, QT[i].b);
                //add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
                movax(i, QT[i].c, "BX");
                add(i, "CMP AX, BX");
                add(i, "MOV AX,1");
                add(i, "JAE O" + to_string(ll));
                add(i, "MOV AX,0");
                add(i, "O" + to_string(ll) + ":");
                ll++;
                //add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
                movd(i, QT[i].d);
            }
            else if (QT[i].a.y == "<=")
            {
                //add(i, "MOV AX, [SI+" + get_add(QT[i].b) + "]");
                movax(i, QT[i].b);
                //add(i, "MOV BX, [SI+" + get_add(QT[i].c) + "]");
                movax(i, QT[i].c, "BX");
                add(i, "CMP AX, BX");
                add(i, "MOV AX,1");
                add(i, "JBE O" + to_string(ll));
                add(i, "MOV AX,0");
                add(i, "O" + to_string(ll) + ":");
                ll++;
                //add(i, "MOV [SI+" + get_add(QT[i].d) + "], AX");
                movd(i, QT[i].d);
            }
            else if (QT[i].a.y == "edfun")
            {
                if (pre != "main")
                    add(i, pre + " ENDP");
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
                        add(i, QT[i].a.y + " PROC");
                    pre = QT[i].a.y;
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
                res.push_back(x);
            }
        }
        res.push_back("MOV AH, 4CH");
        res.push_back("INT 21H");
        res.push_back("dsp proc\n\txor cx, cx\nAAA000:\n\txor dx, dx\nmov bx, 10\ndiv bx\npush dx\ninc cx\ncmp ax, 0\njnz AAA000\nlpp :\npop dx\nadd dx, '0'\nmov ah, 02h\nint 21h\nloop lpp\nmov dx, 10\nmov ah, 02h\nint 21h\nret\ndsp endp");
        res.push_back("CODE ENDS");
        res.push_back("END START");
        better();
    }
    void show()
    {
        for (auto x : res)
            cout << x << endl;
    }
    ~BackPoint();
};