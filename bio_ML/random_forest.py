import pandas
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import classification_report, confusion_matrix, accuracy_score
from sklearn.ensemble import GradientBoostingClassifier
from sklearn import svm





def random_forest_model():
	#reading in file
	df = pandas.read_csv("/Users/rithikrajani/Desktop/CSI_Lab/Fall 2020/Random Forest/AVPC-final.csv") #, dtype = {float}# )
	#testing read
	#print(df)
	#getting row count
	X = df.iloc[:, 16: 777].values
	#print(X)
	Y = df.loc[:,'AVPC-MS-80'].values
	#print(Y)

	X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.3, random_state=1)
	#X_test.fillna(X_test.mean()).values

	sc = StandardScaler()
	X_train = sc.fit_transform(X_train)
	X_test = sc.transform(X_test)

	classifier = RandomForestClassifier(n_estimators=1000, random_state=1)
	classifier.fit(X_train, Y_train)
	Y_pred = classifier.predict(X_test)

	print("TOTAL AVPC")
	print(confusion_matrix(Y_test,Y_pred))
	print(classification_report(Y_test,Y_pred))
	print(accuracy_score(Y_test, Y_pred))



	X = df.iloc[:, 16: 777].values
	#print(X)
	Y = df.loc[:,'AVPC-MS-80-TP'].values
	#print(Y)

	X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.3, random_state=1)
	#X_test.fillna(X_test.mean()).values

	sc = StandardScaler()
	X_train = sc.fit_transform(X_train)
	X_test = sc.transform(X_test)

	classifier = RandomForestClassifier(n_estimators=100, random_state=1)
	classifier.fit(X_train, Y_train)
	Y_pred = classifier.predict(X_test)

	print("TP53 TOTAL")
	print(confusion_matrix(Y_test,Y_pred))
	print(classification_report(Y_test,Y_pred))
	print(accuracy_score(Y_test, Y_pred))


	X = df.iloc[:, 16: 777].values
	#print(X)
	Y = df.loc[:,'AVPC-MS-80-PT'].values
	#print(Y)

	X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.3, random_state=1)
	#X_test.fillna(X_test.mean()).values

	sc = StandardScaler()
	X_train = sc.fit_transform(X_train)
	X_test = sc.transform(X_test)

	classifier = RandomForestClassifier(n_estimators=100, random_state=1)
	classifier.fit(X_train, Y_train)
	Y_pred = classifier.predict(X_test)

	print("PTEN TOTAL")
	print(confusion_matrix(Y_test,Y_pred))
	print(classification_report(Y_test,Y_pred))
	print(accuracy_score(Y_test, Y_pred))


	X = df.iloc[:, 16: 777].values
	#print(X)
	Y = df.loc[:,'AVPC-MS-80-RB'].values
	#print(Y)

	X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.3, random_state=1)
	#X_test.fillna(X_test.mean()).values

	sc = StandardScaler()
	X_train = sc.fit_transform(X_train)
	X_test = sc.transform(X_test)

	classifier = RandomForestClassifier(n_estimators=100, random_state=1)
	classifier.fit(X_train, Y_train)
	Y_pred = classifier.predict(X_test)

	print("RB1 TOTAL")
	print(confusion_matrix(Y_test,Y_pred))
	print(classification_report(Y_test,Y_pred))
	print(accuracy_score(Y_test, Y_pred))

####################################
	#
		



		#2977 - PTEN
		#3576 - RB1
		#4133 - TP53



def main():
	random_forest_model()

if __name__ == "__main__":
	main()