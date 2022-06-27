//======================================================
// Name			: Advising Assistance Program
// Author		: Breanna Hodges
// Version		: 1.0
// Copyright	:
// Description	: Project 2 C++
//======================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

//define structure to hold course information
struct Course {
	string courseNumber;
	string courseName;
	vector<string> preReqs;
	
	Course() {}
};

//define tree structure and create nodes
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};

class CourseTree {
private:
	Node* root;

	void addCourse(Node* node, Course course);
	void inOrder(Node* node);
	void printCourseInfo(string courseNum);
	void loadCourses(string csvPath, CourseTree* coursesTree);

public:
	CourseTree();
	virtual ~CourseTree();
	void DeleteRecursive(Node* node);
	void Insert(Course course);
	Course Search(string courseNumber);
	int NumPrerequisiteCourses(Course course);
	void PrintSampleSchedule();
};

CourseTree::CourseTree() {
	//initialize root to null
	root = nullptr;
}

//destructor
CourseTree::~CourseTree() {
	//recursively delete nodes beginning at root
	DeleteRecursive(root);
}

//used by destructor
void CourseTree::DeleteRecursive(Node* node) {
	if (node) {
		DeleteRecursive(node->left);
		DeleteRecursive(node->right);
		delete node;
	}
}

//insert node into tree
void CourseTree::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		addCourse(root, course);
	}
}

Course CourseTree::Search(string courseNumber) {
	Course course;
	// set current node equal to root
	Node* current = root;

	// keep looping downwards until bottom reached or matching bidId found
	while (current != nullptr) {
		// if match found, return current bid
		if (current->course.courseNumber.compare(courseNumber) == 0) {
			cout << endl << current->course.courseNumber << ": " <<
				current->course.courseName << endl;
			//uses NumPrerequisiteCourses for the current course
			unsigned int size = NumPrerequisiteCourses(current->course);
			cout << "Prerequisite(s): " << endl;
			//loops through prerequisites
			for (int i = 0; i < size; i++) {
				cout << current->course.preReqs.at(i);
				//if there are more than 1 prints comma and following one
				if (i != size - 1) {
					cout << ", " << current->course.preReqs.at(i + 1) << endl;
				}
				else {
					cout << "No prerequisites for this course." << endl;
				}
				return course;
			}
			return course;
		}
		// if courseNumber is smaller than current node then traverse left
		else if (courseNumber.compare(current->course.courseNumber) < 0) {
			current = current->left;
		}
		// else larger so traverse right
		else {
			current = current->right;
		}
		cout << "No course found for " << courseNumber << "." << endl;
		
	}
	return course;
	
}

//returns number of prereqisities for course
int CourseTree::NumPrerequisiteCourses(Course course) {
	int count = 0;
	for (unsigned int i = 0; i < course.preReqs.size(); ++i) {
		count++;
	}
	return count;
}

void CourseTree::inOrder(Node* node) {
	//if node is not null
	if (node != nullptr) {
		//traverse left
		inOrder(node->left);
		//print course information
		cout << node->course.courseNumber << ": " <<
			node->course.courseName << endl;
		//traverse right
		inOrder(node->right);
	}
}

void CourseTree::PrintSampleSchedule() {
	//prints courses in order using inOrder function
	this->inOrder(root);
}

void CourseTree::addCourse(Node* node, Course course) {
	if (node->course.courseNumber.compare(course.courseNumber) > 0) {
		//if no left node
		if (node->left == nullptr) {
			//left node = new course node
			node->left = new Node(course);
		}
		else {
			//recurse left subtree
			this->addCourse(node->left, course);
		}
	}
	else {
		//if no right node
		if (node->right == nullptr) {
			//right node = new course node
			node->right = new Node(course);
		}
		else {
			//recurse right subtree
			this->addCourse(node->right, course);
		}
	}
}


//parses each line with , as the delimiter
vector<string> parseLine(string line) {
	vector<string> row;
	string delimiter = ",";
	int start = 0;
	int end = line.find(delimiter);
	
	while (end != -1) {
		row.push_back(line.substr(start, end - start));
		start = end + delimiter.size();
		end = line.find(delimiter, start);
	}
	row.push_back(line.substr(start, end - start));
	return row;
	
}

vector<vector<string>> readFile(string csvPath) {
	vector<vector<string>> fileData;
	//stream for reading file
	fstream inputFile;
	
	
	//opens file
	inputFile.open(csvPath);

	if (inputFile.is_open()) {
		string line;
		//reads file line by line
		while (getline(inputFile, line)) {
			if (line != "") {
				//uses delimiter to separate each entry
				vector<string> row = parseLine(line);
				fileData.push_back(row);
			}
		}
		//close file
		inputFile.close();
	}
	else {
		//file open error
		cout << "Could not open " << csvPath << endl;
		
	}
	return fileData;

}
//puts courses into the tree from the file
void loadCourses(string csvPath, CourseTree* coursesTree) {
	vector<string> courseNames;
	
	try {
		vector<vector<string>> fileData = readFile(csvPath);
		for (unsigned int i = 0; i < fileData.size(); ++i) {
			vector<string> row = fileData.at(i);
			if (row.size() >= 2) {
				courseNames.push_back(row.at(0));
				Course course;

				course.courseNumber = row.at(0);
				course.courseName = row.at(1);
				//loops prerequisites
				for (int i = 2; i < row.size(); i++) {
					course.preReqs.push_back(row.at(i));
				}
				coursesTree->Insert(course);
			}
		}
	}
	catch (...) {
		cout << "Error" << endl;
	}
}

//function to uppercase input
void to_upper(char& ch) {
	ch = toupper(static_cast<unsigned char>(ch));
}

int main(int argc, char* argv[]) {

	string line;
	//opens and reads file
	fstream courseFile("");
	if (courseFile.is_open()) {
		while (getline(courseFile, line)) {
			cout << line << '\n';
		}
		courseFile.close();
	}

	//process command line argument
	string csvPath, courseNumber;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		//courseNumber = "CSCI100";
		break;
	case 3:
		csvPath = argv[1];
		courseNumber = argv[2];
		break;
	default: 
		csvPath = "";
		break;
	}
	//define Tree to hold courses
	CourseTree* coursesTree = new CourseTree();
	Course course;
	
	//get user input for menu
	int userInput = 0;
	
	//menu loop
	while (userInput != 9) {

		cout << "Welcome to the course planner." << endl;
		cout << "	1. Load Course Data." << endl;
		cout << "	2. Print Course List." << endl;
		cout << "	3. Print Course." << endl;
		cout << "	9. Exit" << endl;
		cout << "\nWhat would you like to do? " << endl;
		cin >> userInput;

		switch (userInput) {

		case 1:
			loadCourses(csvPath, coursesTree);
			if (csvPath.length() == 0) {
				cout << "Enter file name that holds the course data." << endl;
				cin >> csvPath;
				loadCourses(csvPath, coursesTree);
				if (coursesTree == nullptr) {
					coursesTree = new CourseTree();
				}
			}
			break;
		case 2:
			cout << "Here is a sample schedule." << endl;
			coursesTree->PrintSampleSchedule();
			break;
		case 3:
			cout << "Enter a course number." << endl;
			cin >> courseNumber;
			//makes input uppercase
			for_each(courseNumber.begin(), courseNumber.end(), to_upper);


			if (courseNumber == "") {
				cout << "Please input a course number." << endl;
			}
			else {
				coursesTree->Search(courseNumber);
			}
			break;
			
		default:
			if (((userInput > 3) && (userInput < 9)) || (userInput > 9)) {
				cout << userInput << " is not valid." << endl;
			}
			break;
		}


	}
	cout << "Thank you for using the course planner!" << endl;
	return 0;
}