# |   |--- term_60 months <= 0.50
# |   |   |--- weights: [358.00, 3318.00] class: 1
# |   |--- term_60 months >  0.50
# |   |   |--- weights: [196.00, 0.00] class: 0
# |--- FICO_>500 >  0.50
# |   |--- gender_Male <= 0.50
# |   |   |--- weights: [1048.00, 310.00] class: 0
# |   |--- gender_Male >  0.50
# |   |   |--- weights: [633.00, 1065.00] class: 1

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

number = [0,1,1,0,0,0,0,0,0,0,0]
number.reverse()
print(score(number))

number = [0,0,1,0,0,0,0,0,1,1,0]
number.reverse()
print(score(number))

number = [0,0,1,0,0,0,0,0,0,1,1]
number.reverse()
print(score(number))

number = [0,0,1,0,0,0,0,0,1,1,1]
number.reverse()
print(score(number))

number = [0,0,1,0,0,0,0,0,1,0,0]
number.reverse()
print(score(number))

number = [0,0,1,0,0,0,0,1,0,0,1]
number.reverse()
print(score(number))

number = [0,0,0,0,0,0,0,0,1,1,0]
number.reverse()
print(score(number))

number = [0,0,1,0,0,0,0,0,0,1,1]
number.reverse()
print(score(number))

number = [0,0,0,0,0,0,0,1,0,0,1]
number.reverse()
print(score(number))

number = [1,1,1,0,0,0,0,0,1,1,1]
number.reverse()
print(score(number))

