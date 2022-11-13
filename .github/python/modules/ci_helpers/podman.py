#!/usr/bin/env python3
# -*- coding: utf-8 -*-

assert "__main__" != __name__

import os
import sys
import typing
import contextlib
import subprocess


executable = "podman"

def get_image_id(image: str):
    assert isinstance(image, str)
    assert bool(image)
    assert image.strip() == image
    assert 1 == len(image.splitlines())

    _result = subprocess.run(
        (executable, "image", "inspect", "--format={{.Id}}", image),
        check = True, stdin = subprocess.DEVNULL, stdout = subprocess.PIPE
    ).stdout

    assert isinstance(_result, bytes)
    assert bool(_result)
    _result = _result.decode("utf-8").rstrip("\r\n")
    assert isinstance(_result, str)
    assert _result.strip() == _result
    assert 1 == len(_result.splitlines())
    return _result

def load_layers(path: str):
    assert isinstance(path, str)
    assert bool(path)
    assert path[-1] not in ("\r\n")
    assert 1 == len(path.splitlines())
    assert os.path.exists(path)
    assert not os.path.isdir(path)
    return 0 == subprocess.run(
        (executable, "load", f"--input={path}"),
        stdin = subprocess.DEVNULL,
        stdout = subprocess.DEVNULL
    ).returncode

def save_layers(image: str, path: str):
    assert isinstance(image, str)
    assert bool(image)
    assert image.strip() == image
    assert 1 == len(image.splitlines())

    assert isinstance(path, str)
    assert bool(path)
    assert path[-1] not in ("\r\n")
    assert 1 == len(path.splitlines())
    assert not os.path.isdir(path)

    _images = [image]
    _image = get_image_id(image)

    def _parse_image_id(value: bytes):
        assert isinstance(value, bytes)
        if not value: return None
        value = value.decode("utf-8").rstrip("\r\n")
        assert isinstance(value, str)
        assert value.strip() == value
        assert 1 == len(_image.splitlines())
        return value

    while True:
        _image = _parse_image_id(value = subprocess.run(
            (executable, "image", "inspect", "--format={{.Parent}}", _image),
            check = True, stdin = subprocess.DEVNULL, stdout = subprocess.PIPE
        ).stdout)
        if not _image: break
        _images.insert(0, _image)

    _directory = os.path.dirname(path)
    if _directory: os.makedirs(_directory, exist_ok = True)

    subprocess.run((
        executable, "image", "save", "--multi-image-archive",
        "--format=docker-archive", f"--output={path}", *_images
    ), check = True, stdin = subprocess.DEVNULL, stdout = subprocess.DEVNULL)

def build_image(image: str, context: str, pull: bool = False):
    assert isinstance(image, str)
    assert bool(image)
    assert image.strip() == image
    assert 1 == len(image.splitlines())

    assert isinstance(context, str)
    assert bool(context) and os.path.isdir(context)

    assert isinstance(pull, bool)

    _arguments = ["buildah", "build"]
    if pull: _arguments.append("--pull")
    _arguments.extend(("--layers", "--format=docker", f"--tag={image}", context))

    with contextlib.redirect_stdout(sys.stderr): subprocess.run(
        _arguments, check = True, stdin = subprocess.DEVNULL
    )

def run_image(
    image: str,
    command: typing.Union[str, typing.Iterable[str]] = None,
    volumes: typing.Union[str, typing.Iterable[str]] = None,
    environment: typing.Iterable = None,
    working_directory: str = None
):
    assert isinstance(image, str)
    assert bool(image)
    assert image.strip() == image
    assert not image.startswith("-")
    assert 1 == len(image.splitlines())

    def _parse_command():
        if command is None: return
        if isinstance(command, str):
            yield command
            return
        for _item in command:
            assert isinstance(_item, str)
            yield _item

    try: command = tuple(_parse_command())
    finally: del _parse_command

    def _parse_volumes():
        if volumes is None: return
        if isinstance(volumes, str):
            assert bool(volumes)
            assert volumes.strip() == volumes
            assert 1 == len(volumes.splitlines())
            yield f"--volume={volumes}"
            return
        for _item in volumes:
            assert isinstance(_item, str)
            assert bool(_item)
            assert _item.strip() == _item
            assert 1 == len(_item.splitlines())
            yield f"--volume={_item}"

    try: volumes = tuple(_parse_volumes())
    finally: del _parse_volumes

    def _parse_environment():
        if environment is None: return
        try: _source = {**environment}.items()
        except TypeError: _source = tuple(environment)
        _keys = set()
        for _key, _value in _source:
            assert isinstance(_key, str)
            assert bool(_key)
            assert _key.strip() == _key
            assert 1 == len(_key.splitlines())
            assert _key not in _keys
            assert isinstance(_value, str)
            yield f"--env={_key}={_value}"

    try: environment = tuple(_parse_environment())
    finally: del _parse_environment

    if working_directory is not None:
        assert isinstance(working_directory, str)
        assert bool(working_directory)
        assert working_directory.strip() == working_directory
        assert 1 == len(working_directory.splitlines())

    _arguments = [
        executable, "run", "--rm"
    ]

    if working_directory is not None: _arguments.append(f"--workdir={working_directory}")

    _arguments.extend(environment)
    _arguments.extend(volumes)
    _arguments.append(image)
    _arguments.extend(command)

    with contextlib.redirect_stdout(sys.stderr): subprocess.run(
        _arguments, check = True, stdin = subprocess.DEVNULL
    )
