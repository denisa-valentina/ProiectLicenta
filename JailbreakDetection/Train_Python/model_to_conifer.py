import conifer
from pymilo import Import
import pickle
import os
import json
import pandas as pd
# from fxpmath import Fxp
from sklearn.model_selection import train_test_split
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, classification_report
from sklearn.metrics import (
    f1_score,
    accuracy_score,
    recall_score,
    precision_score,
    confusion_matrix,
    ConfusionMatrixDisplay,
    make_scorer,
)
import numpy as np
import datetime
import logging
import sys

logging.basicConfig(stream=sys.stdout, level=logging.WARNING)
logger = logging.getLogger('conifer')
logger.setLevel(logging.DEBUG)

def model_performance_classification_sklearn(model, predictor, target):
    """
    Function to compute different metrics to check classification model performance

    model: classifier
    predictors: independent variables
    target: dependent variable
    """

    # predicting using the independent variables
    pred = model.predict(predictor)

    acc = accuracy_score(target, pred) # to compune Accuracy
    recall = recall_score(target, pred) # to compute Recall
    precision = precision_score(target, pred) # to compute Precision
    f1 = f1_score(target, pred) # to compute F1-score

    # creating a dataframe of metrics
    df_perf = pd.DataFrame(
        {
            "Accuracy": acc,
            "Recall" : recall,
            "Precision" : precision,
            "F1 score": f1,
        },
        index=[0],
    )

    return df_perf

model = Import("random_forest.json").to_model()

with open("X_test.pkl", "rb") as file:
    X_test = pickle.load(file)

x_test = X_test.toarray()

with open("y_test.pkl", "rb") as file:
    y_test = pickle.load(file)

print("Test metrics: ")
print(model_performance_classification_sklearn(model, X_test, y_test))

# config = conifer.backends.xilinxhls.auto_config()

# config['Precision'] = 'ap_fixed<32,18>'
# config['ClockPeriod'] = 5 # 5ns = 200MHz
# config['XilinxPart'] = 'xczu7ev-ffvc1156-2-e'
# config['ProjectName'] = 'my_prj'
# config['OutputDir'] = 'forest_cpp'
# config['Strategy'] = 'latency'

# # target zcu104 board and float interface (the conversion from float to ap_fixed is performed on the FPGA)
# accelerator_config = {'Board' : 'zcu104',
#                       'InterfaceType': 'float'}
# config['AcceleratorConfig'] = accelerator_config

# model_hls = conifer.converters.convert_from_sklearn(model, config)


# model_hls.write()
# # model_hls.compile()
# # model_hls.build()

# with open("x_test.log", "rb") as file:
#     X_test = pickle.load(file)

# with open('forest_cpp/tb_data/tb_input_features.dat', 'wb') as file:
#     np.savetxt(file, X_test, delimiter=' ')


# data = np.loadtxt("forest_cpp/tb_data/tb_input_features.dat", dtype=np.float32)

# print(data.shape)  # should be (6184, 50)

# with open("test_data.h", "w") as f:
#     f.write(f"#define N_VECTORS_DATA {data.shape[0]}\n")  # use shape[0] = 6184
#     f.write("static const float all_vectors[] = {\n")
#     for i, v in enumerate(data.flatten()):
#         f.write(f"    {v}f")
#         if i < len(data.flatten())-1: f.write(",")
#         if i % 8 == 7: f.write("\n")
#     f.write("\n};\n")

# with open("y_test.log", "rb") as file:
#     y_test = pickle.load(file)

# with open("result_data.h", "w") as file:
#     for y in y_test:
#         file.write(f"{y}\n")


# -----------------------------------------------------------------------------------

# report = model_hls.read_report()
# print(report)

# print(f"Latency (Cycles): {report['latency']}")
# print(f"Interval (Cycles): {report['interval']}") # Throughput measure
# print("Resource Usage:")
# print(f"  DSPs: {report['dsp']}")
# print(f"  LUTs: {report['lut']}")
# print(f"  FFs:  {report['ff']}")

# y = model_hls.decision_function(X_test)
# print(y[0])

# print(y)
# with open("result_data.h", 'wb') as file:
#     pickle.dump(y, file)

# synthesize the model, targeting bitfile creation and packaging
# warning: this may take some time!
# model_hls.build(bitfile=True, package=True)