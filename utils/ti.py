fl=open('./Compiler/res/gm-t',encoding="utf-8")
tmp=[]
flg=1
for line in fl.readlines():
    l=str(line)
    for i,val in enumerate(l):
        if val=='·':
            if flg:
                l=l[:i]+'('+l[i+1:]
                flg=not flg
            else:
                l=l[:i]+')'+l[i+1:]
                flg=not flg
    tmp.append(l)
fl.close();
fl=open('./Compiler/res/gm-t','w')
fl.writelines(tmp)