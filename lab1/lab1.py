#!/usr/bin/env python3
# Dominik Matijaca 0036524568

# Unos podataka
s = ["prvo", "drugo", "trece"]
nul = [int(input("Unesite " + s[i] + " rjesenje x_" + str(i) + " karakteristicne jednadzbe: ")) for i in range(3)]

s = ["nultog", "prvog", "drugog"]
cache = [int(input("Unesite vrijednost " + s[i] + " clana niza a_" + str(i) + ": ")) for i in range(3)]

n = int(input("Unesite redni broj n trazenog clana niza: "))

''' 1. Rjesavanje pomocu formule za opce rjesenje homogene rekurzivne relacije '''

# Ako su nultocke iste, koeficijent se mnozi s n
def mul(x, r):
    if x == 0:
        return 1
    if x == 1:
        return r if nul[0] == nul[1] else 1
    if x == 2:
        return (r if nul[0] == nul[2] else 1) * (r if nul[1] == nul[2] else 1)

# Pronaci lambda multiplikatore (x,y,z): rijesi sustav 3 jednadzbe s 3 nepoznanice
sys = [[mul(i, j) * (nul[i] ** j) for i in range(3)] + [cache[j]] for j in range(3)]

afbe = sys[0][0] * sys[1][1] - sys[0][1] * sys[1][0]
ajbi = sys[0][0] * sys[2][1] - sys[0][1] * sys[2][0]
ahde = sys[0][0] * sys[1][3] - sys[0][3] * sys[1][0]
ceag = sys[0][2] * sys[1][0] - sys[0][0] * sys[1][2]

rj = [None, None, None]
rj[2] = (afbe * (sys[0][0] * sys[2][3] - sys[0][3] * sys[2][0]) - ajbi * ahde) / (afbe * (sys[0][0] * sys[2][2] - sys[0][2] * sys[2][0]) + ajbi * ceag)
rj[1] = (ahde + ceag * rj[2]) / afbe
rj[0] = (sys[0][3] - sys[0][1] * rj[1] - sys[0][2] * rj[2]) / sys[0][0]

# Formula opceg rjesenja
a = int(sum([mul(i, n) * rj[i] * (nul[i] ** n) for i in range(3)]))

print("Vrijednost n-tog clana niza pomocu formule: " + str(a))

''' 2. Odredivanje pripadne homogene rekurzivne relacije i racunanje clana niza rekurzivno '''

# Pronaci koeficijente uz x za izraz (x - x_0)(x - x_1)(x - x_2)
koef = [sum(nul), -(nul[0] * nul[1] + nul[0] * nul[2] + nul[1] * nul[2]), nul[0] * nul[1] * nul[2]]

def rek(i):
    global cache, koef
    
    if i >= len(cache):
        # Rekurzivna relacija
        cache.append(sum([koef[j] * rek(i - 1 - j) for j in range(3)]))

    # Optimizacija: pamcenje nizih vrijednosti (dinamika / cache)
    return cache[i]

print("Vrijednost n-tog clana niza iz rekurzije: " + str(rek(n)))