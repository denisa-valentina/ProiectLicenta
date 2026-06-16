import pandas as pd
import numpy as np
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
import pickle
from pymilo import Export, Import
# from fxpmath import Fxp

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

# Load data
data = pd.read_csv('sql_dataset.csv')

# Check for missing values and clean if necessary
data.dropna(inplace=True)

# Split dataset into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(data['Query'], data['Label'], test_size=0.2, random_state=42)

# Transform queries using TF-IDF
vectorizer = TfidfVectorizer(max_features=50)
X_train_tfidf = vectorizer.fit_transform(X_train)
X_test_tfidf = vectorizer.transform(X_test)

# X_train_tfidf.data = Fxp(X_train_tfidf.data, signed=True, n_word=32, n_frac=14).val
# X_test_tfidf.data = Fxp(X_test_tfidf.data, signed=True, n_word=32, n_frac=14).val

# print(X_train_tfidf.data)
# print(X_test_tfidf.data)

# Initialize and train the classifier
model = RandomForestClassifier(
        n_estimators=20,              # You can reduce slightly if needed
        max_depth=5,                  # Limit depth of trees
        min_samples_split=15,          # Prevent overly fine splits
        min_samples_leaf=7,            # Ensure leaves have enough data
        max_features='log2',           # Use only sqrt(n_features) at each split
        class_weight='balanced',       # Keep this for imbalanced data
        random_state=42,
        n_jobs=-1 )

model.fit(X_train_tfidf, y_train)

# for i in range(model.n_estimators):
#     model.estimators_[i].tree_.value[:] = Fxp(model.estimators_[i].tree_.value[:], n_word=32, n_frac=14).val

# Evaluate
print("Train metrics: ")
print(model_performance_classification_sklearn(model, X_train_tfidf, y_train))

print("Test metrics: ")
print(model_performance_classification_sklearn(model, X_test_tfidf, y_test))

Export(model).save("random_forest.json")
with open("X_test.pkl", "wb") as file:
    pickle.dump(X_test_tfidf, file)
with open("y_test.pkl", "wb") as file:
    pickle.dump(y_test, file)
with open("X_train.pkl", "wb") as file:
    pickle.dump(X_train_tfidf, file)
with open("y_train.pkl", "wb") as file:
    pickle.dump(y_train, file)
