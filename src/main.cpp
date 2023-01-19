#include <iostream>
#include <sstream>
#include <fstream>
#include "extra.h"

/* Note: 
	1. You will have to comment main() when unit testing your code because catch uses its own main().
	2. You will submit this main.cpp file and any header files you have on Gradescope. 
*/

using namespace std;

string parseName(int start, string line);
bool validID(int id, MyAVLTree tree);		// checks constraints
bool checkLength(int id);
int countZeros(string line);

int main(){
	MyAVLTree tree;
	int numlines = 0;

	try {
		string numString;
		getline(cin, numString);
		numlines = stoi(numString);

		if (numlines > 1000 || numlines < 1) {
			cout << "unsuccessful\n";
			return 0;
		}
	} catch (exception e) {
		cout << "unsuccessful\n";
		return 0;
	}


	// parse input
	string line;
	while(numlines-- != 0) {
		getline(cin, line);
		string command, name;
		int id;
		istringstream input(line);
		input >> command;

		// check command
		if (command == "insert") {
			name = parseName(command.size() + 1, line);
			line = line.substr(name.size() + 10); // get id

			try {
				id = stoi(line);
				if (countZeros(line) < 0) {
					cout << "unsuccessful\n";
				} else {
					// constraints
					if (validID(id, tree) || countZeros(line) > 0) {
						// insert
						tree.insert(name, id);
						cout << "successful\n";
					} else {
						cout << "unsuccessful\n";
					}
				}
			} catch (exception e) {
				cout << "unsuccessful\n";
			}
		} else if (command == "remove") {

			try {
				id = stoi(line.substr(command.size() + 1));

				// search id
				if (tree.search(id).empty()) {		// doesnt exist
					cout << "unsuccessful\n";
				} else {							// remove node
					cout << "successful\n";
					tree.remove(id);
				}
			} catch (exception e) {
				cout << "unsuccessful\n";
			}

		} else if (command == "search") {
			if (line[command.size() + 1] == '"') {	// search by name
				name = parseName(command.size() + 1, line);
				vector<int> ids = tree.search(name);

				if (ids.size() != 0) {	// print ids
					for (int i = 0; i < ids.size(); i++) {
						string ints = to_string(ids[i]);
						if (ints.size() < 8) {
							for (int j = 0; j < 8 - ints.size(); j++) {
								cout << '0';
							}
						}
						cout << ids[i] << endl;
					}
				} else {
					cout << "unsuccessful\n";
				}
			} else {				// search by id
				line = line.substr(command.size() + 1);
				try {
					id = stoi(line);
					if (countZeros(line) < 0) {
						cout << "unsuccessful\n";
					} else {
						if (checkLength(id) || countZeros(line) > 0) {
							// search
							name = tree.search(id);
							if (name.empty()) {
								cout << "unsuccessful\n";
							} else {
								cout << name << endl;
							}
						} else {
							cout << "unsuccessful\n";
						}
					}
				} catch (exception e) {
					cout << "unsuccessful\n";
				}
			}
		} else if (command == "printInorder") {
			vector<string> inorder = tree.printInorder();

			for (int i = 0; i < inorder.size(); i++) {
				cout << inorder[i];
				if (i + 1 != inorder.size()) {
					cout << ", ";
				}
			}
			cout << "\n";
		} else if (command == "printPreorder") {
			vector<string> preorder = tree.printPreorder();

			for (int i = 0; i < preorder.size(); i++) {
				cout << preorder[i];
				if (i + 1 != preorder.size()) {
					cout << ", ";
				}
			}
			cout << "\n";
		} else if (command == "printPostorder") {
			vector<string> postorder = tree.printPostorder();

			for (int i = 0; i < postorder.size(); i++) {
				cout << postorder[i];
				if (i + 1 != postorder.size()) {
					cout << ", ";
				}
			}
			cout << "\n";
		} else if (command == "printLevelCount") {
			int count = tree.printLevelCount();
			cout << count << "\n";
		} else if (command == "removeInorder") {
			int n = stoi(line.substr(command.size() + 1));

			vector<string> inorder = tree.printInorder();

			if (inorder.size() - 1 < n || inorder.size() == 0) {
				cout << "unsuccessful\n";
			} else {
				cout << "successful\n";
				tree.removeInOrder(n);
			}
		}
	}

	return 0;
}

// get name from quotations
string parseName(int start, string line) {
	string name;
	bool complete = false;
	int index = 1;
	name = line.substr(start, line.size() - 1);

	while (!complete) {
		if (name[index] == '"') {
			name = name.substr(1, index - 1);
			complete = true;
		}
		index++;
	}
	return name;
}
bool validID(int id, MyAVLTree tree) {
	// 8 digits long
	if (checkLength(id)) {
		// checks if unique id
		if (tree.search(id).empty()) {
			return true;
		}
	}
	return false;
}
bool checkLength(int id) {		// checks length
	if (to_string(id).length() == 8) {
		return true;
	}
	return false;
}
int countZeros(string line){
	int count = 0;
	// count leading zeros
	if (line[0] != '0') {		// no leading zeros
		return count;
	}

	for (int i = 0; i < line.size(); i++) {
		if (line[i] == '0') {
			count++;
		} else {
			break;
		}
	}

	// determine if valid length
	line = line.substr(count);
	if (count + line.size() != 8) {
		count = -1;		// invalid length
	}
	return count;
}