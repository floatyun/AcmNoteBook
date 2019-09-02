#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

namespace lly {
	// 所有下标符合C++风格
	// 奢侈做法：使用全局变量开4倍数组。
	// 最小值，最大值，区间和，区间加相同数模板
	// 考场抄代码所有item_type, sum_type 换成ll即可
	struct segment_tree {
		// 最大的初始数组大小和响应的线段树节点数组最大大小。
		const static int kMaxItemSize = 100000;
		const static int kMaxSegTreeSize = kMaxItemSize << 2;

		int item_sz;
		int seg_sz;
		int &n = item_sz;
		int &stn = seg_sz;

		typedef ll item_type;
		typedef ll sum_type;

		struct nd {
			int l, r;
			item_type mx;  // max
			item_type mn;  // min
			sum_type sm;   // sum flag
			// lazy flags
			item_type all_add;  // lazy标记，表示区间内的数都要加上的数
			// other flags
			inline void add(item_type a) {
				all_add += a;
				mx += a;
				mn += a;
				sm += (sum_type)(r-l)*a;
			}
			inline int mid() { return l + (r - l) / 2; }
		};

		item_type a[kMaxItemSize];
		nd nds[kMaxSegTreeSize];

		void init(int cnt) {  // 屏幕输入数组a版本
			n = cnt;
			for (int i = 0; i < n; ++i) scanf("%lld", a+i);//cin >> a[i];
			seg_sz = (2 << (int)(ceil( log2(item_sz) ))) - 1;
		}

		void init(item_type src[], int cnt) {  // 内存数组输入数组a版本
			n = cnt;
			for (int i = 0; i < n; ++i) a[i] = src[i];
			seg_sz = (2 << (int)(ceil( log2(item_sz) ))) - 1;
		}

		inline int parent(int x) { return (x - 1) >> 1; }
		inline int lchild(int x) { return (x << 1) | 1; }
		inline int rchild(int x) { return (x << 1) + 2; }

		inline void build() { build(0, n, 0); }

		inline void set_flags(int root, int i) {  // nds[root]用a[i]设置各类标志
			auto &p = nds[root];
			p.l = i;
			p.r = i + 1;
			p.mx = a[i];
			p.mn = a[i];
			p.sm = a[i];
		}

		inline void merge_flags(int root) {
			auto &p = nds[root];
			auto &l = nds[lchild(root)];
			auto &r = nds[rchild(root)];
			p.l = l.l;
			p.r = r.r;
			p.mx = max(l.mx, r.mx);
			p.mn = min(l.mn, r.mn);
			p.sm = l.sm + r.sm + p.all_add * (sum_type)(p.r - p.l);
		}

		void build(int l, int r, int root) {
			nds[root].all_add = 0;
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
				nds[root].add(val);
				return;
			}
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

		item_type get_max(int l, int r, int root = 0) {
			if (l == nds[root].l && r == nds[root].r) {
				return nds[root].mx;
			}
			// 勿忘加上all_add lazy标记
			int m = nds[root].mid();
			if (r <= m) {  // only left part
				return get_max(l, r, lchild(root)) + nds[root].all_add;
			} else if (l >= m) {  // only right part
				return get_max(l, r, rchild(root)) + nds[root].all_add;
			} else {
				return max(get_max(l, m, lchild(root)),  // left
									get_max(m, r, rchild(root)))  // right
							+ nds[root].all_add;
			}
		}

		item_type get_min(int l, int r, int root = 0) {
			if (l == nds[root].l && r == nds[root].r) {
				return nds[root].mn;
			}
			// 勿忘加上all_add lazy标记
			int m = nds[root].mid();
			if (r <= m) {  // only left part
				return get_min(l, r, lchild(root)) + nds[root].all_add;
			} else if (l >= m) {  // only right part
				return get_min(l, r, rchild(root)) + nds[root].all_add;
			} else {
				return min(get_min(l, m, lchild(root)),  // left
									get_min(m, r, rchild(root)))  // right
							+ nds[root].all_add;
			}
		}

		sum_type get_sum(int l, int r, int root = 0) {
			if (l == nds[root].l && r == nds[root].r) {
				return nds[root].sm;
			}
			// 勿忘加上all_add lazy标记
			int m = nds[root].mid();
			ll lazy = nds[root].all_add * (sum_type)(r - l);
			if (r <= m) {  // only left part
				return get_sum(l, r, lchild(root)) + lazy;
			} else if (l >= m) {  // only right part
				return get_sum(l, r, rchild(root)) + lazy;
			} else {
				return get_sum(l, m, lchild(root))    // left
							+ get_sum(m, r, rchild(root))  // right
							+ lazy;
			}
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
	// 洛谷 P3372 【模板】线段树 1
	
	int n, m;
	//cin>>n>>m;
	scanf("%d%d",&n,&m);
	tr.init(n);
	tr.build();

	int o,x,y;
	ll k;
	for (int i = 0; i < m; ++i) {
		//cin>>o>>x>>y;
		scanf("%d%d%d",&o,&x,&y);
		--x;
		if (o == 1) { // [x,y)内都加上k
			scanf("%lld",&k);
			tr.add(x,y,k);
		} else { // 询问区间和
			auto sum = tr.get_sum(x,y);
			printf("%lld\n",sum);
		}
	}
	return 0;
}
