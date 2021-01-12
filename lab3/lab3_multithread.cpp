#include <cstdio>
#include <cstring>
#include <functional>
#include <thread>

#define DEBUG_THREADS
#define THREADS 12

#define max(k, l) (((k) > (l))? (k) : (l))
#define min(k, l) (((k) < (l))? (k) : (l))

typedef                 char  s8;
typedef  unsigned       char  u8;
typedef  unsigned      short  u16;
typedef                  int  s32;
typedef  unsigned  long long  u64;

typedef struct {
    u64 _[64 * 64 / 64];

    inline bool get(u8 i, u8 j) const {
        return (_[i & 0x3F] >> (j & 0x3F)) & 1;
    }

    inline void set(u8 i, u8 j) {
        _[i & 0x3F] |= 1LL << (j & 0x3F);
    }
} edge;

typedef struct {
    u64 _[64 / 10 + 1];
    
    inline u8 get(u8 i) const {
        return (_[i / 10] >> ((i % 10) * 6)) & 0x3F;
    }
    
    inline void set(u8 i, u8 c) {
        u8 h = (i % 10) * 6;
        _[i / 10] ^= (((_[i / 10] >> h) ^ c) & 0x3F) << h;
    }
} coloring;

s32 main() {
    u8 n;
    edge e = {};

    #define MAXFN 200
    s8 fn[MAXFN];
    printf("Unesite ime datoteke: ");
    fgets(fn, MAXFN, stdin);

    s8* nl;
    if ((nl = strchr(fn, '\n')) != NULL)
        *nl = '\0';

    FILE* f = fopen(fn, "r");
    if (f == NULL) return 1;

    fscanf(f, "%hhu", &n);

    for (u8 i = 0; i < n; i++) {
        for (u8 j = 0; j < n; j++) {
            u8 in;
            fscanf(f, "%hhu", &in);

            if (in)
                e.set(i, j);
        }
    }
    
    fclose(f);
    
    if (n == 0) {
        printf("0\n");
        return 0;
    }

    coloring* _a = (coloring*)malloc((n - 1) * sizeof(coloring));
    memset(_a, 0, (n - 1) * sizeof(coloring));

    u8 best_sol = 66;
    
    std::thread* th[THREADS];
    bool s[n - 1] = {};

    for (u8 i = 0; i < THREADS; i++)
        th[i] = new std::thread([&n, &e, &_a, &s, &best_sol](u8 t) {
            #ifdef DEBUG_THREADS
                printf("thread %d started\n", t);
            #endif

            for (; t < n; t += THREADS) {
                if (best_sol < t) break;

                coloring& a = _a[t - 1] = {};

                std::function<bool(u8, u8)> search = [&n, &e, &a, &t, &search, &best_sol](u8 x, u8 i) {
                    if (i == n) return true;

                    for (u8 j = 0; j <= min(x, t - 1); j++) {
                        for (u8 k = 0; k < i; k++)
                            if (e.get(i, k) && a.get(k) == j)
                                goto next;

                        a.set(i, j);
                        
                        if (search(max(x, j + 1), i + 1))
                            return true;
                        
                        next:;
                    }
                    
                    return false;
                };

                if (search(1, 1)) {
                    best_sol = min(best_sol, t);

                    #ifdef DEBUG_THREADS
                        printf("found best_sol %d\n", best_sol);
                    #endif

                    break;
                }
                
                #ifdef DEBUG_THREADS
                    printf("thread %d switched to %d\n", t, t + THREADS);
                #endif
            }

            #ifdef DEBUG_THREADS
                printf("thread %d completed\n", t);
            #endif

        }, i + 1);
    
    for (u8 i = 0; i < THREADS; i++) {
        if (th[i]->joinable()) {
            #ifdef DEBUG_THREADS
                printf("main joining %d\n", i);
            #endif

            th[i]->join();
        }
        
        delete th[i];
    }  

    printf("%d\n", best_sol);
    for (u8 j = 0; j < n; j++) printf("%d ", _a[best_sol - 1].get(j));
    printf("\n");
    
    return 0;
}