#ifndef _INETADDRESS_H_
#define _INETADDRESS_H_

#include "../Base/Common.h"
#include <netinet/in.h>


class InetAddress : smuduo::copyable {
public:
	//InetAddress()

	explicit InetAddress(uint16_t port, bool loopbackOnly = false); //For listening

	explicit InetAddress(const struct sockaddr_in &saddr_)
			: saddr_(saddr_) {}

	//default copy/assignment are Okay


	const struct sockaddr_in &getSockAddrInet() const {
		return saddr_;
	}

	void setSockAddrInet(const struct sockaddr_in &saddr) {
		saddr_ = saddr;
	}


	std::string toIpPort() const;


private:
//	static bool resolveSlow(const char *hostname, InetAddress *);

//	explicit InetAddress(smuduo::StringArg ipPort); // “1.2.3.4：5678”

	InetAddress(smuduo::StringArg ip, uint16_t port);


	uint32_t ipNetEndian() const { return saddr_.sin_addr.s_addr; }

	uint16_t portNetEndian() const { return saddr_.sin_port; }

	uint16_t portHostEndian() const { return ntohs(saddr_.sin_port); }

	bool operator==(const InetAddress &rhs) const {
		return saddr_.sin_family == rhs.saddr_.sin_family
			   && ipNetEndian() == rhs.ipNetEndian()
			   && portNetEndian() == rhs.portHostEndian();
	}


	sockaddr_in saddr_;
};


#endif