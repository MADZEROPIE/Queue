//Please don't ask why... I saw something like that quite some time ago and wanted to try.
//AND WELL... I LOVE CAPS LOCK
//ENGLISH, ************, DO YOU SPEAK IT?!

#include <iostream> //for output

template<typename T>
class DataKey { //that's some s***
public:
	T data; // probably better use only key w/o data
	int key;
	DataKey(int _key, T& _d = T()) { key = _key; data = _d; }
	bool operator==(const DataKey& b) const { return key == b.key; }
	bool operator>(const DataKey& b) const { return key > b.key; }
	bool operator<(const DataKey& b) const { return key < b.key; }

	bool operator==(const int _key) const { return key == _key; }
	bool operator>(const int _key) const { return key > _key; }
	bool operator<(const int _key) const { return key < _key; }
};

//TODO: RETURN TEMPLATE DATA
template<typename heir> //that's some s*** x2
class Tree {
public:

	Tree(){ static_assert (std::is_base_of<Tree, heir>::value, "YOUR WAIFU IS NOT REAL"); } //CHANGE BEFORE RELEASE
	class Node
	{
	public:
		int data;//key+data
		Node* left = nullptr;
		Node* right = nullptr;
		short int height = 1; // Where is no(?) use for it in other trees, but who cares...
		Node(int _data): data(_data){}
	};

	void insert(int a) { static_cast<heir>(this)->insert(a); }

	Node* search(const int _key) const {
		return static_cast<heir>(this)->search(_key);
	}
	
	void erase (const int _key) {
		return static_cast<heir>(this)->erase(_key);
	}
	
};

//template<typename T>
class AVL_tree : Tree<AVL_tree>{
private:
	Node* root=nullptr;

private:
	//HEIGHT THINGS
	short int height(Node* n) { return n == nullptr ? 0 : n->height; }

	void correctHeight(Node* n) {
		auto hl = height(n->left), hr = height(n->right);
		n->height = (hl > hr ? hl : hr) + 1;
	}

	short int balanceofNode(Node* n) {
		if (n == nullptr) return 0;
		return height(n->left) - height(n->right); //IMPORTANT: LEFT-RIGHT!!!
	}

	//ROTATIONS 
	Node* fixRotation(Node* n) { 
		correctHeight(n);
		if (balanceofNode(n) ==- 2)
		{
			if (balanceofNode(n->right) > 0)
				n->right = rightRotation(n->right);
			return leftRotation(n);
		}
		else if (balanceofNode(n) == 2)
		{
			if (balanceofNode(n->left) < 0)
				n->left = leftRotation(n->left);
			return rightRotation(n);
		}
		return n;
	}

	Node* rightRotation(Node* n) {
		Node* tmp = n->left;
		n->left = tmp->right;
		tmp->right = n;
		correctHeight(n);
		correctHeight(tmp);
		return tmp;
	}

	Node* leftRotation(Node* n) {
		Node* tmp = n->right;
		n->right = tmp->left;
		tmp->left = n;
		correctHeight(n);
		correctHeight(tmp);
		return tmp;
	}

	//HIDDEN METHODS
	Node* insert(Node* n, int a) {
		if (n == nullptr) return new Node(a);
		if (a > n->data) n->right = insert(n->right, a);
		else n->left = insert(n->left, a);
		return fixRotation(n);
	}

	//DELETE NODES
	Node* eraseLeftMax(Node* n) {
		if (n->right == nullptr)
			return n->left;
		n->right = eraseLeftMax(n->right);
		return fixRotation(n);
	}

	Node* eraseRightMin(Node* n) {
		if (n->left == nullptr)
			return n->right;
		n->left = eraseRightMin(n->left);
		return fixRotation(n);
	}

	Node* erase(Node* n, int a) {
		if (n == nullptr) return n;
		if (a < n->data) n->left = erase(n->left, a);
		else if (a > n->data) n->right = erase(n->right, a);
		else { //I DON'T KNOW IF IT'S CORRECT... I JUST WANNA GO SLEEP
			Node* l = n->left, *r= n->right;
			if (l == nullptr && r==nullptr) return r;
			else if(r != nullptr){
				Node* tmp = r;
				while (tmp->left != nullptr) {
					tmp = tmp->left;
				}
				n->data = tmp->data;
				n->right = eraseRightMin(r);
				n->left = l;
				delete tmp;
				return fixRotation(n);
			}
			else {
				Node* tmp = l;
				while (tmp->right != nullptr) {
					tmp = tmp->right;
				}
				n->data = tmp->data;
				n->left = eraseLeftMax(l);
				n->right = r;
				delete tmp;
				return fixRotation(n);
			}
		}
		return fixRotation(n);
	}

	//OUTPUT  ACTUALLY VISUALLY BROKEN WITH 16+ ELEMENTS
	void printLevel(std::ostream& out, Node* n, short int level, short int count) { //This is sh*t actually...
		if (n != nullptr) {
			if (count == 1) {
				out.width(4); out << n->data;
				for (unsigned long int i = 0; i < ((unsigned long int)2 << level); ++i) out << "\t";
				//out << " | ";
			}
			else {
				printLevel(out, n->left, level, count - 1);
				printLevel(out, n->right, level, count - 1);
			}
		}
		else if (count == 1) {// In case where n != nullptr but should be n->height==level
			//out << "     | ";
			for (unsigned long int i = 0; i < ((unsigned long int)2 << level); ++i) out << "\t"; 
		}
	}

public:
	void insert(const int a) {
		root = this->insert(root, a);
	}

	void erase(int a) {
		root = erase(root, a);
	}
	
	friend std::ostream&  operator<<(std::ostream& out, AVL_tree& tr) {
		auto tree_height= tr.root== nullptr ? 0 : tr.root->height;
		for (short int i = 1; i <= tree_height; ++i) {
			tr.printLevel(out, tr.root, tree_height-i,i); //THAT'S LAME
			out << '\n';
		}
		return out;
	}
};