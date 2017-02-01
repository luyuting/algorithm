#!/usr/bin/python
#coding: UTF-8
import os
from collections import deque

import sys
reload(sys)
sys.setdefaultencoding('utf-8')

class Node:

    def __init__(self):
        self.index = -1
        self.trace = None
        self.child = {}


class DictSearch:
    
    def __init__(self, keys = []):
        self.__root = Node()
        self.__keys = keys

        for i in range(len(keys)):
            self.__insert(keys[i], i)
        self.__setTrace()

    
    def __setTrace(self):
        que = deque([self.__root])

        while que:
            node = que.popleft()

            for c, child in node.child.items():
                tmp = node
                while True:
                    tmp = tmp.trace
                    if not tmp:
                        child.trace = self.__root
                        break
                    elif c in tmp.child.keys():
                        child.trace = tmp.child[c]
                        break

                que.append(child)


    def __insert(self, key, index):
        node = self.__root
        key = key.decode('utf-8')
        
        for c in key:
            if c in node.child.keys():
                node = node.child[c]
            else:
                newChild = Node()
                node.child[c] = newChild
                node = newChild

        node.index = index


    def search(self, word):
        node = self.__root
        indexsMap = {}
        mappedKeys = []
        word = word.decode('utf-8')

        for c in word:
            if c in node.child.keys():
                node = node.child[c]
            else:
                while True:
                    node = node.trace
                    if not node:
                        node = self.__root
                        break
                    elif c in node.child.keys():
                        node = node.child[c]
                        break

            if node.index != -1:
               indexsMap[node.index] = True
               tmp = node.trace

               while tmp:
                   if tmp.index != -1:
                       indexsMap[tmp.index] = True
                   tmp = tmp.trace

        mappedKeys = [self.__keys[k] for k, v in indexsMap.items()]

        return mappedKeys


if __name__ == '__main__':
    keys = []

    with open('dict.txt') as f:
        keys = [l.strip() for l in f if l]
    # keys = filter(lambda x: x, keys)

    word = 'protecting@环境卫生和服务质量，都很大程度地影响人们的点评结果和质量';

    dS = DictSearch(keys)
    mappedKeys = dS.search(word)
    keyStr = ','.join(mappedKeys)
    print(keyStr)
