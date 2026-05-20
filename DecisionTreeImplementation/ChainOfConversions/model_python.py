# |   |--- term_60 months <= 0.50
# |   |   |--- weights: [358.00, 3318.00] class: 1
# |   |--- term_60 months >  0.50
# |   |   |--- weights: [196.00, 0.00] class: 0
# |--- FICO_>500 >  0.50
# |   |--- gender_Male <= 0.50
# |   |   |--- weights: [1048.00, 310.00] class: 0
# |   |--- gender_Male >  0.50
# |   |   |--- weights: [633.00, 1065.00] class: 1

import ast

def score(input):
    if input[10] <= 0.5:
        if input[0] <= 0.5:
            var0 = [0.09738846572361262, 0.9026115342763874]
        else:
            var0 = [1.0, 0.0]
    else:
        if input[1] <= 0.5:
            var0 = [0.7717231222385862, 0.22827687776141384]
        else:
            var0 = [0.37279151943462896, 0.627208480565371]
    
    # adaugat
    prediction = 1 if var0[1] > var0[0] else 0

    return prediction

TP = 0
TN = 0
FP = 0
FN = 0
count = 0

with open("X_test_list",  'r') as X, open('Y_test', 'r') as Y:

    # line_X = X.readline()
    # line_Y = Y.readline()

    for line_X, line_Y in zip(X, Y):

        count += 1
        input = ast.literal_eval(line_X)
        # input.reverse()

        expected_Y = ast.literal_eval(line_Y)
        actual_Y = score(input)

        # print("Rezultat: " + str(expected_Y) + " la" + str(actual_Y))

        if expected_Y == 0 and actual_Y == 0:
            TN += 1
        elif expected_Y == 0 and actual_Y == 1:
            FP += 1
        elif expected_Y == 1 and actual_Y == 0:
            FN += 1
        elif expected_Y == 1 and actual_Y == 1:
            TP += 1
    
    print("TN = " + str(TN))
    print("TP = " + str(TP))
    print("FN = " + str(FN))
    print("FP = " + str(FP))
    print("Count = " + str(count))
    
