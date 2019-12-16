#include <bits/stdc++.h>
using namespace std;
struct token
{
    string tp;
    int ind;
};
stack<string> seq;
stack<token> sem;
map<string, int> table = {make_pair("a", 0), make_pair("b", 1)};
vector<vector<vector<string>>> Grammar;
int cnt = 2;
void gen2()
{
    string op;
    token a = sem.top();
    sem.pop();
    token b = sem.top();
    sem.pop();
    cout << op << ", " << a.ind << ", " << b.ind << ", ";
    table[to_string(cnt)] = cnt;
    cout << table[to_string(cnt)];
    sem.push({to_string(cnt), cnt});
    cnt++;
}
void push(string s)
{
    sem.push({"hello", table[s]});
}
class LL1
{
    vector<set<string>> FirstSet;
    vector<set<string>> LastSet;
    vector<vector<vector<string>>> Grammar;
    vector<vector<vector<string>>> LL1Tb;
    map<string, int> ind;
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
                        if (ind.find(x[0]) == ind.end())
                        {
                            ind[x[0]] = Vt_num++;
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
                if (ind.find(x[0]) == ind.end())
                {
                    ind[x[0]] = Vt_num++;
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

    void init_Grammar()
    {
        ifstream fl("./Compiler/res/gm1-tf.txt");
        string line;
        int x = 0;
        Grammar.resize(200);
        while (getline(fl, line))
        {
            stringstream ss(line);
            x++;
            string buff;
            int bg;
            ss >> bg;
            int cnt = 0;
            Grammar[bg].push_back(vector<string>());
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
        }
        Grammar.resize(x);
    }
    void dfun(string a)
    {
        if (a == "·f,2·")
            gen2();
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
                cout << x[0] << endl;
                int loc = 0;
                string a = x[loc];
                int flg;
                int xx=0;
                do
                {
                    if (!isF[stoi(a)])
                        getFirst(stoi(a));
                    FirstSet[c].insert(FirstSet[stoi(a)].begin(), FirstSet[stoi(a)].end());
                    flg = support_e[stoi(a)];
                    xx+=flg;
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
                if(xx==x.size())
                    support_e[c]=1;
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
                    LL1Tb[i][ind[x]] = Grammar[i][j];
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
                            LL1Tb[i][ind[y]] = Grammar[i][j];
                        }
                        flg = support_e[stoi(a)];
                        if (++loc < Grammar[i][j].size())
                            a = Grammar[i][j][loc];
                        else
                        {
                            if (flg)
                                LL1Tb[i][ind[a]] = Grammar[i][j];
                            break;
                        }
                    } while (a[0] >= '0' && a[0] <= '9' && flg);
                }
            }
            if (support_e[i])
            {
                for (auto x : LastSet[i])
                {
                    LL1Tb[i][ind[x]] = {"!"};
                }
            }
        }
    }

public:
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
    }
    void solve(string src)
    {
        stringstream ss(src);
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
                        buff_que.pop();
                        if (buff_que.empty())
                        {
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
};
void Analysis()
{
}
int main()
{
    ifstream fl("./Compiler/demo.txt");
    string line;
    getline(fl, line);
    LL1 a;
    a.solve(line);
}