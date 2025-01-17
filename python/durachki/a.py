def to_str(dic):
    vals = sorted(map(int, set(dic.values())))
    res = ''.join(dic.keys()).upper() + ''.join(vals)
    return res

def to_lst(data):
    res1 = data[0] + str(data[1])
    res1 = res1.replace(' ', '')
    
    res2 = {}
    for var in res1:
        if var in res2:
            res2[var] += 1
        else:
            res2[var] = 1
    
    return to_str(res2)

data = ('United States of America', 92)
print(to_lst(data))
