import pandas
import numpy as np
import matplotlib.pyplot as plt

def extract_AVPCMS():
	#reading in file
	df = pandas.read_csv("/Users/rithikrajani/Desktop/CSI_Lab/Fall 2020/AVPC-MS Extraction/cabazi_67_morphology_cnv.csv", dtype = {'CNV4133' : float})
	#testing read
	print(df)
	#getting row count
	x = len(df.count(axis = 'columns'))
	total_positive = 0
	total_negative = 0
	total_PT = 0
	total_RB = 0
	total_TP = 0

	#new_df_PT = df['CNV2977']


	print("total size: ", x)
	df['AVPC-MS-75'] = False
	df['AVPC-MS-75-PT'] = False
	df['AVPC-MS-75-RB'] = False
	df['AVPC-MS-75-TP'] = False
	df['AVPC-MS-80'] = False
	df['AVPC-MS-80-PT'] = False
	df['AVPC-MS-80-RB'] = False
	df['AVPC-MS-80-TP'] = False


	df.hist(column = 'CNV4133', bins = 250)
	#plt.show()
	count = 0
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

	df = new_df

	y = len(new_df.count(axis = 'columns'))
	

	for i in range(y):
		#print("CNV of PTEN: ", df['CNV2977'][i])
		#print("CNV of RB1: ", df['CNV3576'][i])
		#print("CNV of TP53: ", df['CNV4133'][i])

		PT = df['CNV2977'][i]
		RB = df['CNV3576'][i]
		TP = df['CNV4133'][i]

		ratio = 0.75
		if (PT < ratio):
			total_PT+= 1
			df['AVPC-MS-75-PT'][i] = True
		if (RB < ratio):
			total_RB+= 1
			df['AVPC-MS-75-RB'][i] = True
		if (TP < ratio):
			total_TP+=1
			df['AVPC-MS-75-TP'][i] = True
		if (PT < ratio and RB < ratio):
			total_positive+=1
			df['AVPC-MS-75'][i] = True
		elif (PT < ratio and TP < ratio):
			total_positive+=1
			df['AVPC-MS-75'][i] = True
		elif (RB < ratio and TP < ratio):
			total_positive+=1
			df['AVPC-MS-75'][i] = True
		else:
			total_negative+=1
	print ("Current threshold: ", ratio)
	print ("Total AVPC-MS: ", total_positive)
	#print ("Total AVPC-MS (negative) : ", total_negative)
	print ("AVPC-MS Positive Ratio : ", (total_positive/(total_positive + total_negative)))
	#print ("Negative Ratio : ", (total_negative/(total_positive + total_negative)))
	print ("PTEN Positive Ratio : ", (total_PT/(total_positive + total_negative)))
	print ("TP53 Positive Ratio : ", (total_TP/(total_positive + total_negative)))
	print ("RB1 Positive Ratio : ", (total_RB/(total_positive + total_negative)))

	total_positive = 0
	total_negative = 0
	total_PT = 0
	total_RB = 0
	total_TP = 0

	for i in range(x):
		#print("CNV of PTEN: ", df['CNV2977'][i])
		#print("CNV of RB1: ", df['CNV3576'][i])
		#print("CNV of TP53: ", df['CNV4133'][i])

		PT = df['CNV2977'][i]
		RB = df['CNV3576'][i]
		TP = df['CNV4133'][i]

		ratio = 0.80
		if (PT < ratio):
			total_PT+= 1
			df['AVPC-MS-80-PT'][i] = True
		if (RB < ratio):
			total_RB+= 1
			df['AVPC-MS-80-RB'][i] = True
		if (TP < ratio):
			total_TP+=1
			df['AVPC-MS-80-TP'][i] = True
		if (PT < ratio and RB < ratio):
			total_positive+=1
			df['AVPC-MS-80'][i] = True
		elif (PT < ratio and TP < ratio):
			total_positive+=1
			df['AVPC-MS-80'][i] = True
		elif (RB < ratio and TP < ratio):
			total_positive+=1
			df['AVPC-MS-80'][i] = True
		else:
			total_negative+=1
	print ("Current threshold: ", ratio)
	print ("Total AVPC-MS: ", total_positive)
	#print ("Total AVPC-MS (negative) : ", total_negative)
	print ("AVPC-MS Positive Ratio : ", (total_positive/(total_positive + total_negative)))
	#print ("Negative Ratio : ", (total_negative/(total_positive + total_negative)))
	print ("PTEN Positive Ratio : ", (total_PT/(total_positive + total_negative)))
	print ("TP53 Positive Ratio : ", (total_TP/(total_positive + total_negative)))
	print ("RB1 Positive Ratio : ", (total_RB/(total_positive + total_negative)))

	#count = 0
	

	new_df.to_csv("/Users/rithikrajani/Desktop/CSI_Lab/Fall 2020/AVPC-MS Extraction/AVPC-final.csv")
	total_difference = 0
	for i in range(x):
		MS_80 = df['AVPC-MS-80'][i]
		MS_75 = df['AVPC-MS-75'][i]
		#total_difference = 0
		if (MS_80 and not MS_75):
			total_difference+= 1
			cell_ID = df['unique_id'][i]
			print(cell_ID)
	print("Total Amount: ", total_difference)
		
		





		#print(df['CNV4972'][i])
		#print


		#2977 - PTEN
		#3576 - RB1
		#4133 - TP53



def main():
	extract_AVPCMS()

if __name__ == "__main__":
	main()