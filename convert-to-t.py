fl = open('Compiler/gm2', encoding='utf-8')
GrammarV = []
kTable = ['int', 'main', 'void', 'if', 'else', 'char', 'float', 'continue', 'break',
          'double', 'while', 'for', 'return', 'typedef', 'long', 'bool', 'struct', 'class']
pTable = ['<=', '==', '>=', '!=', '=', '<', '>', '+', '-', '?', ':', '&&', '|', '&', '||',
          '^','>>','<<','%','~','*', '/', '(', ')', '{', '}', '[', ']', ',', ';', '++', '--', '#', '!']
Function=[]
flo = open('Compiler/gm-t', 'w',encoding='utf-8')


class token(object):
    def __init__(self, tp, ind):
        self.tp = tp
        self.ind = ind

    def __str__(self):
        return '(%s, %s)' % (self.tp, self.ind)

    def __repr__(self):
        return '(%s, %s)' % (self.tp, self.ind)


for line in fl.readlines():
    line = line.strip('\n').split('→')
    if line[0] not in GrammarV:
        GrammarV.append(line[0])
    tmp = line[0]
    to = line[1].split('#')
    pre = 0
    flg = 1
    flo.write(str(GrammarV.index(line[0])))
    for single in to:
        tmp = ''
        for i in range(len(single)):
            if single[i] == '<' and flg:
                pre = i
            elif single[i] == '>'and flg:
                if single[pre:i+1] not in GrammarV:
                    GrammarV.append(single[pre:i+1])
                tmp += ' %d' % GrammarV.index(single[pre:i+1])
            elif single[i] == '\'' and flg:
                flg = 0
                pre = i
            elif single[i] == '\'':
                flg = 1
                if single[pre+1:i] in pTable:
                    tmp += ' ' + \
                        token('p', pTable.index(single[pre+1:i])).__str__()
                elif single[pre+1:i] in kTable:
                    tmp += ' ' + \
                        token('k', kTable.index(single[pre+1:i])).__str__()
                else:
                    if single[pre+1:i] not in Function:
                        Function.append(single[pre+1:i])
                    tmp += ' ' + \
                        token('f', Function.index(single[pre+1:i])).__str__()                    
        flo.write(' #'+tmp)
    flo.write('\n')
    print(to)
flo.close()
