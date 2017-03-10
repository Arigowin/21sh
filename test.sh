#!/bin/bash
<<<<<<< HEAD
=======
make re
>>>>>>> Jaahd
cd file_test
SRCS=$(ls)
for FILE in $SRCS
do
	OLD_IFS=$IFS
	IFS=$'\n'
	echo $FILE
<<<<<<< HEAD
	echo "make re && valgrind --leak-check=full --leak-resolution=high ./21sh <" $FILE
	for line in $(cat $FILE)
	do
		echo $line
		echo
	done
	IFS=$OLD_IFS
done
cd -
=======
	valgrind --leak-check=full --leak-resolution=high .././21sh < $FILE 
#	for line in $(cat $FILE)
#	do
#		echo $line
#		echo
#	done
	IFS=$OLD_IFS
done
>>>>>>> Jaahd
