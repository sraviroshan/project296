ITR=100
RERUN=100
I=1
J=1
FILE=lab05_g31_data.csv
touch $FILE
while [ "$I" -le "$RERUN" ]
do
	J=1
	while [ "$J" -le "$ITR" ]
	do
		printf "$I" >>$FILE
		bin/cs296_base "$J" | sed 's/[^0-9]*\([0-9][0-9.]*\).*/,\1/g' | sed ':a;N;$!ba;s/\n//g' >>$FILE
		J=$(($J + 1))
	done
	I=$(($I + 1))
	
done
mv $FILE data/
exit
