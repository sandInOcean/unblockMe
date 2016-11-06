#include <bits/stdc++.h>

using namespace std;

//size of board
const int m = 6;
const int n = 6;

//to represent one brick
struct brick {
	brick() {
		number = srow = scol = erow = ecol = -1;
		vertical = false;
	}
	int number, srow, scol, erow, ecol;
	bool vertical;
};

struct config {
	vector<vector<int> > board;
	struct config *parent;
	string move;
};

vector<int> brickNumber(20,0);
int brickCount = 0;

string  to_string(int t) {
	char str[12];
	sprintf(str,"%d",t);
	string temp;
	temp = str;
	return temp;
}

void printBoard(vector<vector<int> > board) {
	for(int i = 0 ; i < m; i++) {
		for(int j = 0; j < n; j++) {
			printf("%2d ", board[i][j]);
		}
		cout << "\n";
	}
	return;
}
struct brick * preProcessBoard(vector<vector<int> > &board, int index) {
	struct brick *temp;
	temp = NULL;
	int flag;
	
	if(brickNumber[index] > 0) {
		temp = new brick();
		flag = 0;
		for(int i = 0; i < m; i++) {
			for(int j = 0; j < n; j++) {
				if(board[i][j] == index) {
					temp->srow = i;
					temp->scol = j;
					temp->number = index;
					if(j+1 < n && board[i][j+1] == index) {//horizontal
						temp->vertical = false;	
						temp->erow = i;
						temp->ecol = j + brickNumber[index]-1;
					} else {//verical
						temp->vertical = true;
						temp->erow = i + brickNumber[index]-1;
						temp->ecol = j;
					}
					flag = 1;
					break;
				}
			}
			if(flag == 1) break;	
		}
		return temp;
	}
	return NULL;
}

vector<vector<int> > moveLeft(vector<vector<int> >  &oldBoard, int i) {
	struct brick *temp;
	int r,c;
	vector<vector<int> > newBoard(oldBoard);
	temp = preProcessBoard(oldBoard,i);
	if(temp == NULL || temp->vertical == true) {
		delete temp;
		newBoard.clear();
		return  newBoard;
	}
	r = temp->srow;
	c = temp->scol;
	if(r >= 0 && c  > 0) {
		if(oldBoard[r][c-1] == 0) {
			int i;
			for(i = c-1; i < temp->ecol; i++) {
				newBoard[r][i] = temp->number;	
			}
			newBoard[temp->erow][temp->ecol] = 0;
			delete temp;
			return newBoard;
		}
	}
	newBoard.clear();
	delete temp;
	return newBoard;
}

vector<vector<int> > moveRight(vector<vector<int> >  &oldBoard, int i) {
	struct brick *temp;
	int r,c;
	vector<vector<int> > newBoard(oldBoard);
	temp = preProcessBoard(oldBoard,i);
	if(temp == NULL || temp->vertical == true) {
		delete temp;
		newBoard.clear();
		return  newBoard;
	}
	r = temp->erow;
	c = temp->ecol;
	if(r < m && c  < n-1) {
		if(oldBoard[r][c+1] == 0) {
			int i;
			for(i = temp->scol+1; i <= c+1; i++) {
				newBoard[r][i] = temp->number;	
			}
			newBoard[temp->srow][temp->scol] = 0;
			delete temp;
			return newBoard;
		}
	}
	newBoard.clear();
	delete temp;
	return newBoard;
}

vector<vector<int> > moveUp(vector<vector<int> >  &oldBoard, int i) {
	struct brick *temp;
	int r,c;
	vector<vector<int> > newBoard(oldBoard);
	temp = preProcessBoard(oldBoard,i);
	if(temp == NULL || temp->vertical == false) {
		delete temp;
		newBoard.clear();
		return  newBoard;
	}
	r = temp->srow;
	c = temp->scol;
	if(r > 0 && c  >= 0) {
		if(oldBoard[r-1][c] == 0) {
			for(int i = r-1; i < temp->erow; i++) {
			//	cout << i << " " << c << endl;
				newBoard[i][c] = temp->number;	
			}
			//cout << temp->erow << " HHH\n";
			newBoard[temp->erow][temp->ecol] = 0;
			delete temp;
			return newBoard;
		}
	}
	newBoard.clear();
	delete temp;
	return newBoard;
}

vector<vector<int> > moveDown(vector<vector<int> >  &oldBoard, int i) {
	struct brick *temp;
	int r,c;
	vector<vector<int> > newBoard(oldBoard);
	temp = preProcessBoard(oldBoard,i);

	if(temp == NULL || temp->vertical == false) {
		delete temp;
		newBoard.clear();
		return  newBoard;
	}
	r = temp->erow;
	c = temp->ecol;
	if(r < m-1 && c < n) {
		if(oldBoard[r+1][c] == 0) {
			for(int i = temp->srow+1; i <= r+1; i++) {
			//	cout << i << " " << c << endl;
				newBoard[i][c] = temp->number;	
			}
			newBoard[temp->srow][temp->scol] = 0;
			delete temp;
			return newBoard;
		}
	}
	newBoard.clear();
	delete temp;
	return newBoard;
}

void solveBoard(vector<vector<int> > &board,int goal, int doorX, int doorY) {
	queue<struct config *>	que;
	vector<vector<int> > tempV;
	struct config *currBoard, *tempC;
	struct brick *temp;
	vector<struct config *> del;
	map<vector<vector<int> >,int> mp;
	bool found = false;

	currBoard = new config;
	currBoard->board = board;
	currBoard->parent = NULL;
	que.push(currBoard);
	mp[currBoard->board] = 1;
	int cnt = 0;

	while(!que.empty()) {
		cnt++;
		//cout << "H ";
		currBoard = que.front();
		que.pop();
		del.push_back(currBoard);
		temp = preProcessBoard(currBoard->board,goal);
		/*printf("Board %d\n", cnt);
		printBoard(currBoard->board);
		*/
		if((temp->erow == doorX && temp->ecol == doorY)) {
			printf("Solution Found !\n");
			found = true;
			break;
		}
		// for all bricks check all moves that can be performed
		for(int i = 1; i <= brickCount; i++) {
			tempV = moveDown(currBoard->board,i);
			if(tempV.size() > 0 && mp[tempV] <= 0) {
				tempC = new config; tempC->board = tempV; tempC->parent = currBoard; que.push(tempC);
				mp[tempV] = 1; tempC->move = to_string(i) + "-Down";
			}
			tempV = moveUp(currBoard->board,i);
			if(tempV.size() > 0 && mp[tempV] <= 0) {
				tempC = new config; tempC->board = tempV; tempC->parent = currBoard; que.push(tempC);
				 mp[tempV] = 1; tempC->move = to_string(i) + "-Up";
			}
			tempV = moveRight(currBoard->board,i);
			if(tempV.size() > 0 && mp[tempV] <= 0) {
				tempC = new config; tempC->board = tempV; tempC->parent = currBoard; que.push(tempC);
				mp[tempV] = 1; tempC->move = to_string(i) + "-Right";
			}
			tempV = moveLeft(currBoard->board,i);
			if(tempV.size() > 0 && mp[tempV] <= 0) {
				tempC = new config; tempC->board = tempV; tempC->parent = currBoard; que.push(tempC);
				mp[tempV] = 1; tempC->move = to_string(i) + "-Left";
			}
		}
		delete temp;
	}
	//cout << mp.size() << "mp size\n";
	//cout << que.size() << "q size\n";
	if(!found) { // solution not  found
		return;
	}
	// if found solution
	stack<struct config*> stk;
	vector<string> ans;// to store steps of solution

	while(currBoard != NULL) {
		stk.push(currBoard);
		currBoard = currBoard->parent;
	}
	cnt = 1;
	while(!stk.empty()) {
		currBoard = stk.top();
		stk.pop();
		ans.push_back(currBoard->move);
		//printf("Board %d \n", cnt++);
		//printBoard(currBoard->board);
	}
	cout << endl;
	// print stepwise solution
	int sc = 1;
	cout << ans[1];
	for(int i = 2; i < ans.size(); i++) {
		if(ans[i] == ans[i-1]) {
			sc++;
		} else {
			cout << " " << sc << " times\n" << ans[i];
			sc = 1;
		}
	}
	cout << " " << sc << " times\n";

	//deallocating memory
	while(!que.empty()) {
		currBoard = que.front();
		que.pop();
		delete currBoard;
	}
	for(int i = 0; i < del.size(); i++) {
		delete del[i];
	}
	return;
}

bool isCorrect(vector<vector<int> > &board, int brickNum) {
	struct brick *temp = preProcessBoard(board,brickNum);

	if(temp->srow != 2 && temp->erow != 2) return false;

	return true;
}

int main() {
	int x, doorX, doorY, brickNum;
	vector<vector<int> > board(m,vector<int> (n,0)) , tempV;
	struct brick* allbricks;
	
	printf("Enter 6*6 Board Configurations\n");
	for(int i = 0; i < m; i++) {
		for(int j = 0 ; j < n; j++) {
			scanf("%d", &x);
			board[i][j] = x;
		}
	}
	printf("Enter Brick Number you want to send out Door position is at 3,7\n");
	scanf("%d", &brickNum);

	doorX = 2, doorY = 5;

	fill(brickNumber.begin(),brickNumber.end(),0);
	
	for(int i = 0 ; i < m; i++) {
		for(int j = 0 ; j < n; j++) {
			if(board[i][j] != 0) {
				brickNumber[board[i][j]]++;
			}
		}
	}
	brickCount = 0;
	
	for(int i = 1; i < 20; i++) {
		if(brickNumber[i] == 0) { 
			brickCount = i-1;
			break;
		}
	}
	if(!isCorrect(board, brickNum) ) {
		printf("Incorrect Configuration\n");
		return 0;
	} 
	solveBoard(board,brickNum,doorX,doorY);

	return 0;
}