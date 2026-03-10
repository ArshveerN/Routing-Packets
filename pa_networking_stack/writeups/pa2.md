Programming Assignment 2 Writeup
====================

My name: Eshaan Bhathal

My UTORID : bhathale

I would like to credit/thank these classmates for their help: nagraar3

This programming assignment took me about 5 hours to do.

Program Structure and Design of the Router:
The router is built around two main classes: Router and AsyncNetworkInterface. The Router class maintains a vector of AsyncNetworkInterface objects (one per interface) and a routing table stored as a vector of RoutingTableEntry structs, where each entry holds a route prefix, prefix length, optional next hop address, and outgoing interface index. The add_route() method populates the routing table, and the route() method implements the core forwarding logic. For each interface, route() polls for incoming datagrams using maybe_receive(), checks the TTL (dropping packets with TTL <= 1), decrements the TTL, recomputes the IPv4 checksum, and then performs longest-prefix-match lookup over the routing table by building a bitmask from each entry's prefix length and comparing it against the destination address. The entry with the longest matching prefix is selected, and the datagram is forwarded to the appropriate interface using either the entry's next hop or the datagram's destination address for directly attached networks. AsyncNetworkInterface wraps the NetworkInterface from PA1, queuing received IPv4 datagrams asynchronously so the router can process them at its own pace.

Implementation Challenges:
The main challenge was correctly implementing the longest-prefix-match algorithm, specifically constructing the bitmask for each prefix length and handling the edge case where prefix_length is 0 (default route), which requires the mask to be 0 so that it matches any destination. I also had to be careful with the TTL logic to make sure packets with TTL of 1 are dropped rather than forwarded with a TTL of 0, and that the checksum is recomputed after modifying the TTL. Getting the next hop logic right was another challenge — determining when to use the routing table entry's next hop versus the datagram's final destination address for directly connected networks.

Remaining Bugs:
No known bugs. The router passes all provided test cases including same-network routing, multi-host routing, internet routing via default route, hierarchical subnet matching, and TTL expiration scenarios.

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
