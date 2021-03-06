SHELL=/bin/bash

JAVA_HOME?=$(shell dirname $$(dirname $$(readlink -f $$(which javac))))
java_includes:=$(shell find "$(JAVA_HOME)/include" -type d | xargs -rn1 printf "-I%s ")
php_includes:=$(shell php-config --includes)
php_libs:=$(shell echo "$$(php-config --prefix)/lib")
java_libs=$(shell echo "$(JAVA_HOME)/jre/lib/amd64/server")

all: libphp4j.so

test: PhpTest.class
	LD_LIBRARY_PATH=$(php_libs):$(java_libs) java -Djava.library.path=. PhpTest

PhpTest.class: PhpTest.java libphp4j.so
	javac PhpTest.java

libphp4j.so: php4j.o
	$(CC) -g -fPIC -shared -o libphp4j.so php4j.o -lphp7

php4j.o: php4j.c com_github_adsr_php4j_Php.h com_github_adsr_php4j_Zval.h
	$(CC) -g -fPIC $(php_includes) $(java_includes) -c php4j.c -o php4j.o

com_github_adsr_php4j_Php.h: Php.class
	javah com.github.adsr.php4j.Php

com_github_adsr_php4j_Zval.h: Zval.class
	javah com.github.adsr.php4j.Zval

Php.class: Php.java Zval.class
	javac Php.java

Zval.class: Zval.java
	javac Zval.java

clean:
	rm -f libphp4j.so php4j.o
	rm -f com_github_adsr_php4j_Php.h com_github_adsr_php4j_Zval.h
	rm -f Php.class Zval.class PhpTest.class

.PHONY: all test clean
