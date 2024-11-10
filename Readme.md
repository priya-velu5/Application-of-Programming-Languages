 ## Week 6

 ### Twenty
 
 `cd Week6`

 *To compile and create jar files* 

`javac -d bin framework/*.java app1/*.java app2/*.java
 jar cvf framework.jar -C bin Twenty/framework
 jar cvf app1.jar -C bin Twenty/app1
 jar cvf app2.jar -C bin Twenty/app2 `

*to use app1*
Also change the config file to reflect this option. 

`java -cp "framework.jar:app1.jar" Twenty.framework.Main ../../pride-and-prejudice.txt`

*to use app2*
Also change the config file to reflect this option. 

`java -cp "framework.jar:app2.jar" Twenty.framework.Main ../../pride-and-prejudice.txt`

### Seventeen 

*compile*

`javac Seventeen.java`

`java Seventeen ../pride-and-prejudice.txt`

When prompted use any of these classes:
* DataStorageManager
* StopWordManager
* WordFrequencyManager
* WordFrequencyController 

## Week 5

### TwentyOne

`g++ Week5/TwentyOne.cpp -0 w21.out`

` ./Week5/w21.out pride-and-prejudice.txt`

### TwentyTwo
`g++ Week5/TwentyTwo.cpp -0 w22.out`

` ./Week5/w22.out pride-and-prejudice.txt`


### TwentyFive

`cd Week5`

`javac TwentyFive.java`

`java TwentyFive ../pride-and-prejudice.txt`

## Week 4

### Fifteen

This repl has Boost installed for event management handling in cpp. If the forked repo doesn't have it installed, please go to Dependencies->System Dependencies-> Add "boost184"

`g++ Week4/Fifteen.cpp -o Week4/w15.out`

`./Week4/w15.out pride-and-prejudice.txt`

### Thirteen

`node Week4/thirteen.js pride-and-prejudice.txt`


### Twelve
`g++ Week4/Twelve.cpp -o Week4/w12.out`

`./Week4/w12.out pride-and-prejudice.txt`


## Week 3:


### Nine
`
g++ Week3/Nine.cpp -o w9.out
`

`
./Week3/w9.out pride-and-prejudice.txt
`

## To run Week 2:

For Four, Five and Seven (change file name accordingly):
* `
g++ Week2/Five.cpp -o Week2/w5.out
`
* `
./Week2/w5.out
`

Six was implemented in Go. Find it here: https://replit.com/@singaram/APL-CantUseCPP


## To run Week 1:

Command to compile:
`
g++ Week1/count-week1.cpp -o Week1/w1.out
`

Command to run:

`
./Week1/w1.out
`




