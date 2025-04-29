import matplotlib.pyplot as plt    # type: ignore
import numpy as np  # type: ignore
from sklearn import datasets    # type: ignore
from matplotlib import pyplot   # type: ignore
from sklearn.model_selection import train_test_split    # type: ignore
from sklearn.model_selection import cross_val_score # type: ignore
from sklearn.model_selection import StratifiedKFold # type: ignore
from sklearn.neighbors import KNeighborsClassifier  # type: ignore
from sklearn.naive_bayes import GaussianNB    # type: ignore
from sklearn.svm import SVC # type: ignore
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import AdaBoostClassifier
from sklearn.model_selection import learning_curve
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import MinMaxScaler
import scipy.io

def plot_learning_curve(classifier, X, y, steps=10, train_sizes=np.linspace(0.1, 1.0,10), label="", color = "r", axes=None):
    estimator=Pipeline([("scaler", MinMaxScaler()), ("classifier", classifier)])
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 0.3, random_state=101)
    train_scores = []
    test_scores = []
    train_sizes = []
    for i in range(0, X_train.shape[0], X_train.shape[0] // steps):
        if (i == 0):
            continue
        X_train_i = X_train[0:i, :]
        y_train_i = y_train[0:i]
        estimator.fit(X_train_i, y_train_i)
        train_scores.append(estimator.score(X_train_i, y_train_i) * 100)
        test_scores.append(estimator.score(X_test, y_test) * 100)
        train_sizes.append(i + 1)
    if (X_train.shape[0] % steps != 0):
        estimator.fit(X_train, y_train)
        train_scores.append(estimator.score(X_train, y_train) * 100)
        test_scores.append(estimator.score(X_test, y_test) * 100)
        train_sizes.append(X_train.shape[0])

    if axes is None:
        _, axes = plt.subplot(2)

    train_s=np.linspace(10, 100, num=5)
    axes[0].plot(train_sizes, test_scores, "o-", color=color, label=label)
    axes[1].plot(train_sizes, train_scores, "o-", color=color, label=label)

    print("Training Accuracy of ", label, ": ", train_scores[-1],"%")
    print("Testing Accuracy of ", label, ": ", test_scores[-1],"%")
    print("")
    return plt


indianPinesMAT = scipy.io.loadmat('indianR.mat')

X = np.transpose(indianPinesMAT['X'])
y = indianPinesMAT['gth'][0]
target_names = ['type 0', 'type 1', 'type 2', 'type 3', 'type 4', 'type 5', 'type 6', 'type 7', 'type 8', 'type 9', 'type 10', 'type 11', 'type 12', 'type 13', 'type 14', 'type 15']


X_train, X_validation, y_train, y_validation = train_test_split(X, y, test_size = 0.2, random_state=1, shuffle=True)

models = []
models.append(('KNN', KNeighborsClassifier()))
models.append(('NB', GaussianNB()))
models.append(('SVM', SVC(gamma='auto')))

results = []
names = []

for name, model in models:
    kfold = StratifiedKFold(n_splits=10, random_state=1, shuffle=True)
    cv_results = cross_val_score(model, X_train, y_train, cv=kfold, scoring='accuracy')
    results.append(cv_results)
    names.append(name)
    print('%s: %f %% (%f)' % (name, cv_results.mean()*100, cv_results.std()))

pyplot.boxplot(results, labels=names)
pyplot.title('Algorithm Comparison')
pyplot.show()


_, axes = plt.subplots(1, 2, figsize=(12, 5))
num_steps = 10
classifier_labels = {"SVM - RBF": (SVC(kernel="rbf", random_state=1), "blue"),
                    "SVM - Poly": (SVC(kernel="poly", random_state=1), "orange"),
                    "kNN": (KNeighborsClassifier(n_neighbors=5), "purple"),
                    "Gaussian Naive Bayes": (GaussianNB(), "lime"),}

for label in classifier_labels:
    classifier = classifier_labels[label][0]
    color = classifier_labels[label][1]
    plot_learning_curve(classifier, X, y, steps=num_steps, label=label, color=color, axes=axes)

axes[0].set_xlabel("% of Training Examples")
axes[0].set_ylabel("Overall Classification Accuracy")
axes[0].set_title("Model Evaluation - Cross-validation Accuracy")
axes[0].legend()

axes[1].set_xlabel("% of Training Examples")
axes[1].set_ylabel("Training/Recall Accuracy")
axes[1].set_title("Model Evaluation - Training Accuracy")
axes[1].legend()

plt.show()
