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

    $ make

`javac`, `javah`, and `php-config` are expected to be in `PATH`.

### Testing

    $ make test

This will compile and run `PhpTest.java` which executes `test.php` and prints
the result.

### Limitations

* Not thread-safe
* `Zval`s must be freed manually (via `dispose` method)

### TODO

* Fix leak in getHash
* Support for calling Java from PHP
* Support for exceptions, errors, SAPI options, SAPI callbacks
* Control of request init/shutdown
