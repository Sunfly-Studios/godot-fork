/**************************************************************************/
/*  pck_packer.h                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef PCK_PACKER_H
#define PCK_PACKER_H

#include "core/object/ref_counted.h"

class FileAccess;

class PCKPacker : public RefCounted {
	GDCLASS(PCKPacker, RefCounted);

public:
	enum CurveType {
		ECP_DP_NONE,
		ECP_DP_SECP192R1,
		ECP_DP_SECP224R1,
		ECP_DP_SECP256R1,
		ECP_DP_SECP384R1,
		ECP_DP_SECP521R1,
		ECP_DP_BP256R1,
		ECP_DP_BP384R1,
		ECP_DP_BP512R1,
		ECP_DP_CURVE25519,
		ECP_DP_SECP192K1,
		ECP_DP_SECP224K1,
		ECP_DP_SECP256K1,
		ECP_DP_CURVE448,
	};

private:
	Ref<FileAccess> file;
	int alignment = 0;
	uint64_t ofs = 0;

	Vector<uint8_t> key;
	bool enc_dir = false;

	static void _bind_methods();

	struct File {
		String path;
		String src_path;
		uint64_t ofs = 0;
		uint64_t size = 0;
		bool encrypted = false;
		bool removal = false;
		bool require_verification = false;
		Vector<uint8_t> md5;
		Vector<uint8_t> sha256;
	};
	Vector<File> files;

protected:
#ifndef DISABLE_DEPRECATED
	Error _add_file_bind_compat_87696(const String &p_file, const String &p_src, bool p_encrypt);
	static void _bind_compatibility_methods();
#endif // DISABLE_DEPRECATED

public:
	Error pck_start(const String &p_pck_path, int p_alignment = 32, const String &p_key = "0000000000000000000000000000000000000000000000000000000000000000", bool p_encrypt_directory = false);
	Error add_file(const String &p_target_path, const String &p_source_path, bool p_encrypt = false, bool require_verification = false);
	Error add_file_removal(const String &p_target_path);

	Error flush(bool p_verbose = false);
	Error flush_and_sign(const String &p_private_key, PCKPacker::CurveType p_curve, bool p_verbose = false);

	PCKPacker() {}
};

VARIANT_ENUM_CAST(PCKPacker::CurveType);

#endif // PCK_PACKER_H
