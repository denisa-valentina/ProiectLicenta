import functions as f

def main():

    f.init()
    data = f.read_data()
    loan = data.copy()
    
    # f.print_values(loan)

    # dropping infos we don't need
    loan.drop(["ID"], axis=1, inplace=True)

    # in case same value of a feature is spelled in 2 or 
    # more different forms
    f.data_cleaning(loan)

    # Model building. Approach

    # data preparation (the conclusion of decision is dropped)
    X = loan.drop(["isDelinquent"], axis=1)
    # and moved to Y
    y = loan["isDelinquent"]

    # creeaza variabile pentru fiecare
    # posibilitate a unei caracteristici (Male/Female, term 36/60)
    X = f.pd.get_dummies(X, drop_first=True)

    X_train, X_test, \
    y_train, y_test = f.train_test_split(X, y, 
                                        test_size=0.4,
                                        random_state=1)

    # Construirea modelului de arbore de decizie
    arbore = f.DecisionTreeClassifier(criterion="gini", random_state=1)
    arbore.fit(X_train, y_train)

    # f.check_performance(arbore, X_train, y_train, X_test, y_test)
    
    # visualize_tree(arbore, X, "First version of the decission tree")

    # 1. tuning using parameters (pre-pruning)
    f.check_importances(arbore, X.columns)
    
    estimator = f.choose_parameter(X_train, y_train)
    
    # visualize_tree(estimator, X, "Pre-pruning tree")

    # f.check_performance(estimator, X_train, y_train, X_test, y_test)

    f.visualize_tree(estimator, X, "Pre-pruning tree")

    # 2. cost complexity pruning
    pruning_arbore = f.pruning(X, X_train, y_train, X_test, y_test)

    f.visualize_tree(pruning_arbore, X, "Cost Complexity pruning")

    # f.model_comparisons(arbore, estimator, pruning_arbore, \
                    #   X_train, y_train, X_test, y_test)
 
    # feature_names = list(X.columns)

    # with open("output.txt", "w") as file:
    #     print(f.tree.export_text(pruning_arbore, feature_names=feature_names, show_weights=True), file=file)

    # print(feature_names)
    # print(pruning_arbore.feature_names_in_)

    f.Export(pruning_arbore).save("export_model.json")
    
    X_int = X_test.to_numpy()
    with open("X_test", "w") as file:
        for i in range(1000):
            int_list = list(map(int, X_int[i]))
            file.write(''.join(map(str, int_list)))
            file.write('\n')

    Y_int = y_test.to_numpy()      
    with open("Y_test", "w") as file:
        for i in range(1000):
            int_list = Y_int[i]
            file.write(str(int_list))
            file.write('\n')

    return 0

if __name__ == "__main__":
    main()