# php4j

php4j is PHP embedded in Java. It allows you to call PHP code from Java in the
same process. See `PhpTest.java` for example usage.

Alternatives to this are shelling out to PHP (forking) or calling PHP over the
network.

### Requirements

* `php-dev` (>= 7.0.0)
* `openjdk-8-jdk` or `openjdk-9-jdk`

`javac`, `javah`, and `php-config` are expected to be in `PATH`.

### Building

    $ make

### Testing

    $ make test

This will compile and run `PhpTest.java` which executes `test.php` and prints
the result.

### Limitations

* Not thread-safe
* Currently all values are returned as JSON-encoded strings

### TODO

* A `Zval` type instead of JSON'ing everything
* Support for calling Java from PHP
* Support for exceptions, errors, SAPI options, SAPI callbacks
* Control of request init/shutdown
