#include "router.hh"

#include <iostream>
#include <limits>

using namespace std;

// route_prefix: The "up-to-32-bit" IPv4 address prefix to match the datagram's destination address against
// prefix_length: For this route to be applicable, how many high-order (most-significant) bits of
//    the route_prefix will need to match the corresponding bits of the datagram's destination address?
// next_hop: The IP address of the next hop. Will be empty if the network is directly attached to the router (in
//    which case, the next hop address should be the datagram's final destination).
// interface_num: The index of the interface to send the datagram out on.
void Router::add_route(const uint32_t route_prefix,
                        const uint8_t prefix_length,
                        const optional<Address> next_hop,
                        const size_t interface_num)
{
  routing_table_.push_back({ route_prefix, prefix_length, next_hop, interface_num });
}

void Router::route()
{
  for (size_t i = 0; i < interfaces_.size(); ++i) {
    auto incoming_data = interfaces_[i].maybe_receive();
    while (incoming_data.has_value()) {
      auto& dgram = incoming_data.value();

      if (dgram.header.ttl > 1){
        // Decrement TTL and recompute checksum
        dgram.header.ttl--;
        dgram.header.compute_checksum();

        uint32_t dst = dgram.header.dst;

        // Find best route
        int b_prefix = -1;
        size_t b_index = 0;

        for (size_t r = 0; r < routing_table_.size(); ++r) {
          int p_len = routing_table_[r].prefix_length;

          // Build mask: prefix_length 0 means match everything (default route)
          uint32_t mask = 0;
          if (p_len > 0) {
            mask = 0xFFFFFFFF << (32 - p_len);
          }

          if ((dst & mask) == (routing_table_[r].route_prefix & mask)) {
            if (p_len > b_prefix) {
              b_prefix = p_len;
              b_index = r;
            }
          }
        }

        // If we found a match, forward the packet
        if (b_prefix >= 0) {
          auto& b = routing_table_[b_index];
          Address next = Address::from_ipv4_numeric(dst);
          if (b.next_hop.has_value()) {
            next = b.next_hop.value();
          }
          interfaces_[b.interface_num].send_datagram(dgram,next);
        }
        // else: no match, drop the packet
        incoming_data = interfaces_[i].maybe_receive();
      }
      else if (dgram.header.ttl <= 1) {
        incoming_data = interfaces_[i].maybe_receive();
        continue;
      }
    }
  }
}

