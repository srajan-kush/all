#!/bin/bash

# 1. Show network interfaces
ip link show

# 2. Display IP addresses
ip addr show

# 3. Display routing table
ip route show

# 4. Display network statistics
netstat -i

# 5. Ping a host
ping -c 4 google.com

# 6. Traceroute to a host
traceroute google.com

# 7. Display current ARP table
arp -n

# 8. Add a new IP address
sudo ip addr add 192.168.1.100/24 dev eth0

# 9. Delete an IP address
sudo ip addr del 192.168.1.100/24 dev eth0

# 10. Add a default gateway
sudo ip route add default via 192.168.1.1

# 11. Delete a default gateway
sudo ip route del default via 192.168.1.1

# 12. Show DNS resolver configuration
cat /etc/resolv.conf

# 13. Display listening ports
netstat -tuln

# 14. Display all network connections
netstat -a

# 15. Restart networking service (Debian/Ubuntu)
sudo systemctl restart networking

# 16. Restart network service (CentOS/RHEL)
sudo systemctl restart network

# 17. Bring an interface up
sudo ip link set eth0 up

# 18. Bring an interface down
sudo ip link set eth0 down

# 19. Configure DHCP on an interface
sudo dhclient eth0

# 20. Release DHCP lease
sudo dhclient -r eth0

# 21. Show NetworkManager status
nmcli general status

# 22. Restart NetworkManager
sudo systemctl restart NetworkManager

# 23. Display active connections (NetworkManager)
nmcli connection show --active

# 24. Bring a NetworkManager connection down
sudo nmcli connection down <connection-name>

# 25. Bring a NetworkManager connection up
sudo nmcli connection up <connection-name>

# 26. Set static IP using nmcli
sudo nmcli connection modify <connection-name> ipv4.addresses 192.168.1.100/24 ipv4.gateway 192.168.1.1 ipv4.dns "8.8.8.8,8.8.4.4" ipv4.method manual

# 27. Show firewall rules (using `iptables`)
sudo iptables -L -n -v

# 28. Add a firewall rule (using `iptables`)
sudo iptables -A INPUT -p tcp --dport 22 -j ACCEPT

# 29. Save `iptables` rules
sudo iptables-save > /etc/iptables/rules.v4

# 30. Restore `iptables` rules
sudo iptables-restore < /etc/iptables/rules.v4

# End of script
echo "Networking commands script executed."
