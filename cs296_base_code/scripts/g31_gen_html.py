#!/usr/bin/python3

import re
import math
Title="My doc"

def prin(x):
	if (type(x) is list):
		for ele in x:
			print("************************************************")
			prin(ele)
	else:
		print(x)


def remove_brace(mystr):
	if((mystr[0]=='{') and (mystr[-1:] == '}')):
		return mystr[1:-1]
	else:
		return mystr

def rem_nl(mystr):
	return re.sub(r'\n','',mystr)


def html_tag(mystr,tag):
	return "<"+tag+"> "+mystr+" </"+tag+">"+"\n"

def process_item(item):
	doc=""
	item=item.split("\n",1)
	intro=item[0]
	desc=rem_nl(item[1])
	desc=re.sub(r'\\begin\{description\}','',desc)
	desc=re.sub(r'\\end\{description\}','',desc)
	doc = doc + html_tag(remove_brace(intro), "li")
	doc = doc + html_tag(desc,"p")
	return doc


def process_subsec(subsec):
	doc=""
	if("\\item" not in subsec):
		doc=doc+subsec
		return doc
	else:
		subsec=re.sub(r'\\end\{enumerate\}*','',subsec)		#removes end enumerate tag of latex file
		subsec=re.sub(r'\\begin\{enumerate\}','',subsec)
		subsec=subsec.split("\\item")
		intro=rem_nl(subsec[0])
		doc= doc + html_tag(remove_brace(intro),"h3") +"<br>"
		items=subsec[1:]
		doc=doc + "<ol>\n"
		for item in items:
			doc=doc + process_item(item)
			#doc=doc + html_tag(item,"li")
		doc=doc+"</ol>\n"
		return doc

def process_sec(sec):
	doc=""
	if("\\subsection" in sec):
		sec=sec.split("\\subsection")
		secintro=sec[0]
		subsec=sec[1:]
		secintro=secintro.split("\n",1)
		secintro[1]=rem_nl(secintro[1])
		doc=doc + html_tag(remove_brace(secintro[0]),"h2")
		doc=doc + html_tag(secintro[1],"p")
		for subsecs in subsec:
			doc=doc+process_subsec(subsecs)
	else:
		sec=sec.split("\n",1)
		sec[1]=rem_nl(sec[1])
		sec[1]=re.sub(r'\n','',sec[1])
		doc=doc + html_tag(remove_brace(sec[0]),"h2")
		for p in sec[1:]:
			doc=doc + html_tag(p,"p")
	return doc

def add_images():
	doc=""
	for i in range(6):
		img='"../plots/g31_lab09_plot0'+str(i+1)+'.png"'
		doc = doc + '<img src='+img+ 'height="400pt" width="400pt" title="Plot' + str(i+1) +'">' + 'Plot'+str(i+1) + '</img>\n'
	return doc


def process_doc(l):
	doc="<html>\n<head>\n<title>"+Title+"</title>\n</head>\n<body>\n"
	for sec in l:
		doc=doc+process_sec(sec)

	#doc = doc + add_images()
	doc = doc + "</body>\n</html>"
	doc=re.sub(r'[{}]*','',doc)
	return doc



#****************************************MAIN*****************************************

FILE="./doc/report.tex"
imgdir="./plots/"

inp=open(FILE,'r')
data=inp.read()
inp.close()

data=re.sub(r'\\end\{document\}*','',data)		#removes end document tag of latex file
#data=re.sub(r'\\end\{enumerate\}*','',data)		#removes end enumerate tag of latex file
data = re.sub(r'\\begin\{center\}',"",data)
data = re.sub(r'\\end\{center\}',"",data)
#data =remove_brace(re.sub(".includeg[^\{]*","",data))
data =re.sub(r'~[^}]*[}]',"",data) #remove citation
#"<img src="+'"' + remove_brace(re.sub(".includeg[^\{]*","",data)) +'"' +">" + " </img>"
#print(data)
data=re.sub(r'\\bibliographystyle\{plain\}*','',data)
data=re.sub(r'\\bibliography\{./doc/referredto\}*','',data)
data=re.sub(r'\\\\','',data)					#removes '\\' from latex file

#hardcode the images

data=data.replace("\includegraphics[scale=0.3]{./doc/photo1.png}",'<img src="../doc/photo1.png" height="300" width="600"> </img> <br>')
data=data.replace("\includegraphics[scale=0.3]{./doc/idea.png}",'<img src="../doc/idea.png" height="300" width="600"> </img> <br>')
data=data.replace("\includegraphics[scale=0.5]{./plots/g31_lab09_plot01.png}",'<img src="../plots/g31_lab09_plot01.png" height="300" width="600"> </img> <br>')
data=data.replace("\includegraphics[scale=0.5]{./plots/g31_lab09_plot02.png}",'<img src="../plots/g31_lab09_plot02.png" height="300" width="600"> </img> <br>')
data=data.replace("\includegraphics[scale=0.5]{./plots/g31_lab09_plot03.png}",'<img src="../plots/g31_lab09_plot03.png" height="300" width="600"> </img> <br>')
data=data.replace("\includegraphics[scale=0.5]{./plots/g31_lab09_plot04.png}",'<img src="../plots/g31_lab09_plot04.png" height="300" width="600"> </img> <br>')
data=data.replace("\includegraphics[scale=0.5]{./plots/g31_lab09_plot05.png}",'<img src="../plots/g31_lab09_plot05.png" height="300" width="600"> </img> <br>')
data=data.replace("\includegraphics[scale=0.5]{./plots/g31_lab09_plot06.png}",'<img src="../plots/g31_lab09_plot06.png" height="300" width="600"> </img> <br>')
data=data.replace("\includegraphics[scale=0.1]{./doc/deb_output200.png}",'<img src="../doc/deb_output200.png" height="300" width="600"> </img> <br>')
data=data.replace("\includegraphics[scale=0.1]{./doc/rel_output200.png}",'<img src="../doc/deb_output200.png" height="300" width="600"> </img> <br>')
data=data.split("\\section")
data.pop(0)
d_size=len(data)
output=""

s=process_doc(data)
print(s)
