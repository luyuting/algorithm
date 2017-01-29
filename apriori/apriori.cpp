#include "apriori.h"

vector<string> Util::explode(const char* split, const string& str) {
    char *buffer, *p;
    buffer = (char*) malloc(sizeof(char) * (str.size() + 1));
    size_t len = str.copy(buffer, str.size(), 0);
    buffer[len] = '\0';
    vector<string> strs;

    p = strtok(buffer, split);
    while (p != NULL) {
        strs.push_back(p);
        p = strtok(NULL, split);
    }

    free(buffer);
    delete[] p;
    buffer = NULL;
    p = NULL;

    return strs;
}

string Util::implode(const char* glue, const vector<string>& strs) {
    string str;
    vector<string>::const_iterator it = strs.begin();

    str.append(*(it ++));
    while (it != strs.end()) {
        str.append(glue);
        str.append(*it);
        it ++;
    }

    return str;
}

vector<string> Util::diff(const vector<string>& strA, const vector<string>& strB) {
    vector<string> diffStrs;
    map<string, bool> strMap;
    map<string, bool>::iterator mit;

    vector<string>::const_iterator it = strA.begin();
    while (it != strA.end()) {
        strMap.insert(pair<string, bool>(*it, true)); 
        it ++;
    }

    it = strB.begin();
    while (it != strB.end()) {
        mit = strMap.find(*it);
        if (mit != strMap.end()) {
            strMap.erase(mit);
        }
        it ++;
    }

    mit = strMap.begin();
    while (mit != strMap.end()) {
        diffStrs.push_back(mit->first);
        mit ++;
    }

    strMap.clear();

    sort(diffStrs.begin(), diffStrs.end());

    return diffStrs;
}

const char* Apriori::seperation = "|";

const char* Apriori::connection = "->";

Apriori::Apriori(void) {

}

vector<baseMap> Apriori::getFrequentCollections(void) {
    vector<baseMap > frequents;
    vector<string> candItems, items;
    vector<string>::iterator vit;
    baseMap candidate;
    baseMap::iterator bit;
    int count = this->data.size();

    do {
        candidate = this->getCandidateCollections(candidate);

        bit = candidate.begin();
        while (bit != candidate.end()) {
            candItems = Util::explode(seperation, bit->first);

            vit = this->data.begin();
            while (vit != this->data.end()) {
                items = Util::explode(seperation, *vit);
                if (Util::diff(candItems, items).size() == 0) {
                    candidate[bit->first] += 1;
                }
                vit ++;
            }

            candidate[bit->first] /= count;
            bit ++;
        }

        bit = candidate.begin();
        while (bit != candidate.end()) {
            if (bit->second < this->support) {
                candidate.erase(bit);  
            }
            bit ++;
        }

        if (candidate.size() == 0) {
            break;
        }

        frequents.push_back(candidate);
    } while (true);

    return frequents;
}

baseMap Apriori::getCandidateCollections(const baseMap& freq) {
    baseMap candidate;
    int count = freq.size();

    if (count == 0) {
        vector<string> initialData;
        vector<string>::iterator vit, iit;

        vit = this->data.begin();
        while (vit != this->data.end()) {
            initialData = Util::explode(seperation, *vit);

            iit = initialData.begin();
            while (iit != initialData.end()) {
                if (candidate.find(*iit) == candidate.end()) {
                    candidate[*iit] = 0;
                }
                iit ++;
            }
            vit ++;
        }
    } else {
        vector<vector<string> > items;
        vector<string> diff, prepare;
        string prepareStr;

        baseMap::const_iterator bit = freq.begin();
        while (bit != freq.end()) {
            items.push_back(Util::explode(seperation, bit->first));
            bit ++;
        }

        for (int i = 0; i < count; i ++) {
            for (int j = i + 1; j < count; j ++) {
                diff = Util::diff(items[i], items[j]);
                if (diff.size() != 1) {
                    continue;
                }

                prepare = items[j];
                for (int k = 0; k < diff.size(); k ++) {
                    prepare.push_back(diff[k]);
                }

                sort(prepare.begin(), prepare.end());
                prepareStr = Util::implode(seperation, prepare);

                if (candidate.find(prepareStr) == candidate.end()) {
                    candidate[prepareStr] = 0;
                }
            }
        }

    }

    return candidate;
}

vector<vector<string> > Apriori::getProperSubset(const vector<string>& set) {
    vector<vector<string> > subset;
    long total = 1;

    for (int n = 0; n ++ < set.size(); total <<= 1);
    total --;

    for (long i = 1; i < total; i ++) {
        vector<string> vs;
        long mask = i;

        for (int k = 0; k < set.size(); k ++) {
            if (mask & 1 == 1) {
                vs.push_back(set[k]);
            }
            mask >>= 1;
        }
        subset.push_back(vs);
    }
    return subset;
}

Apriori::~Apriori(void) {

}

relMap Apriori::getRelationRules(const vector<vector<string> >& data, double support, double confidence) {
    relMap relation;
    vector<baseMap> frequents;
    vector<baseMap>::iterator vit;
    baseMap frequent, tmpB;
    baseMap::iterator bit;
    vector<string> freq, tmpL, formatData;
    vector<vector<string> > subset;
    vector<vector<string> >::iterator sit;
    vector<vector<string> >::const_iterator cit;
    string subItemStr, targetStr, prepareStr;
    double confidence_v;

    cit = data.begin();
    while (cit != data.end()) {
        formatData.push_back(Util::implode(seperation, *cit));    
        cit ++;
    }
    
    this->data = formatData;
    this->support = support;
    this->confidence = confidence;

    frequents = this->getFrequentCollections();
    vit = frequents.begin();
    while (vit != frequents.end()) { 
        frequent = *vit;
        bit = frequent.begin();

        while (bit != frequent.end()) {
            freq = Util::explode(seperation, bit->first);
            subset = this->getProperSubset(freq);
            sit = subset.begin();

            while (sit != subset.end()) {
                subItemStr = Util::implode(seperation, *sit);
                targetStr = Util::implode(seperation, Util::diff(freq, *sit));

                tmpL.push_back(subItemStr);
                tmpL.push_back(targetStr);

                prepareStr = Util::implode(connection, tmpL);

                tmpL.clear();

                if (relation.find(prepareStr) == relation.end()) {
                    confidence_v = bit->second / frequents[(*sit).size() - 1].find(subItemStr)->second;
                    if (confidence_v >= confidence) {
                        tmpB.insert(pair<string, double>("support", bit->second));
                        tmpB.insert(pair<string, double>("confidence", confidence_v));

                        relation.insert(pair<string, baseMap>(prepareStr, tmpB));

                        tmpB.clear();
                    }
                }

                sit ++;
            }

            bit ++;
        }

        vit ++; 
    }

    return relation;
}

int main() {
    vector<vector<string> > data;
    vector<string> tmpV;
    string tmpS;
    ifstream reader("data.txt");
    char* p;
    const char* splitChars = ",";
    while (getline(reader, tmpS)) {
        vector<double> coordinate;
        p = strtok(const_cast<char*>(tmpS.c_str()), splitChars);
        while (p != NULL) {
            tmpV.push_back(p);
            p = strtok(NULL, splitChars);
        }
        data.push_back(tmpV);
        tmpV.clear();
    }
    reader.close();

    delete[] p;

    double support = 0.2;
    double confidence = 0.5;
    relMap relation;
    relMap::iterator rit;

    Apriori *apr = new Apriori();
    relation = apr->getRelationRules(data, support, confidence);

    rit = relation.begin();
    while (rit != relation.end()) {
        cout << "[" << rit->first << "]\t" << "support: " << rit->second["support"]
            << "\tconfidence: " << rit->second["confidence"] << endl;
        rit ++;
    }
    return 0;
}
