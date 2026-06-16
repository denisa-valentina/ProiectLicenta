import warnings
warnings.filterwarnings("ignore")
from pymilo import Import
import pickle
import numpy as np
import time
import m2cgen as m2c

model = Import("random_forest.json").to_model()

code = m2c.export_to_c(model)

print(code)

with open("output.h", "w") as f:
    f.write(code)