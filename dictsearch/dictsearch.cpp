#include "dictsearch.h"

wstring Util::toWstring(const string& str) {

    setlocale(LC_ALL, "zh_CN.UTF-8");

    const char* _s = str.c_str();
    size_t _dSize = str.size() + 1;

    wchar_t* _d = new wchar_t[_dSize];
    wmemset(_d, 0, _dSize);

    mbstowcs(_d, _s, _dSize);
    wstring wstr(_d);
    
    delete[] _d;

    return wstr;
}

Node::Node(void) {
    this->index = -1;
    this->trace = NULL;
}

Node::~Node(void) {
    map<wchar_t, Node*>::iterator it = this->child.begin();
    while (it != this->child.end()) {
        delete it->second;
        it->second = NULL;
        this->child.erase(it ++);
    }
}

void DictSearch::setTrace(void) {
    Node *node = NULL;
    Node *child = NULL;
    Node *tmp = NULL;
    deque<Node*> que;
    map<wchar_t, Node*>::iterator it, tmpIt;
    wchar_t c = '\0';

    que.push_back(this->root);
    while (!que.empty()) {
        node = que.front();
        que.pop_front();

        it = node->child.begin();
        while (it != node->child.end()) {
            child = it->second;
            c = it->first;

            tmp = node;
            while (true) {
                tmp = tmp->trace; 
                if (tmp == NULL) {
                    child->trace = this->root;
                    break;
                } else {
                    tmpIt = tmp->child.find(c);
                    if (tmpIt != tmp->child.end()) {
                        child->trace = tmpIt->second;
                        break;
                    }
                }

            }

            que.push_back(child);
            it ++;
        }
    }
}

void DictSearch::insert(const string& key, int index) {
    Node *node = this->root;
    Node *newChild = NULL;
    map<wchar_t, Node*>::iterator it;
    wstring wkey = Util::toWstring(key);
    int len = wkey.size();
    wchar_t c = '\0';

    for (int i = 0; i < len; i ++) {
        c = wkey.at(i);
        it = node->child.find(c);
        if (it != node->child.end()) {
            node = it->second;
        } else {
            newChild = new Node();
            node->child.insert(pair<wchar_t, Node*>(c, newChild));
            node = newChild;
        }
    }
    node->index = index;

    // node = NULL;
    // newChild = NULL;
}

DictSearch::DictSearch(const vector<string>& keys) {
    this->root = new Node();
    this->keys = keys;

    int size = keys.size();
    for (int i = 0; i < size; i ++) {
        this->insert(keys[i], i);
    }
    this->setTrace();
}

DictSearch::~DictSearch(void) {
    this->keys.clear();

    delete this->root;
    this->root = NULL;
}

vector<string> DictSearch::search(const string& word) {
    Node *node = this->root;
    Node *tmp = NULL;
    map<int, bool> indexsMap;
    map<int, bool>::iterator mapIt;
    vector<string> mappedKeys;
    map<wchar_t, Node*>::iterator it, tmpIt;
    wstring wword = Util::toWstring(word);
    int len = wword.size();
    wchar_t c = '\0';

    for (int i; i < len; i ++) {
        c = wword.at(i);
        it = node->child.find(c);

        if (it != node->child.end()) {
            node = it->second;
        } else {
            while (true) {
                node = node->trace;
                if (node == NULL) {
                    node = this->root;
                    break;
                } else {
                    tmpIt = node->child.find(c);
                    if (tmpIt != node->child.end()) {
                        node = tmpIt->second;
                        break;
                    }
                }
            }

        }

        if (node->index != -1) {
            indexsMap.insert(pair<int, bool>(node->index, true));
            tmp = node->trace;
            while (true) {
                if (tmp == NULL) {
                    break;
                } else {
                    if (tmp->index != -1) {
                        indexsMap.insert(pair<int, bool>(tmp->index, true));
                    }
                    tmp = tmp->trace;
                }
            }
        }
    }

    mapIt = indexsMap.begin();
    while (mapIt != indexsMap.end()) {
        mappedKeys.push_back(this->keys[mapIt->first]);
        mapIt ++;
    }

    return mappedKeys;
}

int main() {
    vector<string> keys;
    string data;

    ifstream reader("dict.txt");
    while (!reader.eof()) {
        reader >> data;
        keys.push_back(data);
    }
    keys.pop_back();
    reader.close();

    string word = "protecting@环境卫生和服务质量，都很大程度地影响人们的点评结果和质量";
    DictSearch *dS = new DictSearch(keys);
    vector<string> mappedKeys = dS->search(word);

    vector<string>::iterator it = mappedKeys.begin();
    while (it != mappedKeys.end()) {
        cout << *it << endl;
        it ++;
    }

    delete dS;
    dS = NULL;

    return 0;
}
