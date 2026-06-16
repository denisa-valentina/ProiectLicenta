import conifer
from pymilo import Import
import pickle
import os
import json
import m2cgen as m2c
import numpy as np
import datetime
import logging
import sys

logging.basicConfig(stream=sys.stdout, level=logging.WARNING)
logger = logging.getLogger('conifer')
logger.setLevel(logging.DEBUG)

model = Import("random_forest.json").to_model()

code = m2c.export_to_c(model)

with open("output.h", "w") as f:
    f.write(code)

# config = conifer.backends.xilinxhls.auto_config()

# config['Precision'] = 'ap_fixed<32,18>'
# config['ClockPeriod'] = 5 # 5ns = 200MHz
# config['XilinxPart'] = 'xczu7ev-ffvc1156-2-e'
# config['ProjectName'] = 'my_prj'
# config['OutputDir'] = 'forest_cpp'
# config['Strategy'] = 'latency'

# # target zcu104 board and float interface (the conversion from float to ap_fixed is performed on the FPGA)
# accelerator_config = {'Board' : 'zcu104',
#                       'InterfaceType': 'float'}
# config['AcceleratorConfig'] = accelerator_config

# model_hls = conifer.converters.convert_from_sklearn(model, config)


# model_hls.write()
# # model_hls.compile()
# # model_hls.build()

# with open("x_test.log", "rb") as file:
#     X_test = pickle.load(file)

# with open('forest_cpp/tb_data/tb_input_features.dat', 'wb') as file:
#     np.savetxt(file, X_test, delimiter=' ')


# data = np.loadtxt("forest_cpp/tb_data/tb_input_features.dat", dtype=np.float32)

# print(data.shape)  # should be (6184, 50)

# with open("test_data.h", "w") as f:
#     f.write(f"#define N_VECTORS_DATA {data.shape[0]}\n")  # use shape[0] = 6184
#     f.write("static const float all_vectors[] = {\n")
#     for i, v in enumerate(data.flatten()):
#         f.write(f"    {v}f")
#         if i < len(data.flatten())-1: f.write(",")
#         if i % 8 == 7: f.write("\n")
#     f.write("\n};\n")

# with open("y_test.log", "rb") as file:
#     y_test = pickle.load(file)

# with open("result_data.h", "w") as file:
#     for y in y_test:
#         file.write(f"{y}\n")


# -----------------------------------------------------------------------------------

# report = model_hls.read_report()
# print(report)

# print(f"Latency (Cycles): {report['latency']}")
# print(f"Interval (Cycles): {report['interval']}") # Throughput measure
# print("Resource Usage:")
# print(f"  DSPs: {report['dsp']}")
# print(f"  LUTs: {report['lut']}")
# print(f"  FFs:  {report['ff']}")

# y = model_hls.decision_function(X_test)
# print(y[0])

# print(y)
# with open("result_data.h", 'wb') as file:
#     pickle.dump(y, file)

# synthesize the model, targeting bitfile creation and packaging
# warning: this may take some time!
# model_hls.build(bitfile=True, package=True)