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
int cnt=2;
void gen2()
{
    string op;
    token a = sem.top();
    sem.pop();
    token b = sem.top();
    sem.pop();
    cout<<op<<", "<<a.ind<<", "<<b.ind<<", ";
    table[to_string(cnt)]=cnt;
    cout<<table[to_string(cnt)];
    sem.push({to_string(cnt),cnt});
    cnt++;
}
void push(string s)
{
    sem.push({"hello", table[s]});
}
void init()
{
}
int main()
{
    ifstream fl("demo.txt");
    string line;
    while(getline(fl,line))
    {
        
    }
}