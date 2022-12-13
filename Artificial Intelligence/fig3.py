# Brandon Luu (bluu8)
# Creates a random dataset (splits it into the truth and sample) and runs decision tree and knn/crossvalidation.
# This is for figure 3 (smiley face).

from math import *
from random import *
import decisiontree
import crossvalidation

# hyperparameters
show = False # show the data?
kfolds = 10 # num of kfolds
knn = 10 # knn value

# full dataset
dataset = []

def generate(colour, xstart=0, xend=0):
    if colour == "black":
        for i in range(0, 3):
            x = -2.5
            while x < 2.5:
                for a in range(0, 2):
                    dataset.append({"x": x*2.5 + random(), "y": pow(x, 2)/1.25 + random(), "colour": colour})
                x = x + 0.1
    else:
        for i in range(0, 2):
            x = xstart
            y = 6.5
            while y > 5:
                while x < xend:
                    # 1/3 to make point
                    if random() * 3 < 1:
                        dataset.append({"x": x + random()/5, "y": y + random()/5, "colour": colour})
                    x = x + 0.1
                
                x = xstart
                y = y - 0.1

print("Creating figure 3 dataset")
generate("black")
generate("green", -2.5, -1)
generate("red", 2, 3.5)

print("Running decision tree, k-nn, and cross validation")
decisiontree.run(dataset, kfolds, False)
crossvalidation.run(dataset, kfolds, knn, False)

    