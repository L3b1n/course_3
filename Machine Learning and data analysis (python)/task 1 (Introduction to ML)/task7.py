import pandas as pd
from sklearn.base import RegressorMixin

class CityMeanRegressor(RegressorMixin):
    def fit(self, X=None, y=None):
        # pass # YOUR CODE HERE
        if X is not None and y is not None:
            y = pd.DataFrame(y, columns=['average_bill'])
            data = pd.concat([X, y], axis=1)
            self.city_means = data.groupby('city')['average_bill'].mean()
        return self
        
    def predict(self, X=None):
        # pass # YOUR CODE HERE
        if X is not None:
            return X['city'].map(self.city_means).fillna(500)
        else:
            return None