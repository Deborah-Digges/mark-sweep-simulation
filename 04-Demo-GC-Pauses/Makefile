JC = javac
JVM = java

.SUFFIXES: .java .class
.PHONY: clean

.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = MemoryTest.java
MAIN = MemoryTest

default: classes

classes: $(CLASSES:.java=.class)

run: classes
	$(JVM) $(MAIN)

clean:
	$(RM) *.class

