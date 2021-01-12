from random import uniform

postotak = float(input("Vjerojatnost postojanja brida (float u intervalu <0, 1>): "))
n = int(input("Broj čvorova: "))

a = [[1 if uniform(0, 1) < postotak else 0 for i in range(n - j - 1, 0, -1)] for j in range(n)]

print()
print(n)
print()

for i in range(n):
    for j in range(n):
        if i == j:
            print("0", end=" ")
        if i < j:
            print(a[i][j - i - 1] , end=" ")
        if i > j:
            print(a[j][i - j - 1], end=" ")
    print()
print()