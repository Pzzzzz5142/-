#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <bits/stdc++.h>
using namespace std;
#define INT -1
#define CHAR -2
#define BOOL -3
#define CON 1
#define FUN 0
#define TYP 2
#define PARA 3
int i;
int j;

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
    int level;
    int off;
    int fn;
    int entry;
    vector<SynblNode> param;
}; //函数表的结构体

vector<SynblNode> SYNBL;     //符号总表
vector<TapelNode> TAPEL;     //类型表
vector<AinflNode> AINFL;     //数组表
vector<PfinflNode> PFINFL;   //函数表
vector<SynblNode> PARAMETER; //参数表
vector<int> LENL;            //长度表
stack<string> VALL;          //活动记录表

int type;
int num;
string id;
string pre;
bool isRun;
int nowTable;

struct SymbolTableNode
{
    vector<SynblNode> SYNBL; //符号总表
    vector<TapelNode> TAPEL; //类型表
    vector<AinflNode> AINFL; //数组表
    bool link;
    SymbolTableNode(int link_) : link(link_)
    {
    }
};
vector<SymbolTableNode> SYMBOLTABEL; //符号表的数组

struct SemNode
{
    int x;
    string y;
}; //结构体

struct QtNode
{
    SemNode a;
    SemNode b;
    SemNode c;
    SemNode d;
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
void if1()
{
    QT.push_back(QtNode());
    SYMBOLTABEL.push_back(SymbolTableNode(1));
    nowTable++;
    QT[QT.size() - 1].a.x = 0;
    QT[QT.size() - 1].a.y = "if";
    QT[QT.size() - 1].b.x = SEM.top().x;
    QT[QT.size() - 1].b.y = SEM.top().y;
    QT[QT.size() - 1].c.x = -1;
    SEM.pop();
    backaddress.push(QT.size() - 1);
}

void if2()
{
    QT.push_back(QtNode());
    QT[QT.size() - 1].a.x = 0;
    QT[QT.size() - 1].a.y = "ef";
    QT[QT.size() - 1].b.x = -1;
    QT[QT.size() - 1].c.x = -1;
    QT[backaddress.top()].d.x = QT.size();
    backaddress.pop();
    backaddress.push(QT.size() - 1);
    SYMBOLTABEL.pop_back();
    nowTable--;
}

void el()
{
    QT.push_back(QtNode());
    QT[QT.size() - 1].a.x = 0;
    QT[QT.size() - 1].a.y = "else";
    QT[QT.size() - 1].b.x = -1;
    QT[QT.size() - 1].c.x = -1;
    SYMBOLTABEL.push_back(SymbolTableNode(1));
    nowTable++;
    QT[backaddress.top()].d.x = QT.size() - 1;
    backaddress.pop();
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
}

void endif()
{
    QT[backaddress.top()].d.x = QT.size();
    backaddress.pop();
    SYMBOLTABEL.pop_back();
    nowTable--;
}

void wh1()
{
    QT.push_back(QtNode());
    QT[QT.size() - 1].a.x = 0;
    QT[QT.size() - 1].a.y = "wh";
    QT[QT.size() - 1].b.x = -1;
    QT[QT.size() - 1].c.x = -1;
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
    SYMBOLTABEL.push_back(SymbolTableNode(1));
    nowTable++;
    brestack.push({QT.size() - 1, 1});
}

void wh3()
{
    QT.push_back(QtNode());
    QT[QT.size() - 1].a.x = 0;
    QT[QT.size() - 1].a.y = "we";
    QT[QT.size() - 1].b.x = -1;
    QT[QT.size() - 1].c.x = -1;
    QT[QT.size() - 1].d.x = constack.top();
    SYMBOLTABEL.pop_back();
    QT[brestack.top().a].d.x = QT.size();
    nowTable--;
    constack.pop();
}

bool find(string name)
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
    QT[QT.size() - 1].b = SEM.top();
    find(SEM.top().y);
    SEM.pop();
    QT[QT.size() - 1].c = SEM.top();
    find(SEM.top().y);
    SEM.pop();
    QT[QT.size() - 1].d.y = "#t";
    QT[QT.size() - 1].d.x = num;
    num++;
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
    SYNBL.push_back(SynblNode());
    j = SYNBL.size() - 1;
    SYNBL[j].name = name;
    SYNBL[j].type = type;
    SYNBL[j].cat = FUN;
    PFINFL.push_back(PfinflNode());
    SYMBOLTABEL.push_back(SymbolTableNode(0));
    int k = PFINFL.size() - 1;
    SYNBL[j].addr = k;
}

int new_add()
{
    if (isRun)
    {
    }
    else
    {
        return -1;
    }
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

void qua_declare_id(string name) //声明变量
{
    PfinflNode loc = PFINFL[PFINFL.size() - 1];
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

//void qua_if void qua_endif void qua_endifall void quawh void quawdo void quawe
void quafor()
{
    QT.push_back(QtNode());
    QT[QT.size() - 1].a.x = 0;
    QT[QT.size() - 1].a.y = "for";
    QT[QT.size() - 1].b.x = -1;
    QT[QT.size() - 1].c.x = -1;
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
    brestack.push({QT.size() - 1, 1});
}
void quafsav()
{
    QT.push_back(QtNode());
    QT[QT.size() - 1].a.x = 0;
    QT[QT.size() - 1].a.y = "fordone";
    QT[QT.size() - 1].b.x = -1;
    QT[QT.size() - 1].b.y = -1;
    QT[QT.size() - 1].c.x = -1;
    QT[QT.size() - 1].d.x = 0;
    QT[QT.size() - 1].d.y = constack.top();
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
    SYMBOLTABEL.pop_back();
    while (!brestack.empty() && flg)
    {
        bre loc = brestack.top();
        brestack.pop();
        if (loc.tab == 1)
            flg = 1;
        QT[loc.a].d.x = QT.size() - 1;
    }
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
    brestack.push({QT.size() - 1, 0});
}
void qua_run()
{
    isRun = true;
    nowTable = -1;
}