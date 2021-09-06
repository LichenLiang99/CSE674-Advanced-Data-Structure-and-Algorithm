//CSE674 HW2
//Due: 11:59PM, Sunday, September 13
//Do not modify main function.
//Do not introduce new functions.
//In-place implementation is required.  External structures are not allowed.
//For example, you are not allowed to copy linked list values to outside, and then process the data and copy them
//back to linked list.


#include <iostream>
//#include <ctime>

using namespace std;

class Node {
public:
	int value;
	Node* next;
	Node* previous;
	Node(int i) { value = i; next = previous = nullptr; }
	Node() { next = previous = nullptr; }
};

class DoublyLinkedList {
public:
	Node* head;
	Node* tail;
	DoublyLinkedList() { head = tail = nullptr; }
	void makeRandomList(int m, int n);
	void printForward();
	void printBackward();

	//implement the following member function:
	void MergeSortIte(Node* p1, Node* p2);//Implement MergeSort iteratively. Sort a portion of a DoublyLinkedList, from the node pointed by p1 to the node pointed by p2.
	void MergeSortRec(Node* p1, Node* p2); //Implement MergeSort recursively. Sort a portion of DoublyLinkedList, from the node pointed by p1 to the node pointed by p2.
	void Merge(Node* p1, Node* p2, Node* p3, Node* p4); //Merge two portions of a DoublyLinkedList, one from the node pointed by p1 to the node pointed by p2 and
															  //the other from the node pointed by p3 to the node pointed by p4.
	void InsertionSort(Node* p1, Node* p2); //Implement InsertionSort.  Sort a portion of a DoublyLinkedList, from the node pointed by p1 to the node pointed by p2.

	//The function Merge can be called by functions MergeSortIte and MergeSortRec;

};


void DoublyLinkedList::MergeSortIte(Node* p1, Node* p2){

	//if there is only one number
	if (p1 == p2) {
		return;
	}

	//pointers for recording positions after a split
	Node* leftstart;
	Node* rightstart;
	Node* leftend;
	Node* rightend;

	//temp head and tail changes every iteration. Old tail is the tail of previous iteration.
	Node* tempHead = head;
	Node* tempTail = tail;
	Node* OldTail = tail;
	Node* Curr;
	Node* slowpointer;
	Node* fastpointer;


	//loops until current iteration produce the same result as previous iteration
	while (1){

		//split the list into two halves, similar to recursive method
		slowpointer = tempHead;
		fastpointer = tempHead;
		while (fastpointer != nullptr && fastpointer->next != nullptr)
		{
			slowpointer = slowpointer->next;
			fastpointer = fastpointer->next->next;
		}

		//record pointers and disconnect
		leftstart = tempHead;
		leftend = slowpointer->previous;
		rightstart = slowpointer;
		rightend = tempTail;
		leftstart->previous = nullptr;
		leftend->next = nullptr;
		rightstart->previous = nullptr;
		rightend->next = nullptr;

		Merge(leftstart, leftend, rightstart, rightend);

		//record position after the merge
		//starting from the second number, individually merge it into the left list.
		//the current pointer runs all the way until the last number, and left list(sorted list) is updated everyloop
		tempHead = head;
		tempTail = tail;
		leftstart = tempHead;
		leftend = tempHead;
		Curr = leftend->next;
		leftstart->previous = nullptr;
		leftend->next = nullptr;
		while (Curr)
		{
			rightstart = Curr;
			rightend = Curr;
			Curr = Curr->next;
			rightstart->previous = nullptr;
			rightend->next = nullptr;
			Merge(leftstart, leftend, rightstart, rightend);
			leftstart = head;
			leftend = tail;
		}

		//compare the tail, if tail is same as previous iteration, sorting is complete.
		//(current iteration produced the same result as previous iteration)
		tempHead = head;
		tempTail = tail;
		if (OldTail->value == tempTail->value)
		{
			break;
		}

		//otherwise, update the OldTail, for the next iteration's comparison
		OldTail = tempTail;
	}
	head = tempHead;
	tail = tempTail;
}

void DoublyLinkedList::MergeSortRec(Node* p1, Node* p2){

	//if there is only 1 value, set head and tail to that pointer
	if (p1 == p2) {
		head = p1;
		tail = p1;
	}
	
	/*use slow and fast pointers to locate the midway of the list.
	leftstart and leftend are pointer of beginning and end for the left portion of the list after the split
	Similar meaning for the rightstart and rightend*/
	else {
		Node* slowpointer = p1;
		Node* fastpointer = p1;
		Node* leftstart;
		Node* rightstart;
		Node* leftend;
		Node* rightend;

		//as long as the fast pointer or its next is not nullptr, keep moving the pointers.
		//Everytime the fast pointer move by 2, the slow pointer move by 1
		//when fast pointer moved its full length of the list, slowpointer is at the midway
		while (fastpointer != nullptr && fastpointer->next != nullptr)
		{
			fastpointer = fastpointer->next->next;
			slowpointer = slowpointer->next;
		}

		//record the positions
		leftstart = p1;
		leftend = slowpointer->previous;
		rightstart = slowpointer;
		rightend = p2;

		//split the list by setting the corresponding pointers' previous and next into nullptr
		leftstart->previous = nullptr;
		leftend->next = nullptr;

		rightstart->previous = nullptr;
		rightend->next = nullptr;

		//recursive for the left half and save its pointer locations
		MergeSortRec(leftstart, leftend);
		leftstart = head;
		leftend = tail;

		//recursive for the right half and save its pointer locations
		MergeSortRec(rightstart, rightend);
		rightstart = head;
		rightend = tail;

		//merge the two lists
		Merge(leftstart, leftend, rightstart, rightend);
	}
}

void DoublyLinkedList::Merge(Node* p1, Node* p2, Node* p3, Node* p4){

	//The list is initially linked in decending order, then reverse (ascending order) and linked backward.
	//Curr is the current position, CurrHead stores current head(larger value), updatedHead 
	//is used to reverse the list
	Node* CurrHead = nullptr;
	Node* Curr;
	Node* updatedHead = nullptr;

	//when both left and right lists is not empty, compare value and update current head.
	while (p1 != nullptr && p3 != nullptr)
	{
		if (p1->value < p3->value)
		{
			Curr = p1;
			p1 = p1->next;
		}
		else
		{
			Curr = p3;
			p3 = p3->next;
		}
		Curr->next = CurrHead;
		CurrHead = Curr;
	}

	//when right side list is empty, link all the numbers in the left list.
	while (p1 != nullptr)
	{
		Curr = p1;
		p1 = p1->next;
		Curr->next = CurrHead;
		CurrHead = Curr;
	}
	//when left side list is empty, link all the numbers in the right list.
	while (p3 != nullptr)
	{
		Curr = p3;
		p3 = p3->next;
		Curr->next = CurrHead;
		CurrHead = Curr;
	}

	//reverse the list by making the updatedhead the smallest value(ascending order)
	while (CurrHead != nullptr)
	{
		Curr = CurrHead;
		CurrHead = CurrHead->next;
		Curr->next = updatedHead;
		updatedHead = Curr;
	}

	
	//start from the smallest value, form the doubly linked list
	head = updatedHead;
	updatedHead->previous = nullptr;
	while (updatedHead->next != nullptr)
	{
		updatedHead->next->previous = updatedHead;
		updatedHead = updatedHead->next;
	}

	//after iteration, now updatedhead is the tail
	tail = updatedHead;
}

void DoublyLinkedList::InsertionSort(Node* p1, Node* p2) {
	/*p1 is used to track the unsorted remaining, p2 is used as current pointer, 
	p3 is used to track the already sorted portion up to p1, p4 p5 are temporary
	pointer to make sure position are not lost after updating p2*/
	Node* p3 = head, *p4, *p5;

	//p1 start from the second number. Flag 'Moved' is used to track if a number has been re-linked
	p1 = head->next;
	int Moved = 0;


	if (head == nullptr || head->next == nullptr) {
		return;
	}
	
	//p1 runs all the way until the last number
	//p2(current) is reset to p1 everyloop
	while (p1 != nullptr) {
		p2 = p1;
		p3 = head;
		Moved = 0;

		//p3 moves up to p1
		while (p3 != p1) {

			//if current value is less than the p3 value, it must be moved to the left of p3
			if (p2->value < p3->value) {
				p4 = p2->previous;

				//as long as p2 is not the last value(otherwise we have to update tail's pointer)
				if (p1->next != nullptr) {

					//if p3 is head, then p2 is the new head. Update then break out of loop
					if (p3 == head) {
						p4->next = p1->next;
						p1->next->previous = p4;
						p1 = p1->next;

						p2->next = p3;
						p2->previous = nullptr;
						p3->previous = p2;
						head = p2;
						Moved = 1;
						break;

					}
					//if p3 is not the head, then insert p2 between p3 and p3's previous
					else {
						p4->next = p1->next;
						p1->next->previous = p4;
						p1 = p1->next;

						p5 = p3->previous;
						p2->next = p3;
						p3->previous = p2;
						p2->previous = p5;
						p5->next = p2;
						Moved = 1;
						break;
					}
				}

				//if p2 is the last value
				//sorting has completed after the break;
				else {
					//if p3 is not head, then just insert p2 and update tail
					if (p3 != head) {
						p1 = nullptr;
						p4->next = nullptr;
						tail = p4;

						p5 = p3->previous;
						p2->next = p3;
						p3->previous = p2;
						p2->previous = p5;
						p5->next = p2;
						Moved = 1;
						break;
					}
					//if p3 is head, then p2 is the new head. Update both head and tail
					else {
						p1 = nullptr;
						p4->next = nullptr;
						tail = p4;

						p2->next = p3;
						p2->previous = nullptr;
						p3->previous = p2;
						head = p2;
						Moved = 1;
						break;
					}
				}
			}
			//if p2->value > p3->value, then move on with p3 until it meets p1
			else {
				p3 = p3->next;
			}
		}
		//if current value is the largest so far and didn't move, move on to next p1
		if (Moved == 0) {
			p1 = p1->next;
		}
	}
	
}

void DoublyLinkedList::makeRandomList(int m, int n) {
	//srand(time(NULL));
	for (int i = 0; i < m; i++) {
		Node* p1 = new Node(rand() % n);
		p1->previous = tail;
		if (tail) tail->next = p1;
		tail = p1;
		if (!head) head = p1;
	} 
}

void DoublyLinkedList::printForward() {
	cout << endl;
	Node* p1 = head;
	while (p1) {
		cout << p1->value << " ";
		p1 = p1->next;
	}
}

void DoublyLinkedList::printBackward() {
	cout << endl;
	Node* p1 = tail;
	while (p1) {
		cout << p1->value << " ";
		p1 = p1->previous;
	}
}

int main() {
	int a{ 50 }, b{ 30 }; //a:number of numbers, b:max number
	DoublyLinkedList d1, d2, d3;
	d1.makeRandomList(a, b);
	d1.printForward();
	d1.printBackward();

	d1.MergeSortIte(d1.head, d1.tail);
	d1.printForward();
	d1.printBackward();


	d2.makeRandomList(a, b);
	d2.printForward();
	d2.printBackward();

	d2.MergeSortRec(d2.head, d2.tail);
	d2.printForward();
	d2.printBackward();


	d3.makeRandomList(a, b);
	d3.printForward();
	d3.printBackward();

	d3.InsertionSort(d3.head, d3.tail);
	d3.printForward();
	d3.printBackward();

	return 0;
}