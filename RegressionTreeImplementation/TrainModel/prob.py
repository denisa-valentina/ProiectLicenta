import warnings
# reading and manipulating data
import pandas as pd
import numpy as np

# to export/import scikit models
from pymilo import Export

from fxpmath import Fxp

# to split data
from sklearn.model_selection import train_test_split

# libraries to build regression tree
from sklearn.tree import DecisionTreeRegressor
from sklearn import tree

# tune different models
from sklearn.model_selection import GridSearchCV

# data visualization
import matplotlib.pyplot as plt
import seaborn as sns

from ucimlrepo import fetch_ucirepo 

# Evaluate the Tree Regressor's error
from sklearn.metrics import root_mean_squared_error

from sklearn.metrics import (
    f1_score,
    accuracy_score,
    recall_score,
    precision_score,
    confusion_matrix,
    ConfusionMatrixDisplay,
    make_scorer,
)

def init():
    warnings.filterwarnings("ignore")

    pd.set_option("display.max_columns", None)
    pd.set_option("display.max_rows", 200)

def read_data(string, sep=''):
    data = pd.read_csv(string, sep=sep)
    return data

def print_values(dataset):
    print(dataset.head())
    print(dataset.tail())
    print(dataset.info())
    print(dataset.describe(include="all"))

    print(dataset.isnull().sum())

def data_cleaning(data):
    # for column in data.columns:
    #     print(data[column].unique())
    data.fillna(0, inplace=True)

# create labeled barplots
def labeled_barplot(data, feature, perc=False, n=None):
    """
    Barplot with percentage at the top

    data: dataframe
    feature: dataframe column
    perc: whether to display percentages instead of count (default is False)
    n: displays the top n category levels (default is None, i.e., display all levels)
    """

    total = len(data[feature]) # Length of the column
    count = data[feature].nunique()
    if n is None:
        plt.figure(figsize=(count + 2, 6))
    else:
        plt.figure(figsize=(n+2, 6))
    
    plt.xticks(rotation=90, fontsize=15)
    ax = sns.countplot(
        data=data,
        x=feature,
        palette = "Paired",
        order=data[feature].value_counts().index[:n].sort_values(),
    )

    for p in ax.patches:
        if perc == True:
            label = "{:.1f}%".format(
                100 * p.get_height() / total
                # percentage of each class of the category
            )
        else:
            label = p.get_height() # count of each level of the category
        x = p.get_x() + p.get_width() / 2  # width of the plot
        y = p.get_height() # height of the plot

        ax.annotate(
            label,
            (x, y),
            ha="center",
            va="center",
            size=12,
            xytext=(0, 5),
            textcoords="offset points",
        ) # annotate the percentage

    plt.show() # show the plot

def display_barplots(data):
    for column in data.columns:
        labeled_barplot(data, column, perc=True)

def stacked_barplot(data, predictor, target):
    """
    Print the category counts and plot a stacked bar chart

    data: dataframe
    predictor: independent variable
    target: target variable
    """

    count = data[predictor].nunique()
    sorter = data[target].value_counts().index[-1]
    tab1 = pd.crosstab(data[predictor], data[target], 
                        margins=True).sort_values(
                            by=sorter, ascending=False
                        )
    print(tab1)
    print("-" * 120)
    tab = pd.crosstab(data[predictor], data[target],
                      normalize="index").sort_values(
                        by=sorter, ascending=False
                      )
    tab.plot(kind="bar", stacked=True, figsize=(count + 5, 6))
    plt.legend(
        loc="lower left", frameon=False,
    )

    plt.legend(loc="upper left", bbox_to_anchor=(1, 1))
    plt.show()

# defining a function to compute different metrics to check performance
# of a classification model built using sklearn
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

def confusion_matrix_sklearn(model, predictors, target):
    """
    To plot the confusion_matrix with percentages

    model: classifier
    predictors: independent variables
    target: dependent variable
    """

    # metoda utilizata pentru a face predictii pe baza unui
    # model antrenat
    y_pred = model.predict(predictors)
    conf_matr = confusion_matrix(target, y_pred)
    
    labels = np.asarray(
        [
            ["{0:0.0f}".format(item) + 
             "\n{0:.2%}".format(item / conf_matr.flatten().sum())]
             for item in conf_matr.flatten()
        ]
    ).reshape(2, 2)

    plt.figure(figsize=(6, 4))
    sns.heatmap(conf_matr, annot=labels, fmt="")
    plt.ylabel("True label")
    plt.xlabel("Predicted label")
    plt.show()

def check_performance(arbore, X_train, y_train, X_test,  y_test):
    # verificarea performantelor modelului pe setul de date de antrenare
    regression_tree_perf_train = model_performance_classification_sklearn(
                            arbore, X_train, y_train)
    print(regression_tree_perf_train)
    confusion_matrix_sklearn(arbore, X_train, y_train)

    # verificarea performantelor modelului pe setul de date de testare
    decision_tree_perf_test = model_performance_classification_sklearn(
                            arbore, X_test, y_test)
    print(decision_tree_perf_test)
    confusion_matrix_sklearn(arbore, X_test, y_test)

def extract_tree_features(arbore, X_train):
    features = pd.DataFrame(
            arbore.feature_importances_,
            index = X_train.columns)
    
    return list(features)

def visualize_tree(arbore, X, string):
    column_names = list(X.columns)
    feature_names = column_names
    # print(feature_names)

    plt.figure(figsize=(20, 30))
    plt.suptitle(string)

    out = tree.plot_tree(
        arbore,
        feature_names=feature_names,
        filled=True,
        fontsize=9,
        node_ids=True,
        class_names=True,
    )

    for o in out:
        arrow = o.arrow_patch
        if arrow is not None:
            arrow.set_edgecolor("blue")
            arrow.set_linewidth(1.5)
    plt.show()

    # Text report showing the rules of a decision tree
    # print(tree.export_text(arbore, feature_names=feature_names,
                        #    show_weights=True))

def check_importances(arbore, feature_names):
    importances = arbore.feature_importances_
    indices = np.argsort(importances)

    plt.figure(figsize=(12, 12))
    plt.title("Feature importances")
    plt.barh(range(len(indices)), importances[indices], color="violet",
             align="center")
    plt.yticks(range(len(indices)), [feature_names[i] for i in indices])
    plt.xlabel("Relative Importance")
    plt.show()

def choose_parameter(X_train, y_train):
    # alege tipul de clasificator
    estimator = DecisionTreeRegressor(random_state=1)

    parameters = {
        "max_depth": range(2, 15, 1),
        "criterion": ["squared_error", "absolute_error"],
        "splitter": ["best", "random"],
        "min_impurity_decrease": [0.000001, 0.00001, 0.0001],
    }

    # Run the grid search
    grid_obj = GridSearchCV(estimator, parameters, scoring="neg_mean_squared_error", cv=5)
    grid_obj = grid_obj.fit(X_train, y_train)

    # preluam cea mai buna combinatie de parametri
    estimator = grid_obj.best_estimator_

    print("Gasirea celor mai potriviti parametri: " + str(grid_obj.best_params_))
    # print(estimator.fit(X_train, y_train))

    # check_importances(estimator, X_train.columns)

    return estimator

def pruning(X, X_train, y_train, X_test, y_test):
    clf = DecisionTreeRegressor(random_state=1)
    path = clf.cost_complexity_pruning_path(X_train, y_train)
    ccp_alphas, impurities = path.ccp_alphas, path.impurities

    # print(pd.DataFrame(path))

    fig, ax = plt.subplots(figsize=(15, 5))
    ax.plot(ccp_alphas[:-1], impurities[:-1], marker="o",
            drawstyle="steps-post")
    ax.set_xlabel("effective alpha")
    ax.set_ylabel("total impurity of leaves")
    ax.set_title("Total Impurity vs effective alpha for training set")
    plt.show()

    clfs = []
    for ccp_alpha in ccp_alphas:
        clf = DecisionTreeRegressor(random_state=1, ccp_alpha=ccp_alpha)
        clf.fit(X_train, y_train)
        clfs.append(clf)
    print(
        "Number of nodes in the last tree is: {} with ccp_alpha:" \
        "{}".format(clfs[-1].tree_.node_count, ccp_alphas[-1])
    )

    # renuntam la ultimul element, deoarece e aborele trivial 
    # cu un singur nod
    clfs = clfs[:-1]
    ccp_alphas = ccp_alphas[:-1]

    node_counts = [clf.tree_.node_count for clf in clfs]
    depth = [clf.tree_.max_depth for clf in clfs]

    # numarul de noduri in functie de alpha
    fig, ax = plt.subplots(2, 1, figsize=(10, 7))
    ax[0].plot(ccp_alphas, node_counts, marker="o", drawstyle="steps-post")
    ax[0].set_xlabel("alpha")
    ax[0].set_ylabel("number of nodes")
    ax[0].set_title("Number of nodes vs alpha")
    ax[1].plot(ccp_alphas, depth, marker="o", drawstyle="steps-post")
    # adancimea in functie de alpha
    ax[1].set_xlabel("alpha")
    ax[1].set_ylabel("depth of tree")
    ax[1].set_title("Depth vs alpha")
    fig.tight_layout()

    # recall in functie de alpha
    recall_train = []
    for clf in clfs:
        pred_train = clf.predict(X_train)
        values_train = rmse = root_mean_squared_error(y_train, pred_train)
        recall_train.append(values_train)

    recall_test = []
    for clf in clfs:
        pred_test = clf.predict(X_test)
        values_test = root_mean_squared_error(y_test, pred_test)
        recall_test.append(values_test)
    
    fig, ax = plt.subplots(figsize=(15, 5))
    ax.set_xlabel("alpha")
    ax.set_ylabel("Recall")
    ax.set_title("Recall vs alpha for training and testing sets")
    ax.plot(ccp_alphas, recall_train, marker="o", label="train", drawstyle="steps-post")
    ax.plot(ccp_alphas, recall_test, marker="o", label="test", drawstyle="steps-post")
    ax.legend()
    plt.show()

    # crearea modelului utilizand cel mai bun recall 
    # obtinut pe train si test
    index_best_model = np.argmax(recall_test)
    best_model = clfs[index_best_model]
    print(best_model)

    # check_performance(best_model, X_train, y_train, X_test, y_test)
    
    # check_importances(best_model, X.columns)

    return best_model

def life_main():
    data = read_data("Life Expectancy Data.csv")
    
    data_cleaning(data)

    # data preparation (the conclusion of decision is dropped)
    X = data.drop(["Life expectancy "], axis=1)
    Y = data["Life expectancy "]

    # caracteristicile reprezentate prin string-uri (nume tara, ocupatie,
    # au fost extinse)
    X = pd.get_dummies(X, drop_first=True)

    return data, X, Y

def wine_main():
    red = read_data("winequality-red.csv", ';')
    white = read_data("winequality-white.csv", ';')
    dataset = pd.concat([red, white], axis=0, ignore_index=True)

    # data preparation (the conclusion of decision is dropped)
    X = dataset.drop(["quality"], axis=1)
    Y = dataset["quality"]

    return dataset, X, Y



def main():

    init()
    data, X, Y = wine_main()
    
    # print_values(data)

    # display_barplots(data)

    # print(data.columns)

    X_train, X_test, \
    Y_train, Y_test = train_test_split(X, Y, test_size=0.4, random_state=1)

    # arbore = DecisionTreeRegressor(random_state=1)
    # arbore.fit(X_train, Y_train)

    # y_pred = arbore.predict(X_test)
    # # print("Label     :",list(Y_test))
    # # print("Prediction:",list(y_pred))

    # rmse = root_mean_squared_error(Y_test, y_pred)
    # print(f"Dummy Regression Error: {rmse}")

    # visualize_tree(arbore, X, "Prima faza")

    # check_importances(arbore, X.columns)

    estimator = choose_parameter(X_train, Y_train)

    # visualize_tree(estimator, X, "A doua faza")

    y_pred = estimator.predict(X_test)
    # print("Label     :",list(Y_test))
    # print("Prediction:",list(y_pred))

    rmse = root_mean_squared_error(Y_test, y_pred)
    print(f"Dummy Regression Error: {rmse}")

    # pruning_tree = pruning(X, X_train, Y_train, X_test, Y_test)

    # visualize_tree(pruning_tree, X, "A treia faza")

    # y_pred = pruning_tree.predict(X_test)
    # rmse = root_mean_squared_error(Y_test, y_pred)
    # print(f"Dummy Regression Error: {rmse}")

    for i in range(estimator.tree_.node_count):
        if estimator.tree_.feature[i] != -2:
            estimator.tree_.threshold[i] = Fxp(estimator.tree_.threshold[i], n_word=8, n_frac=3).get_val() * 1000
        estimator.tree_.value[i] = Fxp(estimator.tree_.value[i], n_word=8, n_frac=3).get_val() * 1000


    Export(estimator).save("export_model.json")

    X_int = X_test.to_numpy()
    with open("X_test", "w") as file:
        for i in range(X_int.shape[0]):
            # int_list = list(map(int, X_int[i]))
            # file.write(''.join(map(str, int_list)))
            int_list = Fxp(X_int[i], n_word=8, n_frac=3).get_val() * 1000
            file.write(str(list(map(float, int_list))))
            file.write('\n')

    Y_int = Y_test.to_numpy()      
    with open("Y_test", "w") as file:
        for i in range(Y_int.shape[0]):
            int_list = Fxp(Y_int[i], n_word=8, n_frac=3).get_val() * 1000
            file.write(str(int_list))
            file.write('\n')


if __name__ == "__main__":
    main()