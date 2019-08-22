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
    margin_s = 0.25
    margin_d = 2.0
    similar_term = y_true * keras.backend.square(keras.backend.maximum(y_pred - margin_s, 0))
    dissimilar_term = (1 - y_true) * keras.backend.square(keras.backend.maximum(margin_d - y_pred, 0))
    return keras.backend.mean(similar_term + dissimilar_term)

def scale_probabilities(probs):
    probs = numpy.array(probs)
    return probs * (1./sum(probs))


def create_pairs_with_probability(y, n, probs):
    pairs = []
    labels = []

    values = numpy.unique(y)
    index_dict = {}
    for value in values:
        index_dict[value] = numpy.where(y == value)[0]

    # Make sure probabilities sum to 1
    psum = sum([prob for prob in probs.values() if prob != -1])
    for value in values:
        if probs[value] != -1:
            probs[value] *= 1./psum
    print("Normalized probabilities: ", probs)

    # Get similar pairs
    start_time = time.time()
    for value in values:
        if probs[value] == -1: # signal
            n_pair = int(n/4)
        else:
            n_pair = int((n/4)*probs[value])
        print("Process id: %d, number of similar pairs: %d" % (value, n_pair))
        for i in range(n_pair):
            pair = numpy.random.randint(0, len(index_dict[value]), 2)
            pairs.append([index_dict[value][pair[0]], index_dict[value][pair[1]]])
            labels.append(1)
    elapsed_time = time.time() - start_time
    print("Time to make %d similar pairs %.2f" % (int(n/2), elapsed_time))

    # Get dissimilar pairs
    start_time = time.time()
    for value in values:
        if probs[value] == -1: # signal
            n_pair = int(n/4)
            trimmed_array = values[values != value] 
        else:
            n_pair = int((n/4)*probs[value])
            trimmed_array = values[values != value] 
            trimmed_array = trimmed_array[trimmed_array != 0] 
        new_probs = scale_probabilities([probs[val] for val in trimmed_array])
        print("Value %d, sampling other values with probability:" % value, new_probs)
        print("Process id: %d, number of dissimilar pairs: %d" % (value, n_pair))
        for i in range(n_pair):
            idx1 = numpy.random.randint(0, len(index_dict[value]))
            value2 = numpy.random.choice(trimmed_array, 1, p = new_probs)[0] 
            idx2 = numpy.random.randint(0, len(index_dict[value2]))
            pairs.append([index_dict[value][idx1], index_dict[value2][idx2]])
            labels.append(0)
    elapsed_time = time.time() - start_time
    print("Time to make %d dissimilar pairs %.2f" % (int(n/2), elapsed_time))
    return numpy.asarray(pairs), numpy.asarray(labels)

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

def create_pairs_distance_based(sh, probs):
    return create_pairs_with_probability(sh.features_train["process_id"], sh.n_pairs, probs)


def create_pairs_by_strategy(sh, probs):
    if strategy == "random":
        return create_pairs_with_probability(sh.features_train["process_id"], sh.n_pairs, probs)
    elif strategy == "distance_based":
        return create_pairs_distance_based(sh, probs)

#def auc(scores, y, n):
#    triplets = []

#    while 
