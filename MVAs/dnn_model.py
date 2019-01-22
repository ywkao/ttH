import keras

def standard(max_objects, n_features):
  input_objects = keras.layers.Input(shape=(max_objects, n_features), name = 'input')
  
  # 1x1 convolutional layers
  dropout_rate = 0.10
  #conv = keras.layers.Convolution1D(64, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', name = 'conv_1')(input_objects)
  #conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_1')(conv)
  #conv = keras.layers.Convolution1D(32, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', name = 'conv_2')(conv)
  #conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_2')(conv)
  #conv = keras.layers.Convolution1D(16, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', name = 'conv_3')(conv)
  #conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_3')(conv)
  #conv = keras.layers.Convolution1D(6, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', name = 'conv_4')(conv)
  #conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_4')(conv)

  # lstm
  conv = input_objects
  lstm = keras.layers.LSTM(150, implementation=2, name='lstm_1', go_backwards=False)(conv)
  lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_1')(lstm)
  #lstm = keras.layers.LSTM(150, implementation=2, name='lstm_2', go_backwards=False)(lstm)
  #lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_2')(lstm)

  # fully-connected layers
  dropout_rate = 0.10
  dense = keras.layers.Dense(200, activation = 'relu', kernel_initializer = 'lecun_uniform', name = 'dense_1')(lstm)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_1')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', name = 'dense_2')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_2')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', name = 'dense_3')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_3')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', name = 'dense_4')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_4')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', name = 'dense_5')(dense)

  # output
  output = keras.layers.Dense(1, activation = 'sigmoid', kernel_initializer = 'lecun_uniform', name = 'output')(dense)
  optimizer = keras.optimizers.Adam()

  model = keras.models.Model(inputs = [input_objects], outputs = [output])
  model.compile(optimizer = optimizer, loss = 'binary_crossentropy')
  print(model.summary())

  return model

def separate_photons(max_objects, n_features, n_global_features, use_global):
  input_objects = keras.layers.Input(shape=(max_objects, n_features), name = 'input_objects')
  input_global = keras.layers.Input(shape=(n_global_features,), name = 'input_global')

  dropout_rate = 0.2

  # lstm
  conv = input_objects
  lstm = keras.layers.LSTM(150, implementation=2, name='lstm_1', go_backwards=False, return_sequences=True)(conv)
  lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_1')(lstm)
  lstm = keras.layers.LSTM(150, implementation=2, name='lstm_2', go_backwards=False)(lstm)
  lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_2')(lstm)

  if use_global:
    merged_features = keras.layers.concatenate([input_global, lstm])
  else:
    merged_features = lstm 

  # fully-connected layers
  dropout_rate = 0.2
  dense = keras.layers.Dense(200, activation = 'relu', kernel_initializer = 'lecun_uniform', name = 'dense_1')(merged_features)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_1')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', name = 'dense_2')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_2')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', name = 'dense_3')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_3')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', name = 'dense_4')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_4')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', name = 'dense_5')(dense)

  # output
  output = keras.layers.Dense(1, activation = 'sigmoid', kernel_initializer = 'lecun_uniform', name = 'output')(dense)
  optimizer = keras.optimizers.Adam()

  model = keras.models.Model(inputs = [input_global, input_objects], outputs = [output])
  model.compile(optimizer = optimizer, loss = 'binary_crossentropy')
  print(model.summary())

  return model

    