#!/usr/bin/python
#coding: UTF-8
import os


class Apriori:
    

    def __init__(self, data, support, confidence):
        self.__separation = '|'
        self.__connection = '->'
        self.__support = float(support) 
        self.__confidence = float(confidence)
        self.__data = data

    
    def __getFrequentCollections(self):
        frequents = []
        frequent = {}
        count = len(self.__data)

        while True:
            candidate = self.__getCandidateCollections(frequent)
            frequent = {}
            for candK, candV in candidate.items():
                candItems = candK.split(self.__separation)

                for items in self.__data:
                    # list(set(candItems).difference(set(items)))
                    if not [v for v in candItems if v not in items]:
                        candidate[candK] += 1

                candidate[candK] /= count * 1.0

            frequent = dict(filter(lambda (k, v): v >= self.__support, candidate.items()))

            if not frequent:
                break

            frequents.append(frequent)

        return frequents


    def __getCandidateCollections(self, frequent):
        tmp = []
        count = len(frequent)

        if count == 0:
            for k in self.__data:
                tmp.extend(k)
            tmp = list(set(tmp))
                        
        else:
            items = [k.split(self.__separation) for k, v in frequent.items()]

            for i in range(count - 1):
                for j in range(count):
                    diff = [v for v in items[i] if v not in items[j]]
                    if len(diff) != 1:
                        continue

                    prepare = sorted(items[j] + diff)
                    prepareStr = self.__separation.join(prepare)
                    if prepareStr not in tmp:
                        tmp.append(prepareStr)

        tmp.sort()
        candidate = {k: 0 for k in tmp}
        return candidate

   
    def __getProperSubset(self, sets):
        return [[v for k, v in enumerate(sets) if (i >> k) & 1 == 1] for i in range(1, (1 << len(sets)) - 1)]


    def getRelationRules(self):
        relation = {}
        frequents = self.__getFrequentCollections();

        for frequent in frequents:

            for freqK, freqV in frequent.items():
                freq = freqK.split(self.__separation)
                subset = self.__getProperSubset(freq)
    
                for subItem in subset:
                    subItemStr = self.__separation.join(subItem)
                    targetStr = self.__separation.join([v for v in freq if v not in subItem])
                    prepareStr = self.__connection.join([subItemStr, targetStr])
    
                    if prepareStr not in relation.keys():
                        confidenceV = freqV / frequents[len(subItem) - 1][subItemStr]
                        if confidenceV >= self.__confidence:
                            relation[prepareStr] = {
                                'support': freqV,
                                'confidence': confidenceV
                            }
        
        return relation



if __name__ == '__main__':
    data = []
    with open('data.txt') as f:
        data = [l.strip().split(',') for l in f if l]

    apr = Apriori(data, 0.2, 0.5)
    relation = apr.getRelationRules()

    print(relation)
