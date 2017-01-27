#ifndef DICTSEARCH_H_INCLUDED
#define DICTSEARCH_H_INCLUDED

#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <fstream>

using namespace std;

class Util {
public:
    static wstring toWstring(const string& str);
};

class Node {
public:
    int index;
    map<wchar_t, Node*> child;
    Node *trace;

    explicit Node(void);
    ~Node(void);
};

class DictSearch {
private:
    Node *root;
    vector<string> keys;
    void setTrace(void);
    void insert(const string& key, int index);

public:
    explicit DictSearch(const vector<string>& keys);
    ~DictSearch(void);
    vector<string> search(const string& word);
};

#endif // DICTSEARCH_H_INCLUDED
