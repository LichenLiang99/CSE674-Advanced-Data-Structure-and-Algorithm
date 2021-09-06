#include <iostream>
using namespace std;

class node {
public:
	node* p_parent;
	node* p_Lchild;
	node* p_Rchild;
	int value;
	bool color; //true: red, false: black
	node() { p_parent = p_Lchild = p_Rchild = nullptr; }
	node(int i) {
		value = i;
		color = true;
		p_parent = p_Lchild = p_Rchild = nullptr;
	}
};

class tree {
public:
	node* root;
	tree() { root = nullptr; }
	void R_rotate(node* p);//R-rotate at node pointed by p
	void L_rotate(node* p);//L-rotate at node pointed by p
						   //R_rotate and L_rotate will be called by insert_node, delete_node, delete_1, delete_2A, delete_2B, and delete_3.


	void insert_node(int i);
	//For insert, the new node should be the first of nodes with the same value in the order of
	//inorder traversal.


	void delete_node(int i);
	//For delete, the deleted node shoud be the last of nodes with the same value in the order of
	//inorder traversal.
	//For replacement, always recursively replace it with predecessor, unless there is no predecessor. (In this
	//case, replace it with sucessor.)


	void delete_1(node* p); //case 1 of delete_node; p points to the p_parent of double black node
	void delete_2A(node* p); //case 2A of delete_node; p points to the p_parent of double black node
	void delete_2B(node* p);//case 2B of delete_node; p points to the p_parent of double black node
	void delete_3(node* p);//case 3 of delete_node; p points to the p_parent of double black node
						   //All other cases of delete_node should be directly implemented inside the delete_node.

						   //For the following, during traveral, print (value, color) of each node.
						   //For color, print 1 for red and 0 for black.
	void InorderPrint(node* p);//Print all nodes of a subtree rooted at node pointed by p
	void PostorderPrint(node* p);
	void PreorderPrint(node* p);
};

void tree::L_rotate(node* p) {
	node* Parent = p->p_parent;
	node* RightChild = p->p_Rchild;

	//if p does not have parent(p is root), then p's left child becomes the new root
	if (!Parent) {
		root = RightChild;
	}

	//update tree
	else if (Parent->p_Lchild == p) {
		Parent->p_Lchild = RightChild;
	}
	else if (Parent->p_Rchild == p) {
		Parent->p_Rchild = RightChild;
	}

	RightChild->p_parent = Parent;
	p->p_parent = RightChild;
	p->p_Rchild = RightChild->p_Lchild;
	if (RightChild->p_Lchild) {
		RightChild->p_Lchild->p_parent = p;
	}

	RightChild->p_Lchild = p;

}



void tree::R_rotate(node* p) {


	node* Parent = p->p_parent;
	node* LeftChild = p->p_Lchild;

	//if p does not have parent(p is root), then p's left child becomes the new root
	if (!Parent) {
		root = LeftChild;
	}

	//update tree
	else if (Parent->p_Lchild == p) {
		Parent->p_Lchild = LeftChild;
	}
	else if (Parent->p_Rchild == p) {
		Parent->p_Rchild = LeftChild;
	}

	LeftChild->p_parent = Parent;
	p->p_parent = LeftChild;
	p->p_Lchild = LeftChild->p_Rchild;
	if (LeftChild->p_Rchild) {
		LeftChild->p_Rchild->p_parent = p;
	}

	LeftChild->p_Rchild = p;
}

//Inorder, preorder, postorder: if color = true, print 1(red), else 0(black). stop when pointer is null
void tree::InorderPrint(node* p) {
	if (!p) {
		return;
	}

	InorderPrint(p->p_Lchild);
	if (p->color) {
		cout << "(" << p->value << "," << "1" << ") ";
	}
	else {
		cout << "(" << p->value << "," << "0" << ") ";
	}
	InorderPrint(p->p_Rchild);
}

void tree::PreorderPrint(node* p) {
	if (!p) {
		return;
	}

	if (p->color) {
		cout << "(" << p->value << "," <<  "1" << ") ";
	}
	else {
		cout << "(" << p->value << "," << "0" << ") ";
	}

	PreorderPrint(p->p_Lchild);
	PreorderPrint(p->p_Rchild);
}

void tree::PostorderPrint(node* p) {
	if (!p) {
		return;
	}

	PostorderPrint(p->p_Lchild);
	PostorderPrint(p->p_Rchild);
	if (p->color) {
		cout << "(" << p->value << "," << "1" << ") ";
	}
	else {
		cout << "(" << p->value << "," << "0" << ") ";
	}
}



void tree::insert_node(int i) {

	//if no root, assign to new root and set black
	if (root == NULL) {
		root = new node(i);
		root->color = false;
		return;
	}


	node* ParentNode = root;

	//find insert positions
	while (ParentNode != NULL){
		if (i < ParentNode->value){
			if (ParentNode->p_Lchild == NULL){
				break;
			}
			else{
				ParentNode = ParentNode->p_Lchild;
			}
		}
		else{
			if (ParentNode->p_Rchild == NULL){
				break;
			}
			else{
				ParentNode = ParentNode->p_Rchild;
			}
		}
	}

	
	node* newNode = new node(i);

	//compare value and assign newNode to either left or right side
	if (i >= ParentNode->value){
		ParentNode->p_Rchild = newNode;
		newNode->p_parent = ParentNode;

	}
	else{
		ParentNode->p_Lchild = newNode;
		newNode->p_parent = ParentNode;
	}



balance:
	ParentNode = newNode->p_parent;

	//if parent is black, return
	if (!ParentNode->color) {
		return;
	}


	node* Uncle;

	//find uncle
	if (ParentNode->p_parent->p_Lchild == ParentNode) {
		Uncle = ParentNode->p_parent->p_Rchild;
	}
	else {
		Uncle = ParentNode->p_parent->p_Lchild;
	}


	if (Uncle) { 

		//if uncle is red, change grandparent, parent, uncle's color
		if (Uncle->color) {  
			ParentNode->p_parent->color = true; 
			ParentNode->color = false;
			Uncle->color = false;

			//if grandparent does not have parent(is root), change to black
			if (!ParentNode->p_parent->p_parent) {
				ParentNode->p_parent->color = false;
				return;
			}

			//if grandparent's father's color is black, do nothing
			if (!ParentNode->p_parent->p_parent->color) {
				return;
			}

			//if grandparent's father's color is red, see grandparent as new node and restart balance
			if (ParentNode->p_parent->p_parent->color) {
				newNode = ParentNode->p_parent;
				goto balance;
			}
		}
	}


	//if parent node is left child of grandparent
	if (ParentNode->p_parent->p_Lchild == ParentNode) {

		//if newNode is left child of parent node, LL case
		if (ParentNode->p_Lchild == newNode) {
			ParentNode->color = false;
			ParentNode->p_parent->color = true;
			R_rotate(ParentNode->p_parent);
			return;
		}

		//if newNode is right child of parent, LR case
		if (ParentNode->p_Rchild == newNode) {
			node* gp = ParentNode->p_parent;
			L_rotate(ParentNode);
			newNode->color = false;
			gp->color = true;
			R_rotate(gp);
			return;
		}
	}


	//if parent node is right child of grandparent
	if (ParentNode->p_parent->p_Rchild == ParentNode) {

		//if newNode is right child of parent node, RR case
		if (ParentNode->p_Rchild == newNode) {
			ParentNode->color = false;
			ParentNode->p_parent->color = true;
			L_rotate(ParentNode->p_parent);
			return;
		}


		//if newNode is right child of parent node, RL case
		if (ParentNode->p_Lchild == newNode) {
			node* gp = ParentNode->p_parent;
			R_rotate(ParentNode);
			newNode->color = false;
			gp->color = true;
			L_rotate(gp);
			return;
		}
	}


	return;
}

//sibling is black, at least one sibling's child is red.
void tree::delete_1(node* p) {

	if (p == root) {
		return;
	}

	node* Sibling = nullptr;
	node* Parent = p->p_parent;

	//when delete node is on the right
	if (Parent->p_Rchild == p) {
		Sibling = Parent->p_Lchild;

		//LL, update color and rotate
		if (Sibling->p_Lchild && Sibling->p_Lchild->color) {
			Sibling->p_Lchild->color = Sibling->color;
			Sibling->color = Parent->color;
			R_rotate(Parent);
		}


		//LR, update color and rotate
		else if (Sibling->p_Rchild && Sibling->p_Rchild->color) {
			Sibling->p_Rchild->color = Parent->color;
			L_rotate(Sibling);
			R_rotate(Parent);
		}
	}

	//when delete node in on the left
	else if (Parent->p_Lchild == p) {
		Sibling = Parent->p_Rchild;

		//RR, update color and rotate
		if (Sibling->p_Rchild && Sibling->p_Rchild->color) {
			Sibling->p_Rchild->color = Sibling->color;
			Sibling->color = Parent->color;
			L_rotate(Parent);
		}
		//RL, update color and rotate
		else if (Sibling->p_Lchild && Sibling->p_Lchild->color) {
			Sibling->p_Lchild->color = Parent->color;
			R_rotate(Sibling);
			L_rotate(Parent);
		}
	}
	Parent->color = false;
	return;
}

//parent and sibling are black, sibling has no red child
void tree::delete_2A(node* p) {

	if (p == root) {
		return;
	}

	node* Sibling = nullptr;
	node* Parent = p->p_parent;

	//find sibling and set to red
	if (Parent->p_Lchild == p) {
		Sibling = Parent->p_Rchild;
	}
	else {
		Sibling = Parent->p_Lchild;
	}

	Sibling->color = true;


	p = Parent;
	Parent = p->p_parent;

	//find new sibling
	if (Parent && Parent->p_Lchild == p) {
		Sibling = Parent->p_Rchild;
	}
	else if (Parent) {
		Sibling = Parent->p_Lchild;
	}

	//if sibling is black and one of sibling's child is red. delete condition 1
	if ((Sibling && !Sibling->color) && ((Sibling->p_Lchild && Sibling->p_Lchild->color) || (Sibling->p_Rchild && Sibling->p_Rchild->color))) {
		delete_1(p);
	}

	//if sibling is black, check parent's color and correspond to delete condition 2A or 2B
	else if (Sibling && !Sibling->color) {
		if (Parent && !Parent->color) {
			delete_2A(p);
		}
		else if (Parent && Parent->color) {
			delete_2B(p);
		}
	}

	// sibling is red, delete condition 3
	else if (Sibling && Sibling->color) {
		delete_3(p);
	}
}

//parent is red, sibling is black, sibling has no red child
void tree::delete_2B(node* p) {

	if (p == root) {
		return;
	}
	node* Sibling = nullptr;
	node* Parent = p->p_parent;

	//find sibling and update color
	if (Parent->p_Lchild == p) {
		Sibling = Parent->p_Rchild;
	}
	else {
		Sibling = Parent->p_Lchild;
	}
	Sibling->color = true;
	Parent->color = false;
	return;
}

//sibling is red
void tree::delete_3(node* p) {

	if (p == root) {
		return;
	}

	node* Sibling = nullptr;
	node* Parent = p->p_parent;

	//find sibling, update color and rotate
	if (Parent->p_Lchild == p) {
		Sibling = Parent->p_Rchild;
		Parent->color = true;
		Sibling->color = false;
		L_rotate(Parent);
	}
	else if (Parent->p_Rchild == p) {
		Sibling = Parent->p_Lchild;
		Parent->color = true;
		Sibling->color = false;
		R_rotate(Parent);
	}

	//find new sibling
	if (Parent->p_Lchild == p) {
		Sibling = Parent->p_Rchild;
	}
	else {
		Sibling = Parent->p_Lchild;
	}

	///if sibling is black and sibling has red child, condition 1
	if ((Sibling && !Sibling->color) && ((Sibling->p_Lchild && Sibling->p_Lchild->color) || (Sibling->p_Rchild && Sibling->p_Rchild->color))) {
		delete_1(p);
	}

	//if sibling is black, check parent's color and correspond to delete condition 2A or 2B
	else if (Sibling && !Sibling->color) {
		if (Parent && !Parent->color) {
			delete_2A(p);
		}
		else if (Parent && Parent->color) {
			delete_2B(p);
		}
	}
	return;
}

void tree::delete_node(int i) {
	node* p = nullptr;
	node* temp = nullptr;
	node* Replace = nullptr;
	node* Parent = nullptr;
	node* Sibling = nullptr;
	temp = root;


	int double_black = 0;
	node* st[1010];
	int pos = -1;

	//find node that need to be deleted
	while (temp || pos != -1) {

		if (temp && temp->value == i) {
			p = temp;
			break;
		}

		while (temp) {
			st[++pos] = temp;
			temp = temp->p_Lchild;
		}

		temp = st[pos];
		if (temp && temp->value == i) {
			p = temp;
			break;
		}
		pos--;
		temp = temp->p_Rchild;
	}


	if (!p) {
		return;
	}

	//find replace node
	//if node has 2 child, find predecessor
	if (p->p_Lchild && p->p_Rchild) { 
		temp = p->p_Lchild;

		while (temp->p_Rchild) {
			temp = temp->p_Rchild;
		}
		if (temp) {
			Replace = temp;
		}
	}

	//if node has only 1 left child
	else if (p->p_Lchild) {
		Replace = p->p_Lchild;
	}

	//if node has only 1 right child
	else if (p->p_Rchild) {
		Replace = p->p_Rchild;
	}


	Parent = p->p_parent;
	double_black = (!Replace || !Replace->color) && !p->color;
	
	//if didn't find replacement. i.e. p is leaf
	if (!Replace) {

		//if p is root
		if (p == root) {
			root = nullptr;
		}


		else {

			if (!double_black) {

				//if p is left child, change sibling color
				if (p == Parent->p_Lchild) {
					Sibling = Parent->p_Rchild;
					if (Sibling) {
						Sibling->color = true;
					}
				}

				//if p is right child, change sibling color
				else if (p == Parent->p_Rchild) {
					Sibling = Parent->p_Lchild;
					if (Sibling)
						Sibling->color = true;
				}
			}

			//double black case
			else {

				//find sibling
				if (p == Parent->p_Lchild) {
					Sibling = Parent->p_Rchild;
				}
				else if (p == Parent->p_Rchild) {
					Sibling = Parent->p_Lchild;
				}

				//check for which case of delete case is propagated (similar to above)
				if ((Sibling && !Sibling->color) && ((Sibling->p_Lchild && Sibling->p_Lchild->color) || (Sibling->p_Rchild && Sibling->p_Rchild->color))) {
					delete_1(p);
				}
				else if (Sibling && !Sibling->color) {
					if (Parent && !Parent->color) {
						delete_2A(p);
					}
					else if (Parent && Parent->color) {
						delete_2B(p);
					}
				}
				else if (Sibling && Sibling->color) {
					delete_3(p);
				}
			}

			//after leaf is deleted, update pointer
			if (p == Parent->p_Lchild) {
				Parent->p_Lchild = nullptr;
			}
			else {
				Parent->p_Rchild = nullptr;
			}

			delete p;
			return;
		}
	}


	//if node need to be deleted has child
	else if (!p->p_Lchild || !p->p_Rchild) {

		//if p is root, update pointers
		if (p == root) {
			p->p_Lchild = nullptr;
			p->p_Rchild = nullptr;
			Replace->p_parent = nullptr;
			Replace->color = false;
			root = Replace;
			delete p;
		}

		else {

			//update pointers
			if (Parent->p_Lchild == p) {
				Parent->p_Lchild = Replace;
			}
			else {
				Parent->p_Rchild = Replace;
			}
			delete p;
			Replace->p_parent = Parent;

			
			if (!double_black) {
				Replace->color = false;
			}

			//if double black case
			else {

				//find sibling
				if (p == Parent->p_Lchild) {
					Sibling = Parent->p_Rchild;
				}
				else if (p == Parent->p_Rchild) {
					Sibling = Parent->p_Lchild;
				}

				//check for which case of delete case is propagated (similar to above)
				if ((Sibling && !Sibling->color) && ((Sibling->p_Lchild && Sibling->p_Lchild->color) || (Sibling->p_Rchild && Sibling->p_Rchild->color))) {
					delete_1(p);
				}
				else if (Sibling && !Sibling->color) {
					if (Parent && !Parent->color) {
						delete_2A(p);
					}
					else if (Parent && Parent->color) {
						delete_2B(p);
					}
				}
				else if (Sibling && Sibling->color) {
					delete_3(p);
				}
			}
		}
	}

	//exchange value and delete p
	else {
		int temp_value = Replace->value;
		Replace->value = p->value;
		p->value = temp_value;
		delete_node(Replace->value);
	}
}


int main() {
	tree tt;
	//test 1
	tt.insert_node(100);
	tt.insert_node(50);
	tt.insert_node(200);
	tt.insert_node(155);
	tt.insert_node(300);
	tt.InorderPrint(tt.root); //(50 0)(100 0)(155 1)(200 0)(300 1)
	cout << endl;

	tt.insert_node(140);
	tt.insert_node(130);
	tt.InorderPrint(tt.root);//(50 0)(100 0)(130 1)(140 0)(155 1)(200 1)(300 0)
	cout << endl;

	tt.insert_node(120);
	tt.InorderPrint(tt.root);//(50 0)(100 1)(120 1)(130 0)(140 0)(155 0)(200 1)(300 0)
	cout << endl;

	tt.delete_node(50);
	tt.InorderPrint(tt.root);//(100 0)(120 1)(130 0)(140 0)(155 0)(200 1)(300 0)
	cout << endl;

	tt.delete_node(155);
	tt.InorderPrint(tt.root);//(100 0)(120 1)(130 0)(140 0)(200 0)(300 1)
	cout << endl;

	tt.insert_node(400);
	tt.insert_node(150);
	tt.insert_node(210);
	tt.insert_node(220);
	tt.InorderPrint(tt.root);//(100 0)(120 0)(130 0)(140 0)(150 0)(200 1)(210 0)(220 1)(300 0)(400 0)
	cout << endl;

	tt.delete_node(400);
	tt.InorderPrint(tt.root);//(100 0)(120 0)(130 0)(140 0)(150 0)(200 0)(210 0)(220 1)(300 0)
	cout << endl;

	tt.delete_node(130);
	tt.InorderPrint(tt.root);//(100 1)(120 0)(140 0)(150 0)(200 0)(210 0)(220 0)(300 0)
	cout << endl << endl;

	//small test 1
	tree t1;
	t1.insert_node(10);
	t1.insert_node(9);
	t1.insert_node(8);
	t1.insert_node(7);
	t1.InorderPrint(t1.root);//(7 1)(8 0)(9 0)(10 0)
	cout << endl;

	t1.delete_node(10);
	t1.InorderPrint(t1.root);//(7 0)(8 0)(9 0)
	cout << endl << endl;

	//small test 2
	tree t2;
	t2.insert_node(15);
	t2.insert_node(2);
	t2.insert_node(3);
	t2.insert_node(17);
	t2.insert_node(16);
	t2.insert_node(18);
	t2.InorderPrint(t2.root);//(2 0)(3 0)(15 0)(16 1)(17 0)(18 1)
	cout << endl;

	t2.delete_node(2);
	t2.InorderPrint(t2.root);//(3 0)(15 1)(16 0)(17 0)(18 1)
	cout << endl << endl;

	//small test 3
	tree t3;
	t3.insert_node(15);
	t3.insert_node(20);
	t3.insert_node(25);
	t3.insert_node(30);
	t3.InorderPrint(t3.root);//(15 0)(20 0)(25 0)(30 1)
	cout << endl;

	t3.delete_node(30);
	t3.InorderPrint(t3.root);//(15 0)(20 0)(25 0)
	cout << endl;

	t3.delete_node(25);
	t3.InorderPrint(t3.root);//(15 1)(20 0)
	cout << endl << endl;

	//test 3
	tree t;
	t.insert_node(15);
	t.insert_node(2);
	t.insert_node(3);
	t.insert_node(17);
	t.insert_node(16);
	t.insert_node(18);
	t.insert_node(65);
	t.insert_node(13);
	t.insert_node(21);
	t.insert_node(89);
	t.insert_node(71);
	t.insert_node(75);
	t.insert_node(83);
	t.insert_node(26);
	t.insert_node(54);
	t.insert_node(99);
	t.insert_node(63);
	t.insert_node(39);
	t.insert_node(46);
	t.insert_node(52);
	t.insert_node(39);
	t.InorderPrint(t.root);
	cout << endl;
	//(2 0)(3 0)(13 1)(15 0)(16 1)(17 0)(18 0)(21 0)(26 0)(39 0)(39 1)(46 1)(52 0)(54 0)(63 0)(65 1)(71 0)(75 0)(83 1)(89 0)(99 1)

	t.PostorderPrint(t.root);
	cout << endl;

	t.PreorderPrint(t.root);
	cout << endl;


	t.delete_node(71);
	t.InorderPrint(t.root);
	cout << endl;
	//(2 0)(3 0)(13 1)(15 0)(16 1)(17 0)(18 0)(21 0)(26 0)(39 0)(39 1)(46 1)(52 0)(54 0)(63 0)(65 1)(75 0)(83 1)(89 0)(99 0)

	t.delete_node(99);
	t.InorderPrint(t.root);
	cout << endl;
	//(2 0)(3 0)(13 1)(15 0)(16 1)(17 0)(18 0)(21 0)(26 0)(39 0)(39 1)(46 1)(52 0)(54 0)(63 0)(65 1)(75 0)(83 0)(89 0)

	t.delete_node(52);
	t.InorderPrint(t.root);
	cout << endl;
	//(2 0)(3 0)(13 1)(15 0)(16 1)(17 0)(18 0)(21 0)(26 0)(39 0)(39 1)(46 0)(54 0)(63 0)(65 1)(75 0)(83 0)(89 0)

	t.delete_node(63);
	t.InorderPrint(t.root);
	cout << endl;
	//(2 0)(3 0)(13 1)(15 0)(16 1)(17 0)(18 0)(21 0)(26 0)(39 0)(39 0)(46 1)(54 0)(65 1)(75 0)(83 0)(89 0)

	t.delete_node(26);
	t.InorderPrint(t.root);
	cout << endl;
	//(2 0)(3 1)(13 1)(15 0)(16 0)(17 1)(18 0)(21 0)(39 0)(39 0)(46 1)(54 0)(65 1)(75 0)(83 0)(89 0)

	t.delete_node(16);
	t.InorderPrint(t.root);
	cout << endl;
	//(2 0)(3 1)(13 0)(15 0)(17 1)(18 0)(21 0)(39 0)(39 0)(46 1)(54 0)(65 1)(75 0)(83 0)(89 0)

	t.delete_node(65);
	t.InorderPrint(t.root);
	cout << endl;
	//(2 0)(3 1)(13 0)(15 0)(17 1)(18 0)(21 0)(39 0)(39 0)(46 0)(54 1)(75 0)(83 0)(89 0)

	t.delete_node(39);
	t.InorderPrint(t.root);
	cout << endl;
	//(2 0)(3 1)(13 0)(15 0)(17 1)(18 0)(21 0)(39 0)(46 1)(54 0)(75 0)(83 1)(89 0)

	t.delete_node(2);
	t.InorderPrint(t.root);
	cout << endl;
	//(3 0)(13 1)(15 0)(17 1)(18 0)(21 0)(39 0)(46 1)(54 0)(75 0)(83 1)(89 0)

	t.delete_node(21);
	t.InorderPrint(t.root);
	cout << endl;
	//(3 0)(13 1)(15 0)(17 0)(18 0)(39 0)(46 1)(54 0)(75 0)(83 1)(89 0)

	t.delete_node(39);
	t.InorderPrint(t.root);
	cout << endl;
	//(3 0)(13 1)(15 0)(17 0)(18 0)(46 0)(54 0)(75 0)(83 1)(89 0)

	t.delete_node(46);
	t.InorderPrint(t.root);
	cout << endl;
	//(3 0)(13 1)(15 0)(17 0)(18 0)(54 0)(75 1)(83 0)(89 0)

	t.delete_node(17);
	t.InorderPrint(t.root);
	cout << endl;
	//(3 0)(13 0)(15 0)(18 0)(54 0)(75 1)(83 0)(89 0)

	return 0;
}
