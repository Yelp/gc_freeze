# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import unicode_literals

import gc

import gc_freeze


def test_c_extension():
    gc.disable()
    gc_freeze.unfreeze()

    # initially empty
    assert gc_freeze.get_freeze_count() == 0, \
        'The permanent generation should initially be empty.'

    # move everything to the permanent generation
    total = sum(gc_freeze.get_actual_count())
    gc_freeze.freeze()
    assert sum(gc_freeze.get_actual_count()) < total, \
        'Objects should have been removed from the GC generations.'
    assert gc_freeze.get_freeze_count() > 0, \
        'Objects should have been moved to the permanent generation.'

    # move everything back to the oldest generation
    total = gc_freeze.get_actual_count()[2]
    gc_freeze.unfreeze()
    assert gc_freeze.get_actual_count()[2] > total, \
        'The oldest generation should contain more items after unfreezing than before.'
    assert gc_freeze.get_freeze_count() == 0, \
        'The permanent generation should be empty after unfreezing.'

    gc.enable()
