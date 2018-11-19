filename = "test3.txt"
savename = "triple_test3.txt"
#filename = "train3.txt"
#savename = "triple_train3.txt"
with open(filename, "r") as f:
    x = []
    for line in f:
        y = []
        line = line.split()
        y.append(line[0])
        y.append("0")
        y.append(line[1])
        #print(y)
        x.append(y)
    print(x)

with open(savename, "w") as f:
    #f.writelines(x[0])
    for line in x:
        string = line[0] + "\t" + line[1] + "\t" + line[2] + "\n"
        f.write(string)
        #for i in range(3):
        #    #f.write(line[i])
        #    #f.write()
        #    #if i == 3:
