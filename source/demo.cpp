#include <bits/stdc++.h>
using namespace std;
#define INT -1
#define CHAR -2
#define BOOL -3
#define VOID -4
#define CON 1
#define FUN 0
#define TYP 2
#define PARA 3

class LL1
{
    vector<set<string>> FirstSet;
    vector<set<string>> LastSet;
    vector<vector<vector<string>>> Grammar;
    vector<vector<vector<string>>> TGrammar;
    vector<vector<vector<string>>> LL1Tb;
    queue<string> stk_id;
    map<string, int> ind;

    struct SynblNode
    {
        string name;
        int type;
        int cat;
        int addr;
    }; //符号表总表的结构体

    struct TapelNode
    {
        int tval;
        int tpoint;
    }; //类型表的结构体

    struct AinflNode
    {
        int low;
        int up;
        int clp;
        int cten;
    }; //数组表的结构体

    struct PfinflNode
    {
        int off;
        int entry;
        vector<SynblNode> param;
    }; //函数表的结构体

    struct SymbolTableNode
    {
        vector<SynblNode> SYNBL; //符号总表
        vector<TapelNode> TAPEL; //类型表
        vector<AinflNode> AINFL; //数组表
        bool link;
        SymbolTableNode(int link_ = 0) : link(link_)
        {
        }
    };
    vector<SymbolTableNode> SYMBOLTABEL; //符号表的数组

    vector<SynblNode> SYNBL;     //符号总表
    vector<TapelNode> TAPEL;     //类型表
    vector<AinflNode> AINFL;     //数组表
    vector<PfinflNode> PFINFL;   //函数表
    vector<SynblNode> PARAMETER; //参数表
    vector<int> LENL;            //长度表
    stack<string> VALL;          //活动记录表
    stack<string> opl;
    int add_allocate_num = 1;
    int func_num = 0;
    stack<int> backfun;
    stack<int> add_stk;

    int type;
    int nowType;
    int num;
    string id;
    string pre;
    bool isRun;
    int nowTable = -1;

    struct SemNode
    {
        int x;
        string y;
        friend ostream &operator<<(ostream &out, SemNode a)
        {
            if (a.y[0] == '#')
            {
                for (int i = 1; i < a.y.size(); i++)
                    out << a.y[i];
                out << a.x;
                return out;
            }
            if (a.x == 0)
                out << a.y;
            else if (a.x == -1)
                out << "_";
            else
                out << a.x;
            return out;
        }
    }; //结构体

    struct QtNode
    {
        SemNode a;
        SemNode b;
        SemNode c;
        SemNode d;
        friend ostream &operator<<(ostream &out, QtNode a)
        {
            out << a.a << ", " << a.b << ", " << a.c << ", " << a.d << endl;
            return out;
        }
    }; //四元式结构体

    struct bre
    {
        int a;
        int tab;
    };

    vector<QtNode> QT;
    stack<SemNode> SEM;
    stack<int> backaddress;  //匹配栈，存放需要返回填写的四元式
    stack<int> backaddress2; //存放待填写的位置
    stack<bre> brestack;     //
    stack<int> constack;

    int new_add()
    {
        if (isRun)
        {
            return add_allocate_num++;
        }
        else
        {
            int tmp = add_allocate_num++;
            return -tmp;
        }
    }

    void qua_declare_id(string name) //声明变量
    {
        SynblNode node;
        node.cat = 0;
        node.name = name;
        TAPEL.push_back(TapelNode());
        node.type = type;
        node.cat = TYP;
        node.addr = new_add();

        if (nowTable == -1)
        {
            for (auto x : SYNBL)
            {
                if (x.name == node.name)
                {
                    cerr << "SAME NAME FOUND!" << endl;
                    exit(1);
                }
            }
            SYNBL.push_back(node);
        }
        else
        {
            for (auto x : SYMBOLTABEL[nowTable].SYNBL)
            {
                if (x.name == node.name)
                {
                    cerr << "SAME NAME FOUND!" << endl;
                    exit(1);
                }
            }
            SYMBOLTABEL[nowTable].SYNBL.push_back(node);
        }
    }

    void qua_return()
    {
        int x = backfun.top();
        if (FUNSYMBEL.size() <= backfun.top())
            FUNSYMBEL.resize(backfun.top() + 1);
        FUNSYMBEL[backfun.top()] = SYMBOLTABEL[SYMBOLTABEL.size() - 1];
        SYMBOLTABEL.pop_back();
        nowTable--;
        backfun.pop();
        add_allocate_num = add_stk.top();
        add_stk.pop();
    }

    void qua_run()
    {
        isRun = true;
        nowTable = -1;
    }

    void save_op(string op)
    {
        opl.push(pre);
    }

    void pop_type()
    {
        nowType = 0;
    }

    void if1()
    {
        QT.push_back(QtNode());
        SYMBOLTABEL.push_back(SymbolTableNode(1));
        nowTable++;
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "if";
        QT[QT.size() - 1].b.x = SEM.top().x;
        QT[QT.size() - 1].b.y = SEM.top().y;
        QT[QT.size() - 1].c.x = func_num;
        backfun.push(func_num++);
        QT[QT.size() - 1].d.x = -1;
        SEM.pop();
        add_stk.push(add_allocate_num);
        add_allocate_num = 1;
        backaddress.push(QT.size() - 1);
    }

    void if2()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "ef";
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].c.x = -1;
        QT[QT.size() - 1].d.x = -1;
        QT[backaddress.top()].d.x = QT.size();
        backaddress.pop();
        backaddress.push(QT.size() - 1);
        qua_return();
    }

    void el()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "else";
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].c.x = func_num;
        backfun.push(func_num++);
        SYMBOLTABEL.push_back(SymbolTableNode(1));
        nowTable++;
        add_stk.push(add_allocate_num);
        add_allocate_num = 1;
    }

    void endelse()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "ee";
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].c.x = -1;
        QT[backaddress.top()].d.x = QT.size();
        backaddress.pop();
        qua_return();
    }

    void endif()
    {
        if (!backaddress.empty() && QT[backaddress.top()].d.x == -1)
        {
            QT[backaddress.top()].d.x = QT.size();
            backaddress.pop();
        }
    }

    void wh1()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "wh";
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].c.x = func_num;
        backfun.push(func_num++);
        QT[QT.size() - 1].d.x = -1;
        constack.push(QT.size() - 1);
    }

    void wh2()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "do";
        QT[QT.size() - 1].b.x = SEM.top().x;
        QT[QT.size() - 1].b.y = SEM.top().y;
        QT[QT.size() - 1].c.x = -1;
        SEM.pop();
        add_stk.push(add_allocate_num);
        add_allocate_num = 1;
        SYMBOLTABEL.push_back(SymbolTableNode(1));
        nowTable++;
        brestack.push({int(QT.size() - 1), 1});
    }

    void wh3()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "we";
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].c.x = -1;
        QT[QT.size() - 1].d.x = constack.top();
        QT[brestack.top().a].d.x = QT.size();
        constack.pop();
        qua_return();
    }

    int find(string name)
    {
        int flg = 0;
        for (int i = SYMBOLTABEL.size() - 1; i > -1; i--)
        {
            if (flg)
                break;
            for (auto x : SYMBOLTABEL[i].SYNBL)
            {
                if (x.name == name)
                {
                    if (x.cat == TYP)
                    {
                        flg = 1;
                        break;
                    }
                    cerr << "Wrong type!" << endl;
                    exit(1);
                }
            }
            if (SYMBOLTABEL[i].link == 0)
                break;
        }
        if (!flg)
            for (auto x : SYNBL)
            {
                if (x.name == name)
                {
                    if (x.cat == TYP)
                    {
                        flg = 1;
                        break;
                    }
                    cerr << "Wrong type!" << endl;
                    exit(1);
                }
            }
        if (flg == 0)
        {
            cerr << "Not found " << SEM.top().y << endl;
            exit(1);
        }
    }

    void save_type()
    {
        if (pre == "int")
            type = INT;
        else if (pre == "char")
            type = CHAR;
        else if (pre == "bool")
            type = BOOL;
    }

    void GEQ(string op)
    {

        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = op; //分为关系运算和算术运算
        if (op != "=")
        {
            QT[QT.size() - 1].c = SEM.top();
            int tp1, tp2;
            tp1 = find(SEM.top().y);
            SEM.pop();
            QT[QT.size() - 1].b = SEM.top();
            tp2 = find(SEM.top().y);
            SEM.pop();
            QT[QT.size() - 1].d.y = "#t";
            QT[QT.size() - 1].d.x = num;
            type = tp1;
            qua_declare_id("#t" + to_string(num));
            num++;
        }
        else
        {
            QT[QT.size() - 1].d = SEM.top();
            int tp1, tp2;
            if (SEM.top().y[0] != '#')
                tp1 = find(SEM.top().y);
            SEM.pop();
            QT[QT.size() - 1].b = SEM.top();
            QT[QT.size() - 1].c.x = -1;
            if (SEM.top().y[0] != '#')
                tp2 = find(SEM.top().y);
            SEM.pop();
        }
        SEM.push(QT[QT.size() - 1].d);
    }

    void quap(string name)
    {
        int x = 0;
        string y = name;
        SemNode z = {x, y};
        SEM.push(z);
    }

    void fuc_init(string name)
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = name; //分为关系运算和算术运算
        QT[QT.size() - 1].c.x = func_num;
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].d.x = -1;
        backfun.push(func_num++);
        SYNBL.push_back(SynblNode());
        int j = SYNBL.size() - 1;
        SYNBL[j].name = name;
        if (name == "main")
            qua_run();
        add_stk.push(add_allocate_num);
        add_allocate_num = 1;
        SYNBL[j].type = type;
        SYNBL[j].cat = FUN;
        PFINFL.push_back(PfinflNode());
        SYMBOLTABEL.push_back(SymbolTableNode(0));
        nowTable++;
        int k = PFINFL.size() - 1;
        SYNBL[j].addr = k;
    }

    void qua_declare_param(string name) //关于形参1
    {
        PfinflNode loc = PFINFL[PFINFL.size() - 1];
        SynblNode node;
        node.cat = TYP;
        node.name = name;
        TAPEL.push_back(TapelNode());
        for (auto x : SYNBL)
        {
            if (x.name == node.name)
            {
                cerr << "SAME NAME FOUND!" << endl;
                exit(1);
            }
        }
        node.type = type;
        node.addr = new_add();
        loc.param.push_back(node);
    }

    void quafor()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "for";
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].c.x = func_num;
        QT[QT.size() - 1].d.x = -1;
        backfun.push(func_num++);
        add_stk.push(add_allocate_num);
        add_allocate_num = 1;
        SYMBOLTABEL.push_back(SymbolTableNode(1));
        nowTable++;
        constack.push(QT.size() - 1);
    }
    void quafdo()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "judge";
        QT[QT.size() - 1].b.x = SEM.top().x;
        QT[QT.size() - 1].b.y = SEM.top().y;
        SEM.pop();
        brestack.push({int(QT.size() - 1), 1});
    }
    void quafsav()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "fordone";
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].b.y = -1;
        QT[QT.size() - 1].c.x = -1;
        QT[QT.size() - 1].d.x = constack.top();
        QT[brestack.top().a].c.x = QT.size();
        SEM.pop();
    }
    void quafe()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "fe";
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].c.x = -1;
        QT[QT.size() - 1].d.x = constack.top();
        int flg = 1;
        while (!brestack.empty() && flg)
        {
            bre loc = brestack.top();
            brestack.pop();
            if (loc.tab == 1)
                flg = 1;
            QT[loc.a].d.x = QT.size() - 1;
        }
        qua_return();
    }
    void qua_continue()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "continue";
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].c.x = -1;
        constack.push(QT.size() - 1);
    }
    void qua_break()
    {
        QT.push_back(QtNode());
        QT[QT.size() - 1].a.x = 0;
        QT[QT.size() - 1].a.y = "break";
        QT[QT.size() - 1].b.x = -1;
        QT[QT.size() - 1].c.x = -1;
        brestack.push({int(QT.size() - 1), 0});
    }

    struct token
    {
        string tp;
        int ind;
    };
    stack<string> seq;
    stack<token> sem;
    map<string, int> table = {make_pair("a", 0), make_pair("b", 1)};
    int cnt = 2;
    struct
    {
        int to, nx;
    } edge[200];

    int head[100], totE, support_e[100], in[100];

    void add(int fr, int to)
    {
        edge[totE] = {to, head[fr]};
        head[fr] = totE++;
        in[to]++;
    }

    void last_init()
    {
        for (int i = 0; i < Grammar.size(); i++)
        {
            for (auto x : Grammar[i])
            {
                for (int j = 0; j < x.size() - 1; j++)
                {
                    if (x[j][0] >= '0' && x[j][0] <= '9')
                    {
                        if (x[j + 1][0] >= '0' && x[j + 1][0] <= '9')
                        {
                            LastSet[stoi(x[j])].insert(FirstSet[stoi(x[j + 1])].begin(), FirstSet[stoi(x[j + 1])].end());
                        }
                        else
                        {
                            LastSet[stoi(x[j])].insert(x[j + 1]);
                        }
                    }
                    else
                    {
                        if (ind.find(x[j]) == ind.end())
                        {
                            ind[x[j]] = Vt_num++;
                        }
                    }
                }
                int loc = x.size() - 1;
                int flg;
                do
                {
                    string a = x[loc];
                    if (x[loc][0] >= '0' && x[loc][0] <= '9')
                    {
                        if (stoi(a) != i)
                            add(i, stoi(a));
                    }
                    else
                        break;
                    flg = support_e[stoi(a)];
                    loc--;
                    if (loc < 0)
                        break;
                } while (flg);
                if (x[x.size() - 1][0] > '9' || x[x.size() - 1][0] < '0')
                    if (ind.find(x[x.size() - 1]) == ind.end())
                    {
                        ind[x[x.size() - 1]] = Vt_num++;
                    }
            }
        }
    }

    void getLast()
    {
        last_init();
        queue<int> que;
        for (int i = 0; i < Grammar.size(); i++)
        {
            if (in[i] == 0)
                que.push(i);
        }
        while (!que.empty())
        {
            auto pp = que.front();
            que.pop();
            for (int k = head[pp]; k != -1; k = edge[k].nx)
            {
                if (--in[edge[k].to] == 0)
                {
                    que.push(edge[k].to);
                }
                LastSet[edge[k].to].insert(LastSet[pp].begin(), LastSet[pp].end());
            }
        }
    }

    int Vt_num;
    int isF[100];

    string get_id()
    {
        if (stk_id.empty())
        {
            cerr << "id is not enough!" << endl;
            exit(1);
        }
        string pp = stk_id.front();
        stk_id.pop();
        return pp;
    }

    void init_stk_id()
    {
        ifstream fl("./ids");
        string buff;
        while (fl >> buff)
        {
            stk_id.push(buff);
        }
    }

    void init_Grammar()
    {
        ifstream fl("gm1-tf.txt");
        ifstream tfl("gm1-t.txt");
        string line;
        string tline;
        int x = 0;
        Grammar.resize(200);
        TGrammar.resize(200);
        while (getline(fl, line), getline(tfl, tline))
        {
            stringstream ss(line);
            stringstream tss(tline);
            x++;
            string buff;
            string tbuff;
            int bg;
            ss >> bg;
            tss >> bg;
            int cnt = 0;
            Grammar[bg].push_back(vector<string>());
            TGrammar[bg].push_back(vector<string>());
            while (ss >> buff)
            {
                if (buff == "|")
                {
                    Grammar[bg].push_back(vector<string>());
                    cnt++;
                }
                else
                {
                    if (buff == "null")
                    {
                        support_e[bg] = 1;
                        Grammar[bg].pop_back();
                        cnt--;
                    }
                    else
                        Grammar[bg][cnt].push_back(buff);
                }
            }
            cnt = 0;
            while (tss >> tbuff)
            {
                if (tbuff == "|")
                {
                    TGrammar[bg].push_back(vector<string>());
                    cnt++;
                }
                else
                {
                    if (tbuff == "null")
                    {
                        support_e[bg] = 1;
                    }
                    TGrammar[bg][cnt].push_back(tbuff);
                }
            }
        }
        Grammar.resize(x);
        TGrammar.resize(x);
    }
    void dfun(string a)
    {
        if (a == "qua_fuc_init")
        {
            fuc_init(pre);
        }
        else if (a == "qua_declare_param")
        {
            qua_declare_param(pre);
        }
        else if (a == "qua_if1")
        {
            if1();
        }
        else if (a == "qua_if2")
        {
            if2();
        }
        else if (a == "qua_el")
        {
            el();
        }
        else if (a == "qua_endif")
        {
            endif();
        }
        else if (a == "qua_ee")
        {
            endelse();
        }
        else if (a == "qua_save_type")
        {
            save_type();
        }
        else if (a == "qua_op")
        {
            if (opl.empty())
                exit(1);
            GEQ(opl.top());
            opl.pop();
        }
        else if (a == "qua_declare_id")
        {
            qua_declare_id(pre);
        }
        else if (a == "qua_save_op")
        {
            save_op(pre);
        }
        else if (a == "quap")
        {
            quap(pre);
        }
        else if (a == "qua_return")
        {
            qua_return();
        }
        else if (a == "quafor")
        {
            quafor();
        }
        else if (a == "quafdo")
        {
            quafdo();
        }
        else if (a == "quasav")
        {
            quafsav();
        }
        else if (a == "quafe")
        {
            quafe();
        }
        else if (a == "qua_wh1")
        {
            wh1();
        }
        else if (a == "qua_wh2")
        {
            wh2();
        }
        else if (a == "qua_wh3")
        {
            wh3();
        }
    }
    void getFirst(int c)
    {
        if (isF[c])
            return;
        isF[c] = 1;
        for (auto x : Grammar[c])
        {
            if (x[0][0] > '9' || x[0][0] < '0')
            {
                string a = x[0];
                FirstSet[c].insert(a);
            }
            else
            {
                int loc = 0;
                string a = x[loc];
                int flg;
                int xx = 0;
                do
                {
                    if (!isF[stoi(a)])
                        getFirst(stoi(a));
                    FirstSet[c].insert(FirstSet[stoi(a)].begin(), FirstSet[stoi(a)].end());
                    flg = support_e[stoi(a)];
                    xx += flg;
                    loc++;
                    if (loc >= x.size())
                        break;
                    a = x[loc];
                    if (a[0] > '9' || a[0] < '0')
                    {
                        if (flg)
                            FirstSet[c].insert(a);
                        break;
                    }
                } while (flg);
                if (xx == x.size())
                    support_e[c] = 1;
            }
        }
    }
    void getLL1()
    {
        LL1Tb.resize(Grammar.size());
        for (int i = 0; i < Grammar.size(); i++)
        {
            LL1Tb[i].resize(Vt_num);
            for (int j = 0; j < Grammar[i].size(); j++)
            {
                string x = Grammar[i][j][0];
                if (x[0] > '9' || x[0] < '0')
                {
                    LL1Tb[i][ind[x]] = TGrammar[i][j];
                }
                else
                {
                    int loc = 0;
                    string a = Grammar[i][j][loc];
                    int flg;
                    do
                    {
                        for (auto y : FirstSet[stoi(a)])
                        {
                            LL1Tb[i][ind[y]] = TGrammar[i][j];
                        }
                        flg = support_e[stoi(a)];
                        if (++loc < Grammar[i][j].size())
                            a = TGrammar[i][j][loc];
                        else
                            break;
                        if (a[0] > '9' || a[0] < '0')
                        {
                            if (flg)
                                LL1Tb[i][ind[a]] = TGrammar[i][j];
                            break;
                        }
                    } while (a[0] >= '0' && a[0] <= '9' && flg);
                }
            }
            if (support_e[i])
            {
                vector<string> a;
                for (auto x : TGrammar[i])
                    if (x[0] == "null")
                    {
                        a = x;
                        a[0] = string("!");
                    }
                if (a.empty())
                    continue;
                for (auto x : LastSet[i])
                {
                    LL1Tb[i][ind[x]] = a;
                }
            }
        }
    }

public:
    vector<SymbolTableNode> FUNSYMBEL;
    LL1()
    {
        memset(support_e, 0, sizeof(support_e));
        init_Grammar();
        memset(in, 0, sizeof(in));
        memset(head, -1, sizeof(head));
        totE = 0;
        memset(isF, 0, sizeof(isF));
        Vt_num = 0;
        FirstSet.resize(200);
        LastSet.resize(200);
        for (int i = 0; i < Grammar.size(); i++)
            getFirst(i);
        getLast();
        getLL1();
        init_stk_id();
    }
    void solve(string src)
    {
        ifstream ss(src);
        string buff;
        stack<string> stk;
        stack<string> sem;
        queue<string> buff_que;
        while (ss >> buff)
        {
            buff_que.push(buff);
        }
        stk.push("0");
        while (!buff_que.empty())
        {
            auto pp = stk.top();
            auto buff = buff_que.front();
            if (pp[0] > '9' || pp[0] < '0')
            {
                while (pp[0] > '9' || pp[0] < '0')
                {
                    stk.pop();
                    if (pp == "!")
                    {
                    }
                    else if (pp.size() > 2 && pp[0] == 'q' && pp[1] == 'u' && pp[2] == 'a')
                        dfun(pp);
                    else if (pp != buff)
                    {
                        cerr << "WA!" << endl;
                        exit(1);
                    }
                    else
                    {
                        pre = buff;
                        if (pre[0] == '@')
                            pre = get_id();

                        buff_que.pop();
                        if (buff_que.empty())
                        {
                            while (!stk.empty() && support_e[stoi(stk.top())])
                                stk.pop();
                            if (stk.empty())
                            {
                                cout << "OK!" << endl;
                                return;
                            }
                            else
                            {
                                cerr << "WA!" << endl;
                                exit(1);
                            }
                        }
                        buff = buff_que.front();
                    }
                    pp = stk.top();
                }
            }
            else
            {
                stk.pop();
                int t = ind[buff], num = stoi(pp);
                int ff = 0;
                for (int i = LL1Tb[num][t].size() - 1; i > -1; i--)
                {
                    stk.push(LL1Tb[num][t][i]);
                    ff = 1;
                }
                if (ff == 0)
                {
                    cerr << num << ' ' << buff << endl;
                    cerr << "WA!" << endl;
                }
            }
        }
        return;
    }
    void show()
    {
        int t = 0;
        for (auto x : QT)
            cout << "(" << t++ << ")" << x;
    }
};

int main()
{
    LL1 a;
    a.solve("demo.txt");
    a.show();
    return 0;
}