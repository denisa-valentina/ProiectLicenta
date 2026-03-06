import os
import m2cgen
from pymilo import Import
import pythran as pcpp
import hls4ml

print("Current working directory:", os.getcwd())

input_json_path = input("Please enter the name of the json file that stores the tree model: ") + '.json'
if not os.path.isfile(input_json_path):
    print(f"The file '{input_json_path}' was not found. Please check the path and try again.")
else:
    
    model = Import(input_json_path).to_model()
    # features = model.feature_names_in_

    model_to_cpp = m2cgen.export_to_c(model)

    output = input("Please enter the name of the cpp file to put the output code: ") + '.cpp'
    with open(output, "w") as file:
        file.write(model_to_cpp)

# ..\TrainModel\export_model
# model_python
# model_cpp