//
// DNS.h
//
// Library: Net
// Package: NetCore
// Module:  DNS
//
// Definition of the DNS class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_DNS_INCLUDED
#define Net_DNS_INCLUDED


#include "Net.h"
#include "SocketDefs.h"
#include "IPAddress.h"
#include "HostEntry.h"


namespace NetCom {



class Foundation_API DNS
	/// This class provides an interface to the
	/// domain name service.
	///
	/// An internal DNS cache is used to speed up name lookups.
{
public:
	enum HintFlag
	{
		DNS_HINT_NONE           = 0,
#ifdef POCO_HAVE_ADDRINFO
		DNS_HINT_AI_PASSIVE     = AI_PASSIVE,     /// Socket address will be used in bind() call
		DNS_HINT_AI_CANONNAME   = AI_CANONNAME,   /// Return canonical name in first ai_canonname
		DNS_HINT_AI_NUMERICHOST = AI_NUMERICHOST, /// Nodename must be a numeric address string
		DNS_HINT_AI_NUMERICSERV = AI_NUMERICSERV, /// Servicename must be a numeric port number
		DNS_HINT_AI_ALL         = AI_ALL,         /// Query both IP6 and IP4 with AI_V4MAPPED
		DNS_HINT_AI_ADDRCONFIG  = AI_ADDRCONFIG,  /// Resolution only if global address configured
		DNS_HINT_AI_V4MAPPED    = AI_V4MAPPED     /// On v6 failure, query v4 and convert to V4MAPPED format	
#endif
	};

	static HostEntry hostByName(const std::string& hostname, unsigned hintFlags =
#ifdef POCO_HAVE_ADDRINFO
		DNS_HINT_AI_CANONNAME | DNS_HINT_AI_ADDRCONFIG
#else
		DNS_HINT_NONE
#endif		
		);
		/// Returns a HostEntry object containing the DNS information
		/// for the host with the given name. HintFlag argument is only
		/// used on platforms that have getaddrinfo().
		///
		/// Throws a HostNotFoundException if a host with the given
		/// name cannot be found.
		///
		/// Throws a NoAddressFoundException if no address can be
		/// found for the hostname.
		///
		/// Throws a DNSException in case of a general DNS error.
		///
		/// Throws an IOException in case of any other error.
		
	static HostEntry hostByAddress(const IPAddress& address, unsigned hintFlags =
#ifdef POCO_HAVE_ADDRINFO
		DNS_HINT_AI_CANONNAME | DNS_HINT_AI_ADDRCONFIG
#else
		DNS_HINT_NONE
#endif		
		);
		/// Returns a HostEntry object containing the DNS information
		/// for the host with the given IP address. HintFlag argument is only
		/// used on platforms that have getaddrinfo().
		///
		/// Throws a HostNotFoundException if a host with the given
		/// name cannot be found.
		///
		/// Throws a DNSException in case of a general DNS error.
		///
		/// Throws an IOException in case of any other error.

	static HostEntry resolve(const std::string& address);
		/// Returns a HostEntry object containing the DNS information
		/// for the host with the given IP address or host name.
		///
		/// Throws a HostNotFoundException if a host with the given
		/// name cannot be found.
		///
		/// Throws a NoAddressFoundException if no address can be
		/// found for the hostname.
		///
		/// Throws a DNSException in case of a general DNS error.
		///
		/// Throws an IOException in case of any other error.
		
	static IPAddress resolveOne(const std::string& address);
		/// Convenience method that calls resolve(address) and returns 
		/// the first address from the HostInfo.

	static HostEntry thisHost();
		/// Returns a HostEntry object containing the DNS information
		/// for this host.
		///
		/// Throws a HostNotFoundException if DNS information 
		/// for this host cannot be found.
		///
		/// Throws a NoAddressFoundException if no address can be
		/// found for this host.
		///
		/// Throws a DNSException in case of a general DNS error.
		///
		/// Throws an IOException in case of any other error.

	static void reload();
		/// Reloads the resolver configuration.
		///
		/// This method will call res_init() if the Net library
		/// has been compiled with -DPOCO_HAVE_LIBRESOLV. Otherwise
		/// it will do nothing.

	//@ deprecated
	static void flushCache();
		/// Flushes the internal DNS cache.
		///
		/// As of 1.4.2, the DNS cache is no longer used
		/// and this method does not do anything.
		
	static std::string hostName();
		/// Returns the host name of this host.

protected:
	static int lastError();
		/// Returns the code of the last error.
		
	static void error(int code, const std::string& arg);
		/// Throws an exception according to the error code.

	static void aierror(int code, const std::string& arg);
		/// Throws an exception according to the getaddrinfo() error code.
};


 } // namespace Poco::Net


#endif // Net_DNS_INCLUDED
