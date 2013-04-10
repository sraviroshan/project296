#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as Plot
import csv
import math

FILE="data/lab09_g31_data.csv"
Iterations=100
Reruns=100
xarray=np.linspace( 1, 100, 100 )	#generates array of integers 1 to 100

#####################*****************PLOT 1  &&  PLOT 2*******************############################
Avg_S=np.zeros(Iterations)
Avg_C=np.zeros(Iterations)
Avg_V=np.zeros(Iterations)
Avg_P=np.zeros(Iterations)
Avg_L=np.zeros(Iterations)

with open(FILE, 'r') as csvfile:
	rowreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in rowreader:
		Avg_S[int(row[1]) - 1]+=float(row[2])
		Avg_C[int(row[1]) - 1]+=float(row[3])
		Avg_V[int(row[1]) - 1]+=float(row[4])
		Avg_P[int(row[1]) - 1]+=float(row[5])
		Avg_L[int(row[1]) - 1]+=float(row[6])

for i in range(Reruns):
	Avg_S[i]=Avg_S[i]/Reruns
	Avg_C[i]=Avg_C[i]/Reruns
	Avg_V[i]=Avg_V[i]/Reruns
	Avg_P[i]=Avg_P[i]/Reruns
	Avg_L[i]=Avg_L[i]/Reruns

csvfile.close()

Plot.figure(0)
Plot.plot(xarray,Avg_S, color="green", linewidth=1.1, linestyle="-", label="Step time")
Plot.plot(xarray,Avg_L, color="red",  linewidth=1.1, linestyle="-", label="Loop time")
Plot.legend(loc=7,title="Time",shadow=True)
Plot.xlabel("Iteration value")
Plot.ylabel("Time Averaged Over Reruns in ms")
Plot.title("Plot showing the Time Averaged Over Reruns vs. Iteration Values")
Plot.savefig("./plots/g31_lab09_plot01.png")
Plot.show()

Plot.figure(1)
Plot.plot(xarray,Avg_S, color="green", linewidth=1.1, linestyle="-", label="Step time")
Plot.plot(xarray,Avg_C, color="red",  linewidth=1.1, linestyle="-", label="Loop time")
Plot.plot(xarray,Avg_V, color="blue",  linewidth=1.1, linestyle="-", label="Velocity time")
Plot.plot(xarray,Avg_P, color="yellow",  linewidth=1.1, linestyle="-", label="Position time")
Plot.legend(loc=1,title="Time",shadow=True)
Plot.xlabel("Iteration value")
Plot.ylabel("Time Averaged Over Reruns in ms")
Plot.title("Plot showing the Time Averaged Over Reruns vs. Iteration Values")
Plot.savefig("./plots/g31_lab09_plot02.png")
Plot.show()

#######################****************PLOT 3  &&  PLOT 4******************############################
Tot_S=0
Tot_C=0
Tot_V=0
Tot_P=0
Tot_L=0

Avg_S=np.zeros(Reruns)
Avg_C=np.zeros(Reruns)
Avg_V=np.zeros(Reruns)
Avg_P=np.zeros(Reruns)
Avg_L=np.zeros(Reruns)

with open(FILE, 'r') as csvfile:
	rowreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in rowreader:
		Tot_S+=float(row[2])
		Tot_C+=float(row[3])
		Tot_V+=float(row[4])
		Tot_P+=float(row[5])
		Tot_L+=float(row[6])
		if (int(row[1]) == Iterations):
			Avg_S[int(row[0]) - 1]=Tot_S/Iterations
			Avg_C[int(row[0]) - 1]=Tot_C/Iterations
			Avg_V[int(row[0]) - 1]=Tot_V/Iterations
			Avg_P[int(row[0]) - 1]=Tot_P/Iterations
			Avg_L[int(row[0]) - 1]=Tot_L/Iterations
			Tot_S=0
			Tot_C=0
			Tot_V=0
			Tot_P=0
			Tot_L=0

csvfile.close()

Plot.figure(2)
Plot.plot(xarray,Avg_S, color="green", linewidth=1.1, linestyle="-", label="Step time")
Plot.plot(xarray,Avg_L, color="red",  linewidth=1.1, linestyle="-", label="Loop time")
Plot.legend(loc=7,title="Time",shadow=True)
Plot.xlabel("Rerun value")
Plot.ylabel("Time Averaged Over Iterations in ms")
Plot.title("Plot showing the Time Averaged Over Iterations vs. Rerun Values")
Plot.savefig("./plots/g31_lab09_plot03.png")
Plot.show()

Plot.figure(3)
Plot.plot(xarray,Avg_S, color="green", linewidth=1.1, linestyle="-", label="Step time")
Plot.plot(xarray,Avg_C, color="red",  linewidth=1.1, linestyle="-", label="Loop time")
Plot.plot(xarray,Avg_V, color="blue",  linewidth=1.1, linestyle="-", label="Velocity time")
Plot.plot(xarray,Avg_P, color="yellow",  linewidth=1.1, linestyle="-", label="Position time")
Plot.legend(loc=7,title="Time",shadow=True)
Plot.xlabel("Rerun value")
Plot.ylabel("Time Averaged Over Iterations in ms")
Plot.title("Plot showing the Time Averaged Over Iterations vs. Rerun Values")
Plot.savefig("./plots/g31_lab09_plot04.png")
Plot.show()

#######################****************PLOT 5 ******************############################
Step=np.zeros(Iterations)
Step_sq=np.zeros(Iterations)
Step_dev=np.zeros(Iterations)

with open(FILE, 'r') as csvfile:
	rowreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in rowreader:
		s=float(row[2])
		Step[int(row[1])-1]+=s
		Step_sq[int(row[1])-1]+=s*s

csvfile.close()

for i in range(Iterations):
	Step_dev[i]=math.sqrt(((Step_sq[i])-(Step[i]*Step[i]/Iterations))/(Iterations-1))
	Step[i]/=Iterations

Plot.figure(4)
Plot.errorbar(xarray,Step, yerr= Step_dev, color="red",ecolor="blue", linewidth=1.1, linestyle="-", label="Step time")
Plot.legend(loc=1,title="Time with Error",shadow=True)
Plot.xlabel("Iteration value")
Plot.ylabel("Step-Time")
Plot.title("Step-Time with Error-Bars vs Iterations")
Plot.savefig('./plots/g31_lab09_plot05.png')
Plot.show()

#######################****************PLOT 6 ******************############################
Step=np.zeros(Reruns)
Roll=35
with open(FILE, 'r') as csvfile:
	rowreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in rowreader:
		if(int(row[1])==Roll):
			Step[int(row[0])-1]=float(row[2])

csvfile.close()

num_bins = 20
counts, bin_edges = np.histogram(Step, bins=num_bins, normed=False)
cdf = np.cumsum(counts)

Plot.figure(5)
Plot.hist(Step,bins=num_bins,cumulative=False,color="red",rwidth=0.9,label="Step Time")
Plot.plot(bin_edges[1:], cdf,color="blue",linewidth=1.1,linestyle="-",label="Cumulative")
Plot.legend(loc=7,title="Histogram",shadow=True)
Plot.xlabel("Variaton in Step-Time")
Plot.ylabel("No. of Observations")
Plot.title("Variation Frequency Plot of Step-Time for Iteration Value = 35")
Plot.savefig('./plots/g31_lab09_plot06.png')
Plot.show()