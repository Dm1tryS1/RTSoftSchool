#!/bin/bash
mount /dev/sdb1 /home/dmitry/HW1/HW1.1
cp /var/log/syslog /home/dmitry/HW1/HW1.1
cp /var/log/dmesg  /home/dmitry/HW1/HW1.1
umount /dev/sdb1