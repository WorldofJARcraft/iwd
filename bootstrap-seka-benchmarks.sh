#!/bin/sh
if [ -f config.status ]; then
	make maintainer-clean
fi

if [ -d ../ell ]; then
    echo "ell found!";
else
    echo "Cloning ell";
    git clone https://kernel.googlesource.com/pub/scm/libs/ell/ell.git ../ell/;
    (cd ../ell; git checkout 89d4888f1321c03810534a3e1cf464fc5fe368f2);
fi

./bootstrap && \
    ./configure CFLAGS="-march=native -mtune=native -O3" \
        --enable-maintainer-mode \
		--disable-dbus-policy \
		--disable-systemd-service $* && \
    make && \
    echo "Increasing number of keys for user" && \
    echo 1000000 | sudo tee /proc/sys/kernel/keys/maxkeys && \
    echo 25000000 | sudo tee /proc/sys/kernel/keys/maxbytes && \
    echo "Loading pkcs parser module" && \ 
    sudo modprobe pkcs8_key_parser && \ 
    echo "Running Dragonfly benchmarks" && \
    taskset -c 0 ./unit/test-sae && \
    echo "Running 4-Way Handshake and EAP-TLS Benchmarks" && 
    taskset -c 0 ./unit/test-eapol
exit $?

