#!/usr/bin/env sh                                                               
# PRE-REQUISITE:
# eip-dev driver should be loaded without bringing up of interfaces.

# Create a new namespace
ip netns add defeip

# Transfer eth0 to namespace-defeip
ip link set eth0 netns defeip
ip link set ethdtl netns defeip


# iptables rules
ip netns exec defeip iptables -t mangle -N DIVERT

ip netns exec defeip iptables -t mangle -A PREROUTING -i eth0 -j DIVERT

ip netns exec defeip iptables -t mangle -A DIVERT -j ACCEPT

ip netns exec defeip iptables -t mangle -A POSTROUTING \
	-j MARK --set-mark 0x06200001
ip netns exec defeip iptables -t mangle -A POSTROUTING -j ACCEPT


# Assign ip to interfaces eth0
ip netns exec defeip ip addr add 172.168.0.10/24 dev eth0
ip netns exec defeip ip addr add 182.168.0.10/24 dev ethdtl


# Configure ip rule and ip route
ip netns exec defeip echo 1 > /proc/sys/net/ipv4/ip_forward
echo 1 > /proc/sys/net/ipv4/ip_forward

ip netns exec defeip cat /proc/sys/net/ipv4/ip_forward
ip netns exec defeip sysctl -w net.ipv4.ip_forward=1
cat /proc/sys/net/ipv4/ip_forward


# Bring up interface
ip netns exec defeip ip link set eth0 up
ip netns exec defeip ip link set ethdtl up

# Set default route for marked pkt.
ip netns exec defeip ip route add default  via 182.168.0.1  dev ethdtl

# set static ARP
ip netns exec defeip arp -s 182.168.0.1 ab:cd:ef:ab:cd:ef
