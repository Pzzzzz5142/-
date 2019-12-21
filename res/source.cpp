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
int main()
{
    int a;
    int b;
    int c;
    int d;
    a=10;
    b=20;
    for(i=1;i<6;i=i+1)
    {
        a=a*2;
        if(b<10)
        {
            while(c<10)
            {
                d=c+1;
            }
        }
        else 
        {
            putc(F(i));
        }
    }
    putc(a);
    putc(b);
    putc(c);
    putc(d);
}
#