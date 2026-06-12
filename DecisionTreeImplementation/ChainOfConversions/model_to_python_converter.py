import os
import m2cgen
from pymilo import Import

print("Current working directory:", os.getcwd())

input_json_path = input("Please enter the name of the json file that stores the tree model: ") + '.json'
if not os.path.isfile(input_json_path):
    print(f"The file '{input_json_path}' was not found. Please check the path and try again.")
else:
    
    model = Import(input_json_path).to_model()
    # features = model.feature_names_in_

    model_to_python = m2cgen.export_to_python(model) 

    print(model_to_python)
    output = input("Please enter the name of the python file to put the output code: ") + '.py'
    with open(output, "w") as file:
        file.write(model_to_python)

# ..\TrainModel\export_model
# model_python