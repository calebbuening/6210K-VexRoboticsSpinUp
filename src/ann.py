import keras
from keras.models import Model
from keras.layers import Dense, Lambda, BatchNormalization
import pandas

# Define the input layer with 4 units
inputs = BatchNormalization(shape=(4,))

# Define a dense layer with 8 units
x = Dense(8, activation='tanh')(inputs)

# Define another dense layer with 8 units
x = Dense(8, activation='tanh')(x)

# Define the output layer with 3 units
raw_outputs = Dense(3, activation='tanh')(x)

# Use a Lambda layer to scale the outputs to be between -12 and 12
outputs = Lambda(lambda x: x * 12)(raw_outputs)

# Create the model using the input and output layers
model = Model(inputs=inputs, outputs=outputs)

# Compile the model with a loss function and an optimizer
model.compile(loss='mean_squared_error', optimizer='adam', metrics=['accuracy'])

df = pandas.read_csv("C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\nn_data\\data.csv")

dataset = df.values
X = dataset[:,3:11].astype(float)
Y = dataset[:,0:2].astype(float)

model.fit(X,Y, epochs=5000, batch_size=50)
model.summary()
model.predict(1752, 1752, 1752, 1752)