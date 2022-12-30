import numpy as np
from keras.models import Sequential
from keras.layers import LSTM, Dense, BatchNormalization
from keras import backend as K
import pandas
from keras.wrappers.scikit_learn import KerasClassifier
from sklearn.model_selection import cross_val_score, train_test_split
from keras.callbacks import ModelCheckpoint

def stretched_tanh(x):
    return (K.tanh(x) * 1.8288)

model = Sequential()

model.add(BatchNormalization(input_shape=(1,4)))

# Define a dense layer with 8 units
model.add(LSTM(8, activation='tanh', return_sequences=True))

model.add(LSTM(16, activation='tanh', return_sequences=True))

# Define another dense layer with 8 units
model.add(LSTM(8, activation='tanh'))

# Define the output layer with 3 units
model.add(Dense(2, activation=stretched_tanh))

# Compile the model with a loss function and an optimizer
model.compile(loss='mean_squared_error', optimizer='adam', metrics=['accuracy'])

df = pandas.read_csv("C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\nn_data\\data.csv")

X = df.iloc[:,[2,3,4,5]].astype(float)
Y = df.iloc[:,[0,1]].astype(float)

X = np.expand_dims(X, 1)
Y = np.expand_dims(Y, 1)

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.2, shuffle = True, random_state = 8)

checkpoint = ModelCheckpoint("C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\src\\rnn_weight.h5",
                            monitor='val_accuracy',
                            verbose=1,
                            save_best_only=True,
                            mode='max')

model.fit(X_train, Y_train, epochs=50, batch_size=10, verbose=1, callbacks=[checkpoint], validation_data=(X_test, Y_test))
'''# Perform k-fold cross-validation
k = 10
scores = cross_val_score(model, X, Y, cv=k)

# Print the mean and standard deviation of the cross-validation scores
print("Accuracy: %0.4f (+/- %0.4f)" % (scores.mean(), scores.std()))'''

prediction = model.predict([[[1752, 1752, 1752, 1752]]])
print(prediction)
model.save_weights("rnn_model.h5")