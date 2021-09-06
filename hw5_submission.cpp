//HW5  AVL Tree  2020
//Due: Monday (Nov. 2) at 11:59PM
//55 points
//This homework requires more efforts. You should get started as soon as possible.

#include <iostream> //to use cout
#include <algorithm> //to use max function such as  i = max(a, b);
#include <stack>

using namespace std;

//You need to use the following node class for each node of the AVL tree
class node {
public:
	int value;
	int height;//this is tree height. Leaf node is 1; empty node (i.e., NIL) is 0
	node* parent;
	node* l_child;
	node* r_child;
	bool is_left_child; //true if this node is the left child of its parent node; else false;
	bool is_right_child; //true if this node is the right child of its parent node; else false;
	node() {}
	node(int i) { value = i; height = 1; parent = l_child = r_child = nullptr; is_left_child = is_right_child = false; }
};

class avl_tree {
public:
	node* root;
	avl_tree() {
		root = nullptr;
	}

	//************************************************************************************
	//Implement the following member functions
	void add_node(int i);
	//if case of a tie, add i to the FIRST of existing nodes with value i in the in-order sequence

	void delete_node(int i);
	//Delete the node with value i.  in case of multiple nodes with value i, delete the LAST node with value i in the in-order traveral sequence.
	//If the node to delete if a leaf node, just delete it.
	//If the node is parent of only a right leaf node, replace it with the leaf node.
	//For all other cases, replace it with in-oreder predecessor.



	void in_order_traversal(node* p); //such as (2 5) (3 3) (5 2) (6 1) .....  (value height)
									  //Height of leaf node is 1.
	node* height_update(node* p, int& pattern);
	/*
	This function will be invoked by add_node and delete_node.
	p points to the first node that we need to check for possible height update.  We then need to check possible height update toward root.
	All nodes whose heights need to be updated will be performed in this function.
	The function returns a pointer.  If no imbalance is detected, the function returns nullptr; else, it will return the address of action node.
	The variable pattern will be assigned to 0, if no imbalance is detected; else it will be assigned to 1,2,3,4 for LL LR, RL, and RR pattern, respectively.
	*/

	void L_Rotate(node* p);
	//p points to the node at which the rotation will be performed.

	void R_Rotate(node* p);
	//p points to the node at which the rotation will be performed.
};


int main() {
	//Different test cases will be used during grading.
	avl_tree t1;
	t1.add_node(45);
	t1.in_order_traversal(t1.root);
	t1.delete_node(45);
	t1.in_order_traversal(t1.root);
	t1.add_node(45);
	t1.add_node(50);
	t1.add_node(46);
	t1.add_node(30);
	t1.add_node(34);

	t1.in_order_traversal(t1.root);
	t1.delete_node(48);
	t1.in_order_traversal(t1.root);

	//test delete
	//t1.add_node(60);
	//t1.in_order_traversal(t1.root);//(30 1)  (34 2) (45 1) ( 46 3) (50 2)(60 1)

	//t1.delete_node(50);
	//t1.in_order_traversal(t1.root);//(30 1)  (34 2) (45 1) ( 46 3) (60 1)

	//t1.delete_node(30);
	//t1.in_order_traversal(t1.root);//(34 2) (45 1) ( 46 3) (60 1)

	//t1.delete_node(46);
	//t1.in_order_traversal(t1.root);//(34 1) (45 2) (60 1)

	getchar();
	getchar();
	return 0;
}




void avl_tree::add_node(int i) {

	//if there is no root, create new tree and set it root
	if (root == NULL) {
		root = new node(i);
		return;
	}


	std::stack<node*> s;
	node* r = root;

	//find the node for where i should be added to and push this node to stack
	while (r != NULL) {
		s.push(r);

		if (i <= r->value) {
			if (r->l_child == NULL) {
				break;
			}
			else {
				r = r->l_child;
			}
		}
		else {
			if (r->r_child == NULL) {
				break;
			}
			else {
				r = r->r_child;
			}
		}
	}

	//create the new node, compare and add to tree, as well as update 
	node* newNode = new node(i);
	if (i > r->value) {
		r->r_child = newNode;
		newNode->is_right_child = true;
		newNode->is_left_child = false;
		newNode->parent = r;
	}
	else {
		r->l_child = newNode;
		newNode->is_right_child = false;
		newNode->is_left_child = true;
		newNode->parent = r;
	}

	//while stack not empty, pop the first
	while (s.empty() == false) {
		node* child = s.top();
		s.pop();

		//if stack now is empty, assign parent to be null, otherwise it's the next in stack
		node* Parent;
		if (s.empty() == true) {
			Parent = nullptr;
		}
		else {
			Parent = s.top();
		}

		//get the pattern and do related rotations
		int pattern;
		node* find = height_update(child, pattern);
		if (!find) {
			continue;
		}

		switch (pattern) {
		case 1:
			R_Rotate(find);
			break;
		case 2:
			L_Rotate(find->l_child);
			R_Rotate(find);
			break;
		case 3:
			R_Rotate(find->r_child);
			L_Rotate(find);
			break;
		case 4:
			L_Rotate(find);
			break;
		}
	}
	return;
}

void avl_tree::L_Rotate(node* p) {

	node* Parent = p->parent;
	node* r = p->r_child;

	//if p does not have parent(p is root), then r becomes the new root
	if (!Parent) {
		root = r;
	}

	//update relations, r now takes position of p
	r->parent = Parent;
	p->parent = r;
	p->r_child = nullptr;
	r->l_child = p;
	r->is_left_child = p->is_left_child;
	r->is_right_child = p->is_right_child;

	//if p was orginally left child of its parent, then update r and Parent relations
	if (p->is_left_child) {
		if (Parent) { 
			Parent->l_child = r; 
		}
		r->is_left_child = true;
		r->is_right_child = false;
	}

	//if p was orginally right child of its parent, then update r and Parent relations
	else if (p->is_right_child) {
		if (Parent) Parent->r_child = r;
		r->is_left_child = false;
		r->is_right_child = true;
	}

	//update p
	p->is_right_child = false;
	p->is_left_child = true;

	//get p's left and right child's height
	int l_height = 0;
	if (p->l_child != nullptr) {
		l_height = p->l_child->height;
	}

	int r_height = 0;
	if (p->r_child != nullptr) {
		r_height = p->r_child->height;
	}

	//update p's height
	if(l_height > r_height){
		p->height = l_height + 1;
	}
	else {
		p->height = r_height + 1;
	}

	//get r's childs height
	l_height = 0;
	if (r->l_child != nullptr) {
		l_height = r->l_child->height;
	}
	r_height = 0;
	if (r->r_child != nullptr) {
		r_height = r->r_child->height;
	}

	//update r's height
	if (l_height > r_height) {
		r->height = l_height + 1;
	}
	else {
		r->height = r_height + 1;
	}
}

void avl_tree::R_Rotate(node* p) {

	node* Parent = p->parent;
	node* l = p->l_child;

	//if p does not have parent(p is root), then r becomes the new root
	if (!Parent) {
		root = l;
	}

	//update relations, l now takes position of p
	l->parent = Parent;
	p->parent = l;
	p->l_child = nullptr;
	l->r_child = p;
	l->is_left_child = p->is_left_child;
	l->is_right_child = p->is_right_child;

	//if p was orginally left child of its parent, then update l and Parent relations
	if (p->is_left_child) {
		if (Parent) {
			Parent->l_child = l;
		}
		l->is_left_child = true;
		l->is_right_child = false;
	}
	//if p was orginally right child of its parent, then update l and Parent relations
	else if (p->is_right_child) {
		if (Parent) Parent->r_child = l;
		l->is_left_child = false;
		l->is_right_child = true;
	}


	//update p
	p->is_left_child = false;
	p->is_right_child = true;

	//get p's left and right child's height
	int l_height = 0;
	if (p->l_child != nullptr) {
		l_height = p->l_child->height;
	}
	int r_height = 0;
	if (p->r_child != nullptr) {
		r_height = p->r_child->height;
	}

	//update p's height
	if (l_height > r_height) {
		p->height = l_height + 1;
	}
	else {
		p->height = r_height + 1;
	}

	//get l's childs height
	l_height = 0;
	if (l->l_child != nullptr) {
		l_height = l->l_child->height;
	}
	r_height = 0;
	if (l->r_child != nullptr) {
		r_height = l->r_child->height;
	}

	//update l's height
	if (l_height > r_height) {
		l->height = l_height + 1;
	}
	else {
		l->height = r_height + 1;
	}
}

void avl_tree::delete_node(int i) {

	std::stack<node*> st;

	node* p, * ValueIsHere = nullptr, * Predecessor = nullptr, * EveryPre = nullptr;
	node* PredecessorParent = nullptr;

	p = root;

	//when p is NULL and stack is empty, stop
	while (p || !st.empty()) {

		//if p is not NULL, search left branch
		if (p) {
			st.push(p);
			p = p->l_child;
		}

		//when p is NULL, pop from stack and search right branch
		else {
			p = st.top();
			st.pop();

			//found one value
			if (p->value == i) {
				ValueIsHere = p;  
				Predecessor = EveryPre;
			}
			EveryPre = p;

			p = p->r_child;
		}
	}

	//if didn't find value, return
	if (!ValueIsHere) {
		return;
	}

	//if the node is a leaf, just delete it.
	//also update it's parent's child status
	node* h = ValueIsHere;
	if (!h->l_child && !h->r_child) {
		PredecessorParent = h->parent;
		if (h->is_left_child) {
			h->parent->l_child = nullptr;
		}
		else if (h->is_right_child) {
			h->parent->r_child = nullptr;
		}

		//if it's the only node in tree
		if (h->parent == nullptr) {
			root = nullptr;
		}
	}

	//If the node is parent of only a right leaf node, replace it with the leaf node.
	else if ((!h->l_child) && (h->r_child)) {
		PredecessorParent = h->parent;

		//update its parent's child
		if ((!h->r_child->l_child) && (!h->r_child->r_child)) {
			if (h->is_left_child) {
				h->parent->l_child = h->r_child;
			}
			else if (h->is_right_child) {
				h->parent->r_child = h->r_child;
			}

			//update its child's parent
			h->r_child->parent = h->parent;

			//if root if there is only 2 nodes and the original root is deleted
			if (h->r_child->parent == nullptr) {
				root = h->r_child;
			}
		}
	}

	//For all other cases, replace it with in-order predecessor.
	else {
		PredecessorParent = Predecessor->parent;
		if (Predecessor->is_left_child) {
			Predecessor->parent->l_child = nullptr;
		}
		if (Predecessor->is_right_child) {
			Predecessor->parent->r_child = nullptr;
		}


		//update predecessor's child
		node* f = h->parent;
		Predecessor->parent = f;
		Predecessor->l_child = h->l_child;
		Predecessor->r_child = h->r_child;

		//update child's parent to predecessor
		if (h->l_child) {
			h->l_child->parent = Predecessor;
		}
		if (h->r_child) {
			h->r_child->parent = Predecessor;
		}

		Predecessor->is_left_child = h->is_left_child;
		Predecessor->is_right_child = h->is_right_child;

		//if root if there is only 2 nodes and the original root is deleted
		if (Predecessor->parent == nullptr) {
			root = Predecessor;
		}

		//update its parent's child
		if (h->is_left_child) {
			h->parent->l_child = Predecessor;
		}
		else if (h->is_right_child) {
			h->parent->r_child = Predecessor;
		}
	}

	//balance tree
	node* PP = PredecessorParent;
	while (PP){

		//similar to add_node()
		int pattern;
		node* find = height_update(PP, pattern);
		if (!find) {
			PP = PP->parent;
			continue;
		}

		switch (pattern) {
		case 1: 
			R_Rotate(find); 
			break;
		case 2: 
			L_Rotate(find->l_child); 
			R_Rotate(find->parent); 
			break;
		case 3: 
			R_Rotate(find->r_child); 
			L_Rotate(find); 
			break;
		case 4: 
			L_Rotate(find); 
			break;
		}
		PP = PP->parent;
	}

	delete(ValueIsHere);
}


void avl_tree::in_order_traversal(node* root) {

	std::stack<node*> st;
	node* p;
	p = root;

	//when p is null or stack empty, done
	while (p || !st.empty()) {

		//if p is not NULL, search left branch
		if (p) {
			st.push(p);
			p = p->l_child;
		}

		//search right branch
		else {
			p = st.top();
			st.pop();
			cout << "(" << p->value << "," << p->height << ")\t";
			p = p->r_child;
		}
	}
	cout << endl;

}

node* avl_tree::height_update(node* child, int& pattern) {

	//get left height and right height
	int l_height = 0;
	if (child->l_child != nullptr) {
		l_height = child->l_child->height;
	}

	int r_height = 0;
	if (child->r_child != nullptr) {
		r_height = child->r_child->height;
	}

	//compare heights and get child's height
	if (l_height > r_height) {
		child->height = l_height + 1;
	}
	else {
		child->height = r_height + 1;
	}

	//calculate difference, if |difference| < 2 then no violation
	int diff = l_height - r_height;
	if ((diff < 2) && (diff > -2)) {
		pattern = 0;
		return nullptr;
	}

	//otherwise there is violation, find which kind and update pattern
	if (child->l_child){
		if (child->l_child->l_child) {
			pattern = 1;
		}
		else if (child->l_child->r_child) {
			pattern = 2;
		}
	}
	else if (child->r_child){
		if (child->r_child->l_child) {
			pattern = 3;
		}
		else if (child->r_child->r_child) {
			pattern = 4;
		}
	}
	return child;
}

