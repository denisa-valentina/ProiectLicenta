import torch
import torch.nn as nn
import tensorflow as tf
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader
from sklearn.metrics import precision_score, recall_score, f1_score
from sklearn.model_selection import train_test_split
from scipy.sparse import csr_matrix
from sklearn.base import BaseEstimator, ClassifierMixin
import json
from sklearn.feature_extraction.text import TfidfVectorizer
from langdetect import detect, DetectorFactory
from gensim.models import Word2Vec
from sklearn.metrics.pairwise import cosine_similarity
import seaborn as sns
import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap
import pandas as pd
import numpy as np
import os
import pickle
import gensim
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
import string
from tensorflow import keras

import nltk
# nltk.download('stopwords')
# nltk.download('punkt_tab')

stop_words = set(stopwords.words('english'))

def preprocess(text):
  # text = text.lower()
  text = ''.join([word for word in text if word not in string.punctuation])
  tokens = word_tokenize(text)
  tokens = [word for word in tokens if word not in stop_words]
  return tokens

# Wrap PyTorch Model into scikit-learn's BaseEstimator
class SklearnPyTorchWrapper(BaseEstimator, ClassifierMixin):
    def __init__(self, input_dim, epochs=10, batch_size=64, lr=0.001, patience=5):
        self.epochs = epochs
        self.batch_size = batch_size
        self.lr = lr
        self.patience = patience
        self.input_dim = input_dim  # Assign input_dim as an instance attribute
        self.model = self._build_model()

    def _build_model(self):
        # model = tf.keras.Sequential([
        #         tf.keras.layers.Dense(64, activation='relu', input_shape=(self.input_dim,)),
        #         tf.keras.layers.Dropout(0.4),
        #         tf.keras.layers.Dense(32, activation='relu'),
        #         tf.keras.layers.Dropout(0.3),
        #         tf.keras.layers.Dense(1, activation='sigmoid')
        #     ])
        model = tf.keras.Sequential([
                tf.keras.layers.Dense(8, activation='relu', input_shape=(self.input_dim,)),
                # tf.keras.layers.Dense(8, activation='relu'),
                tf.keras.layers.Dense(1, activation='sigmoid')
            ])
        model.compile(
            optimizer=keras.optimizers.RMSprop(learning_rate=self.lr),
            loss='binary_crossentropy',
            metrics=['accuracy']
        )
        return model

    def fit(self, X, y):
        # Split into train and validation sets
        X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, random_state=42)

        early_stop = keras.callbacks.EarlyStopping(monitor='val_loss', patience=self.patience)
        self.model.fit(X_train, y_train,    
                        validation_data=(X_val, y_val),
                        epochs=self.epochs,
                        batch_size=self.batch_size,
                        verbose=1,
                        callbacks=[early_stop])

        return self  # Required for scikit-learn compatibility

    def predict(self, X):
        preds = self.model.predict(X)
        return (preds > 0.5).astype(int).flatten()

    def score(self, X, y):
        preds = self.predict(X)
        return np.mean(preds == y)
    
    def export_model(self):
        self.model.save("neural_network.keras")