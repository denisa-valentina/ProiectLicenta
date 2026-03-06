from myhdl import block, Signal, intbv, always_comb
import model_python as m

@block 
def calculate_score(features, prediction):
    
    @always_comb
    def score():
        if features[10] == 0:
            if features[0] == 0:
                prediction.next = 1 #[0.09738846572361262, 0.9026115342763874]
            else:
                prediction.next = 0 #[1.0, 0.0]
        else:
            if features[1] == 0:
                prediction.next = 0 #[0.7717231222385862, 0.22827687776141384]
            else:
                prediction.next = 1 # [0.37279151943462896, 0.627208480565371]
    return score,

features = Signal(intbv(0)[11:])
prediction = Signal(bool(0))

score_inst = calculate_score(features, prediction)

# Run the simulation
# sim = Simulation(score_inst)

score_inst.convert(hdl="Verilog")