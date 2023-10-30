# accuracy 0.87442

import numpy as np
import keras.applications as applications
from keras.models import Model
from keras.layers import Input, Flatten, Dense
from keras.optimizers import Adam
from keras.utils import to_categorical 

TRAIN_DATA_FILE = 'data/emnist-byclass-train.npz'
TEST_DATA_FILE = 'data/emnist-byclass-test.npz'

# Load training data
data = np.load(TRAIN_DATA_FILE)
train_labels = data['training_labels']
train_images = data['training_images']

# Normalize the data
train_images = train_images.astype('float32') / 255

# One-hot encode the labels
trn_labels = to_categorical(train_labels, num_classes=62)

# Reshape data for ResNet-18
trn_images = train_images.reshape((-1, 28, 28, 1))

# Create a ResNet-18 model
base_model = applications.ResNet50V2(include_top=False, weights=None, input_tensor=Input(shape=(28, 28, 1)))
x = Flatten()(base_model.output)
output_layer = Dense(62, activation='softmax')(x)
model = Model(inputs=base_model.input, outputs=output_layer)

# Compile the model
model.compile(optimizer=Adam(learning_rate=0.001), loss='categorical_crossentropy', metrics=['accuracy'])

# Train the model
history = model.fit(trn_images, trn_labels, epochs=15, batch_size=128, validation_split=0.2)

# Load testing data
test_images = np.load(TEST_DATA_FILE)['testing_images']
tst_images = test_images.astype('float32') / 255
tst_images = tst_images.reshape((-1, 28, 28, 1))

# Make predictions
tst_probabilities = model.predict(tst_images)
tst_categories = tst_probabilities.argmax(axis=1)

# Print results in CSV format
with open('data/pred_results.csv', 'w') as f:
    f.write('Id,Category\n')
    for i, category in enumerate(tst_categories):
        print(f"{i},{category}", file=f)