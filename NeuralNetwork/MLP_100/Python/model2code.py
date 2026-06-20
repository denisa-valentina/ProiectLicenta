# from functions import *
import pickle

from tensorflow import keras

# with open("trained_model_nn_100.pkl", 'rb') as file:
#     model = pickle.load(file)

# model.model.save_weights("mlp_weights.weights.h5")
# input_dim = model.model.input_shape[-1]
# print("INPUT_DIM:", input_dim)  # note this number down

# -----------------------------------------------------------------------

# input_dim = 100

# model_keras = keras.Sequential([
#     keras.layers.Dense(64, activation='relu', input_shape=(input_dim,)),
#     keras.layers.Dropout(0.4),
#     keras.layers.Dense(32, activation='relu'),
#     keras.layers.Dropout(0.3),
#     keras.layers.Dense(1, activation='sigmoid')
# ])

# model_keras.load_weights("mlp_weights.weights.h5")
# model_keras.save("neural_network_k2.h5")

# -----------------------------------------------------------------------

from tensorflow import keras
from keras2c import k2c

model_keras = keras.models.load_model("neural_network_k2.h5")
model_keras.summary()
print('Num layers:', len(model_keras.layers))
k2c(model_keras, "mlp_100")  # adjust args to match keras2c's actual API
