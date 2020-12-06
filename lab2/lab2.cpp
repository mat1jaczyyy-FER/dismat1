// Compiler flags: g++ -m64 -O3 -std=c++17 -Wall
// Dominik Matijaca 0036524568

#include <cstdio>
#include <functional>
#include <string>
#include <thread>

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
    
    /* 1. korak - brisanje mostova iz grafa
        Bilo koji ciklus nikada neće uključivati bridove koji su mostovi
        Stoga za ubrzanje algoritma, isplati ih se prvo pronaći i izbrisati
        Svakim obrisanim mostom, graf će se razdvojiti na 2 nepovezana dijela
        Svaki dio sad nepovezanog grafa imat će vlastiti broj čvorova...
        ...a time i maksimalnu moguću veličinu ciklusa
    */
    
    // Najveći mogući ciklus koji se može pojaviti ako počinjemo iz nekog čvora grafa
    u8 m[16] = {};   // Razbijanjem mostova, početni čvorovi bit će u odvojenim grafovima manje veličine
    memset(m, n, n); // Na početku, to je broj čvorova u grafu
    
    for (u8 i = 0; i < n; i++) {
    	for (u8 j = i + 1; j < n; j++) {
    	    u8 x = (i << 4) + j;
    	    if (!e.get(x)) continue; // Za svaki postojeći brid
    
            // Odspojimo brid
            u8 xm = (j << 4) + i;
            e.unset(x);
            e.unset(xm);
            
            u8 c; // Ako je brid most, ovdje će ostati broj čvorova na jednoj odpovezanoj strani grafa
            
            // BFS funkcija će pokušati pronaći alternativni put za brid koji smo odspojili
            auto bfs = [&e, &c](const u8 i, const u8 j) {  // Tražimo put od i do j
            	u16 v = 0;       // Bitfield - posjećenost čvorova (visited nodes)
	            u16 q = 1 << i;  // Bitfield - čvorovi koje treba posjetiti u sljedećoj iteraciji (queue)
	            c = 0;           // Resetirati broj posjećenih čvorova na 0
	            
	            // Dok god možemo posjetiti neki čvor (počevši od i)
	            while (q) {
	            	u16 qc = q; // Prekopirati queue
	                q = 0;      // Resetirati queue za pamćenje sljedeće iteracije
	                u8 k = 0;   // Čvor koji posjećujemo
	                
	                // Iteracija po queueu - posjećivanje čvorova (dok god ima bitova)
	                while (qc) {
		                // Pronaći sljedeći postavljeni bit, pozicija k je čvor
	                    while (!(qc & 1)) {
	                    	qc >>= 1;
	                        k++;
	                    }
	                    
	                    // Dohvati čvorove na koje je povezan (next nodes)
	                    u16 nn = e.get_n(k);
	             
	                    if ((nn >> j) & 1) // Alternativni put je pronađen i ne treba mijenjati ništa
	                        return (u16)0; // Zato vraćamo 0
	                    
	                    // Označi da je čvor istražen, i dodaj susjede u queue
	                    v |= 1 << k;
	                    q |= nn;
	                    c++;
	                    
	                    // Sljedeći bit
	                    qc >>= 1;
	                    k++;
	                }
	                
	                q &= ~v; // Izbriši iz queuea sve čvorove koji su već posjećeni
	            }
	            
	            // Nije pronađen put. To znači da smo dobili nepovezanost
	            return v; // Vraćamo bitfield s označenim čvorovima u tom dijelu nepovezanog grafa
            };
            
            // Ispitaj je li brid bio most
            u16 split = bfs(i, j);
            
            if (split) { // Ako nismo pronašli alternativan put, brid je bio most
            	// Funkcija adjust_m ažurirat će podatke u m o veličini dijela nepovezanog grafa
                auto adjust_m = [&m, &split, &c]() {
            	    u8 k = 0;
                    
	            	while (split) {
	            	    if (split & 1) // Za svaki čvor
	                        m[k] = c;  // Postavi veličinu
	            
	                    split >>= 1;
	                    k++;
	                }
                };
                
                adjust_m(); // Ažuriraj za dio uz i
            
                // Potraži sve čvorove u suprotnom dijelu
            	split = bfs(j, i);
                adjust_m(); // Ažuriraj za dio uz j
                
            } else {  // Brid nije bio most, ponovno ga povezati
                e.set(x);
                e.set(xm);
            }
        }
    }
    
    // Općenito najveći mogući ciklus za cijeli graf
    u8 b = 0;
    for (u8 i = 0; i < n; i++)
        umax(b, m[i]);

    /* 2. korak - Aktivno traženje najvećeg mogućeg ciklusa
        Počevši od svakog čvora grafa, DFS algoritmom rekurzivno šetamo po
        svim mogućim ciklusima koji započinju (time i završavaju) tim čvorom,
        tražeći najvećeg. Obzirom da pretragom ne mijenjamo stanje grafa,
        idealno je upotrijebiti po jedan thread izvršavanja za svaku početnu točku
        (do 16 threadova). Time se puno bolje iskorištava moć modernih procesora
    */
    
    std::thread th[16];  // Dretve za svaku početnu točku
    s8 r[16] = {};       // Rješenje za svaku početnu točku
    u8 abort_all = 0;    // Ako neki thread dođe do optimalnog rješenja b, signalizira prekid
    
    for (int i = 0; i < 16; i++) { // Početni čvor dretve i
        // Pokreni thread
		th[i] = std::thread([&e, &m, &b, &r, &abort_all](const u8 t) {
			if (m[t] < 3) return; // Graf s manje od 3 čvora ne može imati ciklus
			
            path p = {};  // Pamćenje puta (bez poretka - važno je izbjegavanje čvorova koje smo prošli)
            u8 abort = 0; // Ako thread dođe do optimalnog rješenja za podgraf m[t], signalizira prekid
			
            // Rekurzivna DFS funkcija koja šeta prema svim čvorovima 
            std::function<void(u8)> dfs = [&e, &m, &b, &r, &p, &t, &abort_all, &abort, &dfs](u8 x) {
		        if (abort_all || abort) return; // Ako je signaliziran prekid, obustavljamo pretragu
		
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
		            if (i == t && p.size >= 3 && umax(r[t], p.size) == m[t]) { // Spremi najbolji rezultat
		                abort = 1;  // Ako je bio jednak m[t], već je idealan za podgraf, prekinuti
		                
		                if (r[t] == b)     // Ako je uz to idealan za cijeli graf
                            abort_all = 1; // Prekinuti sve
		
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
		
            // Pokrenuti DFS koji traži cikluse za početni čvor
		    dfs(t);
        }, i); 
	}
	
    // Konačno rješenje zadatka - duljina najvećeg pronađenog ciklusa
	s8 d = 0;

	for (int i = 0; i < 16; i++) {
        // Pričekaj da thread i završi s radom
		if (th[i].joinable())
            th[i].join();
        
        // Spremi bolje rješenje
        umax(d, r[i]);
	}

    // Ispis duljine najvećeg ciklusa
    printf("%d\n", d);

    return 0;  
}