#!/usr/bin/env python3
# Dominik Matijaca 0036524568

# Unos podataka
f = open(input("Unesite ime datoteke: "))

n = int(f.readline())
f.readline()

# e[i] -> popis svih tocaka na koje je tocka i povezana
e = [[i for i in range(n) if j[i]] for j in [[i == "1" for i in f.readline().split()] for j in range(n)]]

f.close()

# Rezultat
r = 0

# DFS algoritam seta po cijelom grafu 
# t -> pocetna tocka, s kojom ciklus mora i zavrsiti
# x -> tocka koju trenutno gledamo, i sirimo se u dubinu iz nje
# b -> tocke koje se ignorira za sirenje (vec posjecene u putu). len(b) je duljina ciklusa
def dfs(t, x, b):
    global e, r
    for i in e[x]:
        if len(b) >= 3 and i == t: # ciklus mora imati minimalnu duljinu 3
            r = max(r, len(b))     # max -> ostavlja najbolji rezultat, tj najdulji ciklus
        
        if i not in b:
            dfs(t, i, b + [i])     # rekurzivno sirenje

# Zapoceti DFS iz svake tocke
for i in range(n):
    dfs(i, i, [i])

# Ispis najduljeg ciklusa
print(r)