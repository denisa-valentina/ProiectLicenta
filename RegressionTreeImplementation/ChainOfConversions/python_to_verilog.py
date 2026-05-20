from myhdl import block, Signal, intbv, always_comb

scale = 16 
min_value = -10
max_value = 10

@block 
def calculate_score(features, prediction):

    @always_comb
    def score():
        if features[10] <= 10625:
            if features[1] <= 125:
                if features[3] <= 11250:
                    if features[8] <= 2875:
                        prediction.next = 5375
                    else:
                        prediction.next = 5750
                else:
                    if features[1] <= 125:
                        prediction.next = 6375
                    else:
                        prediction.next = 5750
            else:
                if features[10] <= 9875:
                    if features[1] <= 375:
                        prediction.next = 5375
                    else:
                        prediction.next = 5000
                else:
                    if features[1] <= 750:
                        prediction.next = 5500
                    else:
                        prediction.next = 4375
        else:
            if features[10] <= 11625:
                if features[2] <= 125:
                    if features[5] <= 8500:
                        prediction.next = 4750
                    else:
                        prediction.next = 5625
                else:
                    if features[0] <= 7250:
                        prediction.next = 6250
                    else:
                        prediction.next = 5875
            else:
                if features[5] <= 15875:
                    if features[9] <= 625:
                        prediction.next = 6000
                    else:
                        prediction.next = 6500
                else:
                    if features[3] <= 1750:
                        prediction.next = 6375
                    else:
                        prediction.next = 6750
    return score,

features = Signal(intbv(0, min=min_value, max=max_value)[11:])
prediction = Signal(intbv(0, min=min_value, max=max_value))

score_ints = calculate_score(features, prediction)

score_ints.convert(hdl="Verilog")
