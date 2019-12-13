#include <bits/stdc++.h>
using namespace std;
struct token
{
    string tp;
    int ind;
};
class LL1
{
    vector<set<string>> FirstSet;
    vector<vector<vector<string>>> &Grammar;
    vector<vector<vector<string>>> LL1Tb;
    map<string,int> ind;
    int cnt;
    int isF[100];
    void dfun(string a)
    {

    }
    void solve(string src)
    {
        stringstream ss(src);
        string buff;
        stack<string> stk;
        while(ss>>buff)
        {
            auto pp=stk.top();
            stk.pop();
            if(pp[0]=='·')
            {
                if(pp[1]=='f')
                    dfun(pp);
                else if(pp!=buff)
                {
                    
                }
            }
            else
            {
                int t=ind[buff],num=stoi(pp);
                for(int i=LL1Tb[num][t].size()-1;i>-1;i--)
                {
                    stk.push(LL1Tb[num][t][i]);
                }                
            }

        }
    }
    void getFirst(int c)
    {
        if (isF[c])
            return;
        for (auto x : Grammar[c])
        {
            if (x[0][0] == '(')
            {
                if(ind.find(x[0])==ind.end())
                {
                    ind[x[0]]=cnt++;
                }
                FirstSet[c].insert(x[0]);
            }
            else
            {
                getFirst(stoi(x[0]));
                FirstSet[c].insert(FirstSet[stoi(x[0])].begin(), FirstSet[stoi(x[0])].end());
            }
        }
    }
    void getLL1()
    {
        LL1Tb.resize(Grammar.size());
        for(int i=0;i<Grammar.size();i++)
        {
            LL1Tb[i].resize(cnt);
            for(int j=0;j<Grammar[i].size();j++)
            {
                string x=Grammar[i][j][0];
                if(x[0]=='(')
                {
                    LL1Tb[i][ind[x]]=Grammar[i][j];
                }
                else
                {
                    for(auto y:FirstSet[stoi(x)])
                    {
                        LL1Tb[i][ind[y]]=Grammar[i][j];
                    }
                }
            }
        }
    }
    LL1(vector<vector<vector<string>>> &Grammar_) : Grammar(Grammar_)
    {
        cnt=0;
        getFirst(0);
        getLL1();
    }
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
void init_Grammar()
{
    ifstream fl("gm-t");
    string line;
    int x=0;
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
            if (buff == "#")
            {
                Grammar[bg].push_back(vector<string>());
                cnt++;
            }
            else
            {
                Grammar[bg][cnt].push_back(buff);
            }
        }
    }
    Grammar.resize(x);
}
void Analysis()
{
}
int main()
{
    ifstream fl("demo.txt");
    string line;
    init_Grammar();
    while (getline(fl, line))
    {
    }
}