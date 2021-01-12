## Rješenja primjera

Bilo bi dobro da tijekom razvoja labosa (dakle, svo vrijeme prije predaje) ispisujete bojanje i provjeravate, pogotovo kad debuggirate logičke pogreške i pogrešne ispise. Služite se skriptom `verify.py` za brzu provjeru točnosti bojanja rješenja.

### `pdf.txt`

Osnovan primjer ulaza programa iz teksta laboratorijske vježbe.

```
Kromatski broj: 3
Primjer bojanja: 0 1 0 1 1 0 1 2
Vrijeme: 0.047s
```

### `15.txt`

Slučajno izgeneriran graf sa 15 čvorova. Vjerojatno nekakav najkompliciraniji primjer koji će se pojaviti na provjeri na Moodleu.

```
Kromatski broj: 6
Primjer bojanja: 0 0 1 1 2 3 2 4 5 5 5 0 4 3 1
Vrijeme: 0.062s
```

### `k15.txt`

K15 graf sa šesnaestim čvorom povezanim za prvog. Dobar algoritam bi trebao gotovo instantno prožvakati ovaj primjer.

```
Kromatski broj: 15
Primjer bojanja: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 1
Vrijeme: 0.063s
```

Sljedeći primjeri imaju `n > 15`, tako da se nemojte pretjerano brinuti ako je vrijeme izvršavanja sporo što se tiče predaje labosa.

### `20.txt`

Slučajno izgeneriran graf sa 20 čvorova. Nije preteško postići dobru brzinu izvršavanja, ali na ovom primjeru se mogu javiti pogreške u logici.

```
Kromatski broj: 5
Primjer bojanja: 0 1 1 2 0 1 3 0 1 0 0 4 2 2 4 3 4 3 1 2
Vrijeme: 0.066s
```

### `30.txt`

Slučajno izgeneriran graf sa 30 čvorova. Naivno rješenje koje sprema graf pomoću matrica i dalje će ovo izračunati iako sporo, dok će rješenje preko bitfieldova instantno prožvakati ovaj primjer.

```
Kromatski broj: 7
Primjer bojanja: 0 1 2 0 0 3 1 0 1 2 2 4 1 5 2 5 6 5 3 6 4 3 2 4 5 0 3 3 6 3
Vrijeme: 0.074s
```

### `37.txt`

Slučajno izgeneriran graf sa 37 čvorova. Ovaj primjer će i efikasna rješenja koja spremaju graf pomoću bitfieldova dovesti na koljena, trebati će nekoliko sekundi za provrtiti ovaj primjer. Ovo je poprilično gust graf, korištena je vjerojatnost brida `0.7`.

```
Kromatski broj: 12
Primjer bojanja: 0 0 1 2 3 4 2 5 2 5 1 4 6 0 7 8 3 3 4 8 9 10 7 11 3 1 10 6 10 5 11 9 11 3 5 7 9
Vrijeme: 8.948s
```

### `64.txt`

Slučajno izgeneriran graf sa 64 čvora. Ovo je poprilično rijedak graf, korištena je vjerojatnost brida `0.05`, pa je taj primjer više tu da memorijski pogura količinu podataka, dok se zapravo rješenje nalazi relativno brzo kako nema mnogo bridova u grafu.

```
Kromatski broj: 3
Primjer bojanja: 0 1 0 0 0 1 0 0 0 0 0 1 1 1 0 2 1 0 2 0 0 0 0 0 1 2 2 0 2 0 1 1 1 1 0 1 1 0 2 0 1 1 2 1 2 1 1 1 2 1 2 2 1 1 1 2 0 1 1 1 1 2 2 0
Vrijeme: 2.591s
```

<sup><sub>Napomena: sva vremena su izvršena na AMD Ryzen 7 2700, sa 64-bitnim g++ compilerom i single-threadanim rješenjem</sub></sup>

## Generator grafova

Generator grafova `generate.py` uzima na unos vjerojatnost da će neki brid grafa postojati i broj čvorova u grafu. Ispisati će slučajno generiranu matricu susjedstva takvog grafa, koji možete provući kroz svoje programsko rješenje kao test.

Provjerite vaše rezultate (bojanja) pomoću `verify.py`, koji na ulaz traži datoteku grafa (u istom obliku kao i za labos), nakon čega na standardni ulaz dolazi bojanje u istom obliku kao i u gore navedenim rješenjima (razmakom odvojeni brojevi koji predstavljaju boje).

**NIJE** zagarantirano da će slučajno generirani graf biti povezan!

<sup><sub>Sve Python skripte koriste Python 3, ne Python 2</sub></sup>