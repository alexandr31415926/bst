#include <iostream>
#include <functional>
#include <stack>
using namespace std;

namespace BT
{
	template <typename T>
	class Node
	{
		using NodePtr = std::unique_ptr<Node<T>>;
		using NodeRPtr = Node*;
		using Nodes = stack<NodeRPtr>;

		T m_v;
		NodePtr m_left;
		NodePtr m_right;
	public:
		Node(const T& p_v) : m_v(p_v) {}
		Node& Insert(const T& p_v)
		{
			if (p_v == m_v)
				return *this;
			else
			{
				if (p_v < m_v)
				{
					if (m_left)
						return m_left->Insert(p_v);
					else
					{
						m_left = GetNewNode(p_v);
						return *m_left;
					}
				}
				else
				{
					if (m_right)
						return m_right->Insert(p_v);
					else
					{
						m_right = GetNewNode(p_v);
						return *m_right;
					}
				}
			}
		}
		Node& NRInsert(const T& p_v)
		{
			if (p_v == m_v)
				return *this;
			else
			{
				auto curN = this;
				while (true)
				{
					if (p_v < curN->m_v)
					{
						if (curN->m_left)
							curN = curN->m_left.get();
						else
						{
							curN->m_left = GetNewNode(p_v);
							return *(curN->m_left);
						}
					}
					else
					{
						if (curN->m_right)
							curN = curN->m_right.get();
						else
						{
							curN->m_right = GetNewNode(p_v);
							return *(curN->m_right);
						}
					}
				}
			}
		}
		const T& GetV() const
		{
			return m_v;
		}
		static NodePtr GetNewNode(const T& p_v)
		{
			auto n = std::make_unique<Node>(p_v);
			return n;
		}
		using F = std::function<void(const Node& p_n)>;
		static void PreOr(Node& p_n, F p_f) // rt-l-r
		{
			p_f(p_n);
			if (p_n.m_left)
				PreOr(*p_n.m_left, p_f);
			if (p_n.m_right)
				PreOr(*p_n.m_right, p_f);
		}
		static void NRPreOr(Node& p_n, F p_f) // rt-l-r
		{
			Nodes nodes;
			nodes.push(&p_n);
			while (!nodes.empty())
			{
				auto node = nodes.top();
				p_f(*node);
				nodes.pop();
				if (node->m_right)
					nodes.push(node->m_right.get());
				if (node->m_left)
					nodes.push(node->m_left.get());
			}
		}
	};
}

using namespace BT;
using NI = Node<int>;
using NIPtr = std::unique_ptr<NI>;

NIPtr GetR()
{
	/*
	 5
   /   \
  3      8
 / \    / \
2   4  6   9
	*/
	auto rt = NI::GetNewNode(5);
	auto& n3 = rt->NRInsert(3);
	n3.NRInsert(2);
	n3.NRInsert(4);
	auto& n8 = rt->NRInsert(8);
	n8.NRInsert(6);
	n8.NRInsert(9);
	return rt;
};

NIPtr GetRR()
{
	/*
	 5
   /   \
  3      8
 / \    / \
2   4  6   9
	*/
	auto rt = NI::GetNewNode(5);
	auto& n3 = rt->NRInsert(3);
	n3.Insert(2);
	n3.Insert(4);
	auto& n8 = rt->Insert(8);
	n8.Insert(6);
	n8.Insert(9);
	return rt;
};

int main()
{
	NI::F f = [](const NI& p_n) {cout << p_n.GetV() << " "; };

	{
		auto rt = GetRR();
		NI::PreOr(*rt, f);
		cout << "\n";
	}

	{
		auto rt = GetR();
		NI::NRPreOr(*rt, f);
		cout << "\n";
	}

	std::cout << "Hello World!\n";
}