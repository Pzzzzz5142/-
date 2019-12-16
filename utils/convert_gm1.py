VN = []
with open('./Compiler/res/gm1.txt', encoding='utf-8') as fl:
    flw = open('./Compiler/res/gm1-t.txt', 'w', encoding='utf-8')
    flwf = open('./Compiler/res/gm1-tf.txt', 'w', encoding='utf-8')
    for line in fl.readlines():
        if line == '\n':
            continue
        line = line.split('->')
        vn = line[0]
        if vn not in VN:
            VN.append(vn)
        vn = VN.index(vn)
        tmp = [str(vn)+' -> ']
        tmp1 = tmp[:]
        scent = line[1].split(' ')
        for word in scent:
            if word == '':
                continue
            if word[0] == '$':
                if word not in VN:
                    VN.append(word)
                tmp.append(str(VN.index(word))+' ')
                tmp1.append(str(VN.index(word))+' ')
            else:
                if word[:3] != 'qua':
                    tmp1.append(word+' ')
                tmp.append(word+' ')
        flw.writelines(tmp)
        flwf.writelines(tmp1)
