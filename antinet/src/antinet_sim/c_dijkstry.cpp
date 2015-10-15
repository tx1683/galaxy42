#include"c_dijkstry.hpp"


void c_dijkstry01::find_route() {
	m_nodes_routed.insert(getID(m_start));
	if(getID(m_start) == getID(m_target)) {
		std::cout << "No route needed! : " << "start_point == end_point" << std::endl;
		return;
	}
	empl_nics_toMap(m_start,0);

	for(m_map_it = m_map_ofRoute.begin();
		m_map_it != m_map_ofRoute.end() && !m_route_ok; ++m_map_it) {
		c_osi2_switch &next_switch = (m_map_it->second.second).get_my_switch();
		empl_nics_toMap(next_switch,m_map_it->first);
		m_nodes_routed.insert(getID(next_switch));
	}
}

void c_dijkstry01::empl_nics_toMap(c_osi2_switch &sw, t_osi2_cost cost_to) {
	for(size_t i = 0; i <= sw.get_last_nic_index(); ++i) {

		c_osi2_nic &nic = sw.get_nic(i);
		t_osi2_cost cost;
		c_osi2_nic * nic2 = nic.get_connected_card_or_null(cost);
		if(!nic2) {
			continue;
		}
		std::pair<c_osi2_nic&,c_osi2_nic&>local_pair(nic,*nic2);
		if(std::find(m_nodes_routed.begin(),m_nodes_routed.end(),getID(nic2->get_my_switch())) == m_nodes_routed.end()) {
			m_map_ofRoute.emplace(cost+cost_to,local_pair);
			m_nodes_routed.insert(getID(nic2->get_my_switch()));
			//std::cout << "adding2 " << getID(nic2->get_my_switch()) << std::endl; // dbg
		}
		if (getID(*nic2) == getID(m_target)) {
			std::cout << "Found route!" << std::endl;
			m_route_ok = true;
			continue;
		}
	}
}

void c_dijkstry01::print_all() {
	std::cout << "check all map:" << std::endl;
	for(auto &ptr_object : m_map_ofRoute) {
		std::cout << "cost: " << ptr_object.first
			  << " for switch: " << getID(ptr_object.second.first) << " -> " << getID(ptr_object.second.second)
			  << "\n\tfor nic: " << ptr_object.second.first << " -> " << ptr_object.second.second << std::endl;
	}
}

void c_dijkstry01::print_uuid_route() {
	std::list<t_osi3_uuid> route;
	if(!m_map_ofRoute.empty()) {
	m_map_it = --m_map_ofRoute.end();
	route.emplace_front(getID(m_map_it->second.second));
	t_osi3_uuid searched = getID(m_map_it->second.first);
	while(m_map_it != m_map_ofRoute.begin()) {
		if(searched == getID(m_map_it->second.second)) {
			route.emplace_front(getID(m_map_it->second.second));
			searched = getID(m_map_it->second.first);
		}
		m_map_it--;
	}
	if(searched == getID(m_map_it->second.first)) {
		route.emplace_front(searched);
	} else {
		route.emplace_front(searched);
		route.emplace_front(getID(m_map_it->second.first));
	}
	} else {
		route.emplace_front(getID(m_start));
	}

	std::cout << "route by switch uuid: " << std::endl;
	for(auto &id : route) {
		std::cout << id << " --> ";
	}
	std::cout << "the_end" << std::endl;

}

void c_dijkstry01::print_name_route(c_world &world) {
	std::list<t_osi3_uuid> route;
	if(!m_map_ofRoute.empty()) {
		m_map_it = --m_map_ofRoute.end();
		route.emplace_front(getID(m_map_it->second.second));
		t_osi3_uuid searched = getID(m_map_it->second.first);
		while(m_map_it != m_map_ofRoute.begin()) {
			if(searched == getID(m_map_it->second.second)) {
				route.emplace_front(getID(m_map_it->second.second));
				searched = getID(m_map_it->second.first);
			}
				m_map_it--;
		}
		if(searched == getID(m_map_it->second.first)) {
			route.emplace_front(searched);
		} else {
			route.emplace_front(searched);
			route.emplace_front(getID(m_map_it->second.first));
		}
	} else {
		route.emplace_front(getID(m_start));
	}
	std::cout << "route by switch name: " << std::endl;
	for(auto &id : route) {
		std::cout << world.find_object_by_uuid_as_object(id).get_name() << " --> ";
	}
	std::cout << "the_end" << std::endl;

}
