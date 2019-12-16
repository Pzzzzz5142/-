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
    } edge[2000];

    int head[1000], totE, support_e[1000], in[1000];

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
                        if (x[j+1][0] >= '0' && x[j+1][0] <= '9')
                        {
                            LastSet[stoi(x[j])].insert(FirstSet[stoi(x[j + 1])].begin(), FirstSet[stoi(x[j + 1])].end());
                        }
                        else
                        {
                            LastSet[stoi(x[j])].insert(x[j + 1]);
                        }
                    }
                }
                if (x[x.size() - 1][0] >= '0' && x[x.size() - 1][0] <= '9')
                {
                    add(i, stoi(x[x.size() - 1]));
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

    int cnt;
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
        for (auto x : Grammar[c])
        {
            if (x[0][0] > '9' || x[0][0] < '0')
            {
                string a = x[0];
                if (ind.find(x[0]) == ind.end())
                {
                    ind[x[0]] = cnt++;
                }
                FirstSet[c].insert(a);
            }
            else
            {
                cout << x[0] << endl;
                string a = x[0];
                getFirst(stoi(a));
                FirstSet[c].insert(FirstSet[stoi(a)].begin(), FirstSet[stoi(a)].end());
            }
        }
    }
    void getLL1()
    {
        LL1Tb.resize(Grammar.size());
        for (int i = 0; i < Grammar.size(); i++)
        {
            LL1Tb[i].resize(cnt);
            for (int j = 0; j < Grammar[i].size(); j++)
            {
                string x = Grammar[i][j][0];
                if (x[0] > '9' || x[0] < '0')
                {
                    LL1Tb[i][ind[x]] = Grammar[i][j];
                }
                else
                {
                    for (auto y : FirstSet[stoi(x)])
                    {
                        LL1Tb[i][ind[y]] = Grammar[i][j];
                    }
                }
            }
        }
    }

public:
    LL1()
    {
        init_Grammar();
        memset(support_e, 0, sizeof(support_e));
        memset(in, 0, sizeof(in));
        memset(head, -1, sizeof(head));
        totE = 0;
        memset(isF, 0, sizeof(isF));
        cnt = 0;
        FirstSet.resize(200);
        LastSet.resize(200);
        getFirst(0);
        getLast();
        getLL1();
    }
    void solve(string src)
    {
        stringstream ss(src);
        string buff;
        stack<string> stk;
        stk.push("0");
        while (ss >> buff)
        {
            auto pp = stk.top();
            stk.pop();
            if (pp[0] == '(')
            {
                if (pp[1] == 'f')
                    dfun(pp);
                else if (pp != buff)
                {
                    cerr << "WA!" << endl;
                    exit(1);
                }
            }
            else
            {
                int t = ind[buff], num = stoi(pp);
                for (int i = LL1Tb[num][t].size() - 1; i > -1; i--)
                {
                    stk.push(LL1Tb[num][t][i]);
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