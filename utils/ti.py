fl=open('./Compiler/res/gm1-tf.txt',encoding="utf-8")
l=[]
for line in fl.readlines():
    line=line.split(' ')
    if line[0] in l:
        print(line[0])
    l.append(line[0])