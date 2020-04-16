/*
 * ZeroTier One - Network Virtualization Everywhere
 * Copyright (C) 2011-2019  ZeroTier, Inc.  https://www.zerotier.com/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * --
 *
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial closed-source software that incorporates or links
 * directly against ZeroTier software without disclosing the source code
 * of your own application.
 */

#ifndef ZT_OSXETHERNETTAP_HPP
#define ZT_OSXETHERNETTAP_HPP

#include <stdio.h>
#include <stdlib.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "../node/Constants.hpp"
#include "../node/MAC.hpp"
#include "../node/InetAddress.hpp"
#include "../node/MulticastGroup.hpp"
#include "../node/Mutex.hpp"

#include "Thread.hpp"

namespace ZeroTier {

class MacEthernetTap
{
public:
	MacEthernetTap(
		const char *homePath,
		const MAC &mac,
		unsigned int mtu,
		unsigned int metric,
		uint64_t nwid,
		const char *friendlyName,
		void (*handler)(void *,void *,uint64_t,const MAC &,const MAC &,unsigned int,unsigned int,const void *,unsigned int),
		void *arg);

	~MacEthernetTap();

	void setEnabled(bool en);
	bool enabled() const;
	bool addIp(const InetAddress &ip);
	bool removeIp(const InetAddress &ip);
	std::vector<InetAddress> ips() const;
	void put(const MAC &from,const MAC &to,unsigned int etherType,const void *data,unsigned int len);
	std::string deviceName() const;
	void setFriendlyName(const char *friendlyName);
	void scanMulticastGroups(std::vector<MulticastGroup> &added,std::vector<MulticastGroup> &removed);
	void setMtu(unsigned int mtu);

	void threadMain()
		throw();

private:
	void (*_handler)(void *,void *,uint64_t,const MAC &,const MAC &,unsigned int,unsigned int,const void *,unsigned int);
	void *_arg;
	uint64_t _nwid;
	Thread _thread;
	std::string _homePath;
	std::string _dev;
	std::vector<MulticastGroup> _multicastGroups;
	Mutex _putLock;
	unsigned int _mtu;
	unsigned int _metric;
	int _shutdownSignalPipe[2];
	int _agentStdin,_agentStdout,_agentStderr,_agentStdin2,_agentStdout2,_agentStderr2;
	long _agentPid;
	volatile bool _enabled;
};

} // namespace ZeroTier

#endif
