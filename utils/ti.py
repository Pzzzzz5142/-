fl=open('./Compiler/res/gm-t')
tmp=[]
for line in fl.readlines():
    line=line.strip('\n').strip('#')
    tmp.append(line+'\n')
fl.close();
fl=open('./Compiler/res/gm-t','w')
fl.writelines(tmp)