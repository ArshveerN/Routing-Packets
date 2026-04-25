# Routing-Packets

An IPv4 router and network interface implementation in C++, built as part of a networking stack project. The router performs longest-prefix-match forwarding across multiple network interfaces, handling ARP resolution, TTL management, and datagram forwarding at the network layer.

## Overview

This project implements two core components of a simplified TCP/IP networking stack:

- **IP Router** — Routes IPv4 datagrams between multiple network interfaces using a configurable routing table and longest-prefix-match lookup.
- **Network Interface** — Bridges the IP (network) layer and Ethernet (link) layer, translating datagrams into Ethernet frames and resolving MAC addresses via ARP.

Together, these components simulate how a real router receives packets on one interface, determines the best outbound path, and forwards them toward their destination.

## How It Works

### Router (`router.cc` / `router.hh`)

The router maintains a routing table of prefix entries, each containing a route prefix, prefix length, optional next-hop address, and outbound interface index. When `route()` is called, the router:

1. Polls every interface for incoming datagrams.
2. Drops any datagram with a TTL ≤ 1 (preventing infinite loops).
3. Decrements the TTL and recomputes the IPv4 header checksum.
4. Performs a longest-prefix-match against the routing table to find the best forwarding rule.
5. Forwards the datagram out the matching interface — either to the next-hop gateway or directly to the destination if the network is directly attached.

### Network Interface (`network_interface.hh`)

The network interface sits at the bottom of the stack and handles:

- **Outbound**: Encapsulating IP datagrams into Ethernet frames, issuing ARP requests when the destination MAC address is unknown, and queuing frames until resolution completes.
- **Inbound**: Accepting Ethernet frames, responding to ARP requests with replies, learning address mappings from ARP traffic, and passing received IPv4 datagrams up the stack.

### WebGet (`webget.cc`)

A minimal HTTP client that opens a raw TCP socket, sends an HTTP/1.1 GET request, and prints the response — demonstrating the socket layer of the stack.

## Project Structure

```
pa_networking_stack/
├── src/
│   ├── router.cc              # Router forwarding logic
│   ├── router.hh              # Router & AsyncNetworkInterface classes
│   ├── network_interface.hh   # Network interface (IP ↔ Ethernet bridge)
│   └── webget.cc              # Simple HTTP GET client
├── tests/
│   ├── router_*.cc            # Router test scenarios
│   ├── net_interface_test_*.cc # Network interface tests
│   └── router_common.hh       # Shared test harness
├── util/                      # Supporting networking primitives
│   ├── ipv4_header.cc/hh      # IPv4 header parsing & serialization
│   ├── ethernet_header.cc/hh  # Ethernet frame headers
│   ├── arp_message.cc/hh      # ARP request/reply handling
│   ├── address.cc/hh          # IP address utilities
│   └── socket.cc/hh           # TCP/UDP socket wrappers
├── etc/                       # CMake build configuration
├── scripts/                   # Build helper scripts
└── CMakeLists.txt             # Top-level build file
```

## Building

```bash
cmake -S pa_networking_stack -B build
cmake --build build
```

## Running Tests

```bash
cd build
ctest --output-on-failure
```

Test scenarios cover same-network routing, multi-host forwarding, internet routing via default routes, hierarchical subnet matching, and TTL expiration.

## Key Concepts

| Concept | Description |
|---|---|
| **Longest-Prefix Match** | The router selects the routing table entry whose prefix matches the most significant bits of the destination address. |
| **TTL (Time-To-Live)** | Each hop decrements the TTL; packets reaching TTL ≤ 1 are dropped to prevent routing loops. |
| **ARP Resolution** | The network interface maps IP addresses to Ethernet MAC addresses, queuing outbound frames while waiting for replies. |
| **Checksum Recomputation** | Modifying the TTL invalidates the IPv4 header checksum, so it must be recalculated before forwarding. |

## Tech Stack

- **Language**: C++20
- **Build System**: CMake
- **Platform**: Linux (Ubuntu)
