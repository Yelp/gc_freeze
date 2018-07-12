# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import unicode_literals

import platform
import sys

from setuptools import Extension
from setuptools import setup


assert platform.python_implementation() == 'CPython', 'Not compatible with non-CPython implementations.'
assert sys.version_info < (3, 7), 'Not compatible with CPython 3.7 and above.'


gc_freeze = Extension(
    name=str('gc_freeze'),
    sources=[str('src/init.c'), str('src/freeze.c'), str('src/util.c')],
    include_dirs=[str('src/')],
)


setup(
    name=str('gc-freeze'),
    version=str('1.1.0'),
    description=str('Garbage collection freezing packaged as an extension for versions of Python before 3.7.'),
    platforms=str('all'),
    ext_modules=[gc_freeze],
)
