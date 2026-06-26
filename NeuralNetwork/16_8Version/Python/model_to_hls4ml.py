from functions import *
import hls4ml
import onnx
import hls4ml.model.profiling as profiling

with open("trained_model_nn.pkl", 'rb') as file:
    model = pickle.load(file)

print(model.model.summary())

model.export_model()

model_keras = keras.models.load_model("neural_network.keras")

# config = hls4ml.utils.config_from_keras_model(model_keras, granularity="model")

# config['Model']['Precision'] = 'ap_fixed<32, 15>'

# print(config)

# model_hls = hls4ml.converters.convert_from_keras_model(
#         model=model_keras,
#         hls_config=config,
#         part="xczu7ev-ffvc1156-2-e",
#         backend='Vitis',
#         clock_period=10)

# model_hls.write()
# model_hls.build()


config = hls4ml.utils.config_from_keras_model(model_keras, granularity="name")

config['Model']['Precision'] = 'ap_fixed<32,15>'   # neschimbat
config['Model']['Strategy'] = 'Resource'
config['LayerName']['dense']['ReuseFactor'] = 5
config['LayerName']['dense_1']['ReuseFactor'] = 4
config['LayerName']['dense_2']['ReuseFactor'] = 1

model_hls = hls4ml.converters.convert_from_keras_model(
        model=model_keras,
        hls_config=config,
        part="xczu7ev-ffvc1156-2-e",
        backend='Vitis',
        clock_period=10)

# profiling.numerical(model=model_keras, hls_model=model_hls)

model_hls.write()

