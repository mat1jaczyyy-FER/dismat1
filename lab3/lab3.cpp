// Compiler flags: g++ -O3 -Wall
// Na Windowsima koristiti mingw-w64: http://mingw-w64.org/doku.php/download/mingw-builds
// Na 64-bitnom sustavu obavezno dodati flag -m64

// Dominik Matijaca 0036524568

#include <cstdio>
#include <cstring>
#include <functional>

#define max(k, l) (((k) > (l))? (k) : (l))
#define min(k, l) (((k) < (l))? (k) : (l))

// Tipovi podataka
typedef                 char  s8;
typedef  unsigned       char  u8;
typedef  unsigned      short  u16;
typedef                  int  s32;
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
} edge;

// Coloring struktura predstavlja pojedinu konfiguraciju boja za graf
typedef struct {
    u64 _; // Bitfield od 64 bita, svaki 4-bitni skup označava boju (n <= 16 čvorova, time i jedinstvenih boja)
    
    // Dohvati boju čvora
    inline u8 get(u8 i) const {
        return (_ >> ((i & 0x0F) << 2)) & 0x0F;
    }
    
    // Postavi boju čvora
    inline void set(u8 i, u8 c) {
        i = (i & 0x0F) << 2;
        _ ^= (((_ >> i) ^ c) & 0x0F) << i;
    }
} coloring;

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
    
    // Graf s 0 čvorova ne treba bojati
    if (n == 0) {
        printf("0\n");
        return 0;
    }
    
    coloring a = {};  // Trenutna konfiguracija boja grafa
    u8 c;             // Kromatski broj
    
    // Rekurzija po svim jedinstvenim konfiguracijama boja grafa
    // x => najviša iskoristiva boja, i => čvor za koji iteriramo boju
    std::function<bool(u8, u8)> search = [&n, &e, &a, &c, &search](u8 x, u8 i) {
        // Ako smo pronašli niz boja koji ispunjava sve uvjete, gotovi smo
        if (i == n) return true;

        // Sve moguće boje, ovako se garantira jedinstvenost oblika
        for (u8 j = 0; j <= min(x, c - 1); j++) {
            // Provjeri ima li neki susjedni čvor već tu boju
            for (u8 k = 0; k < i; k++)  // Samo čvorovi indeksa do i imaju boju, ostali nisu postavljeni
                if (e.get((i << 4) + k) && a.get(k) == j)  // Ako postoji brid i boja bi bila jednaka
                    goto next;  // Pokušaj sljedeću boju

            // Nema konflikta s susjednim čvorovima, postavi boju
            a.set(i, j);
            
            // Odredi boju sljedećeg čvora, prekini ako je rekurzija pronašla rješenje
            if (search(max(x, j + 1), i + 1))
                return true;
            
            next:;
        }
        
        // Došli smo do kraja ove grane rekurzije bez pronađenog rješenja
        return false;
    };
    
    // Provjeravaj kromatske brojeve od 1 uzlazno do n
    for (c = 1; c < n; c++)        // Ne treba provjeravati za c = n, jer je to uvijek moguće
        if (search(1, 1)) break;   // Prekinuti kada pronađe valjano bojanje

    // Ispis kromatskog broja
    printf("%d\n", c);

    // Debug ispis pronađene konfiguracije boja (isključeno po defaultu)
    #ifdef DEBUG_COLORING
        for (u8 j = 0; j < n; j++)
            printf("%d ", a.get(j));

        printf("\n");
    #endif
    
    return 0;
}