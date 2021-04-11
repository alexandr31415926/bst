#include <iostream>
#include <functional>
#include <stack>
#include <queue>
#include <vector>
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
		const T& Value() const;
		bool HasLeft() const;
		bool HasRight() const;
		CRPtr<T> Left() const;
		CRPtr<T> Right() const;
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
		using F = std::function<void(const Node<T>& p_n)>;
		namespace Travsersal
		{
			namespace PreOrder // n-l-r
			{
				template <typename T>
				static void Recursive(const Node<T>& p_node, F<T> p_f);
				template <typename T>
				static void NonRecursive(Node<T>& p_node, F<T> p_f);
			}
			// ---------------------------------------------------
			namespace PostOrder // l-r-rt
			{
				template <typename T>
				static void Recursive(const Node<T>& p_node, F<T> p_f);
				template <typename T>
				static void NonRecursive2(Node<T>& p_node, F<T> p_f);
				template <typename T>
				static void NonRecursive(Node<T>& p_node, F<T> p_f);
			}
			// ---------------------------------------------------
			namespace InOrder // l-r-n
			{
				template <typename T>
				static void Recursive(const Node<T>& p_node, F<T> p_f);
				//nonrecursive
				template <typename T>
				static void NonRecursive(Node<T>& p_node, F<T> p_f);
			}
			// ---------------------------------------------------
			namespace LevelOrder
			{
				template <typename T>
				static void RecursiveFV0(const Node<T>& p_node, F<T> p_f);
				template <typename T>
				static void RecursiveFV(const Node<T>& p_node, F<T> p_f);
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
			template <typename T>
			unsigned Recursive(const Node<T>& p_node, unsigned& p_diameter);
		}
		namespace GetNumberOfLeaves
		{
			template <typename T>
			unsigned Recursive(const Node<T>& p_node);
		}
		namespace AreEqual
		{
			template <typename T>
			bool Recursive(const Node<T>& p_node1, const Node<T>& p_node2);
		}
		namespace PrintAllPaths
		{
			template <typename T>
			void Recursive(const Node<T>& p_node);
		}
		namespace HasSum
		{
			template <typename T>
			bool Recursive(const Node<T>& p_node, const T& p_sum);
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
const T& Node<T>::Value() const
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
Node<T>::CRPtr<T> Node<T>::Left() const
{
	return m_left.get();
}

template<typename T>
Node<T>::CRPtr<T> Node<T>::Right() const
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
void PreOrder::Recursive(const Node<T>& p_node, F<T> p_f) // n-l-r
{
	p_f(p_node);
	if (p_node.HasLeft())
		Recursive(*p_node.Left(), p_f);
	if (p_node.HasRight())
		Recursive(*p_node.Right(), p_f);
}
template<typename T>
void PreOrder::NonRecursive(Node<T>& p_node, F<T> p_f) // n-l-r
{
	Stack<T> nodes;
	nodes.push(&p_node);
	while (!nodes.empty())
	{
		const auto node = nodes.top();
		p_f(*node);
		nodes.pop();
		if (node->HasRight())
			nodes.push(node->Right());
		if (node->HasLeft())
			nodes.push(node->Left());
	}
}
// ---------------------------------------------------
template <typename T>
void PostOrder::Recursive(const Node<T>& p_node, F<T> p_f) // l-r-rt
{
	if (p_node.HasLeft())
		Recursive(*p_node.Left(), p_f);
	if (p_node.HasRight())
		Recursive(*p_node.Right(), p_f);
	p_f(p_node);
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
		auto nextNode = currentNode->Left();
		if (nextNode)
			nodes.push(nextNode);
		nextNode = currentNode->Right();
		if (nextNode)
			nodes.push(nextNode);
	}
	while (!outNodes.empty())
	{
		p_f(*outNodes.top());
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
				nodes.push(currentNode->Right());
			nodes.push(currentNode);

			// Set node as root's left child
			currentNode = currentNode->Left();
		}
		// Pop an item from stack and set it as current node    
		currentNode = nodes.top();
		nodes.pop();

		// If the popped item has a right child and the right child is not
		// processed yet, then make sure right child is processed before root
		if (currentNode->HasRight() && !nodes.empty() && nodes.top() == currentNode->Right())
		{
			nodes.pop();								// remove right child from stack
			nodes.push(currentNode);					// push node back to stack
			currentNode = currentNode->Right();	// change current node`so that the right
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
template <typename T>
void InOrder::Recursive(const Node<T>& p_node, F<T> p_f) // l-r-n
{
	if (p_node.HasLeft())
		Recursive(*p_node.Left(), p_f);
	p_f(p_node);
	if (p_node.HasRight())
		Recursive(*p_node.Right(), p_f);
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
			currentNode = currentNode->Left();
		}
		// Current node must be NULL at this point
		currentNode = nodes.top();
		nodes.pop();
		//---------------------------------------------------
		p_f(*currentNode);
		//---------------------------------------------------
		// we have visited the node and its	left subtree. Now, it's right subtree's turn
		currentNode = currentNode->Right();
	}
}
// ---------------------------------------------------
//first version
template <typename T>
void LevelOrder::RecursiveFV0(const Node<T>& p_node, F<T> p_f)
{
	using Level = vector<T>;
	using Result = vector<Level>;
	std::function<void(Result& p_result, Node& p_node, size_t p_level)> Dfs =
		[&Dfs](Result& p_result, Node& p_node, size_t p_level)
	{
		//How to maintain the current layer list is critical here.
		if (p_result.empty() || p_result.size() < p_level + 1)
			p_result.push_back(Level());
		p_result[p_level].push_back(p_node.Value());
		if (p_node.m_left.get())
			Dfs(p_result, p_node.Left().get(), p_level + 1);
		if (p_node.m_right.get())
			Dfs(p_result, p_node.Right().get(), p_level + 1);
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
void LevelOrder::RecursiveFV(const Node<T>& p_node, F<T> p_f)
{
	std::function<void(const Node<T>& p_node, size_t p_level)> ProcessLevel =
		[p_f, &ProcessLevel](const Node<T>& p_node, size_t p_level)
	{
		if (p_level == 1)
			p_f(p_node);
		else if (p_level > 1)
		{
			ProcessLevel(*p_node.Left(), p_level - 1);
			ProcessLevel(*p_node.Right(), p_level - 1);
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
static void LevelOrder::NonRecursive(Node<T>& p_node, F<T> p_f)
{
	Queue<T> nodes;
	// Enqueue Root and initialize height
	nodes.push(&p_node);

	while (!nodes.empty())
	{
		// Process front of queue and remove it from queue
		const auto currentNode = nodes.front();
		p_f(*currentNode);
		nodes.pop();

		// Enqueue left child
		if (currentNode->HasLeft())
			nodes.push(currentNode->Left());

		// Enqueue right child
		if (currentNode->HasRight())
			nodes.push(currentNode->Right());
	}
}

//============================================================
template <typename T>
unsigned GetHeight::Recursive(const Node<T>& p_node)
{
	unsigned out = 0;
	const int leftHeight = p_node.HasLeft() ? Recursive(*p_node.Left()) : 0;
	const int rightHeight = p_node.HasRight() ? Recursive(*p_node.Right()) : 0;
	out = leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	return out;
}
//============================================================
template <typename T>
unsigned GetDiameter::Recursive(const Node<T>& p_node)
{
	unsigned out = 0;
	const auto leftHeight = p_node.HasLeft() ? GetHeight::Recursive(*p_node.Left()) : 0;
	const auto rightHeight = p_node.HasRight() ? GetHeight::Recursive(*p_node.Right()) : 0;
	const auto leftDiameter = p_node.HasLeft() ? Recursive(*p_node.Left()) : 0;
	const auto rightDiameter = p_node.HasRight() ? Recursive(*p_node.Right()) : 0;
	out = max(leftHeight + rightHeight + 1, max(leftDiameter, rightDiameter));
	return out;
}
template <typename T>
unsigned GetDiameter::Recursive(const Node<T>& p_node, unsigned& p_diameter)
{
	const auto leftDiameter = p_node.HasLeft() ? Recursive(*p_node.Left(), p_diameter) : 0;
	const auto rightDiameter = p_node.HasRight() ? Recursive(*p_node.Right(), p_diameter) : 0;
	p_diameter = max(p_diameter, leftDiameter + rightDiameter);
	return max(leftDiameter, rightDiameter) + 1;
}
//============================================================
template <typename T>
unsigned GetNumberOfLeaves::Recursive(const Node<T>& p_node)
{
	unsigned counter = 0;
	Service::Travsersal::PreOrder::Recursive<T>(p_node, [&counter](const Node<T>& p_node) { counter++; });
	return counter;
}
//============================================================
template <typename T>
bool AreEqual::Recursive(const Node<T>& p_node1, const Node<T>& p_node2)
{
	bool out = true;
	if (!p_node1.HasLeft() && !p_node2.HasLeft() && !p_node1.HasRight() && !p_node2.HasRight())
		out = true;
	else if (p_node1.HasLeft() && p_node2.HasLeft() && p_node1.HasRight() && p_node2.HasRight())
		out = p_node1.Value() == p_node2.Value()
			&& Recursive(*p_node1.Left(), *p_node2.Left())
			&& Recursive(*p_node1.Right(), *p_node2.Right());
	else
		out = false;
	return out;
}
//============================================================
template <typename T>
void PrintAllPaths::Recursive<T>(const Node<T>& p_node)
{
	using Path = std::vector<T>;
	Path path;
	std::function<void(const Node<T>& p_node, Path p_path)> printAllPaths = [&](const Node<T>& p_node, Path p_path)
	{
		p_path.push_back(p_node.Value());
		if (!p_node.HasLeft() && !p_node.HasRight())
		{
			for (const auto& p : p_path)
				cout << p << " ";
			cout << endl;
		}
		else
		{
			if (p_node.HasLeft())
				printAllPaths(*p_node.Left(), p_path);
			if (p_node.HasRight())
				printAllPaths(*p_node.Right(), p_path);
		}
	};
	printAllPaths(p_node, path);
}
//============================================================
template <typename T>
bool HasSum::Recursive(const Node<T>& p_node, const T& p_sum)
{
	const auto remainingSum = p_sum - p_node.Value();
	if (!p_node.HasLeft() && !p_node.HasRight())
		return remainingSum == 0;
	if (p_node.HasLeft() && p_node.HasRight())
		return (Recursive(*p_node.Left(), remainingSum) || Recursive(*p_node.Right(), remainingSum));
	else          
		if (p_node.HasLeft())
			return Recursive(*p_node.Left(), remainingSum);
		else             
			return Recursive(*p_node.Right(), remainingSum);
}
//============================================================

using namespace BST;
using namespace BST::Service;
using namespace BST::Service::Travsersal;
using NI = Node<int>;
using NIPtr = NodePtr<int>;

NIPtr GetTestData1()
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

NIPtr GetTestData1NR()
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

NIPtr GetTestData2()
{
	/*
		 5
	   /   \
	  3      8
	 / \    / \
	2   4  6   9
   / \
  1   10
	*/
	auto n = NI::GetNewNode(5);
	auto& n3 = n->NRInsert(3);
	auto& n2 = n3.NRInsert(2);
	n2.NRInsert(1);
	n2.NRInsert(10);
	n3.NRInsert(4);
	auto& n8 = n->NRInsert(8);
	n8.NRInsert(6);
	n8.NRInsert(9);
	return n;
};

void f(const NI& p_node)
{
	cout << p_node.Value() << " ";
};

void TestOfPreOrder()
{
	cout << "Test of PreOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto testData = GetTestData1();
		PreOrder::Recursive<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto testData = GetTestData1();
		PreOrder::NonRecursive<int>(*testData, f);
		cout << "\n";
	}
}

void TestOfPostOrder()
{
	cout << "Test of PostOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto testData = GetTestData1();
		PostOrder::Recursive<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive, 2 stacks\n";
		auto testData = GetTestData1();
		PostOrder::NonRecursive2<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto testData = GetTestData1();
		PostOrder::NonRecursive<int>(*testData, f);
		cout << "\n";
	}
}

void TestOfInOrder()
{
	cout << "Test of InOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto testData = GetTestData1();
		InOrder::Recursive<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto testData = GetTestData1();
		InOrder::NonRecursive<int>(*testData, f);
		cout << "\n";
	}
}

void TestOfLevelOrder()
{
	cout << "Test of LevelOrder Traversal\n";
	{
		cout << "Recursive\n";
		auto testData = GetTestData1();
		LevelOrder::RecursiveFV<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "Non Recursive\n";
		auto testData = GetTestData1();
		LevelOrder::RecursiveFV<int>(*testData, f);
		cout << "\n";
	}
}

void TestOfRecursiveTraversal()
{
	cout << "Test of Recursive Traversal\n";
	{
		cout << "PreOrder\n";
		const auto& testData = GetTestData1();
		PreOrder::Recursive<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "PostOrder\n";
		const auto& testData = GetTestData1();
		PostOrder::Recursive<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "InOrder\n";
		const auto& testData = GetTestData1();
		InOrder::Recursive<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "LevelOrder\n";
		const auto& testData = GetTestData1();
		LevelOrder::RecursiveFV<int>(*testData, f);
		cout << "\n";
	}
}

void TestOfNonRecursiveTraversal()
{
	cout << "Test of Non Recursive Traversal\n";
	{
		cout << "PreOrder\n";
		const auto& testData = GetTestData1();
		PreOrder::NonRecursive<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "PostOrder, 2 stacks\n";
		const auto& testData = GetTestData1();
		PostOrder::NonRecursive2<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "PostOrder\n";
		const auto& testData = GetTestData1();
		PostOrder::NonRecursive<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "InOrder\n";
		const auto& testData = GetTestData1();
		InOrder::NonRecursive<int>(*testData, f);
		cout << "\n";
	}
	{
		cout << "LevelOrder\n";
		const auto& testData = GetTestData1();
		LevelOrder::NonRecursive<int>(*testData, f);
		cout << "\n";
	}
}

void TestOfRecursiveDiameter()
{
	cout << "Test of Recursive Diameter\n";
	{
		cout << "Diameter\n";
		const auto& testData = GetTestData1();
		cout << GetDiameter::Recursive<int>(*testData);
		cout << "\n";
	}
	{
		cout << "Diameter 2\n";
		const auto& testData = GetTestData1();
		unsigned dim = 0;
		cout << GetDiameter::Recursive<int>(*testData, dim);
		cout << "\n";
		cout << dim + 1;
		cout << "\n";
	}
}

void TestOfRecursiveGetNumberOfLeaves()
{
	cout << "Test of Recursive GetNumberOfLeaves\n";
	{
		cout << "NumberOfLeaves\n";
		const auto testData = GetTestData1();
		cout << GetNumberOfLeaves::Recursive<int>(*testData);
		cout << "\n";
	}
}

void TestOfRecursiveAreEqual()
{
	cout << "Test of Recursive AreEqual\n";
	{
		cout << "AreEqual\n";
		const auto testData11 = GetTestData1();
		const auto testData12 = GetTestData1();
		cout << (AreEqual::Recursive<int>(*testData11, *testData12) ? "yes" : "no");
		cout << "\n";
		const auto testData21 = GetTestData1();
		const auto testData22 = GetTestData2();
		cout << (AreEqual::Recursive<int>(*testData21, *testData22) ? "yes" : "no");
		cout << "\n";
	}
}

void TestOfHasSum()
{
	cout << "Test of Recursive HasSum\n";
	{
		cout << "HasSum\n";
		const auto testData1 = GetTestData1();
		{
			const auto sum = 12;
			cout << sum << " " << (HasSum::Recursive(*testData1, sum) ? "yes" : "no");
			cout << "\n";
		}
		{
			const auto sum = 11;
			cout << sum << " " << (HasSum::Recursive(*testData1, sum) ? "yes" : "no");
			cout << "\n";
		}
	}
}

void TestOfPrintAllPaths()
{
	cout << "Test of Recursive PrintAllPaths\n";
	{
		cout << "PrintAllPaths\n";
		const auto testData = GetTestData1();
		PrintAllPaths::Recursive<int>(*testData);
	}
}

int main()
{
	//TestOfPreOrder();
	//TestOfPostOrder();
	//TestOfInOrder();
	//TestOfLevelOrder();
	//TestOfRecursiveTraversal();
	//TestOfNonRecursiveTraversal();
	//TestOfRecursiveDiameter();
	//TestOfRecursiveGetNumberOfLeaves();
	//TestOfRecursiveAreEqual();
	//TestOfPrintAllPaths();
	TestOfHasSum();

	std::cout << "Hello World!\n";
}
