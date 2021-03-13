MemTotal = 16325036
MemFree = 11440568
MemAvailable = 12679000
Buffers = 151124
Cached = 2042440
SwapCached = 0
Active = 2653388
Inactive = 1211924


data = {}
with open("/proc/meminfo") as f:
    text = f.readlines()


    for i,line in enumerate(text):
        tmp = line.split()
        print(tmp)
        data[tmp[0].strip(":")] = int(tmp[1])
        


buffcached = data['Cached'] + data['Buffers']
used = data['MemTotal'] - data['MemFree'] - data['Cached'] - data['Buffers']


def printGb(name,val):
    print(name+":",val/976563)

printGb("Used Calc",used)
printGb("MemTotal",data['MemTotal'])
