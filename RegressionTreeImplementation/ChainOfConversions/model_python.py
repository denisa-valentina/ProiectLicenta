def score(input):
    if input[10] <= 10625.0:
        if input[1] <= 125.0:
            if input[3] <= 11250.0:
                if input[8] <= 2875.0:
                    var0 = 5375.0
                else:
                    var0 = 5750.0
            else:
                if input[1] <= 125.0:
                    var0 = 6375.0
                else:
                    var0 = 5750.0
        else:
            if input[10] <= 9875.0:
                if input[1] <= 375.0:
                    var0 = 5375.0
                else:
                    var0 = 5000.0
            else:
                if input[1] <= 750.0:
                    var0 = 5500.0
                else:
                    var0 = 4375.0
    else:
        if input[10] <= 11625.0:
            if input[2] <= 125.0:
                if input[5] <= 8500.0:
                    var0 = 4750.0
                else:
                    var0 = 5625.0
            else:
                if input[0] <= 7250.0:
                    var0 = 6250.0
                else:
                    var0 = 5875.0
        else:
            if input[5] <= 15875.0:
                if input[9] <= 625.0:
                    var0 = 6000.0
                else:
                    var0 = 6500.0
            else:
                if input[3] <= 1750.0:
                    var0 = 6375.0
                else:
                    var0 = 6750.0
    return var0
