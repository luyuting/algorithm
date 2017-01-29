#ifndef APRIORI_H_INCLUDED
#define APRIORI_H_INCLUDED

#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

typedef map<string, double > baseMap;
typedef map<string, baseMap > relMap;

class Util {
public:
    static vector<string> explode(const char* split, const string& str);
    static string implode(const char* glue, const vector<string>& strs);
    static vector<string> diff(const vector<string>& strA, const vector<string>& strB); 
};

class Apriori {
private:
    double support;
    double confidence;
    static const char* seperation;
    static const char* connection;
    vector<string> data;
    vector<baseMap> getFrequentCollections(void);
    baseMap getCandidateCollections(const baseMap& freq);
    vector<vector<string> > getProperSubset(const vector<string>& set);

public:
    explicit Apriori(void);
    ~Apriori(void);
    relMap getRelationRules(const vector<vector<string> >& data, double support, double confidence);
};

#endif // APRIORI_H_INCLUDED
