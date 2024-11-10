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