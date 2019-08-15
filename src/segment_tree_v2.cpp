#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

namespace lly {
// 所有下标符合C++风格
// 奢侈做法：使用全局变量开4倍数组。
// 考场抄代码所有item_type, sum_type 换成ll即可
// 支持区间和查询(%mod) 区间集体乘k， 区间集体加k
// kMaxItemSize是最大的原始数据数组的大小
// 区间加 下传所有路径上的mlt标记
// 区间乘 下传所有路径上的mlt标记和add标记
// 注意考场上抄代码敲完一个函数，别的函数可以复制然后粘贴替换，别少替换了。
struct segment_tree {
  // 最大的初始数组大小和响应的线段树节点数组最大大小。
  const static int kMaxItemSize = 100000; // 可手动更换
  const static int kMaxSegTreeSize = kMaxItemSize << 2;

  ll mod;

  int item_sz;
  int seg_sz;
  int &n = item_sz;
  int &stn = seg_sz;

  typedef ll item_type;
  typedef ll sum_type;

  struct nd {
    int l, r;
    sum_type sm;  // sum flag
    // lazy flags
    item_type all_add;  // lazy标记，表示区间内的数都要加上的数
    sum_type all_mlt;   // lazy标记，表示区间内的数都要乘以的数
    // 运算顺序，先乘后加，即先乘以all_mlt再加上all_add；先儿子运算，后父亲运算。
    // 因此区间×k操作，需要把all_add标记乘以k

    // other flags
    inline int mid() { return l + (r - l) / 2; }
    inline void set_basic(int l, int r) {
      this->l = l;
      this->r = r;
      all_add = 0;
      all_mlt = 1;
    }
    inline void add(item_type a, ll mod) {
      a %= mod;
      all_add = (all_add + a) % mod;
      sm = (sm + (sum_type)(r - l) * a) % mod;
    }
    inline void mlt(item_type m, ll mod) {
      m %= mod;
      all_add = (all_add * (sum_type)m) % mod;
      all_mlt = (all_mlt * m) % mod;
      sm = (sm * m) % mod;
    }
  };

  item_type a[kMaxItemSize];
  nd nds[kMaxSegTreeSize];

  void init() {  // 屏幕输入数组a版本,需要先确定n
    for (int i = 0; i < n; ++i) scanf("%lld", a + i);  // cin >> a[i];
    seg_sz = (2 << (int)(ceil(log2(item_sz)))) - 1;
  }

  void init(item_type src[], int cnt) {  // 内存数组输入数组a版本
    n = cnt;
    for (int i = 0; i < n; ++i) a[i] = src[i];
    seg_sz = (2 << (int)(ceil(log2(item_sz)))) - 1;
  }

  inline int parent(int x) { return (x - 1) >> 1; }
  inline int lchild(int x) { return (x << 1) | 1; }
  inline int rchild(int x) { return (x << 1) + 2; }

  inline void build() { build(0, n, 0); }

  inline void set_flags(int root, int i) {  // nds[root]用a[i]设置各类标志
    auto &p = nds[root];
    p.sm = a[i];
  }

  inline void merge_flags(int root) {
    auto &p = nds[root];
    auto &l = nds[lchild(root)];
    auto &r = nds[rchild(root)];
    p.sm = (l.sm + r.sm) % mod * (p.all_mlt) % mod +
           ((sum_type)(p.r - p.l) * p.all_add) % mod;
    p.sm %= mod;
  }

  inline void down_mlt_flag(int root) {
    auto &p = nds[root];
    auto &l = nds[lchild(root)];
    auto &r = nds[rchild(root)];
    l.mlt(p.all_mlt, mod);
    r.mlt(p.all_mlt, mod);
    p.all_mlt = 1;
  }

  inline void down_flags(int root) {  // mlt and add
    auto &p = nds[root];
    auto &l = nds[lchild(root)];
    auto &r = nds[rchild(root)];
    l.mlt(p.all_mlt, mod);
    r.mlt(p.all_mlt, mod);
    p.all_mlt = 1;
    l.add(p.all_add, mod);
    r.add(p.all_add, mod);
    p.all_add = 0;
  }

  void build(int l, int r, int root) {
    nds[root].set_basic(l, r);
    if (l + 1 == r) {
      set_flags(root, l);
      return;
    }
    int m = l + (r - l) / 2;
    build(l, m, lchild(root));
    build(m, r, rchild(root));
    merge_flags(root);
  }

  // [l,r)区间的数都加上val
  void add(int l, int r, item_type val, int root = 0) {
    if (l == nds[root].l && r == nds[root].r) {
      nds[root].add(val, mod);
      return;
    }
    down_mlt_flag(root);
    int m = nds[root].mid();
    if (r <= m) {  // only left part
      add(l, r, val, lchild(root));
    } else if (l >= m) {  // only right part
      add(l, r, val, rchild(root));
    } else {
      add(l, m, val, lchild(root));
      add(m, r, val, rchild(root));
    }
    merge_flags(root);
  }

  // [l,r)区间的数都乘以val
  void mlt(int l, int r, item_type val, int root = 0) {
    if (l == nds[root].l && r == nds[root].r) {
      nds[root].mlt(val, mod);
      return;
    }
    down_flags(root);
    int m = nds[root].mid();
    if (r <= m) {  // only left part
      mlt(l, r, val, lchild(root));
    } else if (l >= m) {  // only right part
      mlt(l, r, val, rchild(root));
    } else {
      mlt(l, m, val, lchild(root));
      mlt(m, r, val, rchild(root));
    }
    merge_flags(root);
  }

  sum_type get_sum(int l, int r, int root = 0) {
    if (l == nds[root].l && r == nds[root].r) {
      return nds[root].sm;
    }
    // 勿忘加上all_add lazy标记
    int m = nds[root].mid();
    ll lazy = (nds[root].all_add * (sum_type)(r - l)) % mod;
    ll tmp;
    if (r <= m) {  // only left part
      tmp = get_sum(l, r, lchild(root));
    } else if (l >= m) {  // only right part
      tmp = get_sum(l, r, rchild(root));
    } else {
      tmp = get_sum(l, m, lchild(root))     // left
            + get_sum(m, r, rchild(root));  // right
    }
    tmp %= mod;
    return (tmp * nds[root].all_mlt % mod + lazy) % mod;
  }

  void out() {
    cout << "n = " << n << "\n";
    for (int i = 0; i < n; ++i) cout << a[i] << " ";
    cout << "\n";
  }
};
};  // namespace lly

lly::segment_tree tr;

int main() {
  // 洛谷 P3373 【模板】线段树 2

  int n, m;
  ll mod;
  // cin>>n>>m;
  scanf("%d%d%lld", &n, &m, &mod);
  tr.n = n;
  tr.mod = mod;
  tr.init();
  tr.build();

  int o, x, y;
  ll k;
  for (int i = 0; i < m; ++i) {
    // cin>>o>>x>>y;
    scanf("%d%d%d", &o, &x, &y);
    --x;
    if (o == 2) {  // [x,y)内都加上k
      scanf("%lld", &k);
      tr.add(x, y, k);
    } else if (o == 3) {  // 询问区间和
      auto sum = tr.get_sum(x, y);
      printf("%lld\n", sum);
    } else {
      scanf("%lld", &k);
      tr.mlt(x, y, k);
    }
  }
  return 0;
}
