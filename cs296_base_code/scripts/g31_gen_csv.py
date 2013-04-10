#!/usr/bin/python3

import os
import subprocess as SP
import re as regex

#************************************VARIABLES***************************************
out_file="lab09_g31_data.csv"
tmp_file="tmp.txt"
Iterations=100
Reruns=100
binpath="./bin/cs296_base "

#inp=open(out_file,'w+')



def gen_csv():
	os.system("rm -f "+out_file)
	outinp=open(out_file,'w')
	os.system("touch "+tmp_file)
	for r in range(Reruns):
		for i in range(Iterations):
			os.system(binpath + str(i+1) +" > " +tmp_file)
			tmpinp=open(tmp_file)
			s=tmpinp.read()
			s=regex.sub(r'[^0-9]*([0-9][0-9.]*).*', r',\1',s)
			s=str(r+1)+s
			tmpinp.close()
			outinp.write(s)
	os.system("rm -f "+tmp_file)
	outinp.close()


#******************************************MAIN****************************************
gen_csv()
