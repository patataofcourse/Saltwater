import argparse
import subprocess
import sys
import types
def hex8(int): return "0x{:08X}".format(int)

if __name__ != "__main__":
    raise RuntimeError("symparse.py may not be loaded as a module")

# prob not gonna use this
"""
class BfdFlags:
    has_reloc = False
    exec_p = False
    has_lineno = False
    has_debug = False
    has_syms = False
    has_locals = False
    dynamic = False
    wp_text = False
    d_paged = False
    bfd_is_relaxable = False
    bfd_traditional_format = False
    bfd_in_memory = False
    bfd_linker_created = False
    bfd_deterministic_output = False
    bfd_compress = False
    bfd_decompress = False
    bfd_plugin = False
    bfd_compress_gabi = False
    bfd_convert_elf_common = False
    bfd_use_elf_stt_common = False
    bfd_archive_full_path = False
    bfd_closed_by_cache = False

    def __init__(self, num = None, **kwargs):
        if num == None:
            for key, val in kwargs:
                if ( 
                    hasattr(self, key)
                    and type(getattr(self, key) != types.FunctionType)
                    and type(val) == bool
                ):
                    setattr(self, key, val)
        else:
            self.has_reloc = num & 0x1 != 0

    def __int__(self):
        out = 0
        out += 1 << 0 if self.has_reloc else 0
        out += 1 << 1 if self.exec_p else 0
        out += 1 << 2 if self.has_lineno else 0
        out += 1 << 3 if self.has_debug else 0
        out += 1 << 4 if self.has_syms else 0
        out += 1 << 5 if self.has_locals else 0
        out += 1 << 6 if self.dynamic else 0
        out += 1 << 7 if self.wp_text else 0
        out += 1 << 8 if self.d_paged else 0
        out += 1 << 9 if self.bfd_is_relaxable else 0
        out += 1 << 10 if self.bfd_traditional_format else 0
        out += 1 << 11 if self.bfd_in_memory else 0
        out += 1 << 12 if bfd_linker_created else 0
        out += 1 << 13 if bfd_deterministic_output else 0
        out += 1 << 14 if bfd_compress else 0
        out += 1 << 15 if bfd_decompress else 0
        out += 1 << 16 if bfd_plugin else 0
        out += 1 << 17 if bfd_compress_gabi else 0
        out += 1 << 18 if bfd_convert_elf_common else 0
        out += 1 << 19 if bfd_use_elf_stt_common else 0
        out += 1 << 20 if bfd_archive_full_path else 0
        out += 1 << 21 if bfd_closed_by_cache else 0
        return out
"""

parser = argparse.ArgumentParser()

parser.add_argument("3gx")
parser.add_argument("symbols")
parser.add_argument("-d", "--demangle", action="store_true")

args = parser.parse_args()

plugin = open(args.__dict__["3gx"], "rb")
symfile = open(args.symbols, "w")

if plugin.read(8) != b"3GX$0002":
    print("Error: Not a 3GX file or incompatible 3GX version")
    exit(1)

plugin.seek(0x88)
num_symbols = int.from_bytes(plugin.read(4), "little")
symbols_offset = int.from_bytes(plugin.read(4), "little")
name_table = int.from_bytes(plugin.read(4), "little")

symbols = []

for n in range(num_symbols):
    plugin.seek(symbols_offset + 0xc * n)
    address = int.from_bytes(plugin.read(4), "little")
    size = int.from_bytes(plugin.read(2), "little")
    flags = int.from_bytes(plugin.read(2), "little")
    name_pos = int.from_bytes(plugin.read(4), "little")

    plugin.seek(name_table + name_pos)
    name = ""
    while True:
        c = plugin.read(1)[0]
        if c == 0: break
        name += chr(c)

    if args.demangle and name.strip() != "":
        (code, dname) = subprocess.getstatusoutput(f"c++filt {name}")
        symbols.append((address, size, flags, dname))
        continue

    symbols.append((address, size, flags, name))

for symbol in symbols:
    address, size, flags, name = symbol[0], symbol[1], symbol[2], symbol[3]
    symfile.write(f"{hex8(address)} {hex8(size) if size != 0 else '----------'} {name}\n")

symfile.close()