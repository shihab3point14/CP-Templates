#include"bits/stdc++.h"
#include <typeindex>
using namespace std;

#define YES cout<<"YES\n"
#define NO cout<<"NO\n"
#define endl "\n"
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define fast_io ios_base::sync_with_stdio(false),cin.tie(NULL),cout.tie(NULL)

string t_case() { static int tc; return "Case " + to_string(++tc) + ':'; }
 
#define TEM template <class... T>
TEM istream& operator>>(istream& in, pair<T...>& p) { return in >> p.first >> p.second; }
TEM ostream& operator<<(ostream& out, const pair<T...>& p) { return out << '(' << p.first << ", " << p.second << ')'; }
#define def_in(cont) TEM istream& operator>>(istream& in, cont<T...>& A) { for (auto& a : A) in >> a; return in; }
#define def_out(cont) TEM ostream& operator<<(ostream& out, const cont<T...>& A) { int i = 0; auto it = A.begin(); while (it != A.end()) out << &" "[!i++] << *it++; return out; }
def_in(vector) def_in(deque) def_out(vector) def_out(deque) def_out(set) def_out(map) def_out(multiset)
 
void dbg_out() { cerr << endl; }
template <typename Head, typename... Tail>
void dbg_out(Head H, Tail... T){cerr << ' ' << H; dbg_out(T...);}
#define dbg(...) cerr <<  "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)

// find_by_order(k): Returns the k-th smallest element (0-indexed).
// order_of_key(x): Returns how many elements are strictly smaller than x.
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
TEM using ordered_set = tree<T..., null_type, less<T...>, rb_tree_tag, tree_order_statistics_node_update>;
def_out(ordered_set)
 
TEM istream& c_in(T&... args) { return ((cin >> args), ...); }
TEM ostream& c_out(const T&... args) { int i = 0; return ((cout << &" "[!i++] << args), ...) << '\n'; }
ostream& c_out(bool b) { return c_out(b ? "YES" : "NO"); }
 
#ifdef DEBUG
TEM ostream& c_err(const T&... args) { int i = 0; return ((cerr << &" "[!i++] << args), ...) << '\n'; }
#else
#define c_err(...)
#endif
#define d_bug(args...) c_err(#args, '=', args)

typedef long long ll;
typedef unsigned long long ull;
#define int ll

const int MOD = 1000000007;
const int inf = 1e18;


void shihab(){
   
}   

int32_t main(){

    fast_io;
 
    int tc;cin >> tc;
    int test_case = 1;

    while(tc--){
        // cout<<"Case "<< test_case++ <<": ";
        shihab();
    }
    return 0;
}