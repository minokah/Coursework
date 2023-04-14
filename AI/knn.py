# Brandon Luu (bluu8)
# Uses my own k-nn algorithm and sklearn's k-NN algorithm.

import pandas
from math import *
from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score
import matplotlib.pyplot as plt

colours = {
    "black": 0,
    "green": 1,
    "red": 2,
}

def run(k, show):
    truthdf = pandas.read_csv("truth.csv")
    sampledf = pandas.read_csv("sample.csv")

    truthcolours = truthdf["colour"].copy()
    #truthdf["colour"] = truthdf["colour"].map(colours)
    #sampledf["colour"] = sampledf["colour"].map(colours)

    cats = ["x", "y"]
    truthx = truthdf[cats]
    truthy = truthdf["colour"]

    samplex = sampledf[cats]
    sampley = sampledf["colour"]

    truthdata = []
    # allocate into truth data table
    for i in truthdf.values:
        entry = {"x": i[0], "y": i[1], "colour": i[2]}
        truthdata.append(entry)

    sampledata = []
    for i in sampledf.values:
        entry = {"x": i[0], "y": i[1], "colour": i[2]}
        sampledata.append(entry)

    # find the best k value for k-nn
    knnscore = 0
    for s in sampledata:
        truthcopy = truthdata.copy()
        for t in truthcopy:
            # do euclidean distance on a copy of the turth
            dist = sqrt(pow((s["x"] - t["x"]), 2) + pow((s["y"] - t["y"]), 2))
            t["distance"] = dist
        
        # sort it
        truthcopy.sort(key=lambda x: x["distance"])

        colours = {}
        largestcol = None
        largestval = 0

        # do k-nn for current i
        for a in range(0, k):
            if not truthcopy[a]["colour"] in colours:
                colours[truthcopy[a]["colour"]] = 0
            colours[truthcopy[a]["colour"]] = colours[truthcopy[a]["colour"]] + 1

            if colours[truthcopy[0]["colour"]] > largestval:
                largestval = colours[truthcopy[a]["colour"]]
                largestcol = truthcopy[a]["colour"]
                
        s["prediction"] = largestcol

    counter = 0
    for a in sampledata:
        if a["colour"] == a["prediction"]:
            counter = counter + 1

    knnscore = counter / len(sampledata)
    # print("Using my algorithm: accuracy for k = {} is {}%".format(k, float(knnscore) * 100))

    # comparing it to sklearn's algorithm
    # find best k from 1 to 10
    knn = KNeighborsClassifier(n_neighbors=k)
    knn.fit(truthx, truthy)
    prediction = knn.predict(samplex)
    score = accuracy_score(sampley, prediction)
    
    #print("k = {}, accuracy = {}".format(i, score))

    # print("Using sklearn: accuracy for k = {} is {}%".format(k, score))

    if show:
        truthdf.plot.scatter(x="x", y="y", c=truthcolours)
        plt.show()

    # return my algorithm's result
    return [float(knnscore), score]