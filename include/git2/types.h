/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_git_types_h__
#define INCLUDE_git_types_h__

#include "common.h"

/**
 * @file git2/types.h
 * @brief libgit2 base & compatibility types
 * @ingroup Git
 * @{
 */
GIT_BEGIN_DECL

/**
 * Cross-platform compatibility types for off_t / time_t
 *
 * NOTE: This needs to be in a public header so that both the library
 * implementation and client applications both agree on the same types.
 * Otherwise we get undefined behavior.
 *
 * Use the "best" types that each platform provides. Currently we truncate
 * these intermediate representations for compatibility with the git ABI, but
 * if and when it changes to support 64 bit types, our code will naturally
 * adapt.
 * NOTE: These types should match those that are returned by our internal
 * stat() functions, for all platforms.
 */
#include <sys/types.h>
#ifdef __amigaos4__
#include <stdint.h>
#endif

#if defined(_MSC_VER)

typedef __int64 git_off_t;
typedef __time64_t git_time_t;

#elif defined(__MINGW32__)

typedef off64_t git_off_t;
typedef __time64_t git_time_t;

#elif defined(__HAIKU__)

typedef __haiku_std_int64 git_off_t;
typedef __haiku_std_int64 git_time_t;

#else /* POSIX */

/*
 * Note: Can't use off_t since if a client program includes <sys/types.h>
 * before us (directly or indirectly), they'll get 32 bit off_t in their client
 * app, even though /we/ define _FILE_OFFSET_BITS=64.
 */
typedef int64_t git_off_t;
typedef int64_t git_time_t;

#endif

#include "buffer.h"
#include "oid.h"

/** Basic type (loose or packed) of any Git object. */
typedef enum {
	GIT_OBJ_ANY = -2,		/**< Object can be any of the following */
	GIT_OBJ_BAD = -1,		/**< Object is invalid. */
	GIT_OBJ__EXT1 = 0,		/**< Reserved for future use. */
	GIT_OBJ_COMMIT = 1,		/**< A commit object. */
	GIT_OBJ_TREE = 2,		/**< A tree (directory listing) object. */
	GIT_OBJ_BLOB = 3,		/**< A file revision object. */
	GIT_OBJ_TAG = 4,		/**< An annotated tag object. */
	GIT_OBJ__EXT2 = 5,		/**< Reserved for future use. */
	GIT_OBJ_OFS_DELTA = 6, /**< A delta, base is given by an offset. */
	GIT_OBJ_REF_DELTA = 7, /**< A delta, base is given by object id. */
} git_otype;

/** An open object database handle. */
typedef struct git_odb git_odb;

/** A custom backend in an ODB */
typedef struct git_odb_backend git_odb_backend;

/** An object read from the ODB */
typedef struct git_odb_object git_odb_object;

/** A stream to read/write from the ODB */
typedef struct git_odb_stream git_odb_stream;

/** A stream to write a packfile to the ODB */
typedef struct git_odb_writepack git_odb_writepack;

/** An open refs database handle. */
typedef struct git_refdb git_refdb;

/** A custom backend for refs */
typedef struct git_refdb_backend git_refdb_backend;

/**
 * Representation of an existing git repository,
 * including all its object contents
 */
typedef struct git_repository git_repository;

/** Representation of a working tree */
typedef struct git_worktree git_worktree;

/** Representation of a generic object in a repository */
typedef struct git_object git_object;

/** Representation of an in-progress walk through the commits in a repo */
typedef struct git_revwalk git_revwalk;

/** Parsed representation of a tag object. */
typedef struct git_tag git_tag;

/** In-memory representation of a blob object. */
typedef struct git_blob git_blob;

/** Parsed representation of a commit object. */
typedef struct git_commit git_commit;

/** Representation of each one of the entries in a tree object. */
typedef struct git_tree_entry git_tree_entry;

/** Representation of a tree object. */
typedef struct git_tree git_tree;

/** Constructor for in-memory trees */
typedef struct git_treebuilder git_treebuilder;

/** Memory representation of an index file. */
typedef struct git_index git_index;

/** An iterator for conflicts in the index. */
typedef struct git_index_conflict_iterator git_index_conflict_iterator;

/** Memory representation of a set of config files */
typedef struct git_config git_config;

/** Interface to access a configuration file */
typedef struct git_config_backend git_config_backend;

/** Representation of a reference log entry */
typedef struct git_reflog_entry git_reflog_entry;

/** Representation of a reference log */
typedef struct git_reflog git_reflog;

/** Representation of a git note */
typedef struct git_note git_note;

/** Representation of a git packbuilder */
typedef struct git_packbuilder git_packbuilder;

/** Time in a signature */
typedef struct git_time {
	git_time_t time; /**< time in seconds from epoch */
	int offset; /**< timezone offset, in minutes */
	char sign; /**< indicator for questionable '-0000' offsets in signature */
} git_time;

/** An action signature (e.g. for committers, taggers, etc) */
typedef struct git_signature {
	char *name; /**< full name of the author */
	char *email; /**< email of the author */
	git_time when; /**< time when the action happened */
} git_signature;

/** In-memory representation of a reference. */
typedef struct git_reference git_reference;

/** Iterator for references */
typedef struct git_reference_iterator  git_reference_iterator;

/** Transactional interface to references */
typedef struct git_transaction git_transaction;

/** Annotated commits, the input to merge and rebase. */
typedef struct git_annotated_commit git_annotated_commit;

/** Representation of a status collection */
typedef struct git_status_list git_status_list;

/** Representation of a rebase */
typedef struct git_rebase git_rebase;

/** Basic type of any Git reference. */
typedef enum {
	GIT_REF_INVALID = 0, /**< Invalid reference */
	GIT_REF_OID = 1, /**< A reference which points at an object id */
	GIT_REF_SYMBOLIC = 2, /**< A reference which points at another reference */
	GIT_REF_LISTALL = GIT_REF_OID|GIT_REF_SYMBOLIC,
} git_ref_t;

/** Basic type of any Git branch. */
typedef enum {
	GIT_BRANCH_LOCAL = 1,
	GIT_BRANCH_REMOTE = 2,
	GIT_BRANCH_ALL = GIT_BRANCH_LOCAL|GIT_BRANCH_REMOTE,
} git_branch_t;

/** Valid modes for index and tree entries. */
typedef enum {
	GIT_FILEMODE_UNREADABLE          = 0000000,
	GIT_FILEMODE_TREE                = 0040000,
	GIT_FILEMODE_BLOB                = 0100644,
	GIT_FILEMODE_BLOB_EXECUTABLE     = 0100755,
	GIT_FILEMODE_LINK                = 0120000,
	GIT_FILEMODE_COMMIT              = 0160000,
} git_filemode_t;

/**
 * A refspec specifies the mapping between remote and local reference
 * names when fetch or pushing.
 */
typedef struct git_refspec git_refspec;

/**
 * Git's idea of a remote repository. A remote can be anonymous (in
 * which case it does not have backing configuration entires).
 */
typedef struct git_remote git_remote;

/**
 * Interface which represents a transport to communicate with a
 * remote.
 */
typedef struct git_transport git_transport;

/**
 * Preparation for a push operation. Can be used to configure what to
 * push and the level of parallelism of the packfile builder.
 */
typedef struct git_push git_push;

/* documentation in the definition */
typedef struct git_remote_head git_remote_head;
typedef struct git_remote_callbacks git_remote_callbacks;

/**
 * This is passed as the first argument to the callback to allow the
 * user to see the progress.
 *
 * - total_objects: number of objects in the packfile being downloaded
 * - indexed_objects: received objects that have been hashed
 * - received_objects: objects which have been downloaded
 * - local_objects: locally-available objects that have been injected
 *    in order to fix a thin pack.
 * - received-bytes: size of the packfile received up to now
 */
typedef struct git_transfer_progress {
	unsigned int total_objects;
	unsigned int indexed_objects;
	unsigned int received_objects;
	unsigned int local_objects;
	unsigned int total_deltas;
	unsigned int indexed_deltas;
	size_t received_bytes;
} git_transfer_progress;

/**
 * Type for progress callbacks during indexing.  Return a value less than zero
 * to cancel the transfer.
 *
 * @param stats Structure containing information about the state of the transfer
 * @param payload Payload provided by caller
 */
typedef int (*git_transfer_progress_cb)(const git_transfer_progress *stats, void *payload);

/**
 * Type for messages delivered by the transport.  Return a negative value
 * to cancel the network operation.
 *
 * @param str The message from the transport
 * @param len The length of the message
 * @param payload Payload provided by the caller
 */
typedef int (*git_transport_message_cb)(const char *str, int len, void *payload);


/**
 * Type of host certificate structure that is passed to the check callback
 */
typedef enum git_cert_t {
	/**
	 * No information about the certificate is available. This may
	 * happen when using curl.
	 */
	GIT_CERT_NONE,
        /**
         * The `data` argument to the callback will be a pointer to
         * the DER-encoded data.
         */
	GIT_CERT_X509,
        /**
         * The `data` argument to the callback will be a pointer to a
         * `git_cert_hostkey` structure.
         */
	GIT_CERT_HOSTKEY_LIBSSH2,
	/**
	 * The `data` argument to the callback will be a pointer to a
	 * `git_strarray` with `name:content` strings containing
	 * information about the certificate. This is used when using
	 * curl.
	 */
	GIT_CERT_STRARRAY,
} git_cert_t;

/**
 * Parent type for `git_cert_hostkey` and `git_cert_x509`.
 */
typedef struct {
	/**
	 * Type of certificate. A `GIT_CERT_` value.
	 */
	git_cert_t cert_type;
} git_cert;

/**
 * Callback for the user's custom certificate checks.
 *
 * @param cert The host certificate
 * @param valid Whether the libgit2 checks (OpenSSL or WinHTTP) think
 * this certificate is valid
 * @param host Hostname of the host libgit2 connected to
 * @param payload Payload provided by the caller
 * @return 0 to proceed with the connection, < 0 to fail the connection
 *         or > 0 to indicate that the callback refused to act and that
 *         the existing validity determination should be honored
 */
typedef int (*git_transport_certificate_check_cb)(git_cert *cert, int valid, const char *host, void *payload);

/**
 * Opaque structure representing a submodule.
 */
typedef struct git_submodule git_submodule;

/**
 * Submodule update values
 *
 * These values represent settings for the `submodule.$name.update`
 * configuration value which says how to handle `git submodule update` for
 * this submodule.  The value is usually set in the ".gitmodules" file and
 * copied to ".git/config" when the submodule is initialized.
 *
 * You can override this setting on a per-submodule basis with
 * `git_submodule_set_update()` and write the changed value to disk using
 * `git_submodule_save()`.  If you have overwritten the value, you can
 * revert it by passing `GIT_SUBMODULE_UPDATE_RESET` to the set function.
 *
 * The values are:
 *
 * - GIT_SUBMODULE_UPDATE_CHECKOUT: the default; when a submodule is
 *   updated, checkout the new detached HEAD to the submodule directory.
 * - GIT_SUBMODULE_UPDATE_REBASE: update by rebasing the current checked
 *   out branch onto the commit from the superproject.
 * - GIT_SUBMODULE_UPDATE_MERGE: update by merging the commit in the
 *   superproject into the current checkout out branch of the submodule.
 * - GIT_SUBMODULE_UPDATE_NONE: do not update this submodule even when
 *   the commit in the superproject is updated.
 * - GIT_SUBMODULE_UPDATE_DEFAULT: not used except as static initializer
 *   when we don't want any particular update rule to be specified.
 */
typedef enum {
	GIT_SUBMODULE_UPDATE_CHECKOUT = 1,
	GIT_SUBMODULE_UPDATE_REBASE   = 2,
	GIT_SUBMODULE_UPDATE_MERGE    = 3,
	GIT_SUBMODULE_UPDATE_NONE     = 4,

	GIT_SUBMODULE_UPDATE_DEFAULT  = 0
} git_submodule_update_t;

/**
 * Submodule ignore values
 *
 * These values represent settings for the `submodule.$name.ignore`
 * configuration value which says how deeply to look at the working
 * directory when getting submodule status.
 *
 * You can override this value in memory on a per-submodule basis with
 * `git_submodule_set_ignore()` and can write the changed value to disk
 * with `git_submodule_save()`.  If you have overwritten the value, you
 * can revert to the on disk value by using `GIT_SUBMODULE_IGNORE_RESET`.
 *
 * The values are:
 *
 * - GIT_SUBMODULE_IGNORE_UNSPECIFIED: use the submodule's configuration
 * - GIT_SUBMODULE_IGNORE_NONE: don't ignore any change - i.e. even an
 *   untracked file, will mark the submodule as dirty.  Ignored files are
 *   still ignored, of course.
 * - GIT_SUBMODULE_IGNORE_UNTRACKED: ignore untracked files; only changes
 *   to tracked files, or the index or the HEAD commit will matter.
 * - GIT_SUBMODULE_IGNORE_DIRTY: ignore changes in the working directory,
 *   only considering changes if the HEAD of submodule has moved from the
 *   value in the superproject.
 * - GIT_SUBMODULE_IGNORE_ALL: never check if the submodule is dirty
 * - GIT_SUBMODULE_IGNORE_DEFAULT: not used except as static initializer
 *   when we don't want any particular ignore rule to be specified.
 */
typedef enum {
	GIT_SUBMODULE_IGNORE_UNSPECIFIED  = -1, /**< use the submodule's configuration */

	GIT_SUBMODULE_IGNORE_NONE      = 1,  /**< any change or untracked == dirty */
	GIT_SUBMODULE_IGNORE_UNTRACKED = 2,  /**< dirty if tracked files change */
	GIT_SUBMODULE_IGNORE_DIRTY     = 3,  /**< only dirty if HEAD moved */
	GIT_SUBMODULE_IGNORE_ALL       = 4,  /**< never dirty */
} git_submodule_ignore_t;

/**
 * Options for submodule recurse.
 *
 * Represent the value of `submodule.$name.fetchRecurseSubmodules`
 *
 * * GIT_SUBMODULE_RECURSE_NO    - do no recurse into submodules
 * * GIT_SUBMODULE_RECURSE_YES   - recurse into submodules
 * * GIT_SUBMODULE_RECURSE_ONDEMAND - recurse into submodules only when
 *                                    commit not already in local clone
 */
typedef enum {
	GIT_SUBMODULE_RECURSE_NO = 0,
	GIT_SUBMODULE_RECURSE_YES = 1,
	GIT_SUBMODULE_RECURSE_ONDEMAND = 2,
} git_submodule_recurse_t;

typedef struct git_writestream git_writestream;

/** A type to write in a streaming fashion, for example, for filters. */
struct git_writestream {
	int (*write)(git_writestream *stream, const char *buffer, size_t len);
	int (*close)(git_writestream *stream);
	void (*free)(git_writestream *stream);
};

/** Representation of .mailmap file state. */
typedef struct git_mailmap git_mailmap;

/** @} */
GIT_END_DECL

#endif
