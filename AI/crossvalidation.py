# Brandon Luu (bluu8)
# Use cross validation using my own algorithm.

import knn
from random import *

# log fold accuracies
accuracies = []
skaccuracies = []

def run(dataset, kfolds=1, k=1, show=False):
    def createfoldfile():
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
    
    #create kfolds number of folds
    for i in range(0, kfolds + 1):
        createfoldfile()
        result = knn.run(k, show)
        accuracies.append(result[0])
        skaccuracies.append(result[1])

    # get the mean
    mean = 0
    for i in range(0, len(accuracies)):
        mean = mean + accuracies[i]
    mean = mean / len(accuracies)

    print("Accuracies (my algorithm) for {} k-folds and {}-NN: {}".format(kfolds, k, accuracies))
    print("Cross validation mean accuracy (my algorithm):", mean*100)

    mean = 0
    for i in range(0, len(skaccuracies)):
        mean = mean + skaccuracies[i]
    mean = mean / len(skaccuracies)

    print("Accuracies (sklearn algorithm) for {} k-folds and {}-NN: {}".format(kfolds, k, skaccuracies))
    print("Cross validation mean accuracy (sklearn algorithm):", mean*100)
    
    