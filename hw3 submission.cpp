//HW3:  Implement DFS and BFS as described in class.
//Due:  11:50PM, Wednesday, September 23.
//You should not add more functions.

#include <iostream>
#include <vector>
#include <queue>//bfs, in-queue
#include <stack>//dfs

using namespace std;

class pos{
public:
	//indicate whether a location has been visited, and its previous location
	int visited;
	pair<char, int> prev;
};


void BFS(vector<vector<pos>>& board, pair<char, int> start_pos, pair <char, int> end_pos);
void DFS_r(vector<vector<pos>>& board, stack<pair<int, int>>& Stack_r, pair<char, int> start_pos, pair <char, int> end_pos);
void DFS_nr(vector<vector<pos>>& board, pair<char, int> start_pos, pair <char, int> end_pos);

int main(){

	//reset position by set its visited to 0
	pos ResetPosition;
	ResetPosition.visited = 0;

	vector<vector<pos>> board(8, vector<pos>(8));// pos b[8][8];

	//initialize 8x8 board by seting all to non-visited
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			board[i][j] = ResetPosition;
		}
	}


	pair<char, int> StartPosition, EndPosition;
	cout << "Enter start position --  x is in a ... h and y is in 1 ... 8" << endl;
	cin >> StartPosition.first >> StartPosition.second;

	cout << "Enter end position -- x is in a ... h and y is in 1 ... 8" << endl;
	cin >> EndPosition.first >> EndPosition.second;


	BFS(board, StartPosition, EndPosition);

	//reset board
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			board[i][j] = ResetPosition;
		}
	}


	DFS_nr(board, StartPosition, EndPosition);

	stack<pair<int, int>> Stack_r;

	//reset board
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			board[i][j] = ResetPosition;
		}
	}

	DFS_r(board, Stack_r, StartPosition, EndPosition);

	return 0;
}


void BFS(vector<vector<pos>>& board, pair<char, int> StartPosition, pair <char, int> EndPosition)
{


	//all 8 possible next position
	pair<char, int> PossiblePosition[8]{
		pair<char, int>(2,1), pair<char, int>(1,2), pair<char, int>(-1,2), pair<char, int>(-2,1),
		pair<char, int>(-2,-1), pair<char, int>(-1,-2), pair<char, int>(1,-2), pair<char, int>(2,-1),
	};


	int x, y;
	pair<char, int> CurrentPosition;
	queue< pair<char, int> > q;

	//push in the start position
	//as long as the queue is not empty..
	q.push(StartPosition);
	while (q.empty() == false){

		//pick the first postion in queue and pop it from queue
		CurrentPosition = q.front();
		q.pop();

		//if current position is the last position, end the loop
		if (CurrentPosition == EndPosition){
			break;
		}


		// loop through the 8 possible positions
		for (int i = 0; i < 8; i++){

			//calculate the next possible position
			pair<char, int> temp = PossiblePosition[i];
			pair<char, int> NextPosition(CurrentPosition.first + temp.first, CurrentPosition.second + temp.second);

			x = NextPosition.first - 'a';
			y = NextPosition.second - 1;

			//check if the next position is allowed(i.e. not outside the board, or has already been visited)
			//if so, set to visited and record the next position's previous for back tracing. Add to queue
			if (0 <= x && x < 8 && 0 <= y && y < 8 && board[x][y].visited == 0){
				board[x][y].visited = 1;
				board[x][y].prev = CurrentPosition;
				q.push(NextPosition);
			}
		}
	}


	//if current position is the last position after breaking out from the loop.
	if (CurrentPosition == EndPosition){


		cout << "Target is reached!" << endl;

		//printing the trace backward
		//as long as current position is not the starting position, keep printing
		while (CurrentPosition != StartPosition){

			cout << "(" << CurrentPosition.first << "," << CurrentPosition.second << ")" << "<-";
			x = CurrentPosition.first - 'a';
			y = CurrentPosition.second - 1;
			CurrentPosition = board[x][y].prev;
		}
		//print the starting position
		cout << "(" << CurrentPosition.first << "," << CurrentPosition.second << ")" << endl << endl;
	}

	//failed to find path
	else{
		cout << "No path available!" << endl;
	}
}

void DFS_r(vector<vector<pos>>& board, stack<pair<int, int>>& Stack_r, pair<char, int> StartPosition, pair <char, int> EndPosition){

	int x, y;
	pair<char, int> CurrentPosition;


	Stack_r.push(StartPosition);

	//if reached end position
	if (StartPosition == EndPosition){
		cout << "Target is reached!" << endl;

		//pop positions from top of the stack and display the trace as long as the stack size > 1
		while (Stack_r.size() > 1){
			CurrentPosition = Stack_r.top();
			Stack_r.pop();
			cout << "(" << CurrentPosition.first << "," << CurrentPosition.second << ")" << "<-";
		}
		//pop the last one in the stack
		CurrentPosition = Stack_r.top();
		Stack_r.pop();
		cout << "(" << CurrentPosition.first << "," << CurrentPosition.second << ")" << endl << endl;


		//set visited to -1, indicate the search is over.
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				board[i][j].visited = -1;
			}
		}
	}
	//not at the end position
	else{

		//8 possible positions
		pair<char, int> directions[8]{
			pair<char, int>(2,1), pair<char, int>(1,2), pair<char, int>(-1,2), pair<char, int>(-2,1),
			pair<char, int>(-2,-1), pair<char, int>(-1,-2), pair<char, int>(1,-2), pair<char, int>(2,-1),
		};

		//calculate the next possible position
		for (int i = 0; i < 8; i++){
			pair<char, int> temp = directions[i];

			pair<char, int> NextPosition(StartPosition.first + temp.first, StartPosition.second + temp.second);
			x = NextPosition.first - 'a';
			y = NextPosition.second - 1;

			//check if the next position is allowed(i.e. not outside the board, or has already been visited)
			//if so, set to visited and record the next position's previous for back tracing. 
			//call recursive function.
			if (0 <= x && x < 8 && 0 <= y && y < 8 && board[x][y].visited == 0){
				board[x][y].visited = 1;
				board[x][y].prev = StartPosition;
				DFS_r(board, Stack_r, NextPosition, EndPosition);
 
				//if the search is not over, set to unvisited.
				if (board[x][y].visited != -1){
					board[x][y].visited = 0;
				}
			}
		}
	}

	//if stack not empty, pop the top one.
	if (Stack_r.empty() == false){
		Stack_r.pop();
	}
}
void DFS_nr(vector<vector<pos>>& board, pair<char, int> StartPosition, pair <char, int> EndPosition){


	//all 8 possible next positions
	pair<char, int> directions[8]{
		pair<char, int>(2,1), pair<char, int>(1,2), pair<char, int>(-1,2), pair<char, int>(-2,1),
		pair<char, int>(-2,-1), pair<char, int>(-1,-2), pair<char, int>(1,-2), pair<char, int>(2,-1),
	};

	int x, y;
	pair<char, int> CurrentPosition;

	stack< pair<char, int> > q;

	//insert start position
	q.push(StartPosition);

	//when the stack is not empty
	while (q.empty() == false){

		//pick the first postion in queue and pop it
		CurrentPosition = q.top();
		q.pop();

		//if reach end position, end loop
		if (CurrentPosition == EndPosition){
			break;
		}

		//loop through 8 possible positions
		for (int i = 0; i < 8; i++){
			pair<char, int> temp = directions[i];

			//calculate next position
			pair<char, int> NextPosition(CurrentPosition.first + temp.first, CurrentPosition.second + temp.second);
			x = NextPosition.first - 'a';
			y = NextPosition.second - 1;

			//check if the next position is allowed(i.e. not outside the board, or has already been visited)
			//if so, set to visited and record the next position's previous for back tracing. 			
			if (0 <= x && x < 8 && 0 <= y && y < 8 && board[x][y].visited == 0){
				board[x][y].visited = 1;
				board[x][y].prev = CurrentPosition;
				q.push(NextPosition);
			}
		}
	}

	//if reached the end, start printing
	if (CurrentPosition == EndPosition){
		cout << "Target is reached!" << endl;

		//keep printing until reached start position
		while (CurrentPosition != StartPosition){
			cout << "(" << CurrentPosition.first << "," << CurrentPosition.second << ")" << "<-";
			x = CurrentPosition.first - 'a';
			y = CurrentPosition.second - 1;
			CurrentPosition = board[x][y].prev;
		}
		//print start position
		cout << "(" << CurrentPosition.first << "," << CurrentPosition.second << ")" << endl << endl;
	}

	//could not find path
	else{
		cout << "No path available!" << endl;
	}
}


//The following might not represent a correct run.  It shows the required output format.
/*
Enter start position --  x is in a ... h and y is in 1 ... 8
b 3
Enter end position -- x is in a ... h and y is in 1 ... 8
f 2

Target is reached!
(f,2)<-(e,4)<-(d,2)<-(b,3)

Target is reached!
(f,2)<-(d,1)<-(c,3)<-(a,2)<-(b,4)<-(a,6)<-(c,7)<-(a,8)<-(b,6)<-(c,4)<-(e,5)<-(g,6)<-(f,8)<-(h,7)<-(f,6)<-(g,8)<-(h,6)<-(f,5)<-(d,4)<-(b,3)

No path avaiable!
*/

