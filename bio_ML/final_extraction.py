import pandas
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import classification_report, confusion_matrix, accuracy_score
from sklearn.ensemble import GradientBoostingClassifier
from sklearn import svm





def other_tasks():
	#reading in file
	df = pandas.read_csv("/Users/rithikrajani/Desktop/CSI_Lab/Fall 2020/Random Forest/AVPC-total.csv") #, dtype = {float}# )
	#testing read
	print(df)
	x = len(df.count(axis = 'columns'))
	#getting row count
	#X = df.iloc[:, 16: 777].values
	#print(X)
	#Y = df.loc[:,'AVPC-MS-80'].values
	#print(Y)
	count = 0

	#df.drop(df.index[843])
	count+=1
	new_df = df
	for i in range(x):
		#print(i)
		quality = df['CNA'][i]
		quality = str(quality)
		#print((quality).type())
		if quality == 'low quality':
			count+=1
			print(df['unique_id'][i])
			new_df = new_df.drop(df.index[i])
	print(new_df['unique_id'])
	print(new_df['CNA'])
	print(count)
	new_df.to_csv("/Users/rithikrajani/Desktop/CSI_Lab/Fall 2020/Random Forest/AVPC-final.csv")










	
		
		



		#2977 - PTEN
		#3576 - RB1
		#4133 - TP53



def main():
	other_tasks()

if __name__ == "__main__":
	main()