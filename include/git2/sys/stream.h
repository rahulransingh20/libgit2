/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_sys_git_stream_h__
#define INCLUDE_sys_git_stream_h__

#include "git2/common.h"
#include "git2/types.h"
#include "git2/proxy.h"

GIT_BEGIN_DECL

#define GIT_STREAM_VERSION 1

/**
 * Every stream must have this struct as its first element, so the
 * API can talk to it. You'd define your stream as
 *
 *     struct my_stream {
 *             git_stream parent;
 *             ...
 *     }
 *
 * and fill the functions
 */
typedef struct git_stream {
	int version;

	int encrypted;
	int proxy_support;
	int (*connect)(struct git_stream *);
	int (*certificate)(git_cert **, struct git_stream *);
	int (*set_proxy)(struct git_stream *, const git_proxy_options *proxy_opts);
	ssize_t (*read)(struct git_stream *, void *, size_t);
	ssize_t (*write)(struct git_stream *, const char *, size_t, int);
	int (*close)(struct git_stream *);
	void (*free)(struct git_stream *);
} git_stream;

typedef struct {
	/** The `version` field should be set to `GIT_STREAM_VERSION`. */
	int version;

	/**
	 * Called to create a new connection to a given host.
	 *
	 * @param out The created stream
	 * @param host The hostname to connect to; may be a hostname or
	 *             IP address
	 * @param port The port to connect to; may be a port number or
	 *             service name
	 * @return 0 or an error code
	 */
	int (*init)(git_stream **out, const char *host, const char *port);

	/**
	 * Called to create a new connection on top of the given stream.  If
	 * this is a TLS stream, then this function may be used to proxy a
	 * TLS stream over an HTTP CONNECT session.  If this is unset, then
	 * HTTP CONNECT proxies will not be supported.
	 *
	 * @param out The created stream
	 * @param in An existing stream to add TLS to
	 * @param host The hostname that the stream is connected to,
	 *             for certificate validation
	 * @return 0 or an error code
	 */
	int (*wrap)(git_stream **out, git_stream *in, const char *host);
} git_stream_registration;

/**
 * Register stream constructors for the library to use
 *
 * If a registration structure is already set, it will be overwritten.
 * Pass `NULL` in order to deregister the current constructor and return
 * to the system defaults.
 *
 * @param registration the registration data
 * @param tls 1 if the registration is for TLS streams, 0 for regular
 *        (insecure) sockets
 * @return 0 or an error code
 */
GIT_EXTERN(int) git_stream_register(
	git_stream_registration *registration, int tls);

/** @name Deprecated TLS Stream Registration Funtions
 *
 * These typedefs and functions are retained for backward compatibility.
 * The newer versions of these functions and structures should be preferred
 * in all new code.
 */

/**@{*/

/**
 * @deprecated Provide a git_stream_registration to git_stream_register
 * @see git_stream_registration
 */
typedef int (*git_stream_cb)(git_stream **out, const char *host, const char *port);

/**
 * Register a TLS stream constructor for the library to use.  This stream
 * will not support HTTP CONNECT proxies.
 *
 * @deprecated Provide a git_stream_registration to git_stream_register
 * @see git_stream_register
 */
GIT_EXTERN(int) git_stream_register_tls(git_stream_cb ctor);

 /**@}*/

GIT_END_DECL

#endif
