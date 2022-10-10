# cd to src before running with "python3 ./python/grip_ann.py"
from pandas import read_csv
from keras.models import Sequential
from keras.layers import Dense, BatchNormalization
from tensorflow.keras.optimizers import Adam
from keras2cpp import export_model

#Determine whether we ant the model to be saved onto the robot
SAVE = False

def create_model():
	# Customize adam learning rate. Changing will require retrain.
	# Read csv into pandas dataframe
	dataframe = read_csv("nn_data/COPilot.csv", header=None)
	# split into input (X) and output (Y) variables (values will be floats as they are floats in the c++ program)
	dataset = dataframe.values
	X = dataset[:,(0:11)].astype(float)
	Y = dataset[:,12].astype(float)
	# create model
	model = Sequential()
	# Add normalization layer
	# This is not an algorithm because I didn't want to write two identical normalization algorithms for python and c++
	model.add(BatchNormalization(input_shape=(12,)))
	# "input" layer
	model.add(Dense(12, activation='relu'))
	# Can customize but will require a retrain
	model.add(Dense(24, activation='relu'))
	model.add(Dense(48, activation='relu'))
	model.add(Dense(64, activation='relu'))
	model.add(Dense(128, activation='relu'))
	model.add(Dense(512, activation='relu'))
	model.add(Dense(256, activation='relu'))
	model.add(Dense(64, activation='relu'))
	model.add(Dense(32, activation='relu'))
	model.add(Dense(8, activation='relu'))
	model.add(Dense(6, activation='relu'))
	model.add(Dense(3, activation='relu'))
	# single neuron output for action value
	# relu activation as first test
	model.add(Dense(1, kernel_initializer='normal', activation='relu'))
	# compile showing accuracy and using mse and adam as they performed the best
	model.compile(loss='mean_squared_error', optimizer=Adam(learning_rate=.001), metrics=['accuracy'])
	# can modify batch size and epochs but will require a retrain
	model.summary()
	model.fit(X, Y, batch_size=50, epochs=5000, verbose=1)
	# print summary to terminal
	model.summary()
	return model


# save model with keras2cpp so we can use it in c++
model = create_model()
if SAVE:
	export_model(model, "keras_ann.model")