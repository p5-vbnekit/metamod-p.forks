#!/usr/bin/env python3
# -*- coding: utf-8 -*-

assert "__main__" != __name__

import io
import os
import pathlib

from . import validators


archive_commands = {
    "zip": "_destination=`readlink -m \"$@\"`; test -n \"$${_destination}\" && cd \"$<\" && zip -9 -r \"$${_destination}\" .",
    "tar.xz": "tar --create --directory=\"$<\" . | xz -9 > \"$@\""
}

platform_descriptions = {
    "linux": {
        "crlf_tool": "dos2unix",
        "main_binary": "addons/metamod/metamod.so",
        "output_extension": "tar.xz"
    },
    "windows": {
        "crlf_tool": "unix2dos",
        "main_binary": "addons/metamod/metamod.dll",
        "output_extension": "zip"
    }
}

def detect_platform(path: str):
    assert isinstance(path, str) and bool(path)
    if not os.path.exists(path): raise FileNotFoundError(path)
    if not os.path.isdir(path): raise RuntimeError(f"is not a directory: {path}")
    for _platform_name in "linux", "windows":
        _binary = platform_descriptions[_platform_name]["main_binary"]
        _binary = os.path.join(path, _binary)
        if not os.path.exists(_binary): continue
        if not os.path.isfile(_binary): continue
        return _platform_name
    return None

def scan_binary_directories(path: str):
    assert isinstance(path, str) and bool(path)

    def _routine(base):
        _path = path if base is None else os.path.join(path, base)
        _platform = detect_platform(path = _path)
        if _platform is not None:
            yield {"path": base, "platform": _platform}
            return
        for _item in os.scandir(_path):
            if not _item.is_dir(): continue
            yield from _routine(base = _item.name if base is None else os.path.join(base, _item.name))

    yield from _routine(base = None)

def produce_makefile(
    build_directory_path: str,
    output_directory_path: str,
    binaries_directory_path: str,
    template_directory_path: str
):
    validators.validate_text_argument(build_directory_path, require = True, require_inline = True, require_stripped = False)
    validators.validate_text_argument(output_directory_path, require = True, require_inline = True, require_stripped = False)
    validators.validate_text_argument(binaries_directory_path, require = True, require_inline = True, require_stripped = False)
    validators.validate_text_argument(template_directory_path, require = True, require_inline = True, require_stripped = False)

    binaries_directory_path = os.path.realpath(os.path.abspath(os.path.normpath(binaries_directory_path)))
    template_directory_path = os.path.realpath(os.path.abspath(os.path.normpath(template_directory_path)))

    assert os.path.isdir(binaries_directory_path)
    assert os.path.isdir(template_directory_path)

    with io.StringIO() as _main_stream:
        print("""
all:
.PHONY: all
.DEFAULT: all
        """.strip(), file = _main_stream)

        _sequence = tuple(scan_binary_directories(path = binaries_directory_path))

        if _sequence:
            _collector_directory_path = os.path.join(build_directory_path, "collector")
            print(f"""
{build_directory_path}:
\tmkdir --parents "$@"
{_collector_directory_path}: {build_directory_path}
\tmkdir --parents "$@"
{output_directory_path}:
\tmkdir --parents "$@"
            """.strip(), file = _main_stream)

            _platforms = set()
            _platform_templates_directory_path = os.path.join(build_directory_path, "templates")

            with io.StringIO() as _item_instructions:
                for _item in _sequence:
                    _item_path = _item["path"]
                    _item_binaries_directory_path = os.path.join(binaries_directory_path, _item_path)
                    _item_plaform_name = _item["platform"]
                    _item_platform_description = platform_descriptions[_item_plaform_name]
                    _item_platform_template_archive_path = os.path.join(_platform_templates_directory_path, f"{_item_plaform_name}.tar")
                    _item_output_archive_extension = _item_platform_description["output_extension"]
                    _item_output_archive_command = archive_commands[_item_output_archive_extension]
                    _item_collector_directory_path = os.path.join(_collector_directory_path, _item_path)
                    _item_output_archive_path = os.path.join(output_directory_path, f"{_item_path}.{_item_output_archive_extension}")
                    _platforms.add(_item_plaform_name)
                    print(f"""
{_item_collector_directory_path}: {_collector_directory_path}
{_item_collector_directory_path}: {_item_platform_template_archive_path}
{_item_collector_directory_path}: {_item_binaries_directory_path}
\tmkdir --parents "$@"
\ttar --extract --file "{_item_platform_template_archive_path}" --directory="$@"
\ttar --create --directory="$<" . | tar --extract --directory="$@"
\tfind "$@" -type f -exec chmod 0644 {{}} \\;
{_item_output_archive_path}: {output_directory_path}
{_item_output_archive_path}: {_item_collector_directory_path}
\tmkdir --parents "{os.path.dirname(_item_output_archive_path)}"
\t{_item_output_archive_command}
all: {_item_output_archive_path}
                    """.strip(), file = _item_instructions)
                _item_instructions = _item_instructions.getvalue().strip()

            assert bool(_platforms)
            print(f"""
{_platform_templates_directory_path}: {build_directory_path}
\tmkdir --parents "$@"
            """.strip(), file = _main_stream)

            for _platform in _platforms:
                _crlf_tool = platform_descriptions[_platform]["crlf_tool"]
                _platform_template_directory_path = os.path.join(_platform_templates_directory_path, _platform)
                _platform_template_archive_path = f"{_platform_template_directory_path}.tar"
                print(f"""
{_platform_template_directory_path}: {_platform_templates_directory_path}
{_platform_template_directory_path}: {template_directory_path}
\tmkdir --parents "$@"
\ttar --create --directory="$<" . | tar --extract --directory="$@"
\tfind "$@" -type f -exec chmod 0644 {{}} \\;
\tfind "$@" -type f -exec {_crlf_tool} {{}} \\;
{_platform_template_archive_path}: {_platform_template_directory_path}
\ttar --create --file="$@" --directory="$<" .
                """.strip(), file = _main_stream)

            print(_item_instructions, file = _main_stream)

        return _main_stream.getvalue()
