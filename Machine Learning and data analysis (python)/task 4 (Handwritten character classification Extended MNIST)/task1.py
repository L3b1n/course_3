# accuracy 0.85923

import numpy as np
import pandas as pd
from keras.models import Sequential
from keras.layers import Conv2D, MaxPooling2D, Flatten, Dense
from keras.utils import to_categorical

# Load training data
data = np.load('data/emnist-byclass-train.npz')
train_labels = data['training_labels']
train_data = data['training_images']

# Load testing data
test_data = np.load('data/emnist-byclass-test.npz')['testing_images']

# Preprocess data
train_data = train_data.reshape(-1, 28, 28, 1)
test_data = test_data.reshape(-1, 28, 28, 1)

# Normalize pixel values
train_data = train_data.astype('float32') / 255
test_data = test_data.astype('float32') / 255

# Convert labels to one-hot encoding
train_labels = to_categorical(train_labels, num_classes=62)

# Create a neural network model
model = Sequential()
model.add(Conv2D(32, kernel_size=(3, 3), activation='relu', input_shape=(28, 28, 1)))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Flatten())
model.add(Dense(128, activation='relu'))
model.add(Dense(62, activation='softmax'))

# Compile the model
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])

# Train the model
model.fit(train_data, train_labels, epochs=10, batch_size=64)

# Make predictions on the test data
predictions = model.predict(test_data)
predicted_labels = np.argmax(predictions, axis=1)

# Create a DataFrame for the submission
submission_df = pd.DataFrame({'Id': range(len(test_data)), 'Category': predicted_labels})

# Save the submission file
submission_df.to_csv('data/submission.csv', index=False)