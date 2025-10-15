/**
 * Ryan Gould
 * CptS 223 PA2
 * AVL Map
 * This is a one file solution. I wanted to try it this way 
 * because i wouldn't have to build and compile more than one 
 * file.
 */





#ifndef PA2
#define PA2

//Libraries
#include <string>
#include <iostream>
#include <fstream> 




//Classes
class AvlMap {
    public:
    struct Node {
        int key; //Zip code
        std::string value; //City
        //pointers to children nodes
        Node* left;
        Node* right;
        int height; //height of node in a tree
        //Constructor that makes node with key and value parameters

Node(int n, const std::string& v)
: key(n), value(v), left(NULL), right(NULL), height(1) {}

    };

//Construcor that starts with an empty tree
AvlMap() : root(NULL), count(0) {}
//Destructor that frees all the dynamically allocated nodes
~AvlMap() { clear(root);}

//Insert function: if the key already exists, this will overwriet its value
bool insert(int key, const std::string& value) {
    bool insertedNew = false; // checks to see if a new node was inserted
    root = insertNode(root, key, value, insertedNew); //recursive insertion from root
    if(insertedNew) count++; //only increase size if new key was added
    return insertedNew; //returns whether or not a new node was inserted

}
 // search function, searches for a value by a key
 bool find(int key, std::string& out) const{
    Node* n = root;
    while(n != NULL)
    {
        if(key < n->key)
         n= n->left;
        else if(key > n->key)
        n = n->right;
        else{
            out = n->value;
            return true;
        }

    }
    return false;
 } 

//remove a key from the map, will return tru if the node was erased
bool erase(int key)
{
    bool erased = false;
    root = eraseNode(root, key, erased);
    if(erased) count--;
    return erased;
}

//size function returns the number of key-value pairs curently stored
int size() const {return count;}

//empty function, checks to see if the tree is empty
bool empty()const {return count == 0;}

//print function, prints N elements of the tree
void printFirstN(int n) const{
    int printed = 0;
    inorderPrint(root, n, printed);
}

//private data members
private:
Node* root; //pointer to root
int count; //number of nodes stored

//height function, returns height of a node(0 if NULL)
int h(Node* n) const {return n ? n->height : 0;}

//update function, updates height of node based on the heights of its children
void update(Node* n)
{
    int hleft = h(n->left);
    int hright = h(n->right);
    n->height = (hleft > hright ? hleft : hright) + 1;
}

//balance factro function, lheight - rheight. Positive means left heavy, negtive means right heavy
int bf(Node* n) const{ return h(n->left) - h(n->right);} 

//right rotation
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* rchild = x->right;
    x->right = y;
    y->left = rchild;
    update(y);
    update(x);
    return x;
}

//left rotation
Node* rotateLeft(Node* x){
    Node* y = x->right;
    Node* lchild = y->left;
    y->left = x;
    x->right = lchild;
    update(x);
    update(y);
    return y;

}

//rebalance function, will rebalance if bf is too high or low
Node* rebalance(Node* n) {
    update(n);
    int balance = bf(n);
    if (balance > 1) {
        if(bf(n->left) < 0)
        n->left = rotateLeft(n->left);
        return rotateRight(n);
    }
    if (balance < -1){
        if(bf(n->right) > 0)
        n->right = rotateRight(n->right);
        return rotateLeft(n);
    }
    return n; //no balancing needed

}

//recursive insert function
Node* insertNode(Node* n, int key, const std::string& value, bool& insertedNew) {
if (n==NULL) { //base case
insertedNew = true;
return new Node(key, value);
}
if(key < n->key)
n->left = insertNode(n->left, key, value, insertedNew);
else if(key > n->key)
n->right = insertNode(n->right, key, value, insertedNew);
else{
    n->value = value;
    insertedNew = false;
    return n; 
}
return rebalance(n);
}

//finds smallest or leftmost node
Node* minNode(Node* n) const {
    while (n && n->left) n = n->left;
    return n;
}

//recursive erase function, removes a node by key. 
Node* eraseNode(Node* n, int key, bool&erased) {
    if(n==NULL) return NULL;
    if(key < n->key)
    n->left = eraseNode(n->left, key, erased);
    else if(key > n->key)
    n->right = eraseNode(n->right, key, erased);
    else{
        erased = true;
        if(n->left == NULL || n->right ==NULL) {
            Node* child = (n->left) ? n->left : n->right;
            delete n;
            return child;
        }
        else{
            Node* success = minNode(n->right);
            n->key = success->key;
            n->value = success->value;
            bool boof = false;
            n->right = eraseNode(n->right, success->key, boof);
        }
    }
    if(n ==NULL) return NULL;
    return rebalance(n);
}


//clear function, free all nodeswith postorder traversal
void clear(Node* n) {
    if(!n) return;
    clear(n->left);
    clear(n->right);
    delete n;
}

//helper function for printing n nodes
void inorderPrint(Node* n, int N, int& printed) const {
    if(!n || printed >= N) return;
    inorderPrint(n->left, N, printed);
    if(printed < N) {
        std::cout << n->key << " => " << n->value << "\n";
        printed ++;
    }
    inorderPrint(n->right, N, printed);
}



};
 static void splitCSV(const std::string& line, std::string& c0, std::string&c1, std::string& c2){
    c0.clear(); c1.clear(); c2.clear();
    int field = 0;
    for(size_t i = 0; i< line.size(); ++i) {
        char ch = line[i];
        if(ch == ',') {
            field++;
            continue;

        }
        if(field == 0 ) c0.push_back(ch);
        else if(field ==1) c1.push_back(ch);
        else if(field == 2) c2.push_back(ch);

    }
 }
 static bool toInt(const std::string& s, int& out) {
    if(s.empty()) return false;
    int value = 0;
    for(size_t i = 0; i < s.size(); ++i) {
        char ch = s[i];
        if(ch < '0' || ch > '9')
        return false;
        int digit = ch - '0';
        value = value * 10 + digit;
    }
    out = value;
    return true;

 }
 int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << "<path to zips.csv>\n";
        std::cerr<< "Expected format: Zip, city, state, etc..\n";
        return 1;
    }
    std::string csvPath = argv[1];
    std::ifstream in(csvPath.c_str());
    if(!in) {
        std::cerr << "Cannot open file: "<< csvPath << "\n";
        return 2;
    }


AvlMap map;


std::string line;
if(!std::getline(in, line)) {
std::cerr << "Empty File: "<< "\n";
return 3;

}
int rows = 0;
while(std::getline(in, line)) {
    if(line.empty()) continue;
    std::string col0, col1, col2;
    splitCSV(line, col0, col1, col2);


    int zip = 0;
    if(!toInt(col0, zip))
    continue;

    std::string value;
    if(!col1.empty()) value += col1;
    if(!col2.empty()) {
        if(!value.empty()) value += ", ";
        value += col2;
    }
if(value.empty()) value = "No city/state";

map.insert(zip, value);
rows++;
}

std::cout<< "Loaded" << rows<< " rows. Unique Zips in AVL: " << map.size() << "\n";

std::cout << "First 10 entries (Zip/city/state): \n";
map.printFirstN(10);

std::cout << "\nType a zip code to search for: (or -1 to exit)\n";
while (true) {
    std::cout << "> ";
    int query = 0;
    if(!(std::cin >> query))
    break;
    if(query == -1) break;

    std::string found;
    if(map.find(query, found))
    std::cout << query << " -> " << found << "\n";
    else
    std::cout << "Not found" << "\n";

}

return 0;
 }




#endif

//Main.cpp






