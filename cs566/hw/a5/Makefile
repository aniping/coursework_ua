all: DES.jar

DES.jar:
	javac -cp java-getopt-1.0.14.jar:. *.java
	jar cvfm $@ manifest *.class

clean:
	rm DES.jar *.class
