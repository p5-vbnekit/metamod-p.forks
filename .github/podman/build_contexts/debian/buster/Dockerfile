ARG base_image=debian:buster

FROM ${base_image}

ENV LANG=C LC_ALL=C DEBIAN_FRONTEND=noninteractive

RUN rm --force --recursive -- /etc/apt/sources.list /etc/apt/sources.list.d
COPY debian.sources /etc/apt/sources.list.d/debian.sources

RUN apt-mark showmanual | xargs --no-run-if-empty -- apt-mark auto --
RUN apt --assume-yes update && apt --assume-yes install -- apt-utils

RUN apt --assume-yes purge -- bsdmainutils ca-certificates dbus make tzdata
RUN dpkg --purge --force-remove-essential -- \
  base-passwd bsdutils fdisk hostname login mount ncurses-bin ncurses-base sysvinit-utils util-linux
RUN apt --assume-yes autoremove && apt --assume-yes update && apt --assume-yes full-upgrade

RUN dpkg --add-architecture i386

RUN apt --assume-yes update && apt --assume-yes install -- cmake g++ g++-multilib
RUN apt --assume-yes update && apt --assume-yes install -- g++-i686-linux-gnu g++-mingw-w64
RUN apt --assume-yes update && apt --assume-yes install -- wine

RUN wine cmd /c echo wine should be ready now && rm --force --recursive -- /root/.wine

RUN rm --force -- /etc/*-
RUN apt-mark auto -- apt-utils
RUN apt --assume-yes update && apt --assume-yes full-upgrade && apt --assume-yes autoremove && \
    dpkg --list | grep -- '^rc' | awk -- '{print $2}' | xargs --no-run-if-empty -- dpkg --purge -- && \
    apt --assume-yes clean
RUN find /tmp /var/tmp /var/lib/apt/lists -mindepth 1 -maxdepth 1 -exec rm --force --recursive -- {} \; 2>/dev/null || true
