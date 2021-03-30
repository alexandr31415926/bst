#include <iostream>
#include <functional>
#include <stack>
#include <queue>
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
		using Stack = stack<NodeRPtr>;
		using Queue = queue<NodeRPtr>;

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
			Stack nodes;
			nodes.push(&p_n);
			while (!nodes.empty())
			{
				const auto node = nodes.top();
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
		//nonrecursive / with 2 stacks
		static void NRPostOrder2(Node& p_n, F p_f) // l-r-n
		{
			Stack nodes;
			Stack outNodes;
			auto currentNode = &p_n;
			nodes.push(currentNode);
			while (!nodes.empty())
			{
				currentNode = nodes.top();
				nodes.pop();
				outNodes.push(currentNode);
				auto nextNode = currentNode->m_left.get();
				if (nextNode)
					nodes.push(nextNode);
				nextNode = currentNode->m_right.get();
				if (nextNode)
					nodes.push(nextNode);
			}
			while (!outNodes.empty())
			{
				p_f(*outNodes.top());
				outNodes.pop();
			}
		}
		static void NRPostOrder(Node& p_n, F p_f) // l-r-n
		{
			Stack nodes;
			auto currentNode = &p_n;
			do
			{
				while (currentNode) // Move to leftmost node
				{
					// Push node's right child and then node to stack.
					if (currentNode->m_right)
						nodes.push(currentNode->m_right.get());
					nodes.push(currentNode);

					// Set node as root's left child
					currentNode = currentNode->m_left.get();
				}
				// Pop an item from stack and set it as current node    
				currentNode = nodes.top();
				nodes.pop();

				// If the popped item has a right child and the right child is not
				// processed yet, then make sure right child is processed before root
				if (currentNode->m_right && !nodes.empty() && nodes.top() == currentNode->m_right.get())
				{
					nodes.pop();								// remove right child from stack
					nodes.push(currentNode);					// push node back to stack
					currentNode = currentNode->m_right.get();	// change current node`so that the right
																// child is processed next
				}
				else // process current node's data and set current node as NULL
				{
					p_f(*currentNode);
					currentNode = nullptr;
				}
			} while (!nodes.empty());
		}
		// ---------------------------------------------------
		//inorder
		//recursive
		static void InOrder(Node& p_n, F p_f) // l-r-n
		{
			if (p_n.m_left)
				InOrder(*p_n.m_left, p_f);
			p_f(p_n);
			if (p_n.m_right)
				InOrder(*p_n.m_right, p_f);
		}
		//nonrecursive
		static void NRInOrder(Node& p_n, F p_f) // l-r-n
		{
			Stack nodes;
			auto currentNode = &p_n;
			while (currentNode || !nodes.empty())
			{
				// Reach the left most Node of the current node
				while (currentNode)
				{
					// place pointer to a tree node on the stack before traversing the node's left subtree
					nodes.push(currentNode);
					currentNode = currentNode->m_left.get();
				}
				// Current node must be NULL at this point
				currentNode = nodes.top();
				nodes.pop();
				//---------------------------------------------------
				p_f(*currentNode);
				//---------------------------------------------------
				// we have visited the node and its	left subtree. Now, it's right subtree's turn
				currentNode = currentNode->m_right.get();
			}
		}
		// ---------------------------------------------------
		//levelorder
		//recursive
		static void LevelOrder(Node& p_n, F p_f)
		{
		}
		//nonrecursive
		static void NRLevelOrder(Node& p_n, F p_f) // l-r-n
		{
			Queue nodes;
			// Enqueue Root and initialize height
			nodes.push(&p_n);

			while (!nodes.empty())
			{
				// Process front of queue and remove it from queue
				const auto currentNode = nodes.front();
				p_f(*currentNode);
				nodes.pop();

				// Enqueue left child
				if (currentNode->m_left)
					nodes.push(currentNode->m_left.get());

				// Enqueue right child
				if (currentNode->m_right)
					nodes.push(currentNode->m_right.get());
			}
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
		cout << "Non Recursive, 2 stacks\n";
		auto n = GetR();
		NI::NRPostOrder2(*n, f);
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

void TestOfLevelOrder()
{
	cout << "Test of LevelOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto n = GetR();
		NI::LevelOrder(*n, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto n = GetR();
		NI::NRLevelOrder(*n, f);
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
	{
		cout << "LevelOrder\n";
		auto n = GetR();
		NI::LevelOrder(*n, f);
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
		cout << "PostOrder, 2 stacks\n";
		auto n = GetR();
		NI::NRPostOrder2(*n, f);
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
	{
		cout << "LevelOrder\n";
		auto n = GetR();
		NI::NRLevelOrder(*n, f);
		cout << "\n";
	}
}

int main()
{
	TestOfPreOrder();
//	TestOfPostOrder();
//	TestOfInOrder();
	TestOfLevelOrder();
//	TestOfRecursiveTraversal();
//	TestOfNonRecursiveTraversal();

	std::cout << "Hello World!\n";
}
