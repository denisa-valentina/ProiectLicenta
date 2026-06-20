from functions import *
import tensorflow

# Load JSON files
file1_path = 'jailbreak_prompts.json'
file2_path = 'regular_prompts.json'

with open(file1_path, 'r', encoding='utf-8') as file1, open(file2_path, 'r', encoding='utf-8') as file2:
    data1 = json.load(file1)
    data2 = json.load(file2)

# Extract prompts from both JSON files
prompts1 = [item['prompt'] for item in data1 if 'prompt' in item]
prompts2 = [item['prompt'] for item in data2 if 'prompt' in item]

# Filter English prompts
DetectorFactory.seed = 0

def is_english(prompt):
    try:
        return detect(prompt) == 'en'
    except:
        return False

english_prompts1 = [prompt for prompt in prompts1 if is_english(prompt)]
english_prompts2 = [prompt for prompt in prompts2 if is_english(prompt)]

# Create dictionaries with labels
jb_prompts_dict = [{"prompt": prompt, "label": "jb"} for prompt in english_prompts1[:1285]]
reg_prompts_dict = [{"prompt": prompt, "label": "reg"} for prompt in english_prompts2[:1285]]

# print(len(jb_prompts_dict))
# print(len(reg_prompts_dict))

eval_jb_prompts_dict = [{"prompt": prompt, "label": "jb"} for prompt in english_prompts1[1285:]]
eval_reg_prompts_dict = [{"prompt": prompt, "label": "reg"} for prompt in english_prompts2[1285:1385]]

# print(len(eval_jb_prompts_dict))
# print(len(eval_reg_prompts_dict))

# Combine all prompts
english_prompts = jb_prompts_dict + reg_prompts_dict

# Extract prompts and labels
prompts = [entry['prompt'] for entry in english_prompts]
labels = [entry['label'] for entry in english_prompts]

df = pd.DataFrame({
    'prompt': prompts,
    'label': labels
})

df['vectorized prompt']=df['prompt'].apply(lambda x: gensim.utils.simple_preprocess(x))

df['nltk vectorized']=df['prompt'].apply(preprocess)

# Encoding the label column
df['label']=df['label'].map({'jb':1,'reg':0})
# Split data into train and test sets
X_train, X_test, y_train, y_test = train_test_split (df['nltk vectorized'], df['label'] , test_size=0.2, random_state=1)

# Train the word2vec model
w2v_model = gensim.models.Word2Vec(X_train,
                                   vector_size=100,
                                   window=5,
                                   min_count=2)

words = set(w2v_model.wv.index_to_key )
X_train_vect = np.array(
    [np.array([w2v_model.wv[i] for i in ls if i in words]) for ls in X_train],
    dtype=object
)
X_test_vect = np.array([np.array([w2v_model.wv[i] for i in ls if i in words]) for ls in X_test], dtype=object)

# Compute sentence vectors by averaging the word vectors for the words contained in the sentence
X_train_vect_avg = []
for v in X_train_vect:
    if v.size:
        X_train_vect_avg.append(v.mean(axis=0))
    else:
        X_train_vect_avg.append(np.zeros(100, dtype=float))

X_test_vect_avg = []
for v in X_test_vect:
    if v.size:
        X_test_vect_avg.append(v.mean(axis=0))
    else:
        X_test_vect_avg.append(np.zeros(100, dtype=float))

x_train=X_train_vect_avg
x_test=X_test_vect_avg
y_train=y_train
y_test=y_test

x_train = np.array(X_train_vect_avg)
x_test = np.array(X_test_vect_avg)
y_train = np.array(y_train)
y_test = np.array(y_test)

# Assuming x_train and y_train are already prepared
pytorch_estimator = SklearnPyTorchWrapper(input_dim=x_train.shape[1], epochs=10, patience=3)

# Train the model
pytorch_estimator.fit(x_train, y_train)

# Make predictions
y_pred_pytorch = pytorch_estimator.predict(x_test)

# Evaluate the model
pytorch_accuracy = pytorch_estimator.score(x_test, y_test)
pytorch_precision = precision_score(y_test, y_pred_pytorch, pos_label=1, zero_division=0)
pytorch_recall = recall_score(y_test, y_pred_pytorch, pos_label=1, zero_division=0)
pytorch_f1 = f1_score(y_test, y_pred_pytorch, pos_label=1, zero_division=0)

# Train set predictions  
y_train_pred = pytorch_estimator.predict(x_train)

# Train metrics
train_accuracy = pytorch_estimator.score(x_train, y_train)
train_precision = precision_score(y_train, y_train_pred)
train_recall = recall_score(y_train, y_train_pred)
train_f1 = f1_score(y_train, y_train_pred)


print(f"PyTorch Classifier - Accuracy (Test): {pytorch_accuracy:.4f}, "
      f"Precision: {pytorch_precision:.4f}, Recall: {pytorch_recall:.4f}, F1: {pytorch_f1:.4f}")

print(f"PyTorch Classifier - Accuracy (Train): {train_accuracy:.4f}, "
      f"Precision: {train_precision:.4f}, Recall: {train_recall:.4f}, F1: {train_f1:.4f}")

with open("trained_model_nn.pkl", 'wb') as file:
    pickle.dump(pytorch_estimator, file)

with open("x_test.pkl", "wb") as file:
    pickle.dump(x_test, file)

with open("x_train.pkl", "wb") as file:
    pickle.dump(x_train, file)

with open("y_test.pkl", "wb") as file:
    pickle.dump(y_test, file)

with open("y_train.pkl", "wb") as file:
    pickle.dump(y_train, file)
