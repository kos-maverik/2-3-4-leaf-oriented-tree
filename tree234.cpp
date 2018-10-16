#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;
// prototypes
class Node;
class Tree;
int N;

// main node class
class Node {
public:
	friend class Tree;
	Node() {}; // empty constructor
	Node(int key1); // constructors with 1-3 keys
	Node(int key1, int key2);
	Node(int key1, int key2, int key3);

	Node *parent;
	vector <int> key;
	vector <Node *> children;

	void insertLeaf(Node *child); // insert leaf to a node
	void createKey(); // create key in parent node
	void deleteLeaf(int item); // remove leaf and its key
	Node* getChild(int item); // search for child
	bool isLeaf(); // if it doesn't contain children
	void print(string prefix, bool isTail); // pretty print tree
};


Node::Node(int key1)
{
	key.push_back(key1);
}


Node::Node(int key1, int key2)
{
	key.push_back(key1);
	key.push_back(key2);
}


Node::Node(int key1, int key2, int key3)
{
	key.push_back(key1);
	key.push_back(key2);
	key.push_back(key3);
}

// returns true if it is a leaf
bool Node::isLeaf()
{
	return (children.empty());
}

// adds a leaf to a node
// the node has to be at the second-to-last level
void Node::insertLeaf(Node* child)
{
	int i = 0;
	int item = child->key[0]; // leaf value
	if (item > children.back()->key[0]) // if bigger
	{
		// put it in the end
		key.push_back((item + children.back()->key[0]) / 2);
		children.push_back(child);
	}
	else
	{
		// find its place
		while (item > children[i]->key[0])
		{
			i++;
		}
		// put it there
		children.insert(children.begin() + i, child);

		createKey(); // parent node's key
	}
	child->parent = this;
}

// creates new parent key, average of two keys
void Node::createKey()
{
	int i = 0;
	bool done = false;
	do
	{
		if (key[i] >= children[i + 1]->key[0])
		{
			// find the key's correct placement
			done = true;
		}
	} while (!done && ++i<key.size());
	// insert key
	key.insert(key.begin() + i, (children[i]->key[0] + children[i + 1]->key[0]) / 2);
}

// deletes leaf from a node
// the node has to be at the second-to-last level
void Node::deleteLeaf(int item)
{
	int i = 0;

	if (children.back()->key[0] == item)
	{
		// if it is the last one
		children.pop_back();
		key.pop_back();
	}
	else
	{
		// find the key
		while (children[i]->key[0] != item)
		{
			i++;
		}
		children.erase(children.begin() + i);
		key.erase(key.begin() + i);
	}
}

// top-down search for the child's key
Node* Node::getChild(int item)
{
	int i;
	for (i = 0; i<key.size(); i++)
	{
		if (item <= key[i])
		{
			return children[i];
		}
	}
	return children[i];
}

// pretty print
// credits: stackoverflow.com
void Node::print(string prefix, bool isTail)
{
	int i = 0;
	cout << prefix << (isTail ? "'--" : "|--");
	cout << " | " << key[i++] << " | " << endl;
	while (i<key.size())
		cout << prefix + "   " << " | " << key[i++] << " | " << endl;
	if (!this->isLeaf()) {
		for (i = 0; i < children.size() - 1; i++) {
			children[i]->print(prefix + (isTail ? "\t" : "|\t"), false);
		}
		if (children.size() >= 1) {
			children.back()->print(prefix + (isTail ? "\t" : "|\t"), true);
		}
	}
}


// tree class
class Tree {
public:
	Tree();
	Node* root;

	bool isEmpty(); // checks if root == NULL
	bool contains(int item); // search value in the leaves
	void insertItem(int item); // insert a leaf with given value
	void split(Node *node); // split upwards
	void deleteItem(int item); // deletes the leaf with given value
	void fix(Node *node); // corrects underflow
	int numberOfNodes(Node* current); // number of nodes
	int* toIntArray(Node* current); // returns leaves' values
};


Tree::Tree()
{
	root = NULL;
}

bool Tree::isEmpty()
{
	return (root == 0);
}

bool Tree::contains(int item)
{
	Node* current = root;
	if (current == 0) // empty tree
	{
		return false;
	}
	while (!current->isLeaf()) // go down a level
	{
		current = current->getChild(item);
	}
	return (current->key[0] == item);
}


int Tree::numberOfNodes(Node* current = NULL) // default null value
{
	static int nodes; // static variable for recursive function
	int i;
	if (root == 0)
	{
		return 0; // empty tree
	}
	else if (current == 0)
	{
		current = root;
		nodes = 0;
	}
	if (!current->isLeaf())
	{
		for (i = 0; i<current->key.size(); i++)
		{
			nodes++;
			numberOfNodes(current->children[i]); // recursion
		}
		numberOfNodes(current->children[i]);
	}
	else
	{
		nodes++;
	}
	return nodes;
}

// returns an int array with the leaves' values in ascending order
int* Tree::toIntArray(Node* current = NULL)
{
	static int i, *intArray = NULL;
	int j;
	if (current == 0) // arxikopoihsh
	{
		i = 0;
		intArray = NULL;
		if (this->isEmpty())
		{
			cout << "The tree is empty.." << endl;
			return intArray;
		}
		else if (root->isLeaf()) // if the tree is a leaf
		{
			intArray = (int *)malloc(sizeof(int));
			intArray[0] = root->key[0];
			return intArray;
		}
		current = root;
	}

	if (!current->isLeaf())
	{
		for (j = 0; j<current->key.size(); j++)
		{
			toIntArray(current->children[j]); // recursion
		}
		toIntArray(current->children[j]);
	}
	else
	{
		// add another item
		intArray = (int *)realloc(intArray, (++i)*sizeof(int));
		intArray[i - 1] = current->key[0];
	}
	return intArray;
}


void Tree::deleteItem(int item)
{
	if (contains(item)) // if exists
	{
		N--;
		if (root->isLeaf()) // if the tree is a leaf
		{
			delete root;
			root = NULL;
		}
		else
		{
			Node *current = root;
			// go to parent node
			while (!current->children[0]->isLeaf())
			{
				current = current->getChild(item);
			}
			current->deleteLeaf(item);

			if (current->key.empty())
			{
				// in case of underflow
				fix(current);
			}
		}
	}
	else // if the item does not exist
	{
		cout << "Cannot find item ( " << item << " )" << endl;
	}
}


void Tree::insertItem(int item)
{
	if (!contains(item)) // if it does not already exist
	{
		N++;
		Node *newleaf = new Node(item);
		if (root == 0) // if the tree is empty
		{
			newleaf->parent = NULL;
			root = newleaf;
		}
		else if (root->isLeaf()) // if the tree is a leaf
		{
			Node *newroot = new Node((item + root->key[0]) / 2);

			if (item>root->key[0])
			{
				newroot->children.push_back(root);
				newroot->children.push_back(newleaf);
			}
			else
			{
				newroot->children.push_back(newleaf);
				newroot->children.push_back(root);
			}
			// create a new tree-leaf
			newleaf->parent = root->parent = newroot;
			root = newroot;
		}
		else
		{
			Node *current = root;

			while (!current->children[0]->isLeaf())
			{
				// find the node which contains this
				current = current->getChild(item);
			}
			current->insertLeaf(newleaf);
			if (current->key.size() == 4)
			{
				// expand the tree
				split(current);
			}
		}
	}
	else // duplicate error
	{
		cout << "Cannot insert data ( " << item << " )" << endl;
	}
}


void Tree::split(Node *node)
{
	// new left child
	Node *left = new Node(), *right = new Node();
	left->key.push_back(node->key[0]);
	left->children.push_back(node->children[0]);
	left->children.push_back(node->children[1]);
	node->children[0]->parent = node->children[1]->parent = left;

	// new right child
	right->key.push_back(node->key[2]);
	right->key.push_back(node->key[3]);
	right->children.push_back(node->children[2]);
	right->children.push_back(node->children[3]);
	right->children.push_back(node->children[4]);
	node->children[2]->parent = node->children[3]->parent =
		node->children[4]->parent = right;

	if (node == root) // if on top of the tree
	{
		Node *newparent = new Node(node->key[1]);
		newparent->children.push_back(left);
		newparent->children.push_back(right);
		left->parent = right->parent = newparent;
		root = newparent;
		root->parent = NULL;
	}
	else
	{
		int i = 0;
		Node *par = node->parent;
		while (par->children[i] != node)
		{
			i++;
		}
		// the new nodes are inserted in the parent
		par->children[i] = left;
		left->parent = par;
		par->children.insert(par->children.begin() + i + 1, right);
		right->parent = par;
		par->key.insert(par->key.begin() + i, node->key[1]);
		if (par->key.size() == 4)
		{
			// recursion
			split(par);
		}
	}
}


void Tree::fix(Node *node)
{
	if (node == root)
	{
		root = root->children[0];
		delete root->parent;
		root->parent = NULL;
	}
	else
	{
		// 2 cases:
		// merge with parent OR
		// move from neighbor node and alter parent node
		// each case has two subcases
		// one for the left neighbor and one for right
		Node *par = node->parent;
		int i = 0, j = 0;

		while (par->children[i] != node)
		{
			i++;
		}

		if (i - 1 >= 0 && par->children[i - 1]->key.size() >= 2)
		{
			// move from left left neihgbor
			Node *geitonas = par->children[--i];
			Node *newnode = new Node(geitonas->children.back()->key[0]);

			node->key.push_back(par->key[i]); // new key
			newnode->parent = node; // the parent's most right key
			node->children.insert(node->children.begin(), newnode);
			par->key[i] = geitonas->key.back(); // change parent

			geitonas->children.pop_back();
			geitonas->key.pop_back();
		}
		else if (i + 1 <= par->key.size() && par->children[i + 1]->key.size() >= 2)
		{
			// move from left neihgbor
			Node *geitonas = par->children[i + 1];
			Node *newnode = new Node(geitonas->children.front()->key[0]);

			node->key.push_back(par->key[i]);
			newnode->parent = node;
			node->children.insert(node->children.end(), newnode);
			par->key[i] = geitonas->key.front();

			geitonas->children.erase(geitonas->children.begin());
			geitonas->key.erase(geitonas->key.begin());
		}
		else if (i - 1 >= 0)
		{
			// merge with left neighbor
			Node *geitonas = par->children[--i];

			geitonas->key.push_back(par->key[i]);
			node->children[0]->parent = geitonas;
			geitonas->children.push_back(node->children[0]);
			// remove parent's key, might cause underflow
			par->key.erase(par->key.begin() + i);
			par->children.erase(par->children.begin() + i + 1);
		}
		else /* if ( i+1 <= par->key.size()) */
		{
			// merge with right neighbor
			Node *geitonas = par->children[i + 1];

			geitonas->key.insert(geitonas->key.begin(), par->key[i]);
			geitonas->children.insert
				(geitonas->children.begin(), node->children[0]);
			node->children[0]->parent = geitonas;

			par->key.erase(par->key.begin() + i);
			par->children.erase(par->children.begin() + i);
		}
		// might cause underflow again
		if (par->key.empty())
		{
			fix(par); // recursive call with parent node
		}
	}
}

// prints an array's values
void print(int a[])
{
	cout << "Leaves : [ ";
	for (int i = 0; i<N-1; i++)
	{
		cout << a[i] << ", ";
	}
	cout << a[N-1] << " ]" << endl << endl;
}


int main()
{
	Tree *mytree = new Tree(); // my tree
	int key, choice, *arr; // array with the array's values

	do
	{
		cout << "Choose one:" << endl
			<< " 0. Exit menu" << endl
			<< " 1. Add leaf" << endl
			<< " 2. Remove leaf" << endl
			<< " 3. Search for a value" << endl
			<< " 4. Print number of nodes" << endl
			<< " 5. Print list of leaves" << endl
			<< " 6. Print tree!" << endl << endl;
		cin >> choice; // input
		cout << endl;
		switch (choice)
		{
		case 0:
			cout << "Bye bye!" << endl;
			break;
		case 1:
		case 2:
		case 3:
			cout << "Give the value of the" <<
				(choice == 3 ? " node:" : " leaf:") << endl;
			cin >> key;
			if (choice == 1)
			{
				mytree->insertItem(key);
			}
			else if (choice == 2)
			{
				mytree->deleteItem(key);
			}
			else if (choice == 3)
			{
				cout << endl << "The tree " << (!mytree->contains(key) ? "doesn't have" : "has")
					<< " the value " << key << endl;
			}
			break;
		case 4:
			cout << endl << "The tree has " <<
				mytree->numberOfNodes() << " node(s)." << endl;
			break;
		case 5:
			if (arr = mytree->toIntArray())
			{
				print(arr);
			}
			break;
		case 6:
			if (mytree->isEmpty())
			{
				cout << "The tree is empty" << endl;
			}
			else
			{
				cout << endl << "Tree:" << endl << endl;
				mytree->root->print("", true);
			}

			break;
		default:
			cout << "Wrong input.." << endl;
			break;
		}
		cout << endl << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl;
		cout << endl;
	} while (choice);
	return 0;
}
