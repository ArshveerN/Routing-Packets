Programming Assignment 2 Writeup
====================

My name: Eshaan Bhathal

My UTORID : bhathale

I would like to credit/thank these classmates for their help: nagraar3

This programming assignment took me about 5 hours to do.

Program Structure and Design of the Router:
The router is built around two main classes: Router and AsyncNetworkInterface. The Router class contains a vector of AsyncNetworkInterface objects (each representing a network interface of the router) and a routing table (a vector of RoutingTableEntry structs). Each RoutingTableEntry struct contains the prefix of a route, the length of that prefix, a next-hop address for the route, and the index of the interface through which the route should be sent. The routing table is populated via the add_route() method, and the route() method implements the logic for routing packets. The route() method polls each interface for datagrams, checks the Time-To-Live (TTL) field of each incoming datagram (dropping datagrams with TTL <= 1), decrements the TTL field, recalculates the datagram’s header checksum, and performs a longest-prefix-match lookup against the routing table. Datagrams are forwarded to the interface indicated in the routing table entry that matched the longest prefix. The AsyncNetworkInterface class is similar to the NetworkInterface class from PA1, except that received datagrams are queued asynchronously so that the router can process them at its own pace.

Implementation Challenges:
The main challenge with implementing the functionality was correctly implementing the longest-prefix-match algorithm. This included creating the bitmask for each prefix length and handling the case where the prefix length was 0 (the default route), which required the mask to be 0 so that it would match any destination. Additionally, the implementation of the TTL (time to live) logic was another challenge. Packets with a TTL of 1 should be dropped rather than forwarded with a TTL of 0, and the checksum should be recomputed when the TTL is modified. Finally, another challenge was implementing the logic that determined whether a packet should use the next hop address from the routing table or the destination address of the datagram if it was a directly connected network.

Remaining Bugs:
No known bugs. The router passes all provided test cases, including same-network routing, multi-host routing, internet routing via default route, hierarchical subnet matching, and TTL expiration scenarios.

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
