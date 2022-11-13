#!/usr/bin/env python3
# -*- coding: utf-8 -*-

assert "__main__" != __name__

import sys
from . import validators


class Class(object):
    def __init__(self, name: str):
        validators.validate_text_argument(name, require = True, require_inline = True, require_stripped = True)
        self.__name = name

    def __enter__(self): print(f"::group::{self.__name}", file = sys.stderr, flush = True)
    def __exit__(self, exception_type, exception_instance, exception_traceback): print("::endgroup::", file = sys.stderr, flush = True)


def make(*args, **kwargs): return Class(*args, **kwargs)
