
# WordWeave Dictionary

WordWeave Dictionary is a C++ application that implements a dictionary using an AVL tree, providing functionalities such as adding, searching, deleting, updating words, and suggesting words based on a partial term.

## Features

- **Add Word**: Add a new word with its meaning to the dictionary.
- **Search Word**: Search for a word and display its meaning.
- **Delete Word**: Delete a word from the dictionary.
- **Update Word**: Update the meaning of an existing word.
- **Suggest Words**: Suggest words based on a partial term.
- **Display Dictionary**: Display all words and their meanings in the dictionary.
- **Save Dictionary**: Save the current state of the dictionary to a file.

## Data Structures

### TreeNode Struct
Represents a node in the AVL tree, which stores a word, its meaning, pointers to its left and right children, and the height of the node.
```cpp
struct TreeNode {
    string word;
    string meaning;
    TreeNode* left;
    TreeNode* right;
    int height;
};
```

### Suggestion Struct
Represents a word suggestion for auto-completion.
```cpp
struct Suggestion {
    string word;
};
```

### Dictionary Struct
Contains the root pointer of the AVL tree and an array of suggestions for word completion.
```cpp
struct Dictionary {
    TreeNode* root;
    Suggestion suggestions[MAX_SUGGESTIONS];
    int numSuggestions;
};
```

## AVL Tree Operations

### Creating and Managing Nodes
- **createNode**: Creates a new tree node with a given word and meaning.
    ```cpp
    TreeNode* createNode(const string& word, const string& meaning) {
        TreeNode* newNode = new TreeNode;
        newNode->word = word;
        newNode->meaning = meaning;
        newNode->left = newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }
    ```

### Height and Balance Factor
- **height**: Calculates the height of a node in the AVL tree.
    ```cpp
    int height(TreeNode* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }
    ```

- **balanceFactor**: Calculates the balance factor of a node in the AVL tree.
    ```cpp
    int balanceFactor(TreeNode* node) {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }
    ```

### Updating Height and Rotations
- **updateHeight**: Updates the height of a node.
    ```cpp
    void updateHeight(TreeNode* node) {
        if (node != nullptr) {
            node->height = max(height(node->left), height(node->right)) + 1;
        }
    }
    ```

- **rightRotate**: Performs a right rotation to balance the AVL tree.
    ```cpp
    TreeNode* rightRotate(TreeNode* y) {
        TreeNode* x = y->left;
        TreeNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }
    ```

- **leftRotate**: Performs a left rotation to balance the AVL tree.
    ```cpp
    TreeNode* leftRotate(TreeNode* x) {
        TreeNode* y = x->right;
        TreeNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }
    ```

### Balancing the Tree
- **balanceTree**: Balances the AVL tree to maintain its properties.
    ```cpp
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
    ```

### Adding Words
- **addWord**: Adds a new word to the AVL tree or updates the meaning if the word already exists.
    ```cpp
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
    ```

### Loading Dictionary
- **loadDictionary**: Reads words and meanings from a file and adds them to the AVL tree.
    ```cpp
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
    ```

### Displaying and Deleting Tree
- **displayTree**: Displays the words and meanings in the AVL tree in sorted order.
    ```cpp
    void displayTree(TreeNode* root) {
        if (root != nullptr) {
            displayTree(root->left);
            cout << root->word << ": " << root->meaning << endl;
            displayTree(root->right);
        }
    }
    ```

- **deleteTree**: Recursively deletes all nodes in the AVL tree.
    ```cpp
    void deleteTree(TreeNode*& root) {
        if (root != nullptr) {
            deleteTree(root->left);
            deleteTree(root->right);
            delete root;
            root = nullptr;
        }
    }
    ```

### Searching and Updating Words
- **searchWord**: Searches for a word in the AVL tree and returns the corresponding node.
    ```cpp
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
    ```

- **updateWord**: Updates the meaning of a word if it exists.
    ```cpp
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
    ```

### Saving Dictionary
- **saveTreeToFile**: Saves the tree to a file in order.
    ```cpp
    void saveTreeToFile(TreeNode* root, ofstream& file) {
        if (root != nullptr) {
            saveTreeToFile(root->left, file);
            file << root->word << " " << root->meaning << endl;
            saveTreeToFile(root->right, file);
        }
    }
    ```

- **saveDictionary**: Saves the dictionary to a file.
    ```cpp
    void saveDictionary(TreeNode* root, const char* fileName) {
        ofstream file(fileName);
        if (!file.is_open()) {
            cerr << "Error opening file for saving: " << fileName << endl;
            return;
        }

        saveTreeToFile(root, file);

        file.close();
    }
    ```

### Deleting Words
- **deleteWord**: Deletes a word from the AVL tree.
    ```cpp
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
            if (root->left == nullptr || root->right == nullptr) {
                TreeNode* temp = root->left ? root->left : root->right;

                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else {
                    *root = *temp;
                }
                delete temp;
            }
            else {
                TreeNode* temp = minValueNode(root->right);

                root->word = temp->word;
                root->meaning = temp->meaning;

                root->right = deleteWord(root->right, temp->word);
            }
        }

        if (root == nullptr) {
            return root;
        }

        updateHeight(root);
        root = balanceTree(root);

        return root;
    }
    ```

### Finding Minimum Value Node
- **minValueNode**: Finds the node with the minimum value.
    ```cpp
    TreeNode* minValueNode(TreeNode* node) {
        TreeNode* current = node;

        while (current && current->left != nullptr) {
            current = current->left;
        }

        return current;
    }
    ```

### Autocomplete Suggestions
- **autoComplete**: Provides suggestions based on a partial term.
    ```cpp
    void autoComplete(TreeNode* root, const string& prefix, Dictionary& dict) {
        if (root == nullptr) {
            return;
        }

        int compareResult = root->word.compare(0, prefix.length(), prefix);

        if (compareResult == 0) {
            if (dict.numSuggestions < MAX_SUGGESTIONS) {
                dict.suggestions[dict.numSuggestions].word = root->word;
                dict.numSuggestions++;
            }
        }

        if (compareResult >= 0) {
            autoComplete(root->left, prefix, dict);
        }

        if (compareResult <= 0) {
            autoComplete(root->right, prefix, dict);
        }
    }
    ```

## Running the Program

### Main Function
The `main` function provides a menu-driven interface to interact with the dictionary.
```cpp
int main() {
    Dictionary dict;
    dict.root = nullptr;

    string word, meaning, prefix;
    int choice;

    do {
        cout << "\nDictionary Menu:\n";
        cout << "1. Add Word\n";
        cout << "2. Search Word\n";
        cout << "3. Delete Word\n";
        cout << "4. Update Word\n";
        cout << "5. Suggest Words\n";
        cout << "6. Display Dictionary\n";
        cout << "7. Save Dictionary\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter word: ";
                cin >> word;
                cout << "Enter meaning: ";
                cin >> meaning;
                addWord(dict.root, word, meaning);
                break;

            case 2:
                cout << "Enter word to search: ";
                cin >> word;
                TreeNode* result;
                result = searchWord(dict.root, word);
                if (result) {
                    cout << "Word: " << result->word << ", Meaning: " << result->meaning << endl;
                } else {
                    cout << "Word not found." << endl;
                }
                break;

            case 3:
                cout << "Enter word to delete: ";
                cin >> word;
                dict.root = deleteWord(dict.root, word);
                break;

            case 4:
                cout << "Enter word to update: ";
                cin >> word;
                cout << "Enter new meaning: ";
                cin >> meaning;
                updateWord(dict.root, word, meaning);
                break;

            case 5:
                cout << "Enter prefix for suggestions: ";
                cin >> prefix;
                dict.numSuggestions = 0;
                autoComplete(dict.root, prefix, dict);
                cout << "Suggestions:\n";
                for (int i = 0; i < dict.numSuggestions; ++i) {
                    cout << dict.suggestions[i].word << endl;
                }
                break;

            case 6:
                cout << "Dictionary contents:\n";
                displayTree(dict.root);
                break;

            case 7:
                saveDictionary(dict.root, "dictionary.txt");
                break;

            case 8:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 8);

    deleteTree(dict.root);

    return 0;
}
```

### Compile and Run
To compile and run the program, use the following commands:
```sh


