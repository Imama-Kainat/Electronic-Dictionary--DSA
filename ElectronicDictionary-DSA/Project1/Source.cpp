#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <windows.h>

using namespace std;

const int MAX_SUGGESTIONS = 10;
//TreeNode: Represents a node in the binary search tree storing words and their meanings.
//Suggestion: Represents a suggestion for word completion.
//Dictionary: Contains a root pointer for the binary search tree and an array of suggestions.
struct TreeNode {
	string word;
	string meaning;
	TreeNode* left;
	TreeNode* right;
	int height;
};

struct Suggestion {
	string word;
};

struct Dictionary {
	TreeNode* root;
	Suggestion suggestions[MAX_SUGGESTIONS];
	int numSuggestions;
};
//Tree Operations:
//createNode: Creates a new tree node with a word, meaning, and initializes pointers.
//height: Calculates the height of a node in the AVL tree.
//balanceFactor: Calculates the balance factor of a node in the AVL tree.
//updateHeight: Updates the height of a node.
//rightRotate, leftRotate: Perform right and left rotations to balance the AVL tree.
//balanceTree: Balances the AVL tree to maintain its properties.
//addWord: Adds a new word to the AVL tree or updates the meaning if the word already exists.
//loadDictionary: Reads words and meanings from a file and adds them to the AVL tree.
//we use const& take value change na ho
TreeNode* createNode(const string& word, const string& meaning) {
	TreeNode* newNode = new TreeNode;
	newNode->word = word;
	newNode->meaning = meaning;
	newNode->left = newNode->right = nullptr;
	newNode->height = 1;
	return newNode;
}

int height(TreeNode* node) {
	if (node == nullptr)
		return 0;
	return node->height;
}

int balanceFactor(TreeNode* node) {
	if (node == nullptr)
		return 0;
	return height(node->left) - height(node->right);
}

void updateHeight(TreeNode* node) {
	if (node != nullptr) {
		node->height = max(height(node->left), height(node->right)) + 1;
	}
}

TreeNode* rightRotate(TreeNode* y) {
	TreeNode* x = y->left;
	TreeNode* T2 = x->right;

	x->right = y;
	y->left = T2;

	updateHeight(y);
	updateHeight(x);

	return x;
}

TreeNode* leftRotate(TreeNode* x) {
	TreeNode* y = x->right;
	TreeNode* T2 = y->left;

	y->left = x;
	x->right = T2;

	updateHeight(x);
	updateHeight(y);

	return y;
}

TreeNode* balanceTree(TreeNode* root) {
	if (root == nullptr) {
		return nullptr;
	}

	updateHeight(root);

	int bf = balanceFactor(root);

	if (bf > 1) {
		if (balanceFactor(root->left) < 0) {
			root->left = leftRotate(root->left);
		}
		return rightRotate(root);
	}
	else if (bf < -1) {
		if (balanceFactor(root->right) > 0) {
			root->right = rightRotate(root->right);
		}
		return leftRotate(root);
	}

	return root;
}

void addWord(TreeNode*& root, const string& word, const string& meaning) {
	if (root == nullptr) {
		root = createNode(word, meaning);
		return;
	}

	int compareResult = word.compare(root->word);

	if (compareResult < 0) {
		addWord(root->left, word, meaning);
	}
	else if (compareResult > 0) {
		addWord(root->right, word, meaning);
	}
	else {
		cout << "Word already exists in the dictionary." << endl;
		root->meaning = meaning;
		cout << "Meaning updated successfully." << endl;
		return;
	}

	updateHeight(root);
	root = balanceTree(root);
}

void loadDictionary(TreeNode*& root, const char* fileName) {
	ifstream file(fileName);
	if (!file.is_open()) {
		cerr << "Error opening file: " << fileName << endl;
		return;
	}

	string word, meaning;
	while (file >> word >> meaning) {
		addWord(root, word, meaning);
	}

	file.close();
}
 
//displayTree: Displays the words and meanings in the AVL tree in sorted order.
//deleteTree: Recursively deletes all nodes in the AVL tree.
void displayTree(TreeNode* root) {
	if (root != nullptr) {
		displayTree(root->left);
		cout << root->word << ": " << root->meaning << endl;
		displayTree(root->right);
	}
}

void deleteTree(TreeNode*& root) {
	if (root != nullptr) {
		deleteTree(root->left);
		deleteTree(root->right);
		delete root;
		root = nullptr;
	}
}
//searchWord: Searches for a word in the AVL tree and returns the corresponding node.
//updateWord: Updates the meaning of a word if it exists.
TreeNode* searchWord(TreeNode* root, const string& word) {
	if (root == nullptr || root->word == word) {
		return root;
	}

	int compareResult = word.compare(root->word);

	if (compareResult < 0) {
		return searchWord(root->left, word);
	}
	else {
		return searchWord(root->right, word);
	}
}

void updateWord(TreeNode*& root, const string& word, const string& newMeaning) {
	TreeNode* result = searchWord(root, word);

	if (result) {
		cout << "Current Word: " << result->word << ", Current Meaning: " << result->meaning << endl;
		result->meaning = newMeaning;
		cout << "Do you want to update the meaning of the word? (Yes/No): ";
		string updateChoice;
		cin >> updateChoice;

		if (updateChoice == "Yes" || updateChoice == "yes") {
			cout << "Word updated successfully." << endl;
		}
		else {
			result->meaning = result->meaning;
			cout << "Update canceled." << endl;
		}
	}
	else {
		cout << "Word not found." << endl;
	}
}

void saveTreeToFile(TreeNode* root, ofstream& file) {
	if (root != nullptr) {
		saveTreeToFile(root->left, file);
		file << root->word << " " << root->meaning << endl;
		saveTreeToFile(root->right, file);
	}
}

void saveDictionary(TreeNode* root, const char* fileName) {
	ofstream file(fileName);
	if (!file.is_open()) {
		cerr << "Error opening file for saving: " << fileName << endl;
		return;
	}

	saveTreeToFile(root, file);

	file.close();
}

TreeNode* deleteWord(TreeNode* root, const string& word) {
	if (root == nullptr) {
		return root;
	}

	int compareResult = word.compare(root->word);

	if (compareResult < 0) {
		root->left = deleteWord(root->left, word);
	}
	else if (compareResult > 0) {
		root->right = deleteWord(root->right, word);
	}
	else {
		if (root->left == nullptr) {
			TreeNode* temp = root->right;
			delete root;
			return temp;
		}
		else if (root->right == nullptr) {
			TreeNode* temp = root->left;
			delete root;
			return temp;
		}

		TreeNode* temp = root->right;
		while (temp->left != nullptr) {
			temp = temp->left;
		}

		root->word = temp->word;
		root->meaning = temp->meaning;

		root->right = deleteWord(root->right, temp->word);
	}

	updateHeight(root);
	return balanceTree(root);
}
//suggestWords, wordSuggestion: Provide suggestions for word completion based on a partial term.
void suggestWords(TreeNode* root, const string& partialTerm, Suggestion suggestions[], int& numSuggestions) {
	if (root == nullptr || numSuggestions >= MAX_SUGGESTIONS) {
		return;
	}

	if (root->word.find(partialTerm) == 0) {
		suggestions[numSuggestions].word = root->word;
		++numSuggestions;
	}

	suggestWords(root->left, partialTerm, suggestions, numSuggestions);
	suggestWords(root->right, partialTerm, suggestions, numSuggestions);
}

void wordSuggestion(TreeNode* root, const string& partialTerm, Suggestion suggestions[], int& numSuggestions) {
	numSuggestions = 0;
	suggestWords(root, partialTerm, suggestions, numSuggestions);
}

void Interface() {
	system("color 4F");
	cout << "\n\n\n\n";
	cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
	cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
	cout << "       } : } : } : } : } : } : } : } : } : } : } : } : } : } : } : } : } :   WELCOME TO THE WORDWEAVE  : { : { : { : { : { : { : { : { : { : { : { : { : { : { : { : {\n\n";
	cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
	cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
	cout << endl;
	cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "---------------------------------------------------------------------PRESENTED BY IMAMA KAINAT AND NOOR FATIMA--------------------------------------------------------------" << endl;
	cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	system("pause");
}
int main() {
	Interface();
	system("cls");
    system("Color 0A");
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE); //just once

	Dictionary dictionary;
	dictionary.root = nullptr;

	loadDictionary(dictionary.root, "Dictionary.txt");

	int choice;
	string wordToAdd, meaningToAdd;

	do {
		SetConsoleTextAttribute(color, 13);
		cout << "\nDictionary Menu:\n";
		cout << "1. Display Dictionary\n";
		cout << "2. Add Word\n";
		cout << "3. Search Word\n";
		cout << "4. Delete Word\n";
		cout << "5. Update Word\n";
		cout << "6. Suggest Words\n";
		cout << "7. Save and Quit\n";
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			cout << "\nDictionary:\n";
			SetConsoleTextAttribute(color, 10);
			displayTree(dictionary.root);
			break;
		case 2:
			cout << endl << endl;
cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
cout << "\t---------------------------------------------------------------------ADD WORD-------------------------------------------------------------" << endl;
cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			SetConsoleTextAttribute(color, 5);
			cout << "\nEnter word and meaning to add:\n";
			cin >> wordToAdd >> meaningToAdd;
			addWord(dictionary.root, wordToAdd, meaningToAdd);
			break;
		case 3:
		{
			cout << endl << endl;
cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
cout << "\t---------------------------------------------------------------------SEARCh WORD-------------------------------------------------------------" << endl;
cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			SetConsoleTextAttribute(color, 10);
			cout << "\nEnter word to search:\n";
			string searchInput;
			cin >> searchInput;
			TreeNode* result = searchWord(dictionary.root, searchInput);
			if (result) {
				cout << "Meaning: " << result->meaning << endl;
			}
			else {
				cout << "Word not found." << endl;
			}
			break;
		}
		case 4:
		{
			cout << endl << endl;
cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
cout << "\t---------------------------------------------------------------------DELETE WORD-------------------------------------------------------------" << endl;
cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			SetConsoleTextAttribute(color, 4);
			cout << "\nEnter word to delete:\n";
			string deleteWordInput;
			cin >> deleteWordInput;
			TreeNode* deleteResult = searchWord(dictionary.root, deleteWordInput);
			if (deleteResult) {
				cout << "Do you want to delete the word '" << deleteWordInput << "'? (Yes/No): ";
				string deleteChoice;
				cin >> deleteChoice;
				if (deleteChoice == "Yes" || deleteChoice == "yes") {
					dictionary.root = deleteWord(dictionary.root, deleteWordInput);
					cout << "Word deleted successfully." << endl;
				}
				else {
					cout << "Deletion canceled." << endl;
				}
			}
			else {
				cout << "Word not found." << endl;
			}
			break;
		}
		case 5:
		{
			cout << endl << endl;
	cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "\t---------------------------------------------------------------------UPDATE WORD-------------------------------------------------------------" << endl;
	cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			SetConsoleTextAttribute(color, 15);
			cout << "\nEnter word to update:\n";
			string updateWordInput;
			cin >> updateWordInput;

			cout << "Enter the new meaning:\n";
			string newMeaningInput;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			getline(cin, newMeaningInput);

			updateWord(dictionary.root, updateWordInput, newMeaningInput);
			break;
		}
		case 6:
		{
			cout << endl << endl;
cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
cout << "\t---------------------------------------------------------------------SUGGEST WORD-------------------------------------------------------------" << endl;
cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			cout << "\nEnter partial term for suggestions:\n";
			string partialTerm;
			cin >> partialTerm;

			wordSuggestion(dictionary.root, partialTerm, dictionary.suggestions, dictionary.numSuggestions);

			if (dictionary.numSuggestions > 0) {
				cout << "Suggestions: ";
				for (int i = 0; i < dictionary.numSuggestions; ++i) {
					cout << dictionary.suggestions[i].word << " ";
				}
				cout << endl;
			}
			else {
				cout << "No suggestions found." << endl;
			}
			break;
		}
		case 7:
			cout << endl << endl;
cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
cout << "\t---------------------------------------------------------------------EXIT PROGRAM-------------------------------------------------------------" << endl;
cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			saveDictionary(dictionary.root, "Dictionary.txt");
			cout << "\nExiting program.\n";
			break;
		default:
			cout << "\nInvalid choice. Please try again.\n";
		}
	} while (choice != 7);

	deleteTree(dictionary.root);

	return 0;
}