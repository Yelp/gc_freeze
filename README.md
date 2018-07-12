gc_freeze
=========

`gc_freeze` is a backport of the new garbage collection freezing features introduced in Python 3.7's `gc` module for earlier versions of Python (CPython) including 2.7.

Background
----------

At Yelp, we need to fork workers from a master process after warmup. Normally, the first garbage collection run after a fork would iterate through all GC-tracked elements, update values, and provoke a full [copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write) on everything, even for objects that would hardly ever be read at all again.

Instagram [added features to Python 3.7](https://instagram-engineering.com/copy-on-write-friendly-python-garbage-collection-ad6ed5233ddf) to move (freeze) read-only objects to a generation that would not be iterated over during a garbage collection run, but since we can not reasonably upgrade our entire infrastructure to the newest Python just for this, we decided to package the feature as a C extension compatible with all versions of Python earlier than 3.7, at least as early as 2.7.

Features
--------

`gc_freeze.freeze()`: Moves all GC-tracked objects to a permanent generation, out of reach from the garbage collector.

`gc_freeze.unfreeze()`: Moves all elements from the permanent generation to the oldest GC-tracked generation.

`gc_freeze.get_freeze_count()`: Returns the number of elements in the permanent generation. **Warning:** This method cycles through the permanent generation to count objects, and shouldn't be called unnecessarily.

`gc_freeze.get_actual_count()`: This method is exclusive to this extension. Returns the actual number of elements in each of the GC-tracked generations in the form of a `(gen0, gen1, gen2)` tuple. This method differs from `gc.get_count()` in that it doesn't only return the value of the `count` field for each generation. **Warning:** This method cycles through each generation to count objects, and shouldn't be called unnecessarily. We use it primarily for testing purposes.

Support
-------

**Tested:**

* CPython 2.7
* CPython 3.4
* CPython 3.5
* CPython 3.6

**Untested:**

While this has not been tested, `gc_freeze` should work in all versions of CPython between 2.6 and 3.6 with no changes. It might be possible to get the extension to work in versions of CPython as early as 2.3 with minimal changes (e.g. by removing the references to `platform.python_implementation()`).

**CPython 3.7+:**

Please use the [built-in methods](https://docs.python.org/3/library/gc.html#gc.freeze) from the `gc` module if using CPython 3.7.

**PyPy and other non-CPython implementations:**

No support is provided for other implementations of Python such as PyPy.

Installation
------------

`python setup.py install`

How to use
----------

Just `import gc_freeze` and call the methods as described in the **Features** section.

In Yelp's use case, we only call `gc_freeze.freeze()` once before forking workers and never actually call `gc_freeze.unfreeze()`.

Architecture
------------

`init.c` is used to instantiate the module with a name, description, and a listing with links of the module's methods. It also triggers initialization of the freezing mechanism.

`freeze.c` holds `generations`, a pointer to the generations tracked by Python's garbage collector, and a list of objects kept in permanent generation in `permanent_generation` (when garbage collection is frozen). These two objects are initialized when `init_freeze()` is called. The rest of the file is made of methods copied from [another implementation](https://github.com/z2kviswanathan/cpython/pull/1/files) of a `gc.freeze()` backport to Python 2.7, and is also almost identical to [the implementation](https://github.com/python/cpython/pull/3705/files) of GC freezing in Python 3.

`util.c` contains copies of C functions that already existed inside the Python interpreter, but were not accessible from the outside.
