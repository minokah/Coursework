# Brandon Luu (bluu8)
# Creates a random dataset (splits it into the truth and sample) and runs decision tree and knn/crossvalidation.
# This is for figure 1 (3 circles).

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

black_length = 4
green_length = 2
red_length = 0.1

def generate(colour, length):
    for i in range(0, 361):
        dataset.append({"x": length * cos((i*pi)/180) + random(), "y": length * sin((i*pi)/180) + random(), "colour": colour})

print("Creating figure 1 dataset")
generate("black", black_length)
generate("green", green_length)
generate("red", red_length)

print("Running decision tree, k-nn, and cross validation")
decisiontree.run(dataset, kfolds, False)
crossvalidation.run(dataset, kfolds, knn, False)