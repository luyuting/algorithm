#ifndef KDTREE_H_INCLUDED
#define KDTREE_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <map>

using namespace std;

class Util {
public:
    static double distance(vector<double> x, vector<double> y);
};

class KdNode {
private:
    int splitDim;
    vector<vector<double> > leftVec;
    vector<vector<double> > rightVec;

public:
    vector<double> coordinate;
    KdNode* left;
    KdNode* right;
    KdNode(vector<vector<double> > coordinateVec, int target, int splitDim);
    ~KdNode(void);
    int getSplitDim(void);
    vector<vector<double> > getLeftVec(void);
    vector<vector<double> > getRightVec(void);
    void done(void);
    double distance(vector<double> coordinate);
};

class KdTree {
private:
    KdNode* root = NULL;
    KdNode* createNode(vector<vector<double> > coordinateVec);

public:
    KdTree(vector<vector<double> > coordinateVec);
    ~KdTree(void);
    vector<double> search(vector<double> coordinate);
};


#endif // KDTREE_H_INCLUDED
