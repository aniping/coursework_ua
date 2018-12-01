all: RSA.jar

RSA.jar:
	javac -cp java-getopt-1.0.14.jar:. *.java
	jar cvfm $@ manifest *.class

clean:
	rm RSA.jar *.class
