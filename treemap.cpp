#include <bits/stdc++.h>
using namespace std; 
using namespace std::chrono;
using lli = long long int;

bool black = true;
bool red = false;

class Vertex {
public:
    Vertex* left;
    Vertex* right; 
    Vertex* par; 
    bool isLeft; 
    int left_size;
    int right_size;

    int key; 
    int val; 
    int cnt;

    Vertex(int _key, bool _isleft) {
        left = nullptr;
        right = nullptr;
        par = nullptr; 
        isLeft = _isleft; 
        left_size = 0;
        right_size = 0; 

        key = _key;
        cnt = 1; 
    }
};

Vertex* leftmost(Vertex* v) {
    if (!v) return nullptr;
    while(true) {
        if (v->left) v = v->left;
        else return v;
    }
    return nullptr; 
}

Vertex* rightmost(Vertex* v) {
    if (!v) return nullptr;
    while(true) {
        if (v->right) v = v->right;
        else return v;
    }
    return nullptr; 
}

Vertex* next(Vertex* v) {
    if (!v) return nullptr;
    if (v->right) return leftmost(v->right); 
    while(true) {
        bool ok = false;
        if (!v) return nullptr;
        if (v->isLeft) ok = true;
        v = v->par;
        if (ok) return v; 
    }
    return nullptr;
}

class TreeMap {
public:
    Vertex* head;
    Vertex* basic_insert(int key, Vertex* v) {
        while(true) {
            if (key == v->key) {
                v->cnt++;
                return v; 
            }
            else if (key < v->key) {
                v->left_size++; 
                if (v->left) v = v->left;
                else {
                    v->left = new Vertex(key, true);
                    v->left->par = v; 
                    return v->left; 
                }
            }
            else {
                v->right_size++; 
                if (v->right) v = v->right;
                else {
                    v->right = new Vertex(key, false);
                    v->right->par = v;
                    return v->right; 
                }
            }
        }
        return nullptr; 
    }

    Vertex* basic_find(int key, Vertex* v) {
        while(true) {
            if (!v) return nullptr;
            if (key == v->key) return v;
            else if (key < v->key) v = v->left;
            else v = v->right; 
        }
        return nullptr;
    }

    void rotate_left(Vertex* z) {
        Vertex *y = z->right;
        z->right = y->left;
        z->right_size = y->left_size; 
        if (y->left) {
            y->left->par = z;
            y->left->isLeft = false;
        }
        y->left = z;
        y->left_size = z->right_size + 1 + z->left_size;
        y->par = z->par; 
        z->par = y;
        y->isLeft = z->isLeft;
        z->isLeft = true; 
        if (!y->par) head = y; 
        else {
            if (y->isLeft) y->par->left = y;
            else y->par->right = y; 
        }
    }

    void rotate_right(Vertex* z) {
        Vertex *y = z->left;
        z->left = y->right;
        z->left_size = y->right_size; 
        if (y->right) {
            y->right->par = z;
            y->right->isLeft = true;
        }
        y->right = z;
        y->right_size = z->left_size + 1 + z->right_size;
        y->par = z->par; 
        z->par = y;
        y->isLeft = z->isLeft;
        z->isLeft = false; 
        if (!y->par) head = y; 
        else {
            if (y->isLeft) y->par->left = y;
            else y->par->right = y; 
        }
    }

    void rebalance(Vertex* v) {
        if (!v) return; 
        Vertex *x, *y, *z;
        x = v; 
        if (x->par) y = x->par;
        else return;
        if (y->par) z = y->par;
        else return;

        while(true) {
            if (!z) return; 
            if (abs(z->left_size - z->right_size) > 1) break; 
            Vertex* tmp = y;
            y = z;
            x = tmp;
            z = z->par; 
        }
            
        if (y->isLeft && x->isLeft) {
            rotate_right(z);
        }
        else if (y->isLeft && !x->isLeft) {
            rotate_left(y);
            rotate_right(z);
        }
        else if (!y->isLeft && x->isLeft) {
            rotate_right(y);
            rotate_left(z);
        }
        else {
            rotate_left(z); 
        }
    }

    void insert(int key) {
        if (!head) {
            head = new Vertex(key, true);
            return;
        }
        rebalance(basic_insert(key, head));
    }

    Vertex* find(int key) {
        if (!head) return nullptr;
        return basic_find(key, head);
    }

    Vertex* min() {
        return leftmost(head); 
    }

    Vertex* max() {
        return rightmost(head); 
    }

    TreeMap() {
        head = nullptr;
    }
};

void dfs(Vertex *v) {
    if (!v) return;
    cout << v->key << endl;
    cout << "Parent: " << (v->par ? v->par->key : -1) << endl;
    cout << "Left: " << (v->left ? v->left->key : -1) << endl;
    cout << "Right: " << (v->right ? v->right->key : -1) << endl;
    cout << "Left size: " << v->left_size << endl;
    cout << "Right size: " << v->right_size << endl;
    cout << "-------\n";
    dfs(v->left);
    dfs(v->right); 
}

int main(void) {
    TreeMap data = TreeMap();
    map <int, int> ref;
    int tot = 1000000;
    vector <int> x;
    for (int i = 0; i < tot; i++) x.push_back(rand()); 

    auto t0 = high_resolution_clock::now();
    for (int i = 0; i < tot; i++) data.insert(x[i]);
    //dfs(data.head);
    auto t1 = high_resolution_clock::now();
    for (int i = 0; i < tot; i++) ref[x[i]] = 0;
    auto t2 = high_resolution_clock::now();

    for (int i = 0; i < tot; i++) data.find(x[i]);
    auto t3 = high_resolution_clock::now();
    for (int i = 0; i < tot; i++) ref.find(x[i]); 
    auto t4 = high_resolution_clock::now();

    int a = (duration_cast<microseconds>(t1 - t0)).count();
    int b = (duration_cast<microseconds>(t2 - t1)).count();
    int c = (duration_cast<microseconds>(t3 - t2)).count();
    int d = (duration_cast<microseconds>(t4 - t3)).count();
    cout << a << " " << b << endl;    
    cout << c << " " << d << endl;    

    // Vertex* v = data.find(10);
    // cout << next(v)->key << endl;
      
    printf("Finished successfully\n");
    return 0;
}