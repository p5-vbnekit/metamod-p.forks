#!/usr/bin/env python3
# -*- coding: utf-8 -*-

assert "__main__" != __name__


def validate_text_argument(
    value: str,
    require: bool = False,
    require_inline: bool = False,
    require_stripped: bool = False
):
    assert isinstance(value, str)
    assert isinstance(require, bool)
    assert isinstance(require_inline, bool)
    assert isinstance(require_stripped, bool)

    if require: assert bool(value)
    elif not value: return

    if require_inline: assert value.splitlines()[0] == value
    if require_stripped: assert value.strip() == value
