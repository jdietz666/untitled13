#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
using namespace std;

struct tree;
struct hash_table_entry;

#define CAT_NAME_LEN 25
#define APP_NAME_LEN 50
#define VERSION_LEN 10
#define UNIT_SIZE 3

struct categories{
    char category[ CAT_NAME_LEN ];   // Name of category
    struct tree *root;   // Pointer to root of search tree for this category
};

struct app_info{
    char category[ CAT_NAME_LEN ]; // Name of category
    char app_name[ APP_NAME_LEN ]; // Name of the application
    char version[ VERSION_LEN ]; // Version number
    float size; // Size of the application
    char units[ UNIT_SIZE ]; // GB or MB
    float price; // Price in $ of the application
};

struct hash_table_entry{
    char app_name[ APP_NAME_LEN ]; 	// Name of the application
    struct tree *app_node;	 // Pointer to node in tree containing the application information
    struct hash_table_entry *next;	 // Pointer to next entry in the chain
};

typedef hash_table_entry *HashTableEntryPtr;

struct tree{ // A binary search tree
    struct app_info record;     // Information about the application
    struct tree *left;               // Pointer to the left subtree
    struct tree *right;            // Pointer to the right subtree
};


//For FindApp output
void printTreeEntry(tree const *t) {
    if (t==0) {
        return;
    }
    cout << "Found Application: " << t -> record.app_name  << endl;
    cout << "   Category: " << t -> record.category << endl;
    cout << "   Application Name: " << t -> record.app_name << endl;
    cout << "   Version: " << t -> record.version << endl;
    cout << "   Size: " << t -> record.size << endl;
    cout << "   Units: " << t -> record.units << endl;
    cout << fixed << setprecision(2)  << "   Price: $" << t -> record.price << endl;
}


tree *treeInsertApp(tree *node, tree *app) {
    if (!node) {
        return app;
    }
    if (strcmp(app -> record.app_name, node -> record.app_name) < 0 ) {
        node->left = treeInsertApp(node->left, app);
    } else {
        node->right = treeInsertApp(node->right, app);
    }
    return node;
}

//for testing of hash table
void printHashTableEntry(size_t i, hash_table_entry const * h) {
    cout << i << ":( app_name = " << h ->app_name << ", app_node = ";
    printTreeEntry(h -> app_node);
    cout << ", next = " << h -> next << " )";
}

//hash function
size_t hash(char const * name) {
    size_t h;
    for (h = 0; *name != 0; name++) {
        h *= 5; h += size_t (*name);
    }
    return h;
}

//for testing of Binary Search Tree
void printBST(tree const * t) {
    if (t == 0) {
        return;
    }
    printBST(t->left);
    cout << "   " << t -> record.app_name << endl;
    printBST(t->right);
}

//find price free 0$
bool searchBSTForFree(tree const *t) {
    if (t == 0) {
        return false;
    }
    bool foundIt = searchBSTForFree(t->left);
    if (t->record.price == 0.0) {
        cout << "   " << t -> record.app_name << endl;
        foundIt = true;
    }
    foundIt |= searchBSTForFree(t->right);
    return foundIt;
}
int countBSTForFree(tree const *t) {
    if (t == 0) {
        return 0;
    }
    int count = countBSTForFree(t->left);
    if (t->record.price == 0.0) {
        count += 1;
    }
    count += countBSTForFree(t->right);
    return count;
}

tree const *searchBSTForName(tree const *t, string const &name) {
    if (t == 0) {
        return 0;
    }
    if (name < t->record.app_name) {
        return searchBSTForName(t->left, name);
    } else if (t->record.app_name < name) {
        return searchBSTForName(t->right, name);
    }
    return t;
}

//Range category name app low, high
bool searchBSTForNames(tree const *t, string const &lo, string const &hi) {
    if (t == 0) {
        return false;
    }
    bool foundIt = searchBSTForNames(t->left, lo, hi);
    if (lo <= t->record.app_name && t->record.app_name <= hi) {
        cout << "   " << t -> record.app_name << endl;
        foundIt = true;
    }
    foundIt |= searchBSTForNames(t->right, lo, hi);
    return foundIt;
}
//count searchBSTForNames
int countBSTForNames(tree const *t, string const &lo, string const &hi) {
    if (t == 0) {
        return false;
    }
    int count = countBSTForNames(t->left, lo, hi);
    if (lo <= t->record.app_name && t->record.app_name <= hi) {
        count += 1;
    }
        count += countBSTForNames(t->right, lo, hi);
        return count;

}

//Range category price low,high
bool searchBSTForPrices(tree const *t, float lo, float hi) {
    if (t == 0) {
        return false;
    }
    bool foundIt = searchBSTForPrices(t->left, lo, hi);
    if (lo <= t->record.price && t->record.price <= hi) {
        cout << "   " << t -> record.app_name << endl;
         foundIt = true;

    }
    foundIt |= searchBSTForPrices(t->right, lo, hi);
    return foundIt;
}

int countBSTForPrices(tree const *t, float lo, float hi) {
    if (t == 0) {
        return false;
    }
        int count = countBSTForPrices(t->left, lo, hi);
    if (lo <= t->record.price && t->record.price <= hi) {
         count += 1;
    }
    count += countBSTForPrices(t->right, lo, hi);
    return count;
}


size_t number_of_cats = 0;
categories *cats = 0;
hash_table_entry ** hashtable = 0;
size_t hashtablesize = 0;
size_t number_of_apps = 0;

int BstNodes(tree const *t) {
    if(t == 0) { return 0;}

    return BstNodes(t->left) + BstNodes(t->right) + 1;
}

int BstDepth(tree const *t) {
    if(t == 0) { return 0;}
    int l = BstDepth(t->left)+1;
    int r = BstDepth(t->right)+1;
    return max(l,r);

}
//delete category name
void treeDeleteApp(tree const *t) {
    if (t == 0) {
        return;
    }
tree **catRoot = 0;
string const catName = t->record.category;
string const appName = t->record.app_name;

    for (int c = 0; c < number_of_cats; c++) {
        if (cats[c].category == catName) {
            catRoot = &cats[c].root;
            break;
        }
    }
    if (catRoot == 0) {
        return;
    }

    tree *parent = 0;
    tree *node = *catRoot;
    bool wentLeft;
    while (node != 0) {
        if (appName < node->record.app_name) {
            parent = node;
            node = node->left;
            wentLeft = true;
        } else if (node->record.app_name < appName) {
            parent = node;
            node = node->right;
            wentLeft = false;
        } else {
            break;
        }
    }

    if (t != node) {
        return;
    }

    tree *last = 0;
    tree *next = node->right;
    while (next != 0) {
        if (next->left == 0) {
            break;
        }
        last = next;
        next = next->left;
    }

    if (next == 0) {
        if (wentLeft) {
            if (parent != 0) {
                parent->left = node->left;
            } else {
                *catRoot = node->left;
            }
        } else {
            if (parent != 0) {
                parent->right = node->right;
            } else {
                *catRoot = node->right;
            }
        }
    } else if (last == 0) {
        if (wentLeft) {
            if (parent != 0) {
                parent->left = next;
            } else {
                *catRoot = next;
            }
        } else {
            if (parent != 0) {
                parent->right = next;
            } else {
                *catRoot = next;
            }
        }
        next->left = node->left;
    } else {
        if (wentLeft) {
            if (parent != 0) {
                parent->left = next;
            } else {
                *catRoot = next;
            }
        } else {
            if (parent != 0) {
                parent->right = next;
            } else {
                *catRoot = next;
            }
        }
        next->left = node->left;
        last->left = next->right;
        next->right = 0;
    }

    size_t i = ::hash(appName.c_str()) % hashtablesize;
    hash_table_entry *hLast;
    hash_table_entry *hNext;
    for (hLast = 0, hNext = hashtable[i]; hNext != 0; hLast = hNext, hNext = hNext->next) {
        if (hNext->app_name == appName) {
            if (hLast != 0) {
                hLast->next = hNext->next;
            } else {
                hashtable[i] = hNext->next;
            }
            delete(hNext->app_node);
            delete(hNext);
            break;
        }
    }
}

void cinGetLine(char *str, int len) {
    string line;
    getline(cin, line);
    size_t nl = line.find_first_of("\r\n");
    if (nl != string::npos) {
        line = line.substr(0, nl);
    }

    strncpy(str, line.c_str(), len);
    str[len - 1] = 0;
}

void cinGetLine(string &line) {
    getline(cin, line);
    size_t nl = line.find_first_of("\r\n");
    if (nl != string::npos) {
        line = line.substr(0, nl);
    }
}

template<typename T> void cinGet(T &var) {
    string line;
    getline(cin, line);
    std::stringstream ss(line);
    ss >> var;
}


void getInput() {
// Read from an input file, the following:
// * number of categories.
//.  + create array of categories
// * for each category, the name of the category.
// * number of applications.
//.  + create the hash table.
// * for each application:
//   - category
//.  - name
//.  - version
//   - size
//.  - units
//.  - price
//.  + create tree node for application.
//.  + add application's tree node to categories binary tree.  Create
//.    hash_node_entry for this application, and add (or link) into hash
//.    table.
cinGet(number_of_cats); // Get number of categories
cats = new categories[number_of_cats]; //Creat memory for #of categories

//Read through categories and save each one
    for (size_t c = 0; c < number_of_cats; c++) {
        string category;
        cinGetLine(cats[c].category, CAT_NAME_LEN);
        cats[c].root = 0;
    }

cinGet(number_of_apps); // Get number of apps.
hashtablesize = number_of_apps * 2; //Size of table. Double of m, first prime# that follows- Done in hash function
hashtable = new HashTableEntryPtr [hashtablesize]; //Creat Hashtable

//Initilize hashtable
    for (size_t i = 0; i < hashtablesize; i++) {
        hashtable [i] = 0;
    }
    //Read through applications, saving each application and their data
        for (size_t a = 0; a < number_of_apps; a++) {
             tree *app = new tree;

             cinGetLine(app -> record.category, CAT_NAME_LEN);
             cinGetLine(app -> record.app_name, APP_NAME_LEN);
             cinGetLine(app -> record.version, VERSION_LEN);
             cinGet(app -> record.size);
             cinGetLine(app -> record.units, UNIT_SIZE);
             cinGet(app -> record.price);
             app -> left = 0; //set left and Right pointers to null
             app -> right = 0;

             hash_table_entry * h = new hash_table_entry;

             //COPY APPLICATIONS NAME FROM TREE NODE INTO HASH TABLE ENTRY
              strncpy(h -> app_name, app -> record.app_name, APP_NAME_LEN);
              h->app_name[APP_NAME_LEN - 1] = 0;
              h->app_node = app;
              size_t i = ::hash(app -> record.app_name) % hashtablesize;
              h->next = hashtable [i];
              hashtable[i] = h;


        // Finding correct category to insert application into categories BST
        for ( size_t c = 0; c < number_of_cats; c++) {
            if (strcmp(app -> record.category, cats[c].category) == 0 ) {
                cats[c].root = treeInsertApp(cats[c].root, app);
                break;
            }
        }
    }

int number_of_cmds; //Number of Commands
cinGet(number_of_cmds); //get lines

    //Read through commands, and
    for (int i = 0; i < number_of_cmds; i++) {
        string line;
        cinGetLine(line);
        stringstream cmd(line);
        string command;
        cmd >> command;

        if (command == "find") {
            cmd >> command;
            if (command == "app") {
                string appName;
                getline(cmd >> ws, appName);

                size_t i = ::hash(appName.c_str()) % hashtablesize;
                bool foundIt = false;
                for (hash_table_entry *hh = hashtable[i]; hh; hh = hh->next) {
                    if (hh->app_name == appName) {
                       printTreeEntry(hh->app_node);
                        foundIt = true;
                        break;
                    }
                }
                         if (!foundIt) {
                            cout << "Application " << appName << " not found." << endl;
                         }

                         cout << endl;
            } else if (command == "category") {
                string catName;
                getline(cmd >> ws, catName);

                bool foundIt = false;
                for ( size_t c = 0; c < number_of_cats; c++) {
                    if (cats[c].category == catName) {
                        cout << "Category " << catName;
                        if (cats[c].root ==0) {
                            cout << " no apps found." << endl;
                        }
                        printBST(cats[c].root);
                        foundIt = true;
                        break;
                    }
                }
                         if (!foundIt) {
                            cout << "Unable to find category " << catName << "." << endl;
                          }
                     cout << endl;
            } else if (command == "price") {
                string price;
                cmd >> price;
                if (price == "free") {
                    for ( size_t c = 0; c < number_of_cats; c++) {
                        if (countBSTForFree(cats[c].root) > 0) {
                            cout << "Free Applications in Category: " << cats[c].category << endl;
                            searchBSTForFree(cats[c].root);
                            cout << endl;
                        }
                    }
                } else {
                    cout << "Unrecognized command: find price " << command << "???" << endl;
                }
            } else {
                cout << "Unrecognized command: find " << command << "???" << endl;
            }
        } else if (command == "range") {
            string catName;
            for (cmd >> command; command != "app" && command != "price"; cmd >> command) {
                catName.append(" " + command);
            }
            catName = catName.substr(1);
            if (command == "app") {
                string lo;
                string hi;
                cmd >> lo;
                cmd >> hi;

                bool foundIt = false;
                for ( size_t c = 0; c < number_of_cats; c++) {
                    if (cats[c].category == catName) {
                        if(countBSTForNames(cats[c].root, lo, hi) > 0) {
                            cout << "Applications in Range (" << lo << ", " << hi << ") in Category: " << catName << endl;
                            searchBSTForNames(cats[c].root, lo, hi);
                            foundIt = true;
                        }
                        break;
                    }
                }
                if (!foundIt) {
                    cout << "Unable to find category " << catName << "." << endl;
                }
                cout << endl;
            } else if (command == "price") {
                float lo;
                float hi;
                cmd >> lo;
                cmd >> hi;

                bool foundIt = false;
                for ( size_t c = 0; c < number_of_cats; c++) {
                    if (cats[c].category == catName) {
                        if(countBSTForPrices(cats[c].root, lo, hi) > 0){
                            cout << fixed << setprecision(2)  << "Applications in Price Range ($" << lo << ",$" << hi << ") in Category: " << catName << endl;
                            searchBSTForPrices(cats[c].root, lo, hi);
                            foundIt = true;
                        }
                        break;
                    }
                }
                if (!foundIt) {
                    cout << "Unable to find category " << catName << "." << endl;
                }
                cout << endl;
            } else {
                cout << "Unrecognized command: range " << command << "???" << endl;
            }
        } else if (command == "delete") {
            tree *catRoot = 0;
            string catName;
            for (cmd >> command; catRoot == 0; cmd >> command) {
                if (!catName.empty()) {
                    catName.append(" " + command);
                } else {
                    catName = command;
                }

                for ( size_t c = 0; c < number_of_cats; c++) {
                    if (cats[c].category == catName) {
                        catRoot = cats[c].root;
                        break;
                    }
                }
            }

            if (catRoot != 0) {
                string appName(command);
                for (cmd >> command; !cmd.eof(); cmd >> command) {
                    appName.append(" " + command);
                }
                appName.append(" " + command);
                if (tree const *app = searchBSTForName(catRoot, appName)) {
                    treeDeleteApp(app);
                    cout << "Application " << appName << " from category " << catName << " successfully deleted." << endl;
                } else {
                    cout << "Application " << appName << " not found in category " << catName << "; unable to delete." << endl;
                }
                cout << endl;
            } else {
                cout << "Unable to find category " << catName << "." << endl;
            }

        }else if(command == "report") {
            for ( size_t c = 0; c < number_of_cats; c++) {
               int depth = BstDepth(cats[c].root);
               int nodes = BstNodes(cats[c].root);
               cout << "Category "
                    << cats[c].category
                     << ": depth = "
                     << depth << ", number of nodes = "
                     << nodes << "." << endl;
            }

        } else {
            cout << "Unrecognized command: " << command << "???" << endl;
        }
    }
}

int main() {
    getInput();
    return 0;
}


