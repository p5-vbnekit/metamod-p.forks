#!/usr/bin/env python3
# -*- coding: utf-8 -*-

assert "__main__" != __name__

import sys
import json
import requests

from . import validators


class UnexpectedResponse(RuntimeError): pass


def scan(api: str, key: str, ref: str, repo: str, token: str):
    validators.validate_text_argument(api, require = True, require_inline = True, require_stripped = True)
    validators.validate_text_argument(key, require = True, require_inline = True, require_stripped = True)
    validators.validate_text_argument(ref, require = True, require_inline = True, require_stripped = True)
    validators.validate_text_argument(repo, require = True, require_inline = True, require_stripped = True)
    validators.validate_text_argument(token, require = True, require_inline = True, require_stripped = True)

    api = api.rstrip("/")

    _page = 0
    _left = None
    _total = None
    _versions = set()
    _key_size = len(key)
    _page_size = 100

    while True:
        _response = requests.get(
            f"{api}/repos/{repo}/actions/caches",
            params = {
                "accept": "application/vnd.github+json",
                "ref": ref, "key": key,
                "page": _page, "per_page": _page_size
            },
            headers = {
                "Accept": "application/vnd.github+json",
                "Authorization": f"Bearer {token}",
                "X-GitHub-Api-Version": "2022-11-28"
            }
        )

        try:
            try: assert 200 == _response.status_code
            except AssertionError:
                print(_response.text.strip(), file = sys.stderr, flush = True)
                raise UnexpectedResponse(f"code = {_response.status_code}")
            _data = json.loads(_response.text)
        finally: _response.close()

        assert isinstance(_data, dict)
        if _total is None:
            _total = _data["total_count"]
            assert isinstance(_total, int)
            assert 0 <= _total
            _left = _total
        else:
            assert isinstance(_data["total_count"], int)
            assert _total == _data["total_count"]

        _data = _data["actions_caches"]
        assert isinstance(_data, list)

        # work around api bug
        _bogus_data_size = len(_data)
        _data = [_item for _item in _data if _item["version"] not in _versions]

        _data_size = len(_data)

        # work around api bug
        if _bogus_data_size > _data_size: assert _left == _data_size

        if _page_size > _data_size: assert _left == _data_size
        else:
            assert _left >= _data_size
            assert _page_size == _data_size

        for _data in _data:
            _key =  _data["key"]
            _size = _data["size_in_bytes"]
            _version = _data["version"]
            assert _version not in _versions
            _versions.add(_version)
            validators.validate_text_argument(_key, require = True, require_inline = True, require_stripped = True)
            validators.validate_text_argument(_version, require = True, require_inline = True, require_stripped = True)
            assert isinstance(_size, int) and (0 < _size)
            if _key == key:
                yield {"key": _key, "size": _size, "version": _version}
                continue
            assert len(_key) > _key_size
            assert _key.startswith(key)

        _left = _left - _data_size
        _page = 1 + _page

        if 0 == _left: break
        assert 0 < _left

def query(api: str, key: str, token: str, version: str):
    validators.validate_text_argument(key, require = True, require_inline = True, require_stripped = True)
    validators.validate_text_argument(version, require = True, require_inline = True, require_stripped = True)

    api = api.rstrip("/")

    _response = requests.get(
        f"{api}/_apis/artifactcache/cache",
        params = { "keys": key, "version": version },
        headers = {
            "Accept": "application/json;api-version=6.0-preview.1",
            "User-Agent": "actions/cache",
            "Authorization": f"Bearer {token}"
        }
    )

    try:
        try: assert 200 == _response.status_code
        except AssertionError:
            print(_response.text.strip(), file = sys.stderr, flush = True)
            raise UnexpectedResponse(f"code = {_response.status_code}")
        _data = json.loads(_response.text)
    finally: _response.close()

    assert isinstance(_data, dict)
    assert _data["cacheKey"] == key
    assert _data["cacheVersion"] == version
    _url =  _data["archiveLocation"]
    _scope = _data["scope"]
    validators.validate_text_argument(_url, require = True, require_inline = True, require_stripped = True)
    validators.validate_text_argument(_scope, require = True, require_inline = True, require_stripped = True)
    return { "url": _url, "scope": _scope }

def download(url: str):
    validators.validate_text_argument(url, require = True, require_inline = True, require_stripped = True)

    _response = requests.get(url, stream = True)
    _chunk_size = 256 * 1024

    try:
        try: assert 200 == _response.status_code
        except AssertionError:
            print(_response.text.strip(), file = sys.stderr, flush = True)
            raise UnexpectedResponse(f"code = {_response.status_code}")
        while True:
            _chunk = _response.raw.read(_chunk_size)
            if not _chunk: break
            yield _chunk

    finally: _response.close()
