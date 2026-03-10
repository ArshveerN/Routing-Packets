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
void Router::add_route( const uint32_t route_prefix,
                        const uint8_t prefix_length,
                        const optional<Address> next_hop,
                        const size_t interface_num )
{
  cerr << "DEBUG: adding route " << Address::from_ipv4_numeric( route_prefix ).ip() << "/"
       << static_cast<int>( prefix_length ) << " => " << ( next_hop.has_value() ? next_hop->ip() : "(direct)" )
       << " on interface " << interface_num << "\n";

  routing_table_.push_back( { route_prefix, prefix_length, next_hop, interface_num } );
}

void Router::route()
{
  for ( size_t i = 0; i < interfaces_.size(); ++i ) {
    auto optional_dgram = interfaces_[i].maybe_receive();
    while ( optional_dgram.has_value() ) {
      auto& dgram = optional_dgram.value();

      // Drop if TTL is 0 or will be 0 after decrement
      if ( dgram.header.ttl <= 1 ) {
        optional_dgram = interfaces_[i].maybe_receive();
        continue;
      }

      // Decrement TTL and recompute checksum
      dgram.header.ttl--;
      dgram.header.compute_checksum();

      uint32_t dst = dgram.header.dst;

      // Find best (longest prefix) matching route
      int best_prefix = -1;
      size_t best_idx = 0;

      for ( size_t r = 0; r < routing_table_.size(); ++r ) {
        uint8_t plen = routing_table_[r].prefix_length;

        // Build mask: prefix_length 0 means match everything (default route)
        uint32_t mask = 0;
        if ( plen > 0 ) {
          mask = 0xFFFFFFFF << ( 32 - plen );
        }

        if ( ( dst & mask ) == ( routing_table_[r].route_prefix & mask ) ) {
          if ( static_cast<int>(plen) > best_prefix ) {
            best_prefix = static_cast<int>(plen);
            best_idx = r;
          }
        }
      }

      // If we found a match, forward the packet
      if ( best_prefix >= 0 ) {
        auto& best = routing_table_[best_idx];
        Address next = Address::from_ipv4_numeric(dst);
        if ( best.next_hop.has_value() ) {
          next = best.next_hop.value();
        }
        interfaces_[best.interface_num].send_datagram(dgram,next );
      }
      // else: no match, drop the packet

      optional_dgram = interfaces_[i].maybe_receive();
    }
  }
}

