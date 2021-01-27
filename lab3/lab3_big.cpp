#include <cstdio>
#include <cstring>
#include <functional>

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
    u8 _[64];
    
    inline u8 get(u8 i) const {
        return _[i & 0x3F];
    }
    
    inline void set(u8 i, u8 c) {
        _[i & 0x3F] = c;
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
    
    coloring a = {};
    u8 c;
    
    std::function<bool(u8, u8)> search = [&n, &e, &a, &c, &search](u8 x, u8 i) {
        if (i == n) return true;

        for (u8 j = 0; j <= min(x, c - 1); j++) {
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
    
    for (c = 1; c < n; c++)
        if (search(1, 1)) break;

    printf("%d\n", c);

    #ifdef DEBUG_COLORING
        for (u8 j = 0; j < n; j++)
            printf("%d ", a.get(j));

        printf("\n");
    #endif
    
    return 0;
}
