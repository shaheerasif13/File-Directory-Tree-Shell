// Muhammad Shaheer
// 19I-2152
// Section-C
// Assignment-3

#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Node class (Queue)
template<typename T>
class QNode {

public:
	T data{};
	QNode<T>* next;
	QNode() { next = nullptr; }	// Default constructor
};

// Queue class
template<typename T>
class Queue {

private:
	QNode<T>* front;
	QNode<T>* rear;

public:
	Queue() { front = rear = nullptr; }	// Default constructor
	bool isEmpty() { return front == nullptr && rear == nullptr; }	// Method to check is queue is empty or not
	QNode<T>* getFront() { return front; }

	void enQueue(T newData) {	// Method to insert node in queue

		// Creating and initializing new node
		QNode<T>* newNode = new QNode<T>();
		newNode->data = newData;

		if (isEmpty()) {	// If queue is empty
			front = rear = newNode;
		}

		else {	// If queue is not empty
			rear->next = newNode;
			rear = newNode;
		}
	}

	void deQueue() {	// Method to delete node from queue

		if (isEmpty()) {	// If queue is empty
			cout << "Queue is empty!" << endl;
		}

		else {	// If queue is not empty
			QNode<T>* targetNode = front;
			front = front->next;
			if (front == nullptr) { rear = nullptr; }	// If last left element is deleted
			delete targetNode;
		}
	}

	int size() {
		QNode<T>* tempNode = front;
		int count = 0;

		while (tempNode != nullptr) {
			count = count + 1;
			tempNode = tempNode->next;
		}

		return count;
	}

	void display() {	// Method to display the queue

		if (isEmpty()) {	// If queue is empty
			cout << "Queue is empty!" << endl;
		}

		else {	// If queue is not empty
			QNode<T>* tempNode = front;

			while (tempNode != nullptr) {
				cout << tempNode->data << endl;
				tempNode = tempNode->next;
			}
		}
	}

	~Queue() {	// Destructor

		while (front != nullptr) {
			QNode<T>* tempNode = front;
			front = front->next;
			delete tempNode;
		}
	}
};

// Node class (N-Array Tree)
template <typename T>
class NATNode {

public:
	T data{};
	NATNode<T>* firstChild;
	NATNode<T>* nextSibling;
	NATNode() { firstChild = nextSibling = nullptr; }	// Default constructor
};

// Global root of file system
NATNode<string>* fileSystemRoot = nullptr;

// Method to reverse a string
void reverseString(string& str) {
	int first = 0;
	int last = str.length() - 1;

	while (first < last) {
		char temp = str[first];
		str[first] = str[last];
		str[last] = temp;
		first = first + 1;
		last = last - 1;
	}
}

// Method to validate path for insertion
bool validatePath(string path) {
	string fileExtension = "";

	for (int i = path.length() - 1; i >= 0; i--) {
		fileExtension = fileExtension + path[i];

		if (path[i] == '.') { break; }
	}

	reverseString(fileExtension);

	if (fileExtension != ".txt" && fileExtension != ".cpp") {
		return true;
	}

	return false;
}

// Method to delete everything of tree from given node
template <typename T>
void deleteFromNode(NATNode<T>*& currentNode) {

	if (currentNode != nullptr) {
		deleteFromNode<T>(currentNode->nextSibling);
		deleteFromNode<T>(currentNode->firstChild);
		delete currentNode;
		currentNode = nullptr;
	}
}

// Method to copy everything in given node to given path
template <typename T>
void copyFromNode(NATNode<T>* currentNode, T targetPath) {

	if (currentNode != nullptr) {
		insertFileFolder(currentNode->data, targetPath, "");
		copyFromNode<T>(currentNode->firstChild, targetPath + "/" + currentNode->data);
		copyFromNode<T>(currentNode->nextSibling, targetPath);
	}
}

// Method to search given file/folder in file system from given node
template <typename T = string>
void searchFromNode(T targetFileFolder, NATNode<T>* currentNode, NATNode<T>*& paths, T targetPath = "") {

	if (currentNode != nullptr) {

		// Saving path in a single linked list if target is found
		if (currentNode->data == targetFileFolder) {

			// Creating and initializing a new node
			NATNode<T>* newNode = new NATNode<T>();
			newNode->data = targetPath + "/" + currentNode->data;

			if (paths == nullptr) {
				paths = newNode;
			}

			else {
				NATNode<T>* tempNode = paths;

				while (tempNode->nextSibling != nullptr) {
					tempNode = tempNode->nextSibling;
				}

				tempNode->nextSibling = newNode;
			}
		}

		searchFromNode<T>(targetFileFolder, currentNode->firstChild, paths, targetPath + "/" + currentNode->data);
		searchFromNode<T>(targetFileFolder, currentNode->nextSibling, paths, targetPath);
	}
}

template <typename T = string>
void createTree(string inputFilePath) {

	// Initializing root of file system
	fileSystemRoot = new NATNode<T>();
	fileSystemRoot->data = "/";

	// Starting file system creation
	ifstream ifile(inputFilePath);

	if (ifile) {
		T path = "";

		while (ifile >> path) {

			// Inserting current path in file system
			NATNode<T>* currentNode = fileSystemRoot;

			for (int i = 0; path[i] != '\0'; i++) {
				bool duplicate = false;

				if (path[i] == '/') {
					T str = "";

					for (int j = i + 1; path[j] != '/' && path[j] != '\0'; j++) {
						str = str + path[j];
					}

					if (str == "") { break; }	// If the last inserted node is a folder

					// Starting insertion in current folder
					// If first child of current node is to be inserted
					if (currentNode->firstChild == nullptr) {
						currentNode->firstChild = new NATNode<T>();
						currentNode->firstChild->data = str;
						currentNode = currentNode->firstChild;
					}

					// If child of current node is to be inserted after first child
					// Current node becomes first child and we traverse in single linked list
					else {
						currentNode = currentNode->firstChild;

						while (currentNode->nextSibling != nullptr &&
							currentNode->data != str /* Duplication Check*/) {
							currentNode = currentNode->nextSibling;
						}

						// If duplicate set flag
						if (currentNode->data == str) { duplicate = true; }

						// Insert only if no duplication
						if (!duplicate) {
							currentNode->nextSibling = new NATNode<T>();
							currentNode->nextSibling->data = str;
							currentNode = currentNode->nextSibling;
						}
					}
				}
			}
		}

		ifile.close();
	}

	else {
		cout << "Error opening file ..." << endl;
	}
}

template <typename T = string>
void levelOrderFunction(string outputFilePath) {

	if (fileSystemRoot != nullptr) {

		// Creating output file
		ofstream ofile(outputFilePath);

		if (ofile) {

			// Creating a queue for storing nodes
			Queue<NATNode<T>*> queue;
			queue.enQueue(fileSystemRoot);

			// Writing to file in level order
			while (!queue.isEmpty()) {
				int nodeCount = queue.size();

				while (nodeCount > 0) {
					NATNode<T>* currentNode = queue.getFront()->data;
					queue.deQueue();
					T data = currentNode->data;
					currentNode = currentNode->firstChild;

					while (currentNode != nullptr) {
						queue.enQueue(currentNode);
						currentNode = currentNode->nextSibling;
					}

					nodeCount = nodeCount - 1;

					if (nodeCount != 0) { ofile << data << " "; }
					else { ofile << data; }
				}

				if (!queue.isEmpty()) {
					ofile << endl;
				}
			}

			ofile.close();
			deleteFromNode(fileSystemRoot);	// Should be out of this if-else
		}

		else {
			cout << "Error creating file ..." << endl;
		}
	}

	else {
		cout << "File system is empty!" << endl;
	}
}

template <typename T = string>
void insertFileFolder(string source, string targetPath, string outputFilePath) {

	if (fileSystemRoot != nullptr) {

		if (validatePath(targetPath)) {

			// Locating last folder in path to insert in it
			NATNode<T>* currentNode = fileSystemRoot;

			for (int i = 0; targetPath[i] != '\0'; i++) {

				if (targetPath[i] == '/') {
					T str = "";

					for (int j = i + 1; targetPath[j] != '/' && targetPath[j] != '\0'; j++) {
						str = str + targetPath[j];
					}

					currentNode = currentNode->firstChild;

					while (currentNode != nullptr && currentNode->data != str) {
						currentNode = currentNode->nextSibling;
					}

					if (currentNode == nullptr) {
						cout << str << " not found!" << endl;
						return;
					}
				}
			}

			// Starting insertion in current folder
			bool duplicate = false;

			// If first child of current node is to be inserted
			if (currentNode->firstChild == nullptr) {
				currentNode->firstChild = new NATNode<T>();
				currentNode->firstChild->data = source;
				currentNode = currentNode->firstChild;
			}

			// If child of current node is to be inserted after first child
			// Current node becomes first child and we traverse in single linked list
			else {
				currentNode = currentNode->firstChild;

				while (currentNode->nextSibling != nullptr &&
					currentNode->data != source /* Duplication Check*/) {
					currentNode = currentNode->nextSibling;
				}

				// If duplicate set flag
				if (currentNode->data == source) { duplicate = true; }

				// Insert only if no duplication
				if (!duplicate) {
					currentNode->nextSibling = new NATNode<T>();
					currentNode->nextSibling->data = source;
					currentNode = currentNode->nextSibling;
				}
			}
		}

		else {
			cout << "Can not insert!" << endl;
		}

		levelOrderFunction(outputFilePath);
	}

	else {
		cout << "File system is empty!" << endl;
	}
}

template <typename T = string>
void deleteFileFolder(string targetPath, string outputFilePath) {

	if (fileSystemRoot != nullptr) {

		// Locating last file/folder in path to delete it
		NATNode<T>* currentNode = fileSystemRoot;
		NATNode<T>* previousNode = nullptr;

		for (int i = 0; targetPath[i] != '\0'; i++) {

			if (targetPath[i] == '/') {
				T str = "";

				for (int j = i + 1; targetPath[j] != '/' && targetPath[j] != '\0'; j++) {
					str = str + targetPath[j];
				}

				previousNode = currentNode;
				currentNode = currentNode->firstChild;

				while (currentNode != nullptr && currentNode->data != str) {
					previousNode = currentNode;
					currentNode = currentNode->nextSibling;
				}

				if (currentNode == nullptr) {
					cout << str << " not found!" << endl;
					return;
				}
			}
		}

		// Starting deletion of target file/folder
		if (previousNode != nullptr) {

			if (previousNode->firstChild == currentNode) {
				previousNode->firstChild = currentNode->nextSibling;
				delete currentNode;
				currentNode = nullptr;
			}

			else if (previousNode->nextSibling == currentNode) {
				previousNode->nextSibling = currentNode->nextSibling;
				deleteFromNode<T>(currentNode);
			}
		}

		levelOrderFunction(outputFilePath);
	}

	else {
		cout << "File system is empty!" << endl;
	}
}

template <typename T = string>
void mergeFolder(string sourcePath, string targetPath, string outputFilePath) {

	if (fileSystemRoot != nullptr) {

		// Locating last file/folder in path to copy it to target path
		NATNode<T>* currentNode = fileSystemRoot;

		for (int i = 0; sourcePath[i] != '\0'; i++) {

			if (sourcePath[i] == '/') {
				T str = "";

				for (int j = i + 1; sourcePath[j] != '/' && sourcePath[j] != '\0'; j++) {
					str = str + sourcePath[j];
				}

				currentNode = currentNode->firstChild;

				while (currentNode != nullptr && currentNode->data != str) {
					currentNode = currentNode->nextSibling;
				}

				if (currentNode == nullptr) {
					cout << str << " not found!" << endl;
					return;
				}
			}
		}

		copyFromNode<T>(currentNode->firstChild, targetPath);
		levelOrderFunction(outputFilePath);
	}

	else {
		cout << "File system is empty!" << endl;
	}
}

template <typename T = string>
void searchFileFolder(string targetFileFolder, string outputFilePath) {

	// Single linked list to store all paths of searched file/folder
	NATNode<T>* targetPaths = nullptr;

	// Starting search of given file/folder
	searchFromNode(targetFileFolder, fileSystemRoot->firstChild, targetPaths);

	// Writing all searched paths to the output file
	ofstream ofile(outputFilePath);

	if (ofile) {
		NATNode<T>* tempNode = targetPaths;

		while (tempNode != nullptr) {
			ofile << tempNode->data;

			if (tempNode->nextSibling != nullptr) {
				ofile << endl;
			}

			tempNode = tempNode->nextSibling;
		}

		ofile.close();
	}

	else {
		cout << "Error creating file ..." << endl;
	}

	// Freeing memory allocation
	while (targetPaths != nullptr) {
		NATNode<T>* tempNode = targetPaths;
		targetPaths = targetPaths->nextSibling;
		delete tempNode;
		tempNode = nullptr;
	}
}