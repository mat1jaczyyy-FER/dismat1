// Compiler flags: g++ -O3
// Na 64-bitnom sustavu dodati flag -m64 za ubrzanje (upotreba 64-bitnih registara na procesoru)

// Dominik Matijaca 0036524568

#include <cstdio>
#include <cstring>
#include <functional>
#include <string>

// U k spremi max od k, l
#define umax(k, l) (k = ((k) > (l))? (k) : (l))

// Tipovi podataka
typedef                 char  s8;
typedef  unsigned       char  u8;
typedef  unsigned      short  u16;
typedef                  int  s32;
typedef  unsigned        int  u32;
typedef  unsigned  long long  u64;

// Edge struktura predstavlja uneseni graf, podržano n <= 16 čvorova
typedef struct {
    u64 _[4]; // Bitfield od 256 bitova, svaki bit označava postojanje brida (16x16 matrica susjedstva)
    
    // Provjeri postojanje brida
    inline bool get(u8 i) const {
        return (_[i >> 6] >> (i & 0x3F)) & 1;
    }

    // Poveži brid
    inline void set(u8 i) {
        _[i >> 6] |= 1LL << (i & 0x3F);
    }
    
    // Odspoji brid
    inline void unset(u8 i) {
        _[i >> 6] &= ~(1LL << (i & 0x3F));
    }
    
    // Dohvati sve bridove za neki čvor
    inline u16 get_n(u8 i) const {
        return _[i >> 2] >> ((i & 0x03) << 4);
    }
} edge;

// Path struktura predstavlja čvorove koje smo prošli na određenom putu
typedef struct {
    u16 _;   // Bitfield od 16 bitova, svaki bit označava jesmo li prošli po čvoru
    u8 size; // Koliko čvorova smo prošli na putu

    // Provjeri jesmo li prošli po čvoru
    inline bool get(u8 i) const {
        return (_ >> i) & 1;
    }

    // Postavi da smo prošli po čvoru
    inline void set(u8 i) {
        _ |= 1 << i;
        size++;
    }

    // Vratili smo se natrag ovim čvorom
    inline void unset(u8 i) {
        _ &= ~(1 << i);
        size--;
    }
} path;

// Main funkcija
s32 main() {
    u8 n;         // Broj čvorova u grafu
    edge e = {};  // Definicija grafa (bridova)

    // Unos imena datoteke
    #define MAXFN 200
    s8 fn[MAXFN];
    printf("Unesite ime datoteke: ");
    fgets(fn, MAXFN, stdin);

    s8* nl; // Izbaciti novi red s kraja imena datoteke
    if ((nl = strchr(fn, '\n')) != NULL)
        *nl = '\0';

    // Otvori datoteku za čitanje
    FILE* f = fopen(fn, "r");
    if (f == NULL) return 1;

    // Unos podataka
    fscanf(f, "%hhu", &n);

    for (u8 i = 0; i < n; i++) {
        for (u8 j = 0; j < n; j++) {
            u8 in;
            fscanf(f, "%hhu", &in);

            if (in)
                e.set((i << 4) + j);  // Postavljanje bridova grafa
        }
    }
    
    fclose(f);
 
    /* Aktivno traženje najvećeg mogućeg ciklusa
        Počevši od svakog čvora grafa, DFS algoritmom rekurzivno šetamo po
        svim mogućim ciklusima koji započinju (time i završavaju) tim čvorom,
        tražeći najvećeg.
    */
   
    s8 r = 0;     // Krajnje rješenje
    path p = {};  // Pamćenje puta (bez poretka - važno je samo izbjegavanje čvorova koje smo prošli)
    u8 t;         // Trenutna početna točka
    u8 abort = 0; // Ako dođe do optimalnog rješenja ciklusa veličine n, signalizira prekid

    // Rekurzivna DFS funkcija
    std::function<void(u8)> dfs = [&n, &e, &r, &p, &t, &abort, &dfs](const u8 x) {
        if (abort) return; // Ako je signaliziran prekid, obustavljamo pretragu

        p.set(x); // Prošli smo kroz čvor x

        // Dohvati čvorove na koje je povezan (next nodes)
        u16 nn = e.get_n(x);
        u8 i = 0;

        // Iteracija po susjednim čvorovima
        while (nn) { 
            // Pronaći sljedeći postavljeni bit, pozicija i je čvor
            while (!(nn & 1)) {
                nn >>= 1;
                i++;
            }
            
            // Ako je sljedeći čvor početni, i prošli smo već bar 3 čvora, imamo ciklus p.size
            if (i == t && p.size >= 3 && umax(r, p.size) == n) { // Spremi najbolji rezultat
                abort = 1;  // Ako je jednak broju točaka u grafu, naišli smo na max ciklus
                return;
            }
            
            // Ako nismo već prošli tim čvorom, šetamo se po njemu rekurzivno u dubinu
            if (!p.get(i))
                dfs(i);
                
            // Sljedeći bit
            nn >>= 1;
            i++;
        }

        // Returnamo unatrag iz čvora x, pa pišemo da nam više nije na putu
        p.unset(x);
    };

    for (t = 0; t < 16; t++)  // Pokrenuti DFS koji traži cikluse za početni čvor
        dfs(t);

    // Ispis duljine najvećeg ciklusa
    printf("%d\n", r);

    return 0;  
}