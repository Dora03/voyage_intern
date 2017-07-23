#!/usr/bin/env python
# -*- coding: utf-8 -*-
import re
import numpy as np
import matplotlib.pyplot as plt

def create_dictionary(path):
	# path='negpos.txt' #dictionary data
	dictionary={}
	fdata = open(path,'r') #fopen
	l = fdata.readline()
	while l != '': #one line read
		line_arr=l.split(':')
		dictionary[line_arr[0]] = line_arr[2]
		l = fdata.readline()
	fdata.close()
	return dictionary

def document_negpos(document,path): # 1 paragraph
	# path = 'negpos.txt'
	dictionary = create_dictionary(path)
	i_negpos = 0
	count = 0
	negpos = 0
	words = document.split(' ')
	for word in words:
		for corpus in dictionary:
			if corpus==word:
				dummy1 = re.match('^[-]]?',dictionary[word]) #+or-?
				dummy2 = re.search('\d+(\.\d+)?',dictionary[word]) #Absolute value
				if dummy1 is None: #if +
					i_negpos += float(dummy2.group())
				else: #if -
					i_negpos -= float(dummy2.group())
				count += 1
	if count!=0: #denominator cannot be 0
		negpos = i_negpos / count
	return negpos

np_path = 'negpos.txt'
f = open('Anne_of_Green_Gables_utf8.txt','r')
fo = open('out_Anne_of_Green_Gables_utf8.txt','w')
l = f.readline()
i = 0
plot_ngps = []
while l != '':
	negpos = document_negpos(l,np_path)
	if l!='\n':
		print(negpos)
		plot_ngps.append(negpos)
		fo.write(str(negpos)+' ')
		i += 1
	l = f.readline()
x = np.arange(0,i,1)
plt.plot(x, plot_ngps)
plt.ylim(-1, 1)
plt.title('Anne of Green Gables')
filename = "output_Anne_of_Green_Gables_utf8.png"
plt.savefig(filename)
f.close
fo.close