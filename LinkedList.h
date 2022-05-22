#include <iostream>
#include <vector>
#include <stdexcept>
//#include <leaker.h>
using namespace std;

template <typename T>
class LinkedList
{
public:
	struct Node
	{
		T data;
		Node* next;
		Node* prev;
		Node();
	};
	void PrintForward() const;
	void PrintReverse() const;
	void PrintForwardRecursive(const Node* node) const;
	void PrintReverseRecursive(const Node* node) const;

	unsigned int NodeCount() const;
	const Node* Find(const T& data) const;
	Node* Find(const T& data);
	void FindAll(vector<Node*>& outData, const T& value) const;
	Node* Head();
	const Node* Head() const;
	Node* Tail();
	const Node* Tail() const;
	Node* GetNode(unsigned int index);
	const Node* GetNode(unsigned int index) const;


	void AddHead(const T& data);
	void AddTail(const T& data);
	void AddNodesHead(const T* data, unsigned int count);
	void AddNodesTail(const T* data, unsigned int count);
	void Clear();
	void InsertAfter(Node* node, const T& data);
	void InsertBefore(Node* node, const T& data);
	void InsertAt(const T& data, unsigned int index);

	LinkedList();
	LinkedList(const LinkedList<T>& list);
	~LinkedList();
	LinkedList<T>& operator=(const LinkedList<T>& rhs);

	bool RemoveHead();
	bool RemoveTail();
	unsigned int Remove(const T& data);
	bool RemoveAt(unsigned int index);

	const T& operator[](unsigned int index) const;
	T& operator[](unsigned int index);
	bool operator==(const LinkedList<T>& rhs) const;
private:
	Node* head;
	Node* tail;
	unsigned int nodeCount;

};

//Function definitions

//Node constructor
template <typename T>
LinkedList<T>::Node::Node() {
	next = nullptr;
	prev = nullptr;
}

//Linked List constructor
template <typename T>
LinkedList<T>::LinkedList() {
	head = nullptr;
	tail = nullptr;
	nodeCount = 0;
}

//Destructor
template <typename T>
LinkedList<T>::~LinkedList() {
	//cout << "Destructor called" << endl;
	while (head != nullptr) {
		Node* next = head->next;
		delete head;
		head = next;
	}
	tail = nullptr;
	nodeCount = 0;
}

//Copy Constructor
template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) {
	this->head = nullptr;
	this->tail = nullptr;
	this->nodeCount = 0;
	Node* listNode = list.head;
	while (listNode != nullptr) {
		this->AddTail(listNode->data);
		listNode = listNode->next;
	}
	delete listNode;
}

//Copy assignment operator
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {
	Clear();
	head = nullptr;
	tail = nullptr;
	this->nodeCount = 0;

	Node* listNode = rhs.head;
	while (listNode != nullptr) {
		this->AddTail(listNode->data);
		listNode = listNode->next;
	}
	delete listNode;
	return *this;
}

template <typename T>
void LinkedList<T>::Clear() {
	while (head != nullptr) {
		Node* next = head->next;
		delete head;
		head = next;
	}
	head = nullptr;
	tail = nullptr;
	nodeCount = 0;
}

template <typename T>
void LinkedList<T>::AddHead(const T& data) {
	Node* item = new Node();
	item->data = data;
	if (nodeCount == 0) {
		head = item;
		tail = item;
	}
	else {
		item->next = head;
		head->prev = item;
		head = item;
	}
	nodeCount++;
}

template <typename T>
void LinkedList<T>::AddTail(const T& data) {
	Node* item = new Node();
	item->data = data;
	item->prev = nullptr;
	item->next = nullptr;
	if (head == nullptr) {
		head = item;
	}
	tail = item;
	Node* latestNode = head;
	while (latestNode->next != nullptr)
			latestNode = latestNode->next;
	if (head != tail) {
		tail->prev = latestNode;
		latestNode->next = tail;
	}
	nodeCount++;
}

template <typename T>
void LinkedList<T>::AddNodesHead(const T* data, unsigned int count) {
	for (int i = count - 1; i >= 0; i--) { //Since adding a head is similar to pushing to a stack, last value must be added first to maintain original order
		AddHead(data[i]);
	}
}

template <typename T>
void LinkedList<T>::AddNodesTail(const T* data, unsigned int count) {
	for (unsigned int i = 0; i < count; i++) { //Normal enqueuing 
		AddTail(data[i]);
	}
}

template <typename T>
unsigned int LinkedList<T>::NodeCount() const {
	return nodeCount;
}

template <typename T>
void LinkedList<T>::PrintForward() const {
	Node* currentNode = head; //Temporary node assigned to head
	do {
		cout << currentNode->data << endl; //Prints out the first value, and iterates through list
		currentNode = currentNode->next;
	} while (currentNode != nullptr);
}

template <typename T>
void LinkedList<T>::PrintReverse() const {
	Node* currentNode = tail;
	do {
		cout << currentNode->data << endl; //Same as PrintForward(), but starts from tail
		currentNode = currentNode->prev;
	} while (currentNode != nullptr);
}

template <typename T>
void LinkedList<T>::PrintForwardRecursive(const Node* node) const {
	if (node != nullptr) {
		cout << node->data << endl;
		PrintForwardRecursive(node->next);
	}
}

template <typename T>
void LinkedList<T>::PrintReverseRecursive(const Node* node) const {
	if (node != nullptr) {
		cout << node->data << endl;
		PrintReverseRecursive(node->prev);
	}
}


//Returns head pointer
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Head() {
	return head;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Head() const {
	return head;
}
//Returns tail pointer
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Tail() {
	return tail;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Tail() const {
	return tail;
}

//Returns node on specific index
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) {
	Node* matched = new Node();
	matched = head;
	for (int i = 0; i < index; i++) {
		matched = matched->next;
		if (matched == nullptr)
			throw out_of_range("Error");
	}
	return matched;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) const {
	Node* matched = new Node();
	matched = head;
	for (int i = 0; i < index; i++) {
		matched = matched->next;
		if (matched == nullptr)
			throw out_of_range("Error");
	}
	return matched;
}

//Overloading the [] operator
template <typename T>
const T& LinkedList<T>::operator[](unsigned int index) const {
	Node* matched = GetNode(index);
	return matched->data;
}

template <typename T>
T& LinkedList<T>::operator[](unsigned int index) {
	Node* matched = GetNode(index);
	return matched->data;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) {
	Node* matching = head;
	while (matching != nullptr) {
		if (matching->data == data)
			return matching;
		else
			matching = matching->next;
	}
	return nullptr;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) const {
	Node* matching = head;
	while (matching != nullptr) {
		if (matching->data == data)
			return matching;
		else
			matching = matching->next;
	}
	return nullptr;
}

template <typename T>
void LinkedList<T>::FindAll(vector<Node*>& outData, const T& value) const {
	Node* matching = head;
	while (matching != nullptr) {
		if (matching->data == value)
			outData.push_back(matching);
		matching = matching->next;
	}
}

template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& data) {
	Node* newNode = new Node();
	Node* loop = head;
	while (loop != node) {
		loop = loop->next;
	}
	newNode->data = data;
	newNode->prev = loop;
	newNode->next = loop->next;
	loop->next->prev= newNode;
	loop->next = newNode;
	nodeCount++;
}

template <typename T>
void LinkedList<T>::InsertBefore(Node* node, const T& data) {
	Node* loop = head;
	while (loop != node) {
		loop = loop->next;
	}
	Node* newNode = new Node();
	newNode->data = data;
	newNode->prev = loop->prev;
	newNode->next = loop;
	loop->prev->next = newNode;
	loop->prev = newNode;
	nodeCount++;
}

template <typename T>
void LinkedList<T>::InsertAt(const T& data, unsigned int index) {
	if (index > nodeCount) {
		throw out_of_range("Error!");
	}
	Node* loop = head;
	for (unsigned int i = 0; i < index; i++) {
		loop = loop->next;
	}
	if (loop == nullptr) {
		AddTail(data);
	}
	else if (loop->prev == nullptr) {
		AddHead(data);
	}
	else {
		Node* newNode = new Node();
		newNode->data = data;
		newNode->prev = loop->prev;
		loop->prev->next = newNode;
		newNode->next = loop;
		loop->prev = newNode; 
		nodeCount++;
	}

}

//Equivalence operator: Checking if two lists are the same
template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& rhs) const {
	if (this->nodeCount != rhs.nodeCount) {
		return false;
	}
	Node* iterator = head;
	Node* rhsNode = rhs.head;
	while (iterator != nullptr) {
		if (iterator->data != rhsNode->data) {
			return false;
		}
		iterator = iterator->next;
		rhsNode = rhsNode->next;
	}
	return true;
}

template <typename T>
bool LinkedList<T>::RemoveHead() {
	if (head != nullptr) {
		Node* temp = head;
		head = temp->next;
		if (head != nullptr)
			head->prev = nullptr;
		else {
			head = nullptr;
			tail = nullptr;
		}
		delete temp;
		nodeCount--;
		return true;
	}
	else {
		return false;
	}
}

template <typename T>
bool LinkedList<T>::RemoveTail() {
	if (tail != nullptr) {
		Node* temp = tail;
		tail = temp->prev;
		if (tail != nullptr)
			tail->next = nullptr;
		else {
			head = nullptr;
			tail = nullptr;
		}
		delete temp;
		nodeCount--;
		return true;
	}
	else {
		return false;
	}
}

template <typename T>
unsigned int LinkedList<T>::Remove(const T& data) {
	Node* iterator = head;
	int count = 0;
	while (iterator != nullptr) {
		Node* tempStore = iterator;
		iterator = iterator->next;
		if (tempStore->data == data) { //If data matches
			if (head == tail) {
				head = nullptr;
				tail = nullptr;
			}
			else if (tempStore == head && iterator != nullptr) { //If head node is being deleted, set next node prevNode to nullptr, make next node head
				iterator->prev = nullptr;
				head = iterator;
			}
			else if (tempStore == tail && tempStore->prev != nullptr) {
				//If tail node is being deleted, set previous node as tail, and set their next node pointer to nullptr
				tail = tempStore->prev;
				tail->next = nullptr;
			}
			else if (tempStore->prev != nullptr && tempStore->next != nullptr){//If node being deleted is between nodes, set previous node next pointer to next node, and set next node previous pointer to previous node
				tempStore->prev->next = iterator;
				iterator->prev = tempStore->prev;
			}
			delete tempStore;
			count++;
			nodeCount--;
		}
	}
	return count;
}

template <typename T>
bool LinkedList<T>::RemoveAt(unsigned int index) {
	if (index > nodeCount)
		return false;
	Node* element = head;
	for (unsigned int i = 0; i < index; i++) {
		element = element->next;
	}
	if (element != nullptr) {
		if (element == head) {
			element->next->prev = nullptr;
			head = element->next;
		}
		else if (element == tail) {
			element->prev->next = nullptr;
			tail = element->prev;
		}
		else {
			element->prev->next = element->next;
			element->next->prev = element->prev;
		}
		delete element;
		nodeCount--;
		return true;
	}
	else
		return false;

}


