#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;

class DoublyLinkedList {
private:
	struct Node {
		int data;
		Node *prev;
		Node *next;

		Node(int val, Node *p = nullptr, Node *n = nullptr) {
			data = val;
			prev = p;
			next = n;
		}
	};

	Node *head;
	Node *tail;

public:
	DoublyLinkedList() {
		head = nullptr;
		tail = nullptr;
	}

	void insert_after(int value, int position) {
		if (position < 0) {
			cout << "Position must be >= 0." << endl;
			return;
		}

		Node *newNode = new Node(value);
		if (!head) {
			head = tail = newNode;
			return;
		}

		Node *temp = head;
		for (int i = 0; i < position && temp; ++i)
			temp = temp->next;

		if (!temp) {
			cout << "Position exceeds list size. Node not inserted.\n";
			delete newNode;
			return;
		}

		newNode->next = temp->next;
		newNode->prev = temp;
		if (temp->next)
			temp->next->prev = newNode;
		else
			tail = newNode;
		temp->next = newNode;
	}

	void delete_val(int value) {
		if (!head) return;

		Node *temp = head;

		while (temp && temp->data != value)
			temp = temp->next;

		if (!temp) return;

		if (temp->prev)
			temp->prev->next = temp->next;
		else
			head = temp->next;

		if (temp->next)
			temp->next->prev = temp->prev;
		else
			tail = temp->prev;

		delete temp;
	}

	// Modified method to return the data value for the node being removed
	int delete_pos(int pos) {
		if (!head) {
			cout << "List is empty." << endl;
			return -1;
		}

		if (pos == 1) {
			return pop_front();
		}

		Node *temp = head;

		for (int i = 1; i < pos; i++) {
			if (!temp) {
				cout << "Position doesn't exist." << endl;
				return -1;
			} else
				temp = temp->next;
		}
		if (!temp) {
			cout << "Position doesn't exist." << endl;
			return -1;
		}

		if (!temp->next) {
			return pop_back();
		}

		int data = temp->data;
		Node *tempPrev = temp->prev;
		tempPrev->next = temp->next;
		temp->next->prev = tempPrev;
		delete temp;
		return data;
	}

	void push_back(int v) {
		Node *newNode = new Node(v);
		if (!tail)
			head = tail = newNode;
		else {
			tail->next = newNode;
			newNode->prev = tail;
			tail = newNode;
		}
	}

	void push_front(int v) {
		Node *newNode = new Node(v);
		if (!head)
			head = tail = newNode;
		else {
			newNode->next = head;
			head->prev = newNode;
			head = newNode;
		}
	}

	// Modified method to return the data value for the node being removed
	int pop_front() {
		if (!head) {
			cout << "List is empty." << endl;
			return -1;
		}

		Node *temp = head;
		int data = temp->data;

		if (head->next) {
			head = head->next;
			head->prev = nullptr;
		} else
			head = tail = nullptr;
		delete temp;
		return data;
	}

	// Modified method to return the data value for the node being removed
	int pop_back() {
		if (!tail) {
			cout << "List is empty." << endl;
			return -1;
		}
		Node *temp = tail;
		int data = temp->data;

		if (tail->prev) {
			tail = tail->prev;
			tail->next = nullptr;
		} else
			head = tail = nullptr;
		delete temp;
		return data;
	}

	~DoublyLinkedList() {
		while (head) {
			Node *temp = head;
			head = head->next;
			delete temp;
		}
	}

	void print() {
		Node *current = head;
		if (!current) {
			cout << "List is empty." << endl;
			return;
		}
		while (current) {
			cout << current->data << " ";
			current = current->next;
		}
		cout << endl;
	}

	void print_reverse() {
		Node *current = tail;
		if (!current) {
			cout << "List is empty." << endl;
			return;
		}
		while (current) {
			cout << current->data << " ";
			current = current->prev;
		}
		cout << endl;
	}

	// This method traverses the doubly linked list starting at the head node, counts each node, and then returns the
	// total number of nodes in the linked list.
	int get_size_of_list() const{
		Node *current = head;
		int count = 0;
		if (!current) {
			cout << "List is empty." << endl;
			return count;
		}
		while (current) {
			count++;
			current = current->next;
		}
		return count;
	}

int main() {
	// cout << MIN_NR + MIN_LS + MAX_NR + MAX_LS;  // dummy statement to avoid compiler warning

	// A vector is initialized and used to store the customer names read from the external data file
	vector<string> customerNames;
	ifstream infile;
	infile.open("names.txt");
	string temp;
	if (infile.good()) {
		while (infile >> temp) {
			customerNames.push_back(temp);
		}
	} else
		cout << "Error opening file" << endl;
	infile.close();

	// The DoublyLinkedList object is initialized and will be used to represent the coffee shop line
	DoublyLinkedList line;

	cout << "Store opens:" << endl;

	// generate 5 random numbers to represent the index of the names in the vector
	// add 5 nodes to the linked list with the indexes as the value
	for (int i = 0; i < 5; i++) {
		int index = rand() % customerNames.size();
		line.push_back(index);
		cout << '\t' << customerNames[index] << " joins the line" << endl;
	}

	// TODO: Fix
	cout << "Resulting line:" << endl;
	for (int i = 1; i <= 5; i++) {
		int index = line.get_data_at_pos(i);
		cout << "\t\t" << customerNames[index] << endl;
	}

	// starting i at 1 because the first time period (when the store opened and 5 customers were added) already happened
	for (int i = 1; i < 20; i++) {
		cout << "Time step #" << i + 1 << ':' << endl;
		// A: 40% - The customer at the beginning of the line is being helped and ordering their coffee
		int probability = rand() % 100 + 1; // should this be used for the whole loop or for each event?
		if (probability <= 40) {
			int index = line.pop_front();
			cout << '\t' << customerNames[index] << " is served" << endl;
		}
		// B: 60% - A new customer joins the end of the line
		if (probability <= 60) {
			int index = rand() % customerNames.size();
			line.push_back(index);
			cout << '\t' << customerNames[index] << " joined the line" << endl;
		}
		// C: 20% - The customer at the end of the line leaves
		if (probability <= 20) {
			int index = line.pop_back();
			cout << '\t' << customerNames[index] << " (at the rear) left the line" << endl;
		}
		// D: 10% - Any customer leaves
		if (probability <= 10) {
			int size = line.get_size_of_list();
			int randomNode = rand() % size + 1; // delete_pos() has 1 as the position of the head node
			int index = line.delete_pos(randomNode);
			cout << '\t' << customerNames[index] << " left the line" << endl;
		}
		// E: 10% - A VIP customer skips the line, goes straight to the counter, and orders
		if (probability <= 10) {
			int index = rand() % customerNames.size();
			line.push_back(index);
			cout << '\t' << customerNames[index] << " (VIP) joins the line" << endl;
			// Unclear if the customer served right away and thus removed from the line
			line.pop_front();
			cout << '\t' << customerNames[index] << " (VIP) is served" << endl;
		}
	}


	return 0;
}
