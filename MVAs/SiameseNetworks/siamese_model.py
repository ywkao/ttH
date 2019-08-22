import tensorflow as tf
import tensorflow.keras as keras
import keras_layer_normalization

import siamese_utils

def siamese(max_objects, n_features, n_global_features, config, predict=False):
    input_objects_1 = keras.layers.Input(shape=(max_objects, n_features), name = 'input_objects_1')
    input_global_1 = keras.layers.Input(shape=(n_global_features,), name = 'input_global_1')

    input_objects_2 = keras.layers.Input(shape=(max_objects, n_features), name = 'input_objects_2')
    input_global_2 = keras.layers.Input(shape=(n_global_features,), name = 'input_global_2')

    shared_network = base_network(max_objects, n_features, n_global_features, config)

    dnn_left =  shared_network([input_global_1, input_objects_1])
    dnn_right = shared_network([input_global_2, input_objects_2])

    distance = keras.layers.Lambda(siamese_utils.euclidean_distance, output_shape=siamese_utils.eucl_dist_output_shape)([dnn_left, dnn_right])

    if predict:
        output = dnn_left # or right
    else:
        output = distance

    model = keras.models.Model(inputs = ([input_global_1, input_objects_1, input_global_2, input_objects_2]), outputs = output)

    learning_rate = config["learning_rate"]
    optimizer = tf.train.AdamOptimizer(learning_rate = learning_rate)
    model.compile(optimizer = optimizer, loss = siamese_utils.contrastive_loss)
    print((model.summary()))

    return model, shared_network


def base_network(max_objects, n_features, n_global_features, config):
    # Hyperparameters
    maxnorm = config["maxnorm"]
    dropout_rate = config["dropout_rate"]

    n_nodes_lstm = config["n_nodes_lstm"]
    n_lstm = config["n_lstm"]

    n_nodes_dense_1 = config["n_nodes_dense_1"]
    n_dense_1 = config["n_dense_1"]
    n_nodes_dense_2 = config["n_nodes_dense_2"]
    n_dense_2 = config["n_dense_2"]

    learning_rate = config["learning_rate"]

    n_manifold = config["n_manifold"]

    if "batch_norm" in list(config.keys()):
        batch_norm = config["batch_norm"]
        batch_momentum = config["batch_momentum"]
    else:
        batch_norm = False

    if "layer_norm" in list(config.keys()):
        layer_norm = config["layer_norm"]
    else:
        layer_norm = False
 

    input_objects = keras.layers.Input(shape=(max_objects, n_features), name = 'input_objects')
    input_global = keras.layers.Input(shape=(n_global_features,), name = 'input_global')

    # LSTM
    lstm = input_objects

    for i in range(n_lstm):
        lstm =  keras.layers.LSTM(n_nodes_lstm, implementation=2, activation='tanh', kernel_constraint = keras.constraints.max_norm(maxnorm), go_backwards=False, return_sequences=(i != (n_lstm-1)), name='lstm_%d' % i)(lstm)
        if layer_norm:
            lstm = keras_layer_normalization.LayerNormalization()(lstm)

    # Merge LSTM output with high-level features in fully-connected layers
    dense = keras.layers.concatenate([input_global, lstm])
    for i in range(n_dense_1):
        dense = keras.layers.Dense(n_nodes_dense_1, activation = 'elu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.max_norm(maxnorm), name = 'dense1_%d' % i)(dense)
        if batch_norm:
            dense = keras.layers.BatchNormalization(momentum = batch_momentum, name = 'dense_batch_norm1_%d' % i)(dense)
        dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout1_%d' % i)(dense)
        

    for i in range(n_dense_2):
        dense = keras.layers.Dense(n_nodes_dense_2, activation = 'elu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.max_norm(maxnorm), name = 'dense2_%d' % i)(dense)
        if batch_norm:
            dense = keras.layers.BatchNormalization(momentum = batch_momentum, name = 'dense_batch_norm2_%d' % i)(dense)
        if i < (n_dense_2 - 1):
            dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout2_%d' % i)(dense)

    dense = keras.layers.Dense(n_manifold, activation = 'elu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.max_norm(maxnorm), name = 'manifold')(dense)

    model = keras.models.Model(inputs = [input_global, input_objects], outputs = [dense])
    print((model.summary()))
    return model
