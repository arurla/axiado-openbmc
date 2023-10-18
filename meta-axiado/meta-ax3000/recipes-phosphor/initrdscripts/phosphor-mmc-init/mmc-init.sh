#!/bin/sh

fslist="proc sys dev run"
rodir=/mnt/rofs
mmcdev="/dev/mmcblk0"
rwfsdev="/dev/disk/by-partlabel/rwfs"
mmcpart=0

cd /
mkdir -p $fslist
mount dev dev -tdevtmpfs
mount sys sys -tsysfs
mount proc proc -tproc
mount tmpfs run -t tmpfs -o mode=755,nodev

# Wait up to 5s for the mmc device to appear. Continue even if the count is
# exceeded. A failure will be caught later like in the mount command.
count=0
while [ $count -lt 5 ]; do
    if [ -e "${mmcdev}" ]; then
        break
    fi
    sleep 1
    count=$((count + 1))
done

# The eMMC GPT labels for the rootfs are rofs-a and rofs-b, and the label for
# the read-write partition is rwfs. Run udev to make the partition labels show
# up. Mounting by label allows for partition numbers to change if needed.
udevd --daemon
udevadm trigger --type=devices --action=add
udevadm settle --timeout=10

while [ 1 ]
do
	# Read the file from the mmc partition table
	read mmcpart < /sys/class/M0/axdevM0/device/mmc_partition

	if [ $mmcpart -eq 1 ]
	then
			partition="rofs-a"
			break
	elif [ $mmcpart -eq 2 ]
	then
			partition="rofs-b"
			break
	else
			# Wait here till the right partition is programmed
			sleep 1
	fi
done

mkdir -p $rodir
if ! mount /dev/disk/by-partlabel/$partition $rodir -t ext4 -o ro; then
    /bin/sh
fi

fsck.ext4 -p "${rwfsdev}"
if ! mount "${rwfsdev}" $rodir/var -t ext4 -o rw; then
    /bin/sh
fi

rm -rf $rodir/var/persist/etc-work/
mkdir -p $rodir/var/persist/etc $rodir/var/persist/etc-work $rodir/var/persist/home/root
mount overlay $rodir/etc -t overlay -o lowerdir=$rodir/etc,upperdir=$rodir/var/persist/etc,workdir=$rodir/var/persist/etc-work

for f in $fslist; do
    mount --move $f $rodir/$f
done

cd ${rodir}
exec switch_root -c /dev/console $rodir /sbin/init
