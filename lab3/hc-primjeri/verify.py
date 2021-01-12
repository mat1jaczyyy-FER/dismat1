f = open(input("Unesite ime datoteke: "))

n = int(f.readline())
f.readline()

# e[i] -> popis svih tocaka na koje je tocka i povezana
e = [[i for i in range(n) if j[i]] for j in [[i == "1" for i in f.readline().split()] for j in range(n)]]

print("Graf učitan!")

coloring = [int(i) for i in input("Unesite bojanje grafa: ").split()]

correct = True

for i in range(n):
    for j in e[i]:
        if coloring[j] == coloring[i]:
            correct = False
            print("Pogreška - povezani čvorovi " + str(i) + " i " + str(j) + " imaju istu boju " + str(coloring[i]) + "!")

if correct:
    print("Bojanje grafa je valjano!")