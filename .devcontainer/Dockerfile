FROM mcr.microsoft.com/devcontainers/cpp:0-ubuntu-22.04

ARG USERNAME=vscode
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential \
       cmake \
       ninja-build \
       git \
       wget \
       curl \
       ca-certificates \
       python3 \
       python3-pip \
    # Qt5 development packages (kept for compatibility).
    # Note: Qt6 packages are intentionally omitted from apt install because
    # their package names and availability vary by distribution and apt
    # repositories; install Qt6 separately (Qt online installer or distro
    # backports) if you need Qt6 inside the container.
    qtbase5-dev \
    qttools5-dev-tools \
    qtbase5-dev-tools \
    libqt5svg5-dev \
       libx11-dev \
       libxrandr-dev \
       libxcb1-dev \
       libxcb-randr0-dev \
       libxcb-xinerama0-dev \
       libxcb-xfixes0-dev \
       libxcb-keysyms1-dev \
       libx11-xcb-dev \
       libxcb-util0-dev \
       libasound2-dev \
       libjack-jackd2-dev \
       libpulse-dev \
       libavcodec-dev \
       libavformat-dev \
       libavutil-dev \
       libswscale-dev \
       libavdevice-dev \
       libv4l-dev \
       pkg-config \
       clang-format \
       clang-tidy \
       lldb \
       gdb \
       locales \
    && rm -rf /var/lib/apt/lists/*

RUN locale-gen en_US.UTF-8 || true

ENV LANG=en_US.UTF-8 \
    LC_ALL=en_US.UTF-8

WORKDIR /workspace

CMD ["sleep", "infinity"]
