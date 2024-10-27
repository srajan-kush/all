#!/bin/bash

# Set the name of the .pcap file
PCAP_FILE="122cs0060.pcap"
# Prompt the user to input the IP address
echo "Please enter the IP address:"
read ip_address

# Prompt the user to input the port number
echo "Please enter the port number:"
read port_number

# 1. Count number of TCP packets sent to destination port 12345
echo "1. Number of TCP packets sent to destination port $port_number:"
tcpdump -nn -r "$PCAP_FILE" 'tcp dst port ' $port_number | wc -l

# 2. Count number of UDP packets received from specific IP and port 12345
echo "2. Number of UDP packets received from $ip_address and port $port_number:"
tcpdump -nn -r "$PCAP_FILE" "udp src port $port_number and src host $ip_address" | wc -l
echo ""

# 3. Draw a plot showing number of TCP, UDP, ICMP, ARP packets
echo "3. Counting packets of each type:"
TCP_COUNT=$(tcpdump -nn -r "$PCAP_FILE" 'tcp' | wc -l)
UDP_COUNT=$(tcpdump -nn -r "$PCAP_FILE" 'udp' | wc -l)
ICMP_COUNT=$(tcpdump -nn -r "$PCAP_FILE" 'icmp' | wc -l)
ARP_COUNT=$(tcpdump -nn -r "$PCAP_FILE" 'arp' | wc -l)

echo "TCP packets: $TCP_COUNT"
echo "UDP packets: $UDP_COUNT"
echo "ICMP packets: $ICMP_COUNT"
echo "ARP packets: $ARP_COUNT"

# Save counts to a file for plotting
echo "$TCP_COUNT $UDP_COUNT $ICMP_COUNT $ARP_COUNT" > counts.txt

# Plot using Python script (ensure matplotlib is installed)
echo "Drawing plot of packet counts..."
python3 -c "
import matplotlib.pyplot as plt

# Read counts from file
with open('counts.txt') as f:
    tcp_count, udp_count, icmp_count, arp_count = map(int, f.readline().split())

protocols = ['TCP', 'UDP', 'ICMP', 'ARP']
counts = [tcp_count, udp_count, icmp_count, arp_count]

plt.bar(protocols, counts)
plt.xlabel('Protocol')
plt.ylabel('Number of Packets')
plt.title('Packet Distribution by Protocol')
plt.show()
"
echo ""

# 4. Find which destination host receives the highest number of packets
echo "4. Destination host receiving the highest number of packets:"
tcpdump -nn -r "$PCAP_FILE" 'ip' | awk '{print $3}' | cut -d. -f1-4 | sort | uniq -c | sort -nr | head -n 1
echo ""

# 5. Find which source host and port number is sending the highest number of packets
echo "5. Source host and port sending the highest number of packets:"
tcpdump -nn -r "$PCAP_FILE" 'tcp' | awk '{print $2, $4}' | sort | uniq -c | sort -nr | head -n 1
echo ""

# 6. Find which source host and how many types of protocol packets, and count of each protocol packet
echo "6. Source host and count of each protocol packet:"
tcpdump -nn -r "$PCAP_FILE" 'ip' | awk '{print $2, $1}' | sort | uniq -c
echo ""

# 7. Find the number of packets exchanged in a netcat TCP flow
# Assuming netcat uses port 12345
echo "7. Number of packets exchanged in a netcat TCP flow (port $port_number):"
tcpdump -nn -r "$PCAP_FILE" "tcp and port $port_number " | wc -l
echo ""

# 8. Count the number of netcat TCP and UDP flows
echo "8."
echo "Number of netcat TCP flows:"
tcpdump -nn -r "$PCAP_FILE" 'tcp' | awk '{print $2, $4, $5}' | sort | uniq | wc -l
echo ""

echo "Number of netcat UDP flows:"
tcpdump -nn -r "$PCAP_FILE" 'udp' | awk '{print $2, $4, $5}' | sort | uniq | wc -l
echo ""



# Make the script executable:
# chmod +x lab_2_122cs0060.sh

# Run the script:
# ./lab_2_122cs0060.sh
