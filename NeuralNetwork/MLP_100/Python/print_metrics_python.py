from functions import *
from sklearn.metrics import confusion_matrix


with open("trained_model_nn_100.pkl", 'rb') as file:
    model = pickle.load(file)

with open("x_test.pkl", "rb") as file:
    x_test = pickle.load(file)

# with open("y_test.pkl", "rb") as file:
#     y_test = pickle.load(file)

with open("result_data.dat", 'r') as file:
    y_test = [int(line.strip()) for line in file if line.strip()]

# with open("y_test.pkl", "wb") as file:
#     pickle.dump(y_test, file)


# Make predictions
y_pred_pytorch = model.predict(x_test)

y_float_preds = model.model.predict(x_test)

y_int_pred =(y_float_preds > 0.5).astype(int).flatten()

with open("python_csim_result.log", 'w') as file:
    for y in y_float_preds:
        file.write(str(float(y)) + '\n')

with open("python_result.dat", 'w') as file:
    for y in y_int_pred:
        file.write(str(int(y)) + '\n')

# Evaluate the model
pytorch_accuracy = model.score(x_test, y_test)
pytorch_precision = precision_score(y_test, y_pred_pytorch, pos_label=1, zero_division=0)
pytorch_recall = recall_score(y_test, y_pred_pytorch, pos_label=1, zero_division=0)
pytorch_f1 = f1_score(y_test, y_pred_pytorch, pos_label=1, zero_division=0)


print(f"PyTorch Classifier - Accuracy (Test): {pytorch_accuracy:.4f}, "
      f"Precision: {pytorch_precision:.4f}, Recall: {pytorch_recall:.4f}, F1: {pytorch_f1:.4f}")

tn, fp, fn, tp = confusion_matrix(y_test, y_pred_pytorch).ravel()



