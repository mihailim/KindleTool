/*
**  KindleTool, create.h
**
**  Copyright (C) 2011-2012  Yifan Lu
**  Copyright (C) 2012-2023  NiLuJe
**  Concept based on an original Python implementation by Igor Skochinsky & Jean-Yves Avenard,
**    cf., http://www.mobileread.com/forums/showthread.php?t=63225
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __KINDLETOOL_CREATE_H
#define __KINDLETOOL_CREATE_H

#include "kindle_tool.h"

typedef struct
{
	// NOTE: We resort to a nonstring (and non-portable, heh!) attribute because it's true, this isn't NULL terminated,
	//       so this easily shuts up the many, many, GCC 8 strncpy warnings...
	//       This is helpful in the few cases where we do want to keep using strncpy instead of memcpy,
	//       because of its NULL-padding assurance.
	char                   magic_number[MAGIC_NUMBER_LENGTH] __attribute__((nonstring));
	BundleVersion          version;
	struct rsa_private_key sign_pkey;
	uint64_t               source_revision;
	uint64_t               target_revision;
	uint32_t               magic_1;
	uint32_t               magic_2;
	uint32_t               minor;
	uint16_t               num_devices;
	Device*                devices;
	Platform               platform;
	Board                  board;
	uint32_t               header_rev;
	CertificateNumber      certificate_number;
	unsigned char          optional;
	unsigned char          critical;
	uint16_t               num_meta;
	char**                 metastrings;
} UpdateInformation;

// This is modeled after libarchive's bsdtar...
struct kttar
{
	unsigned char* buff;
	size_t         buff_size;
	char**         to_sign_and_bundle_list;
	char**         tweaked_to_sign_and_bundle_list;
	unsigned int   sign_and_bundle_index;
	bool           has_script;
	size_t         tweak_pointer_index;
};

static const char* convert_bundle_version(BundleVersion);

static struct rsa_private_key get_default_key(void);
static int                    sign_file(FILE*, const struct rsa_private_key*, FILE*);

static int metadata_filter(struct archive*, void*, struct archive_entry*);
static int write_file(const struct kttar*, struct archive*, struct archive*, struct archive_entry*);
static int write_entry(const struct kttar*, struct archive*, struct archive*, struct archive_entry*);
static int copy_file_data_block(const struct kttar*, struct archive*, struct archive*, struct archive_entry*);
static int create_from_archive_read_disk(struct kttar*,
					 struct archive*,
					 const char*,
					 bool,
					 const char*,
					 const unsigned int);

static int kindle_create_package_archive(const int,
					 char**,
					 const unsigned int,
					 const struct rsa_private_key*,
					 const unsigned int,
					 const unsigned int);
static int kindle_create(const UpdateInformation*, FILE*, FILE*, const bool);
static int kindle_create_ota_update_v2(const UpdateInformation*, FILE*, FILE*, const bool);
static int kindle_create_signature(const UpdateInformation*, FILE*, FILE*);
static int kindle_create_ota_update(const UpdateInformation*, FILE*, FILE*, const bool);
static int kindle_create_recovery(const UpdateInformation*, FILE*, FILE*, const bool);
static int kindle_create_recovery_v2(const UpdateInformation*, FILE*, FILE*, const bool);

#endif
