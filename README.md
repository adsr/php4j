# php4j

php4j is PHP embedded in Java. It allows you to call PHP code from Java in the
same process. See `PhpTest.java` for example usage.

Alternatives to this are shelling out to PHP (forking) or calling PHP over the
network.

### Requirements

* PHP compiled with `--enable-embed` (>= 7.0.0)
* JDK (>= 8, earlier versions probably work, but untested)

On Debian/Ubuntu this is `openjdk-*-jdk` and `libphp7.*-embed`.

### Building

`javac`, `javah`, and `php-config` are expected to be in `PATH`.

    $ make

### Testing

This will compile and run `PhpTest.java` which executes `test.php` and prints
the result.

    $ make test

### Limitations

* Not thread-safe
* Currently all values are returned as JSON-encoded strings

### TODO

* A `Zval` type instead of JSON'ing everything
* Support for calling Java from PHP
* Support for exceptions, errors, SAPI options, SAPI callbacks
* Control of request init/shutdown
