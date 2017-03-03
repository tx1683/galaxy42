#pragma once
#ifndef cable_UDP_ADDR_HPP
#define cable_UDP_ADDR_HPP

#include "../base/cable_base_addr.hpp"
#include <boost/asio.hpp>

class c_cable_udp_addr final : public c_cable_base_addr {
	public:
		typedef boost::asio::ip::udp::endpoint t_addr; ///< actuall raw address. Instace of this will be in m_addrdata

		c_cable_udp_addr() = default;
		c_cable_udp_addr(const t_addr &endpoint);
		c_cable_udp_addr(const std::string &ip_string); ///< Converts string like "100.200.50.50:32000"
		void print(std::ostream & ostr) const override;
		/// is my address the same (cable type, and address) to another.
		bool is_same(const c_cable_base_addr &other) const override;

		/// return -1 if I am smaller, 0 if same, +1 if bigger, then the other address. Compares also across different cable-types
		int compare(const c_cable_base_addr &other) const override;
};

#endif // cable_UDP_ADDR_HPP
