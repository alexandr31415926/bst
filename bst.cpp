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
	//----------------------------------------------------------------------------
	template <typename T>
	class Node
	{
	private:
		T m_value;
		NodePtr<T> m_left;
		NodePtr<T> m_right;
	public:
		template <typename T>
		using RPtr = Node<T>*;
		template <typename T>
		using CRPtr = const RPtr<T>;
		//--------------------------------------------------------------
		Node(const T& p_value);
		//--------------------------------------------------------------
		//insert
		//recursive
		Node& Insert(const T& p_value);
		//nonrecursive
		Node& NRInsert(const T& p_value);
		const T& GetValue() const;
		bool HasLeft() const;
		bool HasRight() const;
		CRPtr<T> GetLeft() const;
		CRPtr<T> GetRight() const;
		//============================================================
		static NodePtr<T> GetNewNode(const T& p_value);
		//============================================================
	};
	namespace Service
	{
		template <typename T>
		using Stack = stack<Node::RPtr<T>>;
		template <typename T>
		using Queue = queue<Node::RPtr<T>>;

		template <typename T>
		using F = std::function<void(const T& p_value)>;
		namespace Travsersal
		{
			namespace PreOrder // n-l-r
			{
				template <typename T>
				static void Recursive(Node<T>& p_node, F<T> p_f);
				template <typename T>
				static void NonRecursive(Node<T>& p_node, F<T> p_f);
			}
			// ---------------------------------------------------
			namespace PostOrder // l-r-rt
			{
				template <typename T>
				static void Recursive(Node<T>& p_node, F<T> p_f);
				template <typename T>
				static void NonRecursive2(Node<T>& p_node, F<T> p_f);
				template <typename T>
				static void NonRecursive(Node<T>& p_node, F<T> p_f);
			}
			// ---------------------------------------------------
			namespace InOrder // l-r-n
			{
				template <typename T>
				static void Recursive(Node<T>& p_node, F<T> p_f);
				//nonrecursive
				template <typename T>
				static void NonRecursive(Node<T>& p_node, F<T> p_f);
			}
			// ---------------------------------------------------
			namespace LevelOrder
			{
				template <typename T>
				static void RecursiveFV0(Node<T>& p_node, F<T> p_f);
				template <typename T>
				static void RecursiveFV(Node<T>& p_node, F<T> p_f);
				template <typename T>
				static void NonRecursive(Node<T>& p_node, F<T> p_f);
			}
		}
		namespace GetHeight
		{
			template <typename T>
			unsigned Recursive(const Node<T>& p_node);
		}
		namespace GetDiameter
		{
			template <typename T>
			unsigned Recursive(const Node<T>& p_node);
		}
	}
}
//----------------------------------------------------------------------------

using namespace BST;
//----------------------------------------------------------------------------
template<typename T>
Node<T>::Node(const T& p_value) : m_value(p_value) {}
//--------------------------------------------------------------
//insert
//recursive
template<typename T>
Node<T>& Node<T>::Insert(const T& p_value)
{
	if (p_value == m_value)
		return *this;
	else
	{
		if (p_value < m_value)
		{
			if (m_left)
				return m_left->Insert(p_value);
			else
			{
				m_left = GetNewNode(p_value);
				return *m_left;
			}
		}
		else
		{
			if (m_right)
				return m_right->Insert(p_value);
			else
			{
				m_right = GetNewNode(p_value);
				return *m_right;
			}
		}
	}
}

//nonrecursive
template<typename T>
Node<T>& Node<T>::NRInsert(const T& p_value)
{
	if (p_value == m_value)
		return *this;
	else
	{
		auto curN = this;
		while (true)
		{
			if (p_value < curN->m_value)
			{
				if (curN->m_left)
					curN = curN->m_left.get();
				else
				{
					curN->m_left = GetNewNode(p_value);
					return *(curN->m_left);
				}
			}
			else
			{
				if (curN->m_right)
					curN = curN->m_right.get();
				else
				{
					curN->m_right = GetNewNode(p_value);
					return *(curN->m_right);
				}
			}
		}
	}
}

template<typename T>
const T& Node<T>::GetValue() const
{
	return m_value;
}

template<typename T>
bool Node<T>::HasLeft() const
{
	return m_left.get();
}

template<typename T>
bool Node<T>::HasRight() const
{
	return m_right.get();
}

template<typename T>
Node<T>::CRPtr<T> Node<T>::GetLeft() const
{
	return m_left.get();
}

template<typename T>
Node::CRPtr<T> Node<T>::GetRight() const
{
	return m_right.get();
}
//============================================================
template<typename T>
NodePtr<T> Node<T>::GetNewNode(const T& p_value)
{
	return std::make_unique<Node>(p_value);
}

//============================================================

using namespace BST::Service;
using namespace BST::Service::Travsersal;

template<typename T>
void PreOrder::Recursive(Node<T>& p_node, F<T> p_f) // n-l-r
{
	p_f(p_node.GetValue());
	if (p_node.HasLeft())
		Recursive(*p_node.GetLeft(), p_f);
	if (p_node.HasRight())
		Recursive(*p_node.GetRight(), p_f);
}
template<typename T>
void PreOrder::NonRecursive(Node<T>& p_node, F<T> p_f) // n-l-r
{
	Stack<T> nodes;
	nodes.push(&p_node);
	while (!nodes.empty())
	{
		const auto node = nodes.top();
		p_f(node->GetValue());
		nodes.pop();
		if (node->HasRight())
			nodes.push(node->GetRight());
		if (node->HasLeft())
			nodes.push(node->GetLeft());
	}
}
// ---------------------------------------------------
template <typename T>
void PostOrder::Recursive(Node<T>& p_node, F<T> p_f) // l-r-rt
{
	if (p_node.HasLeft())
		Recursive(*p_node.GetLeft(), p_f);
	if (p_node.HasRight())
		Recursive(*p_node.GetRight(), p_f);
	p_f(p_node.GetValue());
}
//nonrecursive / with 2 stacks
template <typename T>
void PostOrder::NonRecursive2(Node<T>& p_node, F<T> p_f) // l-r-n
{
	Stack<T> nodes;
	Stack<T> outNodes;
	Node::RPtr<T> currentNode = &p_node;
	nodes.push(currentNode);
	while (!nodes.empty())
	{
		currentNode = nodes.top();
		nodes.pop();
		outNodes.push(currentNode);
		auto nextNode = currentNode->GetLeft();
		if (nextNode)
			nodes.push(nextNode);
		nextNode = currentNode->GetRight();
		if (nextNode)
			nodes.push(nextNode);
	}
	while (!outNodes.empty())
	{
		p_f((*outNodes.top()).GetValue());
		outNodes.pop();
	}
}
template <typename T>
void PostOrder::NonRecursive(Node<T>& p_node, F<T> p_f) // l-r-n
{
	Stack<T> nodes;
	auto currentNode = &p_node;
	do
	{
		while (currentNode) // Move to leftmost node
		{
			// Push node's right child and then node to stack.
			if (currentNode->HasRight())
				nodes.push(currentNode->GetRight());
			nodes.push(currentNode);

			// Set node as root's left child
			currentNode = currentNode->GetLeft();
		}
		// Pop an item from stack and set it as current node    
		currentNode = nodes.top();
		nodes.pop();

		// If the popped item has a right child and the right child is not
		// processed yet, then make sure right child is processed before root
		if (currentNode->HasRight() && !nodes.empty() && nodes.top() == currentNode->GetRight())
		{
			nodes.pop();								// remove right child from stack
			nodes.push(currentNode);					// push node back to stack
			currentNode = currentNode->GetRight();	// change current node`so that the right
														// child is processed next
		}
		else // process current node's data and set current node as NULL
		{
			p_f(currentNode->GetValue());
			currentNode = nullptr;
		}
	} while (!nodes.empty());
}
// ---------------------------------------------------
template <typename T>
void InOrder::Recursive(Node<T>& p_node, F<T> p_f) // l-r-n
{
	if (p_node.HasLeft())
		Recursive(*p_node.GetLeft(), p_f);
	p_f(p_node.GetValue());
	if (p_node.HasRight())
		Recursive(*p_node.GetRight(), p_f);
}
//nonrecursive
template <typename T>
void InOrder::NonRecursive(Node<T>& p_node, F<T> p_f) // l-r-n
{
	Stack<T> nodes;
	auto currentNode = &p_node;
	while (currentNode || !nodes.empty())
	{
		// Reach the left most Node of the current node
		while (currentNode)
		{
			// place pointer to a tree node on the stack before traversing the node's left subtree
			nodes.push(currentNode);
			currentNode = currentNode->GetLeft();
		}
		// Current node must be NULL at this point
		currentNode = nodes.top();
		nodes.pop();
		//---------------------------------------------------
		p_f(currentNode->GetValue());
		//---------------------------------------------------
		// we have visited the node and its	left subtree. Now, it's right subtree's turn
		currentNode = currentNode->GetRight();
	}
}
// ---------------------------------------------------
//first version
template <typename T>
void LevelOrder::RecursiveFV0(Node<T>& p_node, F<T> p_f)
{
	using Level = vector<T>;
	using Result = vector<Level>;
	std::function<void(Result& p_result, Node& p_node, size_t p_level)> Dfs =
		[&Dfs](Result& p_result, Node& p_node, size_t p_level)
	{
		//How to maintain the current layer list is critical here.
		if (p_result.empty() || p_result.size() < p_level + 1)
			p_result.push_back(Level());
		p_result[p_level].push_back(p_node.GetValue());
		if (p_node.m_left.get())
			Dfs(p_result, p_node.GetLeft().get(), p_level + 1);
		if (p_node.m_right.get())
			Dfs(p_result, p_node.GetRight().get(), p_level + 1);
	};

	Result result;
	Dfs(result, p_node, 0);
	for (const auto& level : result)
	{
		for (const auto& item : level)
		{
			cout << item << " ";
		}
	}
}
template <typename T>
void LevelOrder::RecursiveFV(Node<T>& p_node, F<T> p_f)
{
	std::function<void(const Node<T>& p_node, size_t p_level)> ProcessLevel =
		[p_f, &ProcessLevel](const Node<T>& p_node, size_t p_level)
	{
		if (p_level == 1)
			p_f(p_node.GetValue());
		else if (p_level > 1)
		{
			ProcessLevel(*p_node.GetLeft(), p_level - 1);
			ProcessLevel(*p_node.GetRight(), p_level - 1);
		}
	};
	const auto height = GetHeight::Recursive(p_node);
	for (size_t i = 1; i <= height; ++i)
	{
		ProcessLevel(p_node, i);
	}
}
//nonrecursive
template <typename T>
static void NRLevelOrder(const Node<T>& p_node, F<T> p_f)
{
	Queue nodes;
	// Enqueue Root and initialize height
	nodes.push(&p_node);

	while (!nodes.empty())
	{
		// Process front of queue and remove it from queue
		const auto currentNode = nodes.front();
		p_f((*currentNode).m_value);
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
template <typename T>
unsigned GetHeight::Recursive(const Node<T>& p_node)
{
	unsigned out = 0;
	const int leftHeight = p_node.HasLeft() ? Recursive(*p_node.GetLeft()) : 0;
	const int rightHeight = p_node.HasRight() ? Recursive(*p_node.GetRight()) : 0;
	out = leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	return out;
}
//============================================================
template <typename T>
unsigned GetDiameter::Recursive(const Node<T>& p_node)
{
	unsigned out = 0;
	return out;
}
//============================================================

using namespace BST;
using namespace BST::Service;
using namespace BST::Service::Travsersal;
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

void f(const int& p_value)
{
	cout << p_value << " ";
};

void TestOfPreOrder()
{
	cout << "Test of PreOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto n = GetR();
		PreOrder::Recursive<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto n = GetR();
		PreOrder::NonRecursive<int>(*n, f);
		cout << "\n";
	}
}

void TestOfPostOrder()
{
	cout << "Test of PostOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto n = GetR();
		PostOrder::Recursive<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive, 2 stacks\n";
		auto n = GetR();
		PostOrder::NonRecursive2<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto n = GetR();
		PostOrder::NonRecursive<int>(*n, f);
		cout << "\n";
	}
}

void TestOfInOrder()
{
	cout << "Test of InOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto n = GetR();
		InOrder::Recursive<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto n = GetR();
		InOrder::NonRecursive<int>(*n, f);
		cout << "\n";
	}
}

void TestOfLevelOrder()
{
	cout << "Test of LevelOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto n = GetR();
		LevelOrder::RecursiveFV<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto n = GetR();
		LevelOrder::RecursiveFV<int>(*n, f);
		cout << "\n";
	}
}

void TestOfRecursiveTraversal()
{
	cout << "Test of Recursive Traversal\n";
	{
		cout << "PreOrder\n";
		const auto& n = GetR();
		PreOrder::Recursive<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "PostOrder\n";
		const auto& n = GetR();
		PostOrder::Recursive<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "InOrder\n";
		const auto& n = GetR();
		InOrder::Recursive<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "LevelOrder\n";
		const auto& n = GetR();
		LevelOrder::RecursiveFV<int>(*n, f);
		cout << "\n";
	}
}

void TestOfNonRecursiveTraversal()
{
	cout << "Test of Non Recursive Traversal\n";
	{
		cout << "PreOrder\n";
		const auto& n = GetR();
		PreOrder::NonRecursive<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "PostOrder, 2 stacks\n";
		const auto& n = GetR();
		PostOrder::NonRecursive2<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "PostOrder\n";
		const auto& n = GetR();
		PostOrder::NonRecursive<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "InOrder\n";
		const auto& n = GetR();
		InOrder::Recursive<int>(*n, f);
		cout << "\n";
	}
	{
		cout << "LevelOrder\n";
		const auto& n = GetR();
		LevelOrder::RecursiveFV<int>(*n, f);
		cout << "\n";
	}
}

int main()
{
	//TestOfPreOrder();
	//TestOfPostOrder();
	//TestOfInOrder();
	//TestOfLevelOrder();
	TestOfRecursiveTraversal();
	TestOfNonRecursiveTraversal();

	std::cout << "Hello World!\n";
}
