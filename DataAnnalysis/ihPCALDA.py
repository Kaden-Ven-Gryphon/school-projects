import scipy.io
import matplotlib.pyplot as plt  # type: ignore
import numpy as np  # type: ignore
import pandas as pd         # type: ignore
from sklearn.datasets import load_iris  # type: ignore
from sklearn.decomposition import PCA   # type: ignore
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis    # type: ignore


indianPinesMAT = scipy.io.loadmat('indianR.mat')

X = np.transpose(indianPinesMAT['X'])
y = indianPinesMAT['gth'][0]
target_names = ['type 0', 'type 1', 'type 2', 'type 3', 'type 4', 'type 5', 'type 6', 'type 7', 'type 8', 'type 9', 'type 10', 'type 11', 'type 12', 'type 13', 'type 14', 'type 15']

pca = PCA(n_components=4)
X_pca = pca.fit(X).transform(X)

ev = pca.explained_variance_ratio_

print("Explained variance: %s" % str(ev*100))

plt.figure()
pc=[]
for i in range(4):
    pc.append('PC' + str(i+1))
plt.bar(pc, ev*100)
plt.title('Variance Ratio of Indian Pines PCs')
plt.ylabel('Variance Ratio')

pca = PCA(n_components=2)
X_pca = pca.fit(X).transform(X)
lda = LinearDiscriminantAnalysis(n_components=2)
X_lda = lda.fit(X, y).transform(X)


plt.figure()
colors = ["tab:blue", "tab:orange", "tab:green", "tab:red", "tab:purple", "tab:brown", "tab:pink", "tab:gray", "tab:olive", "tab:cyan", "k", "gray", "r", "teal", "navy", "gold"]
lw = 2

for color, i, target_name in zip(colors, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], target_names):
    plt.scatter(
        X_pca[y == i, 0], X_pca[y == i, 1], color=color, alpha=0.8, lw=lw, label=target_name
    )
plt.legend(loc="best", shadow=False, scatterpoints=1)
plt.title("PCA of Indian Pines dataset")

plt.figure()
for color, i, target_name in zip(colors, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], target_names):
    plt.scatter(
        X_lda[y == i, 0], X_lda[y == i, 1], alpha=0.8, color=color, label=target_name
    )
plt.legend(loc="best", shadow=False, scatterpoints=1)
plt.title("LDA of Indian Pines dataset")

plt.show()
