int i;
int j;
int F(int i)
{
    if(i<=1)
    {
        return 1;
    }
    return F(i-1)*i;
}
int LL(int x,int y)
{
    return x+y;
}
int main()
{
    int a;
    int b;
    int c;
    int d;
    a=1;
    d=0;
    for(i=0;i<10;i=i+1)
    {
        a=a+b;
        c=0;
        while(c<10)
        {
            d=d+1;
            c=c+1;
        }
    }
    LL(i);
    putc(d);
}
#