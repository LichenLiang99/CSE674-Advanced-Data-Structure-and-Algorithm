//HW4A: Dijkstra's algorithm with HEAP
//HW4B: Use the same design for Prim's algorithm
//For each x->y edges, also create a y->x edge with the same weight
//HW4A Due: Friday Oct. 2; HW4B Due: Tuesday, Oct. 6
//Complexity: O((N+E) log N)
#include <iostream>
#include <vector>
#include <list>
#include <fstream>

using namespace std;


class Node
{
public:
	int cost;  //cost to a node
	int from;  //the from node to this node
	bool checked;
	int h_index; //the index in heap for this node
	Node() : cost(9999), from(-1), checked(false), h_index(-1)
	{
	}
};

//void Path(vector<list<pair<int, int>>>& graph, vector<Node>& table, vector<int>& heap);

void MST(vector<list<pair<int, int>>>& graph, vector<Node>& table, vector<int>& heap); //HW4B


//when there is a new value added to heap, update heap by comparing and swapping with parent
void HeapMoveUp(vector<Node>& table, vector<int>& heap, int position) {

	int Parent;
	int temp, h_index;

	Parent = (position - 1) / 2;

	//when parent is not -1 and position not at top
	//there is room to swap
	while (Parent >= 0 && position >= 1) {

		//if parent > child cost, swap the two, update positions
		if (table[heap[Parent]].cost > table[heap[position]].cost) {

			table[heap[position]].h_index = Parent;
			table[heap[Parent]].h_index = position;

			temp = heap[position];
			heap[position] = heap[Parent];
			heap[Parent] = temp;

			position = Parent;
			Parent = (position - 1) / 2;
		}
		else {
			break;
		}
	}
}

//when the last element in the heap is swapped to the top,
//compare with child and keep updating until in no violation
void HeapMoveDown(vector<Node>& table, vector<int>& heap, int position, int HeapSize) {

	int Child;

	//make sure within the valid range, get its first child and compare
	while (2 * position + 1 < HeapSize) {

		Child = 2 * position + 1;

		//move to the right child if cost of left child > right child
		if (Child < HeapSize - 1 && table[heap[Child]].cost > table[heap[Child + 1]].cost) {
			Child++;
		}

		//if parent cost > child cost, swap the two and update positions
		if (table[heap[position]].cost > table[heap[Child]].cost) {

			table[heap[position]].h_index = Child;
			table[heap[Child]].h_index = position;

			int t = heap[position];
			heap[position] = heap[Child];
			heap[Child] = t;

			position = Child;
		}
		else {
			break;
		}
	}
}


int main() {

	vector<list<pair<int, int>>> graph;
	vector<Node> table;
	vector<int> heap;


	int SourceNode;
	int NumberOfNodes;
	int NumberOfEdges;

	//read in the file and firts 3 lines of data and resize the graph
	ifstream fin("graph.txt");
	fin >> SourceNode >> NumberOfNodes >> NumberOfEdges;
	graph.resize(NumberOfNodes);

	//read each pair and add to graph
	for (int i = 0; i < NumberOfEdges; i++) {
		int Source, Destination, Cost;
		fin >> Source >> Destination >> Cost;
		graph[Source].push_back(pair<int, int>(Destination, Cost));
		graph[Destination].push_back(pair<int, int>(Source, Cost)); //added for Prim to make it bidirectional
	}
	fin.close();


	//initialize
	table.resize(0);
	table.resize(NumberOfNodes);
	table[SourceNode].checked = true;
	heap.clear();

	//Prim
	MST(graph, table, heap);

}





void MST(vector<list<pair<int, int>>>& graph, vector<Node>& table, vector<int>& heap){
	//find the source node
	int Source;
	for (int i = 0; i < table.size(); i++){
		if (table[i].checked == true){
			Source = i;
		}
	}

	//initialize the source node and add to heap
	table[Source].from = Source;
	table[Source].cost = 0;
	table[Source].checked = true;
	heap.push_back(Source);

	//while heap is not empty..
	while (heap.empty() == false){

		//pick the mininum element
		int minIndex = heap[0];
		table[minIndex].h_index = -2;
		table[minIndex].checked = true;

		// remove the mininum element
		int temp = heap.back();
		heap.pop_back();

		//if heap is not empty, exchange last and first node
		//and update the heap
		if (heap.empty() == false){
			heap[0] = temp;
			table[temp].h_index = 0;
			HeapMoveDown(table, heap, 0, heap.size());
		}

		//relax each edge
		list<pair<int, int>>::iterator iter;
		for (iter = graph[minIndex].begin(); iter != graph[minIndex].end(); iter++){

			//get the destination and cost
			int Destination = iter->first;
			int cost = iter->second;

			//if the destination has not been visited and the current cost is greater than new cost,
			//then update the new cost and 'from' node
			if (table[Destination].checked == false && table[Destination].cost > cost){
				table[Destination].cost = cost;
				table[Destination].from = minIndex;

				//if not in the heap, add to heap 
				if (table[Destination].h_index == -1){
					heap.push_back(Destination);
					table[Destination].h_index = heap.size() - 1;
				}

				//update heap
				HeapMoveUp(table, heap, table[Destination].h_index);
			}
		}
	}

	//backtrace the route and calculate total
	int total = 0;
	for (int i = 0; i < table.size(); i++){

		if (i != Source && table[i].cost != 9999){
			cout << i << " " << table[i].from << " " << table[i].cost << endl;
			total += table[i].cost;
		}
	}
	cout << "The overall cost is " << total << "." << endl;
}

/*
The following screenshot might not be a correct run.  It shows the required output format.

//sample screenshot for Dijkstra's algorithm
cost from 0 to 0 is : 0  from node is 0  Path: 0
cost from 0 to 1 is : 7  from node is 2  Path: 1<-2<-0
cost from 0 to 2 is : 4  from node is 0  Path: 2<-0
cost from 0 to 3 is : 10  from node is 1  Path: 3<-1<-2<-0
cost from 0 to 4 is : 14  from node is 3  Path: 4<-3<-1<-2<-0
cost from 0 to 5 is : 9  from node is 1  Path: 5<-1<-2<-0
*/

/*
//sample input file graph.txt
0 //source code
6 //# of nodes in graph
12 //# of edges in graph
0 5 16//an edge from node 0 to node 5 with a weight (cost) of 16
0 4 15
0 2 4
0 3 12
0 1 9
1 3 3
1 5 2
2 3 14
2 1 3
3 4 4
4 5 1
5 3 2
*/


/*
//sample screenshot for Prim's algorithm; 1 2 3 is the same as 2 1 3
1 2 3
2 0 4
3 5 2
4 5 1
5 1 2
The overall cost is 12.
*/

