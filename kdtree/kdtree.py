#!/usr/bin/python
#coding: UTF-8
import os
import math

class Util:

    @staticmethod
    def distance(x, y):
        return math.sqrt(sum([(x[i] - y[i]) ** 2 for i in range(len(x))]))


class KdNode:

    def __init__(self, coordinateVec, target, splitDim):
        self.left = None
        self.right = None
        self.coordinate = coordinateVec[target]
        self.__splitDim = splitDim
        self.__leftVec = []
        self.__rightVec = []

        if len(coordinateVec) == 1:
            self.__splitDim = -1        

        for i, v in enumerate(coordinateVec):
            compare = v[splitDim] - self.coordinate[splitDim]
            if compare > 0:
                self.__rightVec.append(v)
            elif i != target:
                self.__leftVec.append(v)


    def getSplitDim(self):
        return self.__splitDim


    def getLeftVec(self):
        return self.__leftVec


    def getRightVec(self):
        return self.__rightVec


    def done(self):
        self.__leftVec = []
        self.__rightVec = []


    def distance(self, coordinate):
        return Util.distance(self.coordinate, coordinate)


class KdTree:

    def __init__(self, coordinateVec):
        nodeStack = []
        leftFlag = True
        root = self.__createNode(coordinateVec)
        node = self.__createNode(root.getLeftVec())
        
        self.__root = root

        nodeStack.append(root)
        while True:
            if leftFlag:
                root.left = node
            else:
                root.right = node
                root.done()

            if not node:
                if not nodeStack:
                    break
                root = nodeStack.pop()
                leftFlag = False
                node = self.__createNode(root.getRightVec())
            else:
                root = node
                nodeStack.append(root)
                leftFlag = True
                node = self.__createNode(root.getLeftVec())


    def __createNode(self, coordinateVec):
        size = len(coordinateVec)
        if size == 0:
            return None

        dim = len(coordinateVec[0])
        splitDim = 0
        maxVar = 0
        midPos = size // 2

        trans = [[r[i] for r in coordinateVec] for i in range(dim)]
        avg = [sum(r) / size for r in trans]
        var = [sum([(k - avg[i]) ** 2 for k in r]) for r in trans] 

        for i, v in enumerate(var):
            if v > maxVar:
                maxVar = v
                splitDim = i

        order = sorted(trans[splitDim])
        keyMap = {v: i for i, v in enumerate(trans[splitDim])}
        return KdNode(coordinateVec, keyMap[order[midPos]], splitDim)
        


    def search(self, coordinate):
        nodeStack = []
        leftVisitStack = []
        node = self.__root
        nearest = None
        trace = False
        flag = True

        while flag:
            if not node:
                node = nodeStack.pop()
                leftVisitStack.pop()
                trace = True
            else:
                splitDim = node.getSplitDim()
                if splitDim != -1:
                    nodeStack.append(node)
                    
                    if coordinate[splitDim] <= node.coordinate[splitDim]:
                        node = node.left
                        leftVisitStack.append(True)
                    else:
                        node = node.right
                        leftVisitStack.append(False)
                else:
                    trace = True

            if trace:
                tDis = node.distance(coordinate)
                if not nearest or tDis < distance:
                    distance = tDis
                    nearest = node

            while trace:
                if not nodeStack:
                    flag = False
                    break

                node = nodeStack.pop()
                leftVisit = leftVisitStack.pop()

                tDis = node.distance(coordinate)
                if distance > tDis:
                    distance = tDis
                    nearest = node

                splitDim = node.getSplitDim()
                if distance ** 2 > (node.coordinate[splitDim] - coordinate[splitDim]) ** 2:
                    if leftVisit:
                        node = node.right
                    else:
                        node = node.left
                    trace = False

        return nearest.coordinate


if __name__ == '__main__':
    coordinateVec = []
    with open('points.txt') as f:
        coordinateVec = [[float(k) for k in l.strip().split(',')] for l in f if l]

    point = [2, 4.5]
    
    kdT = KdTree(coordinateVec)
    nearest = kdT.search(point)
    
    print(','.join([str(k) for k in nearest]))
    print('distance: {0}'.format(Util.distance(nearest, point)))
