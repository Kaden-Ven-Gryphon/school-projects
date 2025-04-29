import matplotlib.pyplot as plt  # type: ignore
import numpy as np  # type: ignore
import pandas as pd         # type: ignore
from sklearn.datasets import load_iris  # type: ignore
from sklearn.decomposition import PCA   # type: ignore
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis    # type: ignore

iris = load_iris()

X = iris.data
y = iris.target
target_names = iris.target_names

pca = PCA(n_components=4)
X_pca = pca.fit(X).transform(X)

ev = pca.explained_variance_ratio_

print("Explained variance: %s" % str(ev*100))

plt.figure()
pc=[]
for i in range(4):
    pc.append('PC' + str(i+1))
plt.bar(pc, ev*100)
plt.title('Variance Ratio of Iris PCs')
plt.ylabel('Variance Ratio')
#plt.show()

pca = PCA(n_components=2)
X_pca = pca.fit(X).transform(X)
lda = LinearDiscriminantAnalysis(n_components=2)
X_lda = lda.fit(X, y).transform(X)


plt.figure()
colors = ["navy", "turquoise", "darkorange"]
lw = 2

for color, i, target_name in zip(colors, [0, 1, 2], target_names):
    plt.scatter(
        X_pca[y == i, 0], X_pca[y == i, 1], color=color, alpha=0.8, lw=lw, label=target_name
    )
plt.legend(loc="best", shadow=False, scatterpoints=1)
plt.title("PCA of IRIS dataset")

plt.figure()
for color, i, target_name in zip(colors, [0, 1, 2], target_names):
    plt.scatter(
        X_lda[y == i, 0], X_lda[y == i, 1], alpha=0.8, color=color, label=target_name
    )
plt.legend(loc="best", shadow=False, scatterpoints=1)
plt.title("LDA of IRIS dataset")

plt.show()
