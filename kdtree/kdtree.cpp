#include "kdtree.h"

double Util::distance(vector<double> x, vector<double> y) {
    double distance = 0;
    int dim = x.size();
    for (int i = 0; i < dim; i ++) {
        distance += pow(x[i] - y[i], 2);
    }
    return sqrt(distance);
}


KdNode::KdNode(vector<vector<double> > coordinateVec, int target, int splitDim) {
    this->splitDim = coordinateVec.size() == 1? -1: splitDim;
    this->coordinate = coordinateVec[target];
    double compare = 0;
    int nodeCount = coordinateVec.size();
    for (int i = 0; i < nodeCount; i ++) {
        compare = coordinateVec[i][splitDim] - this->coordinate[splitDim];
        if (compare > 0) {
            this->rightVec.push_back(coordinateVec[i]);
        } else if (i != target) {
            this->leftVec.push_back(coordinateVec[i]);
        }
    }
}

KdNode::~KdNode(void) {
    this->coordinate.clear();
    if (this->left != NULL) {
        delete this->left;
        this->left = NULL;
    }
    if (this->right != NULL) {
        delete this->right;
        this->right = NULL;
    }
}

int KdNode::getSplitDim(void) {
    return this->splitDim;
}

vector<vector<double> > KdNode::getLeftVec(void) {
    return this->leftVec;
}

vector<vector<double> > KdNode::getRightVec(void) {
    return this->rightVec;
}

void KdNode::done(void) {
    this->leftVec.clear();
    this->rightVec.clear();
}

double KdNode::distance(vector<double> coordinate) {
    return Util::distance(this->coordinate, coordinate);
}

KdNode* KdTree::createNode(vector<vector<double> > coordinateVec) {
    vector<vector<double> > vecs = coordinateVec;
    int nodeCount = vecs.size();
    // if coordinateVec is empty, return NULL
    if (nodeCount == 0) {
        return NULL;
    }
    int dim = vecs[0].size();
    int splitDim = 0;
    vector<double> var(dim, 0);
    vector<double> avg(dim, 0);
    vector<double> order(nodeCount, 0);
    map<double, int> keyMap;
    double maxVar = 0;
    int midPos = nodeCount / 2;
    for (int i = 0; i < nodeCount; i ++) {
        for (int j = 0; j < dim; j ++) {
            avg[j] += vecs[i][j];
        }
    }
    for (int j = 0; j < dim; j ++) {
        avg[j] /= nodeCount;
    }
    for (int i = 0; i < nodeCount; i ++) {
        for (int j = 0; j < dim; j ++) {
            var[j] += pow(vecs[i][j] - avg[j], 2);
        }
    }

    for (int j = 0; j < dim; j ++) {
        if (var[j] > maxVar) {
            maxVar = var[j];
            splitDim = j;
        }
    }

    for (int i = 0; i < nodeCount; i ++) {
        order[i] = vecs[i][splitDim];
        keyMap.insert(pair<double, int>(order[i], i));
    }
    sort(order.begin(), order.end());
    int idx = keyMap.find(order[midPos])->second;
    return new KdNode(vecs, idx, splitDim);
}

KdTree::KdTree(vector<vector<double> > coordinateVec) {
    stack<KdNode*> nodeStack;
    bool leftFlag = true;
    KdNode* root = this->createNode(coordinateVec);
    KdNode* node = this->createNode(root->getLeftVec());
    this->root = root;
    nodeStack.push(root);

    while (true) {
        if (leftFlag) {
            root->left = node;
        } else {
            root->right = node;
            root->done();
        }

        if (node == NULL) {
            if (nodeStack.empty()) {
                break;
            }
            root = nodeStack.top();
            nodeStack.pop();
            leftFlag = false;
            node = this->createNode(root->getRightVec());
        } else {
            root = node;
            nodeStack.push(root);
            leftFlag = true;
            node = this->createNode(root->getLeftVec());
        }
    }
}

KdTree::~KdTree(void) {
    delete this->root;
    this->root = NULL;
}

vector<double> KdTree::search(vector<double> coordinate) {
    stack<KdNode*> nodeStack;
    stack<bool> leftVisitStack;
    KdNode* node = this->root;
    KdNode* nearest = NULL;
    int splitDim = 0;
    bool leftVisit = false;
    bool trace = false;
    bool flag = true;
    double distance = 0.0;
    double tDis = 0.0;

    while (flag) {
        
        if (node == NULL) {
            node = nodeStack.top();
            nodeStack.pop();
            leftVisitStack.pop();
            trace = true;
        } else {
            splitDim = node->getSplitDim();
            if (splitDim != -1) {
                nodeStack.push(node);
                if (coordinate[splitDim] <= node->coordinate[splitDim]) {
                    node = node->left;
                    leftVisitStack.push(true);
                } else {
                    node = node->right;
                    leftVisitStack.push(false);
                }
            } else {
                trace = true;
            }
        }

        if (trace) {
            tDis = node->distance(coordinate);
            if (nearest == NULL || tDis < distance) {
                distance = tDis;
                nearest = node;
            }
        }

        while (trace) {
            if (nodeStack.empty()) {
                flag = false;
                break;
            }
            node = nodeStack.top();
            nodeStack.pop();
            leftVisit = leftVisitStack.top();
            leftVisitStack.pop();
            tDis = node->distance(coordinate);
            if (distance > tDis) {
                distance = tDis;
                nearest = node;
            }
            splitDim = node->getSplitDim();
            if (pow(distance, 2) > pow(node->coordinate[splitDim] - coordinate[splitDim], 2)) {
                node = leftVisit? node->right: node->left;
                trace = false;
            }
        }
    }
    // node = NULL;
    return nearest->coordinate;
}

int main() {
    vector<vector<double> > coordinateVec;
    string data;
    ifstream reader("points.txt");
    char* p;
    const char* splitChars = ",";
    while (getline(reader, data)) {
        vector<double> coordinate;
        p = strtok(const_cast<char*>(data.c_str()), splitChars);
        while (p != NULL) {
            coordinate.push_back(atof(p));
            p = strtok(NULL, splitChars);
        }
        coordinateVec.push_back(coordinate);
    }
    reader.close();

    delete[] p;

    vector<double> point;
    point.push_back(2);
    point.push_back(4.5);

    KdTree* kdT = new KdTree(coordinateVec);
    vector<double> nearest = kdT->search(point);
    vector<double>::iterator it = nearest.begin();
    cout << "neareast: ";
    while (it != nearest.end()) {
        printf("%.2lf ", *it);
        it ++;
    }
    cout << "\ndistance: " << Util::distance(nearest, point) << endl;

    delete kdT;
    kdT = NULL;
    return 0;
}
