## Assumptions:

1. Matrix list and Scalar list are independent of each other.
2. Supports multiple operations -- just like piping result of 1st operation is use as one of the nput for next operation.
3. If incorrect option is read or the inputs are not compatible for the next operation the execution stops.
4. Division operation gives floor of value obtained after dividing
5. Nothing is printed in output file if any error occurs while execution
6.While running MakeFile
-> i. make will create separate folders for as ask in question
-> ii. if user wants to give his test case he can give the path of that test cases(relative) and output will be availble in run.out file whereas log messages will be present in log.txt => both in pwd
-> iii. on make tests the test cases made by us will run. 

7.log messages are printed in log.txt

(while creating libraries need to mention inline before (setMatricesinitialize_resultMatrix) and (initialize_resultMatrix) (else error: multiple reference) but while building, linking libraries u need to remove inline (else error: undefined reference))

