#!/bin/bash

# Introduction
echo "Khaas aapke liye: "
echo "Let's start the network debugging:"

# Step 1: Check Network Configuration
echo "Step 1: Checking Network Configuration:"

# Display network interfaces
echo "1. Displaying network interfaces:"
echo "This shows all network interfaces (like Ethernet, Wi-Fi) and their statuses."
ip a
echo "" 

# Display routing table
echo "2. Displaying routing table:"
echo "This shows the routing table, which tells how data should be routed through the network."
echo ""
ip route
echo ""

# Display DNS configuration
echo "3. Displaying DNS configuration:"
echo "This shows the DNS servers your system uses to resolve domain names to IP addresses."
cat /etc/resolv.conf
echo ""

# Step 2: Test Connectivity
echo "Step 2: Testing Connectivity:"

# Test connectivity to local network
echo "1. Pinging local network (192.168.1.1):"
echo "This tests if your computer can communicate with a device on your local network."
ping -c 4 192.168.1.1
echo ""

# Test connectivity to external IP
echo "2. Pinging external IP (8.8.8.8):"
echo "This tests if your computer can communicate with a server on the internet without using DNS."
ping -c 4 8.8.8.8
echo ""

# Test connectivity to a domain
echo "3. Pinging domain (google.com):"
echo "This tests if your computer can resolve a domain name to an IP address and communicate with the server."
ping -c 4 google.com
echo ""

# Step 3: Check Network Services
echo "Step 3: Checking Network Services:"

# Check listening services
echo "1. Checking listening services:"
echo "This shows all services currently listening for network connections on your computer."
ss -tuln
echo ""

# Check open connections
echo "2. Checking open connections:"
echo "This shows all current network connections to and from your computer."
ss -an
echo ""

# Step 4: Disconnect and Reconnect to the Network
echo "Step 4: Disconnecting and Reconnecting the Network Interface:"

# Disconnect network interface
echo "1. Disconnecting network interface (eth0):"
echo "This temporarily disables your network interface to simulate a network reset."
sudo ip link set eth0 down
sleep 5
echo ""

# Reconnect network interface
echo "2. Reconnecting network interface (eth0):"
echo "This re-enables your network interface."
sudo ip link set eth0 up
sleep 5
echo ""

# Step 5: Restart Network Services
echo "Step 5: Restarting Network Services:"

# Restart NetworkManager service
echo "1. Restarting NetworkManager:"
echo "This restarts the NetworkManager service, which manages network connections."
sudo systemctl restart NetworkManager
echo ""

# Conclusion
echo "Network debug completed."
echo "All steps are done. If you still have network issues, you may need to check your network hardware or contact your network administrator."
echo ""


# Make the script executable:
# chmod +x network_debug.sh

# Run the script:
# ./network_debug.sh
