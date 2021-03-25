#include <iostream>
#include <functional>
#include <stack>
using namespace std;

namespace BST
{
	template <typename T>
	class Node;
	template <typename T>
	using NodePtr = std::unique_ptr<Node<T>>;

	template <typename T>
	class Node
	{
	private:
		using NodeRPtr = Node*;
		using Nodes = stack<NodeRPtr>;

		T m_v;
		NodePtr<T> m_left;
		NodePtr<T> m_right;
	public:
		Node(const T& p_v) : m_v(p_v) {}
		//--------------------------------------------------------------
		//insert
		//recursive
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
		//nonrecursive
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
		//============================================================
		static NodePtr<T> GetNewNode(const T& p_v)
		{
			return std::make_unique<Node>(p_v);
		}
		using F = std::function<void(const Node& p_n)>;
		//============================================================
		//traversal
		// ---------------------------------------------------
		//preorder
		//recursive
		static void PreOrder(Node& p_n, F p_f) // n-l-r
		{
			p_f(p_n);
			if (p_n.m_left)
				PreOrder(*p_n.m_left, p_f);
			if (p_n.m_right)
				PreOrder(*p_n.m_right, p_f);
		}
		//nonrecursive
		static void NRPreOrder(Node& p_n, F p_f) // n-l-r
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
		// ---------------------------------------------------
		//postorder
		//recursive
		static void PostOrder(Node& p_n, F p_f) // l-r-rt
		{
			if (p_n.m_left)
				PostOrder(*p_n.m_left, p_f);
			if (p_n.m_right)
				PostOrder(*p_n.m_right, p_f);
			p_f(p_n);
		}
		//nonrecursive
		static void NRPostOrder(Node& p_n, F p_f) // l-r-n
		{
			Nodes nodes;
			auto currentNode = &p_n;
			bool isDone = false;
			while (!isDone)
			{
				nodes.push(currentNode);
				const auto nextNode = currentNode->m_left.get();
				if (nextNode)
					currentNode = nextNode;
				else
					isDone = true;
			}
			p_f(*currentNode);
		}
		// ---------------------------------------------------
		//inorder
		//recursive
		static void InOrder(Node& p_n, F p_f) // l-r-n
		{
			if (p_n.m_left)
				PostOrder(*p_n.m_left, p_f);
			p_f(p_n);
			if (p_n.m_right)
				PostOrder(*p_n.m_right, p_f);
		}
		//nonrecursive
		static void NRInOrder(Node& p_n, F p_f) // l-r-n
		{
		}
		//============================================================
		// diameter
		// recursive
		unsigned GetDiameter()
		{
			unsigned out = 0;
			return out;
		}
		//============================================================
	};
	//namespace Service
	//{
	//	//============================================================
	//	template <typename T>
	//	static NodePtr<T> GetNewNode(const T& p_v)
	//	{
	//		return std::make_unique<Node>(p_v);
	//	}
	//	template <typename T>
	//	using F = std::function<void(const Node<T>& p_n)>;
	//};
}

using namespace BST;
using NI = Node<int>;
using NIPtr = NodePtr<int>;

NIPtr GetR()
{
	/*
     5
   /   \
  3      8
 / \    / \
2   4  6   9
	*/
	auto n = NI::GetNewNode(5);
	auto& n3 = n->NRInsert(3);
	n3.NRInsert(2);
	n3.NRInsert(4);
	auto& n8 = n->NRInsert(8);
	n8.NRInsert(6);
	n8.NRInsert(9);
	return n;
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
	auto n = NI::GetNewNode(5);
	auto& n3 = n->NRInsert(3);
	n3.Insert(2);
	n3.Insert(4);
	auto& n8 = n->Insert(8);
	n8.Insert(6);
	n8.Insert(9);
	return n;
};

void f(const NI& p_n) 
{ 
	cout << p_n.GetV() << " "; 
};

void TestOfPreOrder()
{
	cout << "Test of PreOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto n = GetR();
		NI::PreOrder(*n, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto n = GetR();
		NI::NRPreOrder(*n, f);
		cout << "\n";
	}
}

void TestOfPostOrder()
{
	cout << "Test of PostOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto n = GetR();
		NI::PostOrder(*n, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto n = GetR();
		NI::NRPostOrder(*n, f);
		cout << "\n";
	}
}

void TestOfInOrder()
{
	cout << "Test of InOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto n = GetR();
		NI::InOrder(*n, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto n = GetR();
		NI::NRInOrder(*n, f);
		cout << "\n";
	}
}

void TestOfRecursiveTraversal()
{
	cout << "Test of Recursive Traversal\n";
	{
		cout << "PreOrder\n";
		auto n = GetR();
		NI::PreOrder(*n, f);
		cout << "\n";
	}
	{
		cout << "PostOrder\n";
		auto n = GetR();
		NI::PostOrder(*n, f);
		cout << "\n";
	}
	{
		cout << "InOrder\n";
		auto n = GetR();
		NI::PreOrder(*n, f);
		cout << "\n";
	}
}

void TestOfNonRecursiveTraversal()
{
	cout << "Test of Non Recursive Traversal\n";
	{
		cout << "PreOrder\n";
		auto n = GetR();
		NI::NRPreOrder(*n, f);
		cout << "\n";
	}
	{
		cout << "PostOrder\n";
		auto n = GetR();
		NI::NRPostOrder(*n, f);
		cout << "\n";
	}
	{
		cout << "InOrder\n";
		auto n = GetR();
		NI::NRPreOrder(*n, f);
		cout << "\n";
	}
}

int main()
{
	TestOfPreOrder();
	TestOfPostOrder();
	TestOfInOrder();
	TestOfRecursiveTraversal();
	TestOfNonRecursiveTraversal();

	std::cout << "Hello World!\n";
}