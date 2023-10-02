import numpy as np

from sklearn.base import ClassifierMixin

class MostFrequentClassifier(ClassifierMixin):
    # Predicts the rounded (just in case) median of y_train
    def fit(self, X=None, y=None):
        '''
        Parameters
        ----------
        X : array like, shape = (n_samples, n_features)
        Training data features
        y : array like, shape = (_samples,)
        Training data targets
        '''
        # pass # YOUR CODE HERE
        if y is not None:
            unique_classes, counts = np.unique(y, return_counts=True)
            most_frequent_index = np.argmax(counts)
            self.most_frequent = unique_classes[most_frequent_index]
        return self
        
    def predict(self, X=None):
        '''
        Parameters
        ----------
        X : array like, shape = (n_samples, n_features)
        Data to predict
        '''
        # pass # YOUR CODE HERE
        return np.full(X.shape[0], self.most_frequent)