ARG base_image=debian:bullseye

FROM ${base_image}

ENV LC_ALL C
ENV container docker
ENV DEBIAN_FRONTEND noninteractive

COPY sources.list /etc/apt/sources.list

RUN _packages=`apt-mark showmanual`; test -n "${_packages}" || exit 0; apt-mark auto ${_packages}
RUN apt update --assume-yes && apt install --assume-yes apt-utils
RUN apt update --assume-yes && apt full-upgrade --assume-yes

RUN dpkg --add-architecture i386

RUN apt update --assume-yes && apt install --assume-yes cmake g++ g++-multilib
RUN apt update --assume-yes && apt install --assume-yes g++-i686-linux-gnu g++-mingw-w64
RUN apt update --assume-yes && apt install --assume-yes wine

RUN wine cmd /c echo wine should be ready now

RUN apt update --assume-yes && apt --assume-yes full-upgrade && apt autoremove --assume-yes && apt purge --assume-yes '~c'
RUN apt clean --assume-yes && for _path in /tmp /var/tmp /var/lib/apt/lists; do find "${_path}" -mindepth 1 -maxdepth 1 -exec rm -rf {} \;; done
