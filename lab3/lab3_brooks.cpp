#include <cstdio>
#include <cstring>
#include <functional>
#include <vector>

#pragma GCC target("popcnt")
#include <bits/stdc++.h>

#define max(k, l) (((k) > (l))? (k) : (l))
#define min(k, l) (((k) < (l))? (k) : (l))

typedef                 char  s8;
typedef  unsigned       char  u8;
typedef  unsigned      short  u16;
typedef                  int  s32;
typedef  unsigned  long long  u64;

typedef struct {
    u64 _[64];

    inline bool get(u8 i, u8 j) const {
        return (_[i & 0x3F] >> (j & 0x3F)) & 1;
    }

    inline void set(u8 i, u8 j) {
        _[i & 0x3F] |= 1LL << (j & 0x3F);
    }

    inline u64 get_n(u8 i) const {
        return _[i & 0x3F];
    }

    inline void set_n(u8 i, u64 x) {
        _[i & 0x3F] = x;
    }

    inline void del_col(u8 i, u8 j) {
        _[i & 0x3F] = ((_[i & 0x3F] >> (j + 1)) << j) | (_[i & 0x3F] & (j? (-1ULL) >> (64 - j) : 0));
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

typedef struct {
    u64 nodes;
    u8 n;
    u8 upper;
    edge e;
} component;

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

    u64 all = 0;
    std::vector<component> components;
    u8 lower = 1;

    for (u8 i = 0; i < n; i++) {
        if (all >> i & 1) continue;

        components.push_back({});
        component& comp = components[components.size() - 1];

        auto bfs = [&e, &lower, &all, &comp](const u8 i) {
            u64 q = 1ULL << i;

            s8 deg = -1;
            u8 maxdeg = 1;
            
            while (q) {
                u64 qc = q;
                q = 0;
                
                for (u8 k = 0; qc; k++, qc >>= 1) {
                    if (!(qc & 1)) continue;

                    comp.nodes |= 1ULL << k;
                    all |= 1ULL << k;
                    comp.n++;

                    u64 nn = e.get_n(k);
                    q |= nn;

                    u8 c = __builtin_popcountll(nn);
                    maxdeg = max(maxdeg, c);
                    
                    if (deg == -1) deg = c;
                    else if (deg != c) deg = -2;
                }

                q &= ~comp.nodes;
            }

            if (deg == 2 || deg == comp.n - 1)
                lower = max(deg + 1, lower);
            
            else comp.upper = maxdeg;
        };
        
        bfs(i);

        u64 nodes = comp.nodes << (64 - n);
        
        for (u8 j = 0, k = 0; comp.nodes; k++, comp.nodes >>= 1)
            if (comp.nodes & 1)
                comp.e.set_n(j++, e.get_n(k));
        
        for (s8 k = n - 1; k >= 0; k--, nodes <<= 1)
            if (!(nodes & (1ULL << 63)))
                for (u8 j = 0; j < comp.n; j++)
                    comp.e.del_col(j, k);
    }

    coloring a = {};

    for (u8 i = 0; i < components.size(); i++) {
        component& comp = components[i];
        
        if (comp.upper <= lower) continue;

        while (lower != comp.upper) {
            u8 mid = (lower + comp.upper) / 2;

            std::function<bool(u8, u8)> search = [&comp, &a, &mid, &search](u8 x, u8 i) {
                if (i == comp.n) return true;

                for (u8 j = 0; j <= min(x, mid - 1); j++) {
                    for (u8 k = 0; k < i; k++)
                        if (comp.e.get(i, k) && a.get(k) == j)
                            goto next;

                    a.set(i, j);
                    
                    if (search(max(x, j + 1), i + 1))
                        return true;
                    
                    next:;
                }
                
                return false;
            };

            if (search(1, 1)) comp.upper = mid;
            else lower = mid + 1;
        }
    }

    printf("%d\n", lower);

    return 0;
}