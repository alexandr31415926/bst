//Should I pass an std::function by const-reference?
//https://stackoverflow.com/questions/18365532/should-i-pass-an-stdfunction-by-const-reference
//Binary Trees in C++: Part 1
//https://www.cprogramming.com/tutorial/lesson18.html
//Level Order Tree Traversal
//https://www.geeksforgeeks.org/level-order-tree-traversal/

#include <functional>
#include <iostream>
#include <stack>
using namespace std;

template <typename T>
class BST
{
	class TN;
	typedef TN* TNPtr;
	struct TN
	{
		TNPtr m_l;
		TNPtr m_r;
		T m_d;
		TN(const T& p_d)
			: m_d(p_d)
			, m_l(nullptr)
			, m_r(nullptr)
		{
		}
	};
	TNPtr m_root = nullptr;

public:
	void Insert(const T& p_d)
	{
		if (m_root == nullptr)
		{
			m_root = new TN(p_d);
			m_root->m_d = p_d;
		}
		else
		{
			auto c = m_root;
			auto cPrev = c;
			bool isLeft = true;
			while (c != nullptr)
			{
				cPrev = c;
				if (p_d < c->m_d)
				{
					c = c->m_l;
					isLeft = true;
				}
				else if (p_d > c->m_d)
				{
					c = c->m_r;
					isLeft = false;
				}
				else
				{
					return;
				}
			}
			if (isLeft)
				cPrev->m_l = new TN(p_d);
			else
				cPrev->m_r = new TN(p_d);
		}
	}

	template <typename T>
	friend class BSTOp;
};

template <typename T>
class BSTOp
{
	typedef stack<typename BST<T>::TNPtr> Nodes;
public:
	template <typename T>
	using NodeF = std::function<void(const T& p_d)>;
	template <typename T>
	using BSTF = std::function<void(const BST<T>& p_bst, const NodeF<T>& p_f)>;

	static void PreO(const BST<T>& p_bst, const NodeF<T>& p_f)
	{
		std::function<void(typename BST<T>::TNPtr p_n, const NodeF<T> & p_f)> H = [&](typename BST<T>::TNPtr p_n, const NodeF<T>& p_f) -> void
		{
			if (!p_n)
				return;
			p_f(p_n->m_d);
			H(p_n->m_l, p_f);
			H(p_n->m_r, p_f);
		};
		H(p_bst.m_root, p_f);
	}

	static void PreONR(const BST<T>& p_bst, const NodeF<T>& p_f) // NLR
	{
		auto c = p_bst.m_root;
		if (!c)
			return;
		Nodes nodes;
		nodes.push(c);
		while (!nodes.empty())
		{
			c = nodes.top();
			p_f(c->m_d);
			nodes.pop();
			if (c->m_r)
				nodes.push(c->m_r);
			if (c->m_l)
				nodes.push(c->m_l);
		}
	}

	static void PostO(const BST<T>& p_bst, const NodeF<T>& p_f)
	{
		std::function<void(typename BST<T>::TNPtr p_n, const NodeF<T> & p_f)> H = [&](typename BST<T>::TNPtr p_n, const NodeF<T>& p_f) -> void
		{
			if (!p_n)
				return;
			H(p_n->m_l, p_f);
			H(p_n->m_r, p_f);
			p_f(p_n->m_d);
		};
		H(p_bst.m_root, p_f);
	}

	static void PostONR(const BST<T>& p_bst, const NodeF<T>& p_f) // LRN
	{
		auto c = p_bst.m_root;
		if (!c)
			return;
		Nodes nodes;
		do
		{
			while (c)
			{
				if (c->m_r)
					nodes.push(c->m_r);
				nodes.push(c);
				c = c->m_l;
			}
			c = nodes.top();
			nodes.pop();
			if (c->m_r && !nodes.empty() && nodes.top() == c->m_r)
			{
				nodes.pop();
				nodes.push(c);
				c = c->m_r;
			}
			else
			{
				p_f(c->m_d);
				c = nullptr;
			}
		} while (!nodes.empty());
	}

	static void InO(const BST<T>& p_bst, const NodeF<T>& p_f)
	{
		std::function<void(typename BST<T>::TNPtr p_n, const NodeF<T> & p_f)> H = [&](typename BST<T>::TNPtr p_n, const NodeF<T>& p_f) -> void
		{
			if (!p_n)
				return;
			H(p_n->m_l, p_f);
			p_f(p_n->m_d);
			H(p_n->m_r, p_f);
		};
		H(p_bst.m_root, p_f);
	}

	static void InONR(const BST<T>& p_bst, const NodeF<T>& p_f) // LNR
	{
		auto c = p_bst.m_root;
		if (!c)
			return;
		Nodes nodes;
		c = p_bst.m_root;
		while (c != nullptr || !nodes.empty())
		{
			while (c != nullptr)
			{
				nodes.push(c);
				c = c->m_l;
			}
			c = nodes.top();
			p_f(c->m_d);
			nodes.pop();
			c = c->m_r;
		}
	}

	static int GetH(typename BST<T>::TNPtr p_n)
	{
		if (!p_n)
			return 0;
		else
		{
			int lH = GetH(p_n->m_l);
			int rH = GetH(p_n->m_r);
			if (lH > rH)
				return lH + 1;
			else
				return rH + 1;
		}
	}

	static void LeO(const BST<T>& p_bst, const NodeF<T>& p_f)
	{
		auto ProcessGivenLevel = [](typename BST<T>::TNPtr p_n, int p_level, const NodeF<T>& p_f)
		{
			auto Lambda = [](typename BST<T>::TNPtr p_n, int p_level, const NodeF<T>& p_f, auto& p_lambda)
			{
				if (p_n == nullptr)
					return;
				if (p_level == 1)
					p_f(p_n->m_d);
				else if (p_level > 1)
				{
					p_lambda(p_n->m_l, p_level - 1, p_f, p_lambda);
					p_lambda(p_n->m_r, p_level - 1, p_f, p_lambda);
				}
			};
			Lambda(p_n, p_level, p_f, Lambda);
		};
		auto h = GetH(p_bst.m_root);
		for (int i = 1; i <= h; ++i)
			ProcessGivenLevel(p_bst.m_root, i, p_f);
	}

	static void NodePrint(const T& p_d)
	{
		cout << p_d << ' ';
	}

	static inline NodeF<T> NodePrintLambda = [](const T& p_d) { NodePrint(p_d); };

	static void Proc(const BSTF<T>& p_procF, const BST<T>& p_bst, const NodeF<T>& p_f)
	{
		p_procF(p_bst, p_f);
	}
};

typedef BST<int> BSTI;
BSTI&& GetData()
{
	BSTI t;
	t.Insert(11);
	t.Insert(9);
	t.Insert(13);
	t.Insert(6);
	t.Insert(15);
	t.Insert(10);
	t.Insert(14);
	t.Insert(8);
	t.Insert(12);
	t.Insert(7);
	t.Insert(16);
	//             11
	//            /   \
	//          9      13
	//         / \    /  \
	//       6    1012    15
	//        \          /  \
	//          8      14    16
	//         /
	//       7
	return move(t);
}

template <typename T>
void Test001001(const BST<T>& p_t)
{
	BSTOp<T>::PreO(p_t, BSTOp<T>::NodePrintLambda);
	cout << '\n';
}

template <typename T>
void Test001002(const BST<T>& p_t)
{
	BSTOp<T>::PreO(p_t, &BSTOp<T>::NodePrint);
	cout << '\n';
}

template <typename T>
void Test001003(const BST<T>& p_t)
{
	BSTOp<int>::Proc(&BSTOp<int>::PreO, p_t, &BSTOp<int>::NodePrint);
	cout << '\n';
}

template <typename T>
void Test001004(const BST<T>& p_t)
{
	BSTOp<int>::Proc(&BSTOp<int>::PreONR, p_t, &BSTOp<int>::NodePrint);
	cout << '\n';
}

template <typename T>
void Test002001(const BST<T>& p_t)
{
	BSTOp<T>::PostO(p_t, BSTOp<T>::NodePrintLambda);
	cout << '\n';
}

template <typename T>
void Test002002(const BST<T>& p_t)
{
	BSTOp<T>::PostO(p_t, &BSTOp<T>::NodePrint);
	cout << '\n';
}

template <typename T>
void Test002003(const BST<T>& p_t)
{
	BSTOp<int>::Proc(&BSTOp<int>::PostO, p_t, &BSTOp<int>::NodePrint);
	cout << '\n';
}

template <typename T>
void Test002004(const BST<T>& p_t)
{
	BSTOp<int>::Proc(&BSTOp<int>::PostONR, p_t, &BSTOp<int>::NodePrint);
	cout << '\n';
}

template <typename T>
void Test003001(const BST<T>& p_t)
{
	BSTOp<T>::InO(p_t, BSTOp<T>::NodePrintLambda);
	cout << '\n';
}

template <typename T>
void Test003002(const BST<T>& p_t)
{
	BSTOp<T>::InO(p_t, &BSTOp<T>::NodePrint);
	cout << '\n';
}

template <typename T>
void Test003003(const BST<T>& p_t)
{
	BSTOp<int>::Proc(&BSTOp<int>::InO, p_t, &BSTOp<int>::NodePrint);
	cout << '\n';
}

template <typename T>
void Test003004(const BST<T>& p_t)
{
	BSTOp<int>::Proc(&BSTOp<int>::InONR, p_t, &BSTOp<int>::NodePrint);
	cout << '\n';
}

template <typename T>
void Test004001(const BST<T>& p_t)
{
	BSTOp<T>::LeO(p_t, BSTOp<T>::NodePrintLambda);
	cout << '\n';
}

int main()
{
	{
		auto t = GetData();
		Test001001(t);
		Test001002(t);
		Test001003(t);
		Test001004(t);
		Test002001(t);
		Test002002(t);
		Test002003(t);
		Test002004(t);
		Test003001(t);
		Test003002(t);
		Test003003(t);
		Test003004(t);
		Test004001(t);
	}
	cout << "\nTE" << endl;
	cin.get();
	return 0;
}
