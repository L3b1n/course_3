# Лабораторная работа 4.Контрольная работа. Классификация рукописных символов Extended MNIST
Вам нужно обучить классификатор 62 видов рукописных символов на базе EMNIST. Классы в наборе данных не сбалансированы. Можно использовать любые фреймворки и архитектуры сетей, но внешние данные использовать нельзя. Формат выполнения задания -- конкурс на платформе Kaggle. Инвайт на конкурс на Kaggle: https://www.kaggle.com/t/68d90abce3934511ac255def7075027a (при регистрации указывайте имя и фамилию)

Оценивание будет производиться на основе набранного качества классификации на private выборке, а также занятого студентом места (если в топ-5). В день доступно 5 попыток. Для тестирования на private выборке необходимо выбрать одну посылку (по умолчанию берется посылка с лучшим public-результатом).

Исходный код для обучения моделей надо прикрепить на EduFPMI (будет сверяться в случае подозрения на списывание)!

Успехов!

__Tutorials и полезные материалы:__
 * Основы Keras Functional API: https://www.tensorflow.org/guide/keras/functional (Обратите внимание на на возможность Run in Colab)
 * Callback-и в Keras: https://keras.io/callbacks/
 * Пример обучения логистической регрессии с демонстрацией: https://www.kaggle.com/code/mirzarahim/kannada-mnist-digits-model-interpret-visualize
 * Пример обучения сетей: https://www.kaggle.com/code/sauravjoshi23/kannada-mnist-comparing-accuracy-of-various-models
 * MNIST State-of-the-art: https://paperswithcode.com/sota/image-c lassification-on-mnist

## Objective
In this homework, we will train a model for classifying handwritten letters and digits in Extended-MNIST (EMNIST) dataset.

The EMNIST include 52 letters (capital and small letters) and 10 digits. We will use the default dataset (emnist-byclass). The By_Class split includes 62 classes comprising [0-9], [a-z] and [A-Z], while the By_Merge split has merged the uppercase and lowercase letters of C, I, J, K, L, M, O, P, S, U, V, W, X, Y and Z, and include only 47 classes.

You are free to use any neural network architecture, any DL library (Keras, PyTorch, TensorFlow, mxnet, CNTK, …). __Usage of external data or pre-trained networks is prohibited!__

Please use your real name and surname as a team name for this competition.

## Input Data
We've converted the EMNIST binary data to NumPy npz format. The code below shows how to load training data:  
```python
import numpy as np
# Load training data
data = np.load('emnist-byclass-train.npz')
train_labels = data['training_labels']
train_data = data['training_images']

# Load testing data
test_data = np.load('emnist-byclass-test.npz')['testing_images']
```

## Output Format
A submission file of prediction results should be a CSV file containing two columns: Id and Category, separated by ','.

The file should contain a header and have the following format:  
Id,Category  
0,15  
1,7  
2,30  
3,22  
...

We will use categorical accuracy as the metric. There are 62 classes (0 ~ 9 & A ~ Z & a ~z) in ByClass split of EMNIST dataset, which are labeled sequentially as 0, 1, 2, …, 61.

## Kaggle Notebook Example
You can refer to the notebook to see how to train a model on Kaggle: 
https://www.kaggle.com/kuantinglai/109-1-ntut-dl-app-hw1-demo

## References
[EMNIST Official Website](https://www.nist.gov/itl/products-and-services/emnist-dataset)
[EMNIST README](https://www.itl.nist.gov/iaui/vip/cs_links/EMNIST/Readme.txt)
[Paper: EMNIST: an extension of MNIST to handwritten letters](https://arxiv.org/abs/1702.05373v1)

## Evaluation
A submission file of prediction results should be a CSV file containing two columns: Id and Category, separated by ','.

The file should contain a header and have the following format:  
Id,Category  
0,15  
1,7  
2,30  
3,22  
...

We will use categorical accuracy as the metric. There are 62 classes (0 ~ 9 & A ~ Z & a ~z) in ByClass split of EMNIST dataset, which are labeled sequentially as 0, 1, 2, …, 61.