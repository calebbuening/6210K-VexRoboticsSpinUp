# cd to src before running with "python3 ./python/ann.py"
#import the required libraries
import pandas as pd
import numpy as np
from keras.models import Sequential
from keras.layers import BatchNormalization, LSTM, Dense
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.callbacks import ModelCheckpoint
from keras2cpp import export_model
import smogn

# edit this boolean if producing and not just testing
SAVE = True

# create the model as a function
def create_model(use_smogn=False):
	# Customize adam learning rate. Changing will require retrain.
	# Read csv into pandas dataframe
	dataframe = pd.read_csv("../data.csv")
	if use_smogn: dataframe = smogn.smoter(data=dataframe, y="Speed")
	# split into input (X) and output (Y) variables (values will be floats as they are floats in the c++ program)
	dataset = dataframe.values
	print(dataset)
	X = dataset[:,1:6].astype(float)
	print(X)
	Y = dataset[:,0].astype(float)
	X = np.expand_dims(X, 1)
	Y = np.expand_dims(Y, 1)
	# create model
	model = Sequential()
	# Add normalization layer
	# This is not an algorithm because I didn't want to write two identical normalization algorithms for python and c++
	model.add(BatchNormalization(input_shape=(1, 5)))
	# "input" layer
	model.add(LSTM(5, return_sequences=True))
	# Can customize but will require a retrain
	model.add(LSTM(12, return_sequences=True))
	model.add(LSTM(24, return_sequences=True))
	model.add(LSTM(20, return_sequences=True))
	model.add(LSTM(16, return_sequences=True))
	model.add(LSTM(8, return_sequences=True))
	model.add(LSTM(4))
	# single neuron output for action value
	# relu activation as first test
	model.add(Dense(1, kernel_initializer='normal', activation='softmax'))
	# compile showing accuracy and using mse and adam as they performed the best
	model.compile(loss='categorical_crossentropy', optimizer=Adam(learning_rate=.005), metrics=['accuracy'])
	# checkpoint
	filepath="rnn-weights-best.hdf5"
	checkpoint = ModelCheckpoint(filepath, monitor='accuracy', verbose=1, save_best_only=True, mode='max')
	callbacks_list = [checkpoint]
	# can modify batch size and epochs but will require a retrain
	model.summary()
	model.fit(X, Y, batch_size=10, epochs=100, verbose=1, callbacks=callbacks_list)
	return model

def load_model(use_smogn=False):
	model = Sequential()
	dataframe = pd.read_csv("data.csv")
	if use_smogn: dataframe = smogn.smoter(data=dataframe, y="Speed")
	# split into input (X) and output (Y) variables (values will be floats as they are floats in the c++ program)
	dataset = dataframe.values
	X = dataset[:,1:6].astype(float)
	Y = dataset[:,0].astype(float)
	model.add(BatchNormalization(input_shape=(5,)))
	# "input" layer
	model.add(LSTM(5, activation='relu', return_sequences=True))
	# Can customize but will require a retrain
	model.add(LSTM(9, activation='relu', return_sequences=True))
	model.add(LSTM(12, activation='relu', return_sequences=True))
	model.add(LSTM(8, activation='relu', return_sequences=True))
	model.add(LSTM(6, activation='relu', return_sequences=True))
	model.add(LSTM(3, activation='relu'))
	# single neuron output for action value
	# relu activation as first test
	model.add(Dense(1, kernel_initializer='normal', activation='tanh'))
	model = model.load_weights("rnn-weights-best.hdf5")
	model.compile(loss='categorical_crossentropy', optimizer=Adam(learning_rate=.001), metrics=['accuracy'])
	# checkpoint
	filepath="weights-best.hdf5"
	checkpoint = ModelCheckpoint(filepath, monitor='accuracy', verbose=1, save_best_only=True, mode='max')
	callbacks_list = [checkpoint]
	# can modify batch size and epochs but will require a retrain
	model.summary()
	model.fit(X, Y, batch_size=10, epochs=5000, verbose=1, callbacks=callbacks_list)
	return model

# save model with keras2cpp so we can use it in c++
new = input("Train new model? [Y/n] ")
if new == "Y" or "y":
	model = create_model()
	if SAVE:
		export_model(model, "rnn_blocker.model")
elif new == "N" or "n":
	model = load_model()
	if SAVE:
		export_model(model, "rnn_blocker.model")
