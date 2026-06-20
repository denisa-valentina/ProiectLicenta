import pickle
import numpy as np

with open("y_test.pkl", "rb") as file:
    y_test = pickle.load(file)

with open("x_test.pkl", "rb") as file:
    x_test = pickle.load(file)

with open("result_data.dat", 'w') as file:
    for y in y_test:
        file.write(str(y))
        file.write('\n')


with open('my-hls-test/tb_data/tb_input_features.dat', 'wb') as file:
    np.savetxt(file, x_test, delimiter=' ')

data = np.loadtxt("my-hls-test/tb_data/tb_input_features.dat", dtype=np.float32)

with open("test_data.h", "w") as f:
    f.write(f"#define N_VECTORS_DATA {data.shape[0]}\n")  # use shape[0] = 6184
    f.write("static const float all_vectors[] = {\n")
    for i, v in enumerate(data.flatten()):
        f.write(f"    {v}f")
        if i < len(data.flatten())-1: f.write(",")
        if i % 100 == 99: f.write("\n")
    f.write("\n};\n")

# labels = y_test.values.astype(int)  # sau y_test.values.astype(int)
labels = np.array(y_test).astype(int)

with open("result_data.h", "w") as f:
    f.write(f"#define N_LABELS {labels.shape[0]}\n")
    f.write("static const int all_labels[] = {\n")
    for i, v in enumerate(labels):
        f.write(f"    {v}")
        if i < len(labels) - 1:
            f.write(",")
        f.write("\n")
    f.write("\n};\n")