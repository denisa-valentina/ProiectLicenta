# from functions import *
import pickle
import numpy as np
import tensorflow

# with "env" enviroment (source env/bin/activate)

# with open("trained_model_nn.pkl", 'rb') as file:
#     model = pickle.load(file)

# model.model.save("neural_network_k2.h5")

# weights = model.model.get_weights()
# np.savez("weights_raw.npz", *weights)

# print("INPUT_DIM:", model.model.input_shape[-1])
# for i, w in enumerate(weights):
#     print(f"Layer weight {i}: shape {w.shape}")

# -----------------------------------------------------------------------

# with keras2_env (conda activate keras2_env)

input_dim = 25

from tensorflow import keras
from keras2c import k2c

model_keras = keras.Sequential([
    keras.layers.Dense(32, activation='relu', input_shape=(input_dim,)),
    keras.layers.Dropout(0.4),
    keras.layers.Dense(16, activation='relu'),
    keras.layers.Dropout(0.3),
    keras.layers.Dense(1, activation='sigmoid')
])

loaded = np.load("weights_raw.npz")
weights = [loaded[f"arr_{i}"] for i in range(len(loaded.files))]
model_keras.set_weights(weights)

model_keras.summary()
print('Num layers:', len(model_keras.layers))

from keras2c import k2c
k2c(model_keras, "mlp_c")
