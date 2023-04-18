# cd to src before running with "python3 ./python/ann.py"
#import the required libraries
import pandas as pd
from keras.models import Sequential
from keras.layers import BatchNormalization, Dense
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.callbacks import ModelCheckpoint
from keras2cpp import export_model
import smogn

# edit this boolean if producing and not just testing
SAVE = False

# create the model as a function
def create_model(use_smogn=False):
	# Customize adam learning rate. Changing will require retrain.
	# Read csv into pandas dataframe
	dataframe = pd.read_csv("C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\data\\data.csv")
	if use_smogn: dataframe = smogn.smoter(data=dataframe, y="Speed")
	# split into input (X) and output (Y) variables (values will be floats as they are floats in the c++ program)
	dataset = dataframe.values
	print(dataset)
	X = dataset[:,1:6].astype(float)
	print(X)
	Y = dataset[:,0].astype(float)
	# create model
	model = Sequential()
	# Add normalization layer
	# This is not an algorithm because I didn't want to write two identical normalization algorithms for python and c++
	model.add(BatchNormalization(input_shape=(5,)))
	# "input" layer
	model.add(Dense(5, activation='relu'))
	# Can customize but will require a retrain
	model.add(Dense(9, activation='relu'))
	model.add(Dense(12, activation='relu'))
	model.add(Dense(8, activation='relu'))
	model.add(Dense(6, activation='relu'))
	model.add(Dense(3, activation='relu'))
	# single neuron output for action value
	# relu activation as first test
	model.add(Dense(1, kernel_initializer='normal', activation='linear'))
	# compile showing accuracy and using mse and adam as they performed the best
	model.compile(loss='mean_squared_error', optimizer=Adam(learning_rate=.001), metrics=['accuracy'])
	# checkpoint
	filepath="C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\src\\weights-best.hdf5"
	checkpoint = ModelCheckpoint(filepath, monitor='accuracy', verbose=1, save_best_only=True, mode='max')
	callbacks_list = [checkpoint]
	# can modify batch size and epochs but will require a retrain
	model.summary()
	model.fit(X, Y, batch_size=50, epochs=5000, verbose=1, callbacks=callbacks_list)
	return model

def load_model(use_smogn=False):
	model = Sequential()
	dataframe = pd.read_csv("C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\data\\data.csv")
	if use_smogn: dataframe = smogn.smoter(data=dataframe, y="Speed")
	# split into input (X) and output (Y) variables (values will be floats as they are floats in the c++ program)
	dataset = dataframe.values
	X = dataset[:,1:6].astype(float)
	Y = dataset[:,0].astype(float)
	model.add(BatchNormalization(input_shape=(5,)))
	# "input" layer
	model.add(Dense(5, activation='relu'))
	# Can customize but will require a retrain
	model.add(Dense(9, activation='relu'))
	model.add(Dense(12, activation='relu'))
	model.add(Dense(8, activation='relu'))
	model.add(Dense(6, activation='relu'))
	model.add(Dense(3, activation='relu'))
	# single neuron output for action value
	# relu activation as first test
	model.add(Dense(1, kernel_initializer='normal', activation='linear'))
	model = model.load_weights("C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\src\\weights-best.hdf5")
	model.compile(loss='mean_squared_error', optimizer=Adam(learning_rate=.001), metrics=['accuracy'])
	# checkpoint
	filepath="C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\src\\weights-best.hdf5"
	checkpoint = ModelCheckpoint(filepath, monitor='accuracy', verbose=1, save_best_only=True, mode='max')
	callbacks_list = [checkpoint]
	# can modify batch size and epochs but will require a retrain
	model.summary()
	model.fit(X, Y, batch_size=50, epochs=5000, verbose=1, callbacks=callbacks_list)
	return model

# save model with keras2cpp so we can use it in c++
new = input("Train new model? [Y/n] ")
if new == "Y" or "y":
	model = create_model()
	if SAVE:
		export_model(model, "C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\src\\auton_blocker.model")
elif new == "N" or "n":
	model = load_model()
	if SAVE:
		export_model(model, "C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\src\\auton_blocker.model")
