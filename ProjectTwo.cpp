//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Anthony Wilkinson
// Version     : 1.0
// Date        : 8/12/25
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <fstream> // for file reading functions
#include <sstream> // for split string
#include <cctype> // for toupper
#include <algorithm> // for transform function
#include <memory> // for smart pointer

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prereqs;
};

// Internal structure for tree node
struct Node {

    Course course;
    Node* left;
    Node* right;

    // Constructors initialized with intializer list for effeciency
    Node() : left(nullptr), right(nullptr) { 
    }

    Node(const Course& aCourse) : course(aCourse), left(nullptr), right(nullptr) {
    }
};



//============================================================================
// Class declarations
//============================================================================


class BinarySearchTree {
private:
    Node* root;
    void addNode(Node* node, const Course& course);
    void printNodes(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    ~BinarySearchTree();
    void Add(const Course& course);
    void Display();
    void Remove(string courseNumber);
    Course Search(const string& courseNumber);

};

class Parser {
private:
    ifstream file;
    string filename;
    unordered_set<string> courseNumbers;

    // explicit destructor not necessary as ifstream closes file on destruction.
public:
    Parser(const string& filename);
    bool ValidPrerequisite(const string& prereq) const;
    void ParseData(BinarySearchTree& dataStructure);

};

//============================================================================
// Helper Functions
//============================================================================

// Convert a single character to uppercase
unsigned char toUpperChar(unsigned char c) {
    return toupper(c);
}

// Convert string to upper case for case-insensitive checking
string toUpper(const string& text) {
    string converted = text;
    // perform the function toUpperChar on every character in string from beggining to end
    transform(converted.begin(), converted.end(), converted.begin(), toUpperChar);
    return converted;
}

// Print out all strings in Vector with comma in between.
void displayPrereqs(const vector<string>& prereqs) {
    cout << "Prerequisites: ";
    for (size_t i = 0; i < prereqs.size(); ++i) {
        cout << prereqs[i];
        if (i < prereqs.size() - 1) {
            cout << ", ";
        }
    }
}

// Print out course number and course nam.
void displayCourse(Course course) {
    cout << course.courseNumber << ", " << course.courseName << endl;
}

// Get up to first comma in comma-seperate string
string getFront(const string& line) {
    size_t commaPos = line.find(',');
    return (commaPos == string::npos) ? line : line.substr(0, commaPos);
}

// Split comma-seperated string by commas and add each section to vector
vector<string> splitLine(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string section;
    while (getline(ss, section, ',')) {
        result.push_back(section);
    }

    return result;
}

// Populate course with values from vector. Validate all vector values after the first two.
Course createCourse(const vector<string>& row, const Parser& parser) {
    Course course;
    course.courseNumber = row.at(0);
    course.courseName = row.at(1);
    for (size_t i = 2; i < row.size(); ++i) {
        if (parser.ValidPrerequisite(row.at(i))) {
            course.prereqs.push_back(row.at(i));
        }
    }
    return course;
}

//============================================================================
// Binary Search Tree class definition
//============================================================================

// Default constructor
BinarySearchTree::BinarySearchTree() : root(nullptr) {
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
    while (root != nullptr) {
        Remove(root->course.courseName);
    }  
}


// Recurse through BST to find proper place for node, then instert it there.
void BinarySearchTree::addNode(Node* node, const Course& course) {

    // If node should be to the left of current node
    if (node->course.courseNumber > course.courseNumber) {
        // If left child is nullptr
        if (node->left == nullptr) {
            node->left = new Node(course);
            return;
        }
        else {
            // recurse down left subtree
            addNode(node->left, course);
        }
    }
    // If node should be to the right of the current node
    else {
        // If right child is nullptr
        if (node->right == nullptr) {
            node->right = new Node(course);
            return;
        }
        else {
            // recurse down right subtree
            addNode(node->right, course);
        }
    }
}

// Recurse through BST and print nodes in alphanumeric order
void BinarySearchTree::printNodes(Node* node) {
    if (node == nullptr) {
        return;
    }
    printNodes(node->left);
    displayCourse(node->course);
    printNodes(node->right);
}

// Recurse through BST to find node to remove. Then remove it.
Node* BinarySearchTree::removeNode(Node* node, string courseNumber) {

    if (node == nullptr) {
        return node;
    }
    // if match found, delete node and return pointer to appropiate node.
    if (node->course.courseNumber == courseNumber) {
        if ((node->left == nullptr) && (node->right == nullptr)) {
            delete node;
            return nullptr;
        }
        if ((node->left != nullptr) && (node->right == nullptr)) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        if ((node->left == nullptr) && (node->right != nullptr)) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        // reaches here if matched node has two children.
        Node* temp = node->right;
        while (temp->left != nullptr) {
            temp = temp->left;
        }
        // make node bid (right) equal to temp bid (left).
        node = new Node(*temp);
        // remove right node using recursive call.
        removeNode(node->right, courseNumber);
        // return node.
        return node;
    }
    // check for match as parent node. Allows assignment of pointer to new node.
    // prevent dereferencing of nullptr
    if (node->left != nullptr) {
        if (node->left->course.courseNumber == courseNumber) {
            node->left = removeNode(node->left, courseNumber);
            return node;
        }
    }
    if (node->right != nullptr) {
        // check for match and if so, remove right node using recursive call
        if (node->right->course.courseNumber == courseNumber) {
            node->right = removeNode(node->right, courseNumber);
            return node;
        }
    }
    // Determine which path to recurse down to allow effecient search.
    if (node->course.courseNumber > courseNumber) {
        removeNode(node->left, courseNumber);
    }
    // otherwise recurse down the right subtree
    else {
        removeNode(node->right, courseNumber);
    }
    return node;
}

// Determine whether node should be placed at root or somewhere
// within BST and act accordingly.
void BinarySearchTree::Add(const Course& course) {
    // if root equarl to null ptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(course);
    }
    // else
    else {
        // add Node root and bid
        addNode(root, course);
    }
}

// Begin recursion through BST at root.
void BinarySearchTree::Display() {
    printNodes(root);
}

// Remove node with matching coursenumber
void BinarySearchTree::Remove(string courseNumber) {
    // Prevent calls to bid off a nullptr
    if (root == nullptr) {
        return;
    }
    // Get value for new root when root is deleted
    if (root->course.courseNumber == courseNumber) {
        root = removeNode(root, courseNumber);
    }
    // Begin recursion through BST
    else {
        removeNode(root, courseNumber);
    }
}

// Find and return course with matching number
Course BinarySearchTree::Search(const string& courseNumber) {
    // set current node equal to root
    Node* currentNode = root;
    string upperCourseNum = toUpper(courseNumber);
    // keep looping downwards until bottom reached or matching bidId found
    while (currentNode != nullptr) {
        // if match found, return current bid
       
        if (currentNode->course.courseNumber == upperCourseNum) {
            return currentNode->course;
        }
        // if bid is smaller than current node then traverse left
        if (upperCourseNum < currentNode->course.courseNumber) {
            currentNode = currentNode->left;
        }
        // else larger so traverse right
        else {
            currentNode = currentNode->right;
        }
    }
    Course course;
    return course;
}

//============================================================================
// Parser class definition
//============================================================================

// Open text file and create set of valid prerequisites
Parser::Parser(const string& filename) {

    // Open file
    file.open(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    // Get valid Prerequisites 
    string line;
    while (getline(file, line)) {
        // Read up to first comma and add to courseNumbers
        courseNumbers.insert(getFront(line));
    }
        //Return to start of File
        file.clear();
        file.seekg(0, ios::beg);
}

// Confirm that prerequisite is valid
bool Parser::ValidPrerequisite(const string& prereq) const{

    //For all courseNumbers 
    for (const string& courseNum : courseNumbers) {
        //If match found
        if (prereq == courseNum) {
            return true;
        }
    }
    // if no match found
    return false;
}

// Read data from text file and add to data structure
void Parser::ParseData(BinarySearchTree& dataStructure) {

    if (!file.is_open()) {
        throw std::runtime_error("File not open");
    }

    string line;
    // While file end hasn’t been reached
    while (getline(file, line)) {
        vector<string> row = splitLine(line);
        Course course = createCourse(row, *this);
        dataStructure.Add(course);
    }
}

       
int main()
{
    string filename = "CS 300 ABCU_Advising_Program_Input.csv"; // default
    // Initialize classes 
    BinarySearchTree bst;
    std::unique_ptr<Parser> parser;// allow parser to be used outside of try block.
    char inputSelection = 'n';
    // prompt user for filename and initialize parser
    while (true) {
        // Allow user to select default file
        cout << "Use Default file? (y/n)" << endl;
        cin >> inputSelection;
        cin.ignore(1000, '\n');
        if (toUpperChar(inputSelection) == 'Y') {
            parser = std::make_unique<Parser>(filename);
            break;
        }
        // Prompt user for custom filename
        else {
            cout << "Insert Data Flilename: ";
            getline(cin, filename);
            try {
                parser = std::make_unique<Parser>(filename);
                cout << "Using file: " << filename << endl;
                break;
            }
            catch (const std::runtime_error& e) {
                cout << endl;
                std::cerr << e.what() << std::endl;
            }
        }
    }
    cout << endl;
    // read as char to prevent errors when non-numeric characters are inserted
    char selection = '0';
    cout << "Welcome to the course planner." << endl;
    // While true allows loop to be broken inside loop as soon as character is read
    while (true) {
        cout << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";
        cin >> selection;
        // Breaking here allows default case to catch all values that aren't on menu
        if (selection == '9') {
            break;
        }

        switch (selection) {
            // Load data onto data structure
            case '1': {
                parser->ParseData(bst);
                break;
            }  
            // Print all courses
            case '2': {
                cout << "Here is a sample schedule: " << endl;
                cout << endl;
                bst.Display();
                break;
            }
            // Find and print detailed information about specific course
            case '3': {
                string courseNum;
                cout << "What course do you want to know about? ";
                cin >> courseNum;
                Course course = bst.Search(courseNum);
                // Print info only if course is found. 
                // Prevents errors from trying to read data that doesn't exist.
                if (!course.courseNumber.empty()) {
                    displayCourse(course);
                    displayPrereqs(course.prereqs);
                    cout << endl;
                }
                else {
                    cout << "Course Number " << courseNum << " not found." << endl;
                }
                break;
            }
            default: {
                cout << selection << " is not a valid option." << endl;
                break;
            }
            
        }
        // Clear buffer incase of multiple characters being input
        cin.ignore(1000, '\n');
    }
    // Print upon loop exit (9 is selected).
    cout << "Thank you for using the course planner!";
}

