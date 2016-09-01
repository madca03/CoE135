sudo rmmod date
sudo dmesg -C
make
sudo insmod date.ko
dmesg | tail -5
date
