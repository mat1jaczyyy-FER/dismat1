#include <cstdio>
#include <functional>
#include <string>

#define max(a, b) ((a > b)? a : b)

typedef                 char  s8;
typedef  unsigned       char  u8;
typedef  unsigned      short  u16;
typedef                  int  s32;
typedef  unsigned        int  u32;
typedef  unsigned  long long  u64;

typedef struct {
    u64 _[4];

    inline void set(u8 i) {
        _[i >> 6] |= 1LL << (i & 0x3F);
    }
    
    inline u16 get_n(u8 i) {
        return _[i >> 2] >> ((i & 0x03) << 4);
    }
} edge;

typedef struct {
    u16 _;
    u8 size;

    inline bool contains(u8 i) {
        return (_ >> i) & 1;
    }

    inline void set(u8 i) {
        _ |= 1 << i;
        size++;
    }

    inline void unset(u8 i) {
        _ &= ~(1 << i);
        size--;
    }
} path;

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

    path p = {};

    for (u8 i = 0; i < n; i++) {
        u8 bits = 0;

        for (u8 j = 0; j < n; j++) {
            u8 in;
            fscanf(f, "%hhu", &in);

            if (in) {
                e.set((i << 4) + j);
                bits++;
            }
        }

        if (bits < 2)
            p.set(i);
    }

    fclose(f);

    const u8 b = p.size;
    s8 r = 0;
    u8 t = 0;
    u8 abort = 0;

    std::function<void(u8)> dfs = [&n, &e, &b, &r, &p, &t, &abort, &dfs](u8 x) {
        if (abort) return;

        p.set(x);

        u16 a = e.get_n(x);
        u8 i = 0;

        while (a) {
            while (!(a & 1)) {
                a >>= 1;
                i++;
            }
            
            if (i == t && p.size - b >= 3 && (r = max(p.size, r)) == n - b) {
                abort = 1;
                return;
            }
            
            if (!p.contains(i))
                dfs(i);
                
            a >>= 1;
            i++;
        }

        p.unset(x);
    };

    for (; t < 16; t++) dfs(t);

    printf("%d\n", r - b);

    return 0;  
}