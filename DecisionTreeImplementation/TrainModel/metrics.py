from functions import *
import pickle

with open("x_test.pkl", "rb") as file:
    x_test = pickle.load(file)

with open("y_test.pkl", "rb") as file:
    y_test = pickle.load(file)

with open("model.pkl", "rb") as file:
    model = pickle.load(file)

# Make predictions
y_pred = model.predict(x_test)

# Evaluate the model
accuracy = model.score(x_test, y_test)
precision = precision_score(y_test, y_pred, pos_label=1, zero_division=0)
recall = recall_score(y_test, y_pred, pos_label=1, zero_division=0)
f1 = f1_score(y_test, y_pred, pos_label=1, zero_division=0)

print(f"Accuracy: {accuracy:.4f}, "
      f"Precision: {precision:.4f}, Recall: {recall:.4f}, F1: {f1:.4f}")
