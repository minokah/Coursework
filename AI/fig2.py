# Brandon Luu (bluu8)
# Creates a random dataset (splits it into the truth and sample) and runs decision tree and knn/crossvalidation.
# This is for figure 2 (black dots everywhere with 2 blobs of green and red).

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

def generate(colour, limit, clustered=False):
    base = random() * limit

    # limit how much green and red there are
    amount = 100
    if clustered:
        amount = 30

    for i in range(0, amount):
        # black for dispersed
        if not clustered:
            dataset.append({"x": random() * limit + random(), "y": random() * limit + random(), "colour": colour})
        else:
            dataset.append({"x": base + random()/10, "y": base + random()/10, "colour": colour})

print("Creating figure 2 dataset")
generate("black", random() * 5)
generate("green", random() * 4, True)
generate("red", random() * 4, True)

print("Running decision tree, k-nn, and cross validation")
decisiontree.run(dataset, kfolds, False)
crossvalidation.run(dataset, kfolds, knn, False)