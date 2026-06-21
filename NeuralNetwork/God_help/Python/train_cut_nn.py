from functions import *

# with open("trained_model_nn.pkl", 'rb') as file:
#     model = pickle.load(file)

with open("x_test.pkl", "rb") as file:
    x_test = pickle.load(file)

with open("y_test.pkl", "rb") as file:
    y_test = pickle.load(file)

with open("y_train.pkl", "rb") as file:
    y_train = pickle.load(file)

with open("x_train.pkl", "rb") as file:
    x_train = pickle.load(file)

# Assuming x_train and y_train are already prepared
pytorch_estimator = SklearnPyTorchWrapper(input_dim=x_train.shape[1], epochs=10, patience=3)

print(pytorch_estimator.model.summary())

# Train the model
pytorch_estimator.fit(x_train, y_train)

# Make predictions
y_pred_pytorch = pytorch_estimator.predict(x_test)

# Evaluate the model
pytorch_accuracy = pytorch_estimator.score(x_test, y_test)
pytorch_precision = precision_score(y_test, y_pred_pytorch, pos_label=1, zero_division=0)
pytorch_recall = recall_score(y_test, y_pred_pytorch, pos_label=1, zero_division=0)
pytorch_f1 = f1_score(y_test, y_pred_pytorch, pos_label=1, zero_division=0)

# Train set predictions  
y_train_pred = pytorch_estimator.predict(x_train)

# Train metrics
train_accuracy = pytorch_estimator.score(x_train, y_train)
train_precision = precision_score(y_train, y_train_pred)
train_recall = recall_score(y_train, y_train_pred)
train_f1 = f1_score(y_train, y_train_pred)


print(f"PyTorch Classifier - Accuracy (Test): {pytorch_accuracy:.4f}, "
      f"Precision: {pytorch_precision:.4f}, Recall: {pytorch_recall:.4f}, F1: {pytorch_f1:.4f}")

print(f"PyTorch Classifier - Accuracy (Train): {train_accuracy:.4f}, "
      f"Precision: {train_precision:.4f}, Recall: {train_recall:.4f}, F1: {train_f1:.4f}")

with open("trained_model_nn_cut.pkl", 'wb') as file:
    pickle.dump(pytorch_estimator, file)

# with open("x_test_100.pkl", "wb") as file:
#     pickle.dump(x_test, file)

# with open("x_train_100.pkl", "wb") as file:
#     pickle.dump(x_train, file)

# with open("y_test_100.pkl", "wb") as file:
#     pickle.dump(y_test, file)

# with open("y_train_100.pkl", "wb") as file:
#     pickle.dump(y_train, file)