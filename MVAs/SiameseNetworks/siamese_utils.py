import tensorflow as tf
import keras
import numpy
import random

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
    margin = 1
    similar_term = (1 - y_true) * keras.backend.square(y_pred)
    dissimilar_term = y_true * keras.backend.square(keras.backend.maximum(margin - y_pred, 0))
    return keras.backend.mean(similar_term + dissimilar_term)


def create_pairs(y, n):
    pairs = []
    labels = []

    while len(labels) < n:
        pair = numpy.random.randint(0, len(y), 2)
        label = 0 if y[pair[0]] == y[pair[1]] else 1 # 0 = similar, 1 = dissimilar
        #if label == 0:
        #    if random.random() < 0.75:
        #        continue
        pairs.append(pair)
        labels.append(label)

    return numpy.asarray(pairs), numpy.asarray(labels)

#def auc(scores, y, n):
#    triplets = []

#    while 
