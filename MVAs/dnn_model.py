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


def baseline_leptonic_v1(max_objects, n_features, max_leptons, n_lepton_features, n_global_features, no_global, no_lstm, n_lstm):
  input_objects = keras.layers.Input(shape=(max_objects, n_features), name = 'input_objects')
  input_leptons = keras.layers.Input(shape=(max_leptons, n_lepton_features), name = 'input_leptons')
  input_global = keras.layers.Input(shape=(n_global_features,), name = 'input_global')

  # lstm
  maxnorm = 3
  batch_momentum = 0.6
  recurrent_dropout = 0.0
  nonrecurrent_dropout = 0.0
  lstm = keras.layers.LSTM(n_lstm, dropout = nonrecurrent_dropout, recurrent_dropout = recurrent_dropout, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_1', go_backwards=False, return_sequences=True)(input_objects)
  #lstm = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_batchnorm_1")(lstm)
  #lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_1')(lstm)
  lstm = keras.layers.LSTM(n_lstm, dropout = nonrecurrent_dropout, recurrent_dropout = recurrent_dropout, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_2', go_backwards=False, return_sequences=True)(lstm)
  #lstm = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_batchnorm_2")(lstm)
  #lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_2')(lstm)
  #lstm = keras.layers.LSTM(n_lstm, dropout = 0.0, recurrent_dropout = 0.0, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_3', go_backwards=False, return_sequences=True)(lstm)
  #lstm = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_batchnorm_3")(lstm)
  #lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_3')(lstm)
  lstm = keras.layers.LSTM(n_lstm, dropout = nonrecurrent_dropout, recurrent_dropout = recurrent_dropout, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_4', go_backwards=False)(lstm)
  #lstm = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_batchnorm_4")(lstm)
  #lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_4')(lstm)

  # lepton lstm
  lstm_lepton = keras.layers.LSTM(n_lstm, dropout = nonrecurrent_dropout, recurrent_dropout = recurrent_dropout, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_lepton_1', go_backwards=False, return_sequences=True)(input_leptons)
  #lstm_lepton = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_lepton_batchnorm_1")(lstm)
  #lstm_lepton = keras.layers.Dropout(dropout_rate, name = 'lstm_lepton_dropout_1')(lstm)
  lstm_lepton = keras.layers.LSTM(n_lstm, dropout = nonrecurrent_dropout, recurrent_dropout = recurrent_dropout, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_lepton_2', go_backwards=False, return_sequences=True)(lstm_lepton)
  #lstm_lepton = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_lepton_batchnorm_2")(lstm)
  #lstm_lepton = keras.layers.Dropout(dropout_rate, name = 'lstm_lepton_dropout_2')(lstm)
  #lstm_lepton = keras.layers.LSTM(n_lstm, dropout = 0.0, recurrent_dropout = 0.0, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_lepton_3', go_backwards=False, return_sequences=True)(lstm)
  #lstm_lepton = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_lepton_batchnorm_3")(lstm)
  #lstm_lepton = keras.layers.Dropout(dropout_rate, name = 'lstm_lepton_dropout_3')(lstm)
  lstm_lepton = keras.layers.LSTM(n_lstm, dropout = nonrecurrent_dropout, recurrent_dropout = recurrent_dropout, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_lepton_4', go_backwards=False)(lstm_lepton)



  if no_global:
    print "Using LSTM only"
    merged_features = lstm
  elif no_lstm:
    print "Using global only"
    merged_features = input_global
  else:
    print "Using LSTM + global"
    merged_features = keras.layers.concatenate([input_global, lstm, lstm_lepton])

  # fully-connected layers
  maxnorm = 3
  dropout_rate = 0.1
  dense = keras.layers.Dense(300, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_1')(merged_features)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_1')(dense)
  dense = keras.layers.Dense(200, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_2')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_2')(dense)
  dense = keras.layers.Dense(200, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_3')(dense)
  #dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_3')(dense)
  #dense = keras.layers.Dense(32, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_4')(dense)
  #dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_4')(dense)
  #dense = keras.layers.Dense(32, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_5')(dense)
  #dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_5')(dense)

  # output
  output = keras.layers.Dense(1, activation = 'sigmoid', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'output')(dense)
  optimizer = keras.optimizers.Adam()

  model = keras.models.Model(inputs = [input_global, input_objects, input_leptons], outputs = [output])
  model.compile(optimizer = optimizer, loss = 'binary_crossentropy')
  print(model.summary())

  return model


def baseline_v1(max_objects, n_features, n_global_features, no_global, no_lstm, n_lstm):
  input_objects = keras.layers.Input(shape=(max_objects, n_features), name = 'input_objects')
  input_global = keras.layers.Input(shape=(n_global_features,), name = 'input_global')

  maxnorm = 3
  # 1x1 convolutional layers
  dropout_rate = 0.10
  do_conv = False
  if do_conv:
    conv = keras.layers.Convolution1D(64, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_1')(input_objects)
    conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_1')(conv)
    conv = keras.layers.Convolution1D(64, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_2')(conv)
    conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_2')(conv)
    conv = keras.layers.Convolution1D(32, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_3')(conv)
    #conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_3')(conv)
    conv = keras.layers.Convolution1D(5, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_4')(conv)
    #conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_4')(conv)

  else:
    conv = input_objects

  # lstm
  batch_momentum = 0.6
  recurrent_dropout = 0.0
  nonrecurrent_dropout = 0.0
  lstm = keras.layers.LSTM(n_lstm, dropout = nonrecurrent_dropout, recurrent_dropout = recurrent_dropout, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_1', go_backwards=False, return_sequences=True)(conv)
  #lstm = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_batchnorm_1")(lstm)
  #lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_1')(lstm)
  lstm = keras.layers.LSTM(n_lstm, dropout = nonrecurrent_dropout, recurrent_dropout = recurrent_dropout, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_2', go_backwards=False, return_sequences=True)(lstm)
  #lstm = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_batchnorm_2")(lstm)
  #lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_2')(lstm)
  #lstm = keras.layers.LSTM(n_lstm, dropout = 0.0, recurrent_dropout = 0.0, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_3', go_backwards=False, return_sequences=True)(lstm)
  #lstm = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_batchnorm_3")(lstm)
  #lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_3')(lstm)
  lstm = keras.layers.LSTM(n_lstm, dropout = nonrecurrent_dropout, recurrent_dropout = recurrent_dropout, implementation=2, activation = 'tanh',  kernel_constraint = keras.constraints.maxnorm(maxnorm), recurrent_constraint = keras.constraints.maxnorm(maxnorm), name='lstm_4', go_backwards=False)(lstm)
  #lstm = keras.layers.normalization.BatchNormalization(momentum = batch_momentum, name = "lstm_batchnorm_4")(lstm)
  #lstm = keras.layers.Dropout(dropout_rate, name = 'lstm_dropout_4')(lstm)

  if no_global:
    print "Using LSTM only"
    merged_features = lstm
  elif no_lstm:
    print "Using global only"
    merged_features = input_global
  else:
    print "Using LSTM + global"
    merged_features = keras.layers.concatenate([input_global, lstm])

  # fully-connected layers
  maxnorm = 3
  dropout_rate = 0.1
  dense = keras.layers.Dense(300, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_1')(merged_features)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_1')(dense)
  dense = keras.layers.Dense(200, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_2')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_2')(dense)
  dense = keras.layers.Dense(200, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_3')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_3')(dense)
  dense = keras.layers.Dense(200, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_4')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_4')(dense)
  dense = keras.layers.Dense(200, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_5')(dense)
  #dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_5')(dense)

  # output
  output = keras.layers.Dense(1, activation = 'sigmoid', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'output')(dense)
  optimizer = keras.optimizers.Adam()

  model = keras.models.Model(inputs = [input_global, input_objects], outputs = [output])
  model.compile(optimizer = optimizer, loss = 'binary_crossentropy')
  print(model.summary())

  return model

def cnn_v1(max_objects, n_features, n_global_features, no_global, no_lstm):
  input_objects = keras.layers.Input(shape=(max_objects, n_features), name = 'input_objects')
  input_global = keras.layers.Input(shape=(n_global_features,), name = 'input_global')

  maxnorm = 2
  # 1x1 convolutional layers
  dropout_rate = 0.15
  conv = keras.layers.Convolution1D(128, 2, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_1')(input_objects)
  conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_1')(conv)
  conv = keras.layers.Convolution1D(128, 2, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_2')(conv)
  conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_2')(conv)
  conv = keras.layers.Convolution1D(128, 2, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_3')(conv)
  conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_3')(conv)
  conv = keras.layers.Convolution1D(128, 2, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_4')(conv)
  conv = keras.layers.Flatten()(conv)

  if no_global:
    print "Using LSTM only"
    merged_features = conv
  elif no_lstm:
    print "Using global only"
    merged_features = input_global
  else:
    print "Using LSTM + global"
    merged_features = keras.layers.concatenate([input_global, conv])
  # fully-connected layers
  dropout_rate = 0.1
  dense = keras.layers.Dense(200, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_1')(merged_features)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_1')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_2')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_2')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_3')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_3')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_4')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_4')(dense)
  dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_5')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_5')(dense)

  # output
  output = keras.layers.Dense(1, activation = 'sigmoid', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'output')(dense)
  optimizer = keras.optimizers.Adam()

  model = keras.models.Model(inputs = [input_global, input_objects], outputs = [output])
  model.compile(optimizer = optimizer, loss = 'binary_crossentropy')
  print(model.summary())

  return model


def gru_v1(max_objects, n_features, n_global_features, no_global, no_lstm, n_lstm):
  input_objects = keras.layers.Input(shape=(max_objects, n_features), name = 'input_objects')
  input_global = keras.layers.Input(shape=(n_global_features,), name = 'input_global')

  maxnorm = 2
  # 1x1 convolutional layers
  dropout_rate = 0.15
  #conv = keras.layers.Convolution1D(64, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_1')(input_objects)
  #conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_1')(conv)
  #conv = keras.layers.Convolution1D(64, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_2')(conv)
  #conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_2')(conv)
  #conv = keras.layers.Convolution1D(32, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_3')(conv)
  #conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_3')(conv)
  #conv = keras.layers.Convolution1D(6, 1, kernel_initializer = 'lecun_uniform', activation = 'relu', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'conv_4')(conv)
  #conv = keras.layers.Dropout(dropout_rate, name = 'conv_dropout_4')(conv)

  conv = input_objects

  # lstm
  batch_momentum = 0.6
  dropout_rate = 0.1

  gru = keras.layers.GRU(n_lstm, implementation=1, name='gru_1', return_sequences=True)(conv)
  gru = keras.layers.Dropout(dropout_rate, name = 'gru_dropout_1')(gru)
  gru = keras.layers.GRU(n_lstm, implementation=1, name='gru_2', return_sequences=False)(gru)
  gru = keras.layers.Dropout(dropout_rate, name = 'gru_dropout_2')(gru)

  if no_global:
    print "Using LSTM only"
    merged_features = gru
  elif no_lstm:
    print "Using global only"
    merged_features = input_global
  else:
    print "Using LSTM + global"
    merged_features = keras.layers.concatenate([input_global, gru])

  # fully-connected layers
  dropout_rate = 0.2
  dense = keras.layers.Dense(300, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_1')(merged_features)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_1')(dense)
  dense = keras.layers.Dense(300, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_2')(dense)
  dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_2')(dense)
  #dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_3')(dense)
  #dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_3')(dense)
  #dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_4')(dense)
  #dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_4')(dense)
  #dense = keras.layers.Dense(100, activation = 'relu', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'dense_5')(dense)
  #dense = keras.layers.Dropout(dropout_rate, name = 'dense_dropout_5')(dense)

  # output
  output = keras.layers.Dense(1, activation = 'sigmoid', kernel_initializer = 'lecun_uniform', kernel_constraint = keras.constraints.maxnorm(maxnorm), name = 'output')(dense)
  optimizer = keras.optimizers.Adam()

  model = keras.models.Model(inputs = [input_global, input_objects], outputs = [output])
  model.compile(optimizer = optimizer, loss = 'binary_crossentropy')
  print(model.summary())

  return model

