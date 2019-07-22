import tensorflow as tf
import tensorflow.keras as keras
import numpy
import random
import time

def euclidean_distance(vects):
    x, y = vects
    sum_square = keras.backend.sum(keras.backend.square(x - y), axis=1, keepdims=True)
    return keras.backend.sqrt(keras.backend.maximum(sum_square, keras.backend.epsilon()))


def eucl_dist_output_shape(shapes):
    shape1, shape2 = shapes
    return (shape1[0], 1)


def contrastive_loss(y_true, y_pred):
    '''Contrastive loss from Hadsell-et-al.'06
    http://yann.lecun.com/exdb/publis/pdf/hadsell-chopra-lecun-06.pdf
    '''
    margin_s = 1
    margin_d = 1
    similar_term = y_true * keras.backend.square(keras.backend.maximum(y_pred - margin_s, 0))
    dissimilar_term = (1 - y_true) * keras.backend.square(keras.backend.maximum(margin_d - y_pred, 0))
    return keras.backend.mean(similar_term + dissimilar_term)


def create_pairs(y, n):
    y = numpy.array(y)
    pairs = []
    labels = []

    values = numpy.unique(y)
    index_dict = {}
    for value in values:
        index_dict[value] = numpy.where(y == value)[0]
        #print(index_dict[value])

    # Get similar pairs 
    # Need to generalize to N classes
    start_time = time.time()
    for value in values:
        for i in range(int(n/4)):
            pair = numpy.random.randint(0, len(index_dict[value]), 2)
            #print(value, [index_dict[value][pair[0]], index_dict[value][pair[1]]])
            pairs.append([index_dict[value][pair[0]], index_dict[value][pair[1]]])
            labels.append(1)
    elapsed_time = time.time() - start_time
    print("Time to make %d similar pairs %.2f" % (int(n/2), elapsed_time))

    # Get dissimilar pairs
    # Need to generalize to N classes with option to sample unevenly (i.e. according to cross section)
    start_time = time.time()
    for i in range(int(n/2)):
        idx1 = numpy.random.randint(0, len(index_dict[values[0]]))
        idx2 = numpy.random.randint(0, len(index_dict[values[1]]))
        pairs.append([index_dict[values[0]][idx1], index_dict[values[1]][idx2]])
        labels.append(0)
    elapsed_time = time.time() - start_time
    print("Time to make %d dissimilar pairs %.2f" % (int(n/2), elapsed_time))

    """
    while len(labels) < n:
        pair = numpy.random.randint(0, len(y), 2)
        label = 1 if y[pair[0]] == y[pair[1]] else 0 # 1 = similar, 0 = dissimilar
        #if label == 0:
        #    if random.random() < 0.75:
        #        continue
        pairs.append(pair)
        labels.append(label)
    """

    return numpy.asarray(pairs), numpy.asarray(labels)

#def auc(scores, y, n):
#    triplets = []

#    while 
