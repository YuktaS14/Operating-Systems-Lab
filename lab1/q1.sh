#!/bin/bash

# Checking whether no of arguments given are correct or not 
if [ $# -ne 2 ]
then
	echo "Wrong number of  arguments received" 
	echo "usage : $0 <arg1> <arg2>"
	echo "<arg1> : responses.csv that contains the details of the groups in which the students wish to do their labs" 
	echo "<arg2> : students_list.csv that contains the roll numbers of the students who have registered for a course"
	exit 1 
else
	echo "usage : $0 <arg1> <arg2>"
	echo "<arg1> : responses.csv that contains the details of the groups in which the students wish to do their labs" 
    echo "<arg2> : students_list.csv that contains the roll numbers of the students who have registered for a course"
	echo "Did u give arguments correctly?(type yes/no)"
	read truth_value 
	
	if [ $truth_value  == "no" ]
	then 
		exit 1 
	fi
fi


# Input files
responses_list=$1
rollnumber_list=$2


# Checking given files exist or not 
if [ ! -e "$responses_list" ]
then
	echo "$responses_list file doesn't exist"
	exit 1
elif [ ! -e $rollnumber_list ]
then
	echo "$rollnumber_list file doesn't exist"
	exit 1
fi

# Getting fields 3,4 from responses by delimiter "," and replacing ' " ' with '' 
# Removing first line (sed 1d) and replacing "," with "_" 
cut -d',' -f3,4 $responses_list | sed 's/"//g' | sed 1d | sed 's/,/_/g' > temp.txt

# -F value in awk is the field seperator 
# Temporary File "group_ID.txt" contains group ids with underscores
awk -F'_' '{ if ($2 == "") print $1 ;
	     else if ( $2 <$1 && $2!= "") print $2"_"$1 ; 
	     else print $1"_" $2}' temp.txt | sort | uniq > group_ID.txt

# Printing roll numbers of group ids as two seperate things into a file and using unique after sort to get the roll numbers which are duplicates
awk -F'_' '{ if($2 == "") print $1 ; 
	     else print $1"\n"$2}'  group_ID.txt | sort > roll_no.txt

uniq -d roll_no.txt > temp.txt

# File "temp.txt" contains duplicate entries of roll numbers
# Printing error messages for the duplicates
{
	echo
	echo "Duplicate entries are found for the following Roll Numbers:"
	echo "----------------------------------------------------------- "
	cat temp.txt
	echo 
	echo
	echo "Group IDs with duplicate Roll Numbers: "
	echo "-------------------------------------- "

	for line in $(cat temp.txt )
	do
		echo "Duplicate entry '$line' in group IDs :" 
	       	echo "$(grep $line group_ID.txt)"
		echo
	done

} >&2 

grep -v -f temp.txt group_ID.txt > temp1.txt

# File "rollno_uniq.txt" contains unique roll numbers who responded to form 
uniq roll_no.txt > rollno_uniq.txt

# Here -v selects the lines of rollno_uniq which do not match with any line of "rollnumber_list" file i.e. invalid roll numbers
grep -v -f $rollnumber_list rollno_uniq.txt > temp.txt
echo
echo "Invalid roll numbers in responses:"
echo "---------------------------------- "
cat temp.txt
echo
echo
echo "Group ids containing invalid roll numbers:"
echo "------------------------------------------ "
for line in $( cat temp.txt )
        do
                echo "Invalid entry '$line' in group ID :"
                echo "$(grep $line group_ID.txt)"
                echo
        done

echo
grep -v -f temp.txt temp1.txt > group_ID.txt

# Here -v selects the lines of rollno_list which do not match with any line of "rollno_uniq.txt" file i.e. students who didn't fill the form
echo
echo "Students who have not responded to the form:"
echo "-------------------------------------------- "
grep -v -f rollno_uniq.txt $rollnumber_list 

rm -f temp.txt
rm -f rollno_uniq.txt
rm -f roll_no.txt
rm -f temp1.txt
