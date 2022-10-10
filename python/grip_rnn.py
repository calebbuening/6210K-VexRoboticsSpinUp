# cd to src before running with "python3 ./python/grip_rnn.py"
from pandas import read_csv
from keras.models import Sequential
from keras.layers import Dense, BatchNormalization, LSTM
from tensorflow.keras.optimizers import Adam
import numpy as np
from keras2cpp import export_model

def create_model():
	# create adam optimizer with custom learning rate
	adam = Adam(learning_rate=0.01)
	# read csv into pandas dataframe
	dataframe = read_csv("nn_data/COPilot.csv", header=None)
	# get values as floats because that's what the c++ program does
	dataset = dataframe.values.astype(float)
	# split into input (X) and output (Y) variables
	X = dataset[:,1:8]
	Y = dataset[:,0]
	X = np.expand_dims(X, 1)
	Y = np.expand_dims(Y, 1)
	# create model
	model = Sequential()
	# add normalization layer so I don't have to write algorithims in 2 languages
	model.add(BatchNormalization(input_shape=(1, 5)))
	# "input" layer
	# can customize neurons but changing will require a retrain
	model.add(LSTM(32, return_sequences=True))
	model.add(LSTM(56, return_sequences=True))
	model.add(LSTM(28, return_sequences=True))
	model.add(LSTM(9, return_sequences=True))
	model.add(LSTM(5))
	# activate with sigmoid to restrict values
	model.add(Dense(1, kernel_initializer='normal', activation='sigmoid'))
	# compile with mse and adam showing accuracy
	model.compile(loss='mean_squared_error', optimizer=adam, metrics=['accuracy'])
	# can modify these values but changing will require a retrain
	model.fit(X, Y, batch_size=10, epochs=10, verbose=1)
	model.summary()
	return model

# save model
model = create_model()
export_model(model, "keras_rnn.model")
no_fire_test = np.expand_dims(np.array([]), 0) # Add values for prediction here and below
fire_test = np.expand_dims(np.array([]), 0)
#Following lines should return 1 from the NN
fire = np.array([fire_test])
no_fire = np.array([no_fire_test])
prediction_fire = model.predict(fire)
prediction_no_fire = model.predict(no_fire)
print(prediction_fire)
print(prediction_no_fire)