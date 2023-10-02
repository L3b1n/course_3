import numpy as np
import pandas as pd
from sklearn.base import ClassifierMixin

class RubricCityMedianClassifier(ClassifierMixin):
    def fit(self, X=None, y=None):
        if X is not None and y is not None:
            data = X.groupby(['modified_rubrics', 'city'])['average_bill'].median().reset_index()
            self.city_rubric_medians = {(row['modified_rubrics'], row['city']): row['average_bill'] for _, row in data.iterrows()}
        return self
    
    def predict(self, X=None):
        if X is not None:
            predictions = []
            for _, row in X.iterrows():
                modified_rubrics = row['modified_rubrics']
                city = row['city']
                median_value = self.city_rubric_medians.get((modified_rubrics, city), np.nan)
                predictions.append(median_value)
            return np.array(predictions)
        else:
            return None