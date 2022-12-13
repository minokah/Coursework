# Brandon Luu (bluu8)
# Uses sklearn's Decision Tree.
# Required files: truth.csv and sample.csv

import pandas
from sklearn.tree import DecisionTreeClassifier
import matplotlib.pyplot as plt
from random import *

def run(dataset, kfolds, show):
    # ground truth file
    truthFile = open("truth.csv", "w")
    truthFile.write("x,y,colour\n")

    # sample data file
    sampleFile = open("sample.csv", "w")
    sampleFile.write("x,y,colour\n")

    for i in dataset:
    # write to sample or truth for the folds
        if random() * kfolds < 1:
            sampleFile.write("{},{},{}\n".format(i["x"], i["y"], i["colour"]))
        else:
            truthFile.write("{},{},{}\n".format(i["x"], i["y"], i["colour"]))

    truthFile.close()
    sampleFile.close()
    
    truthdf = pandas.read_csv("truth.csv")
    sampledf = pandas.read_csv("sample.csv")

    colours = {
        "black": 0,
        "green": 1,
        "red": 2,
    }

    truthcolours = truthdf["colour"].copy()
    truthdf["colour"] = truthdf["colour"].map(colours)
    sampledf["colour"] = sampledf["colour"].map(colours)

    cats = ["x", "y"]
    truthx = truthdf[cats]
    truthy = truthdf["colour"]

    dtree = DecisionTreeClassifier()
    dtree = dtree.fit(truthx, truthy)

    samplex = sampledf[cats]
    sampley = sampledf["colour"]

    print("Decision tree mean accuracy:", dtree.score(samplex, sampley) * 100)

    if show:
        truthdf.plot.scatter(x="x", y="y", c=truthcolours)
        plt.show()