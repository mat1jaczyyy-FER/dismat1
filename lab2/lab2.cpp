#include <cstdio>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

#define MAXN 16
#define max(a, b) ((a > b)? a : b)

int t;
int r = 0;
std::unordered_set<int> s;
std::stack<int> st;
std::vector<int> e[MAXN];

void dfs(int x) {
    st.push(x);
    s.insert(x);

    for (int i = 0; i < e[x].size(); i++) {
        if (e[x][i] == t && st.size() >= 3)
            r = max(st.size(), r);
        
        if (!s.count(e[x][i]))
            dfs(e[x][i]);
    }

    s.erase(x);
    st.pop();
}

int main() {
    #define MAXFN 200
    char fn[MAXFN];
    printf("Unesite ime datoteke: ");
    fgets(fn, MAXFN, stdin);

    char* p;
    if ((p = strchr(fn, '\n')) != NULL)
        *p = '\0';

    FILE* f = fopen(fn, "r");
    if (!f) return 1;

    int n;
    fscanf(f, "%d", &n);

    int temp;

    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
        fscanf(f, "%d", &temp);
        if (temp) e[i].push_back(j);
    }

    for (int i = 0; i < n; i++) dfs(t = i);
    printf("%d\n", r);

    return 0;  
}