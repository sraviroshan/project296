#frequency distribution corresponding to iteration value of rollno 05

{
	rollno=35
	if($2==rollno){
		stepsum=stepsum+$3;
		steps[$1]=$3;  #$1 range from 0 to maxrerun
	}
	if($1>maxrerun)  #(maxrerun+1) is no of observations corr to iteration value = rollno 
		maxrerun=$1
}
END{
	avg=stepsum/(maxrerun+1)
	for (x = 0; x <maxrerun+1 ; x++){
		print (steps[x]>avg) ? steps[x]-avg  : avg - steps[x] ; #if only absolute values needed
		#print (steps[x]-avg);
	}
		
}
