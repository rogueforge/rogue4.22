# Rogue 4.22

Reconstructed source for Rogue 4.22 (Toy, Arnold, Wichman; Berkeley 1982).
Compiled with a modified 4.1BSD toolchain, it byte-exactly reproduces the
installed binary `4.1a/games/rogue` (md5 `2f4f378ed9fae989f4f801abe8092bbd`;
identical copies in `4.1.snap/usr/games/` and `4.1/4.0.upgrade/usr/games/`)
from CSRG Archive CD-ROM Disc 1.
`rogue.me` is the guide source (`4.1.snap/usr/doc/rogue`,
md5 `8ad216ace97fd98246a40df6b0e6ea48`). `rogue.6` is the manual page from
4.1BSD (`4.1.snap/usr/man/man6/rogue.6`,
md5 `f84eef587bc38bc0823f2cd029df030b`).

The binary was stripped, so identifier names are inferred (mostly from other
rogue sources); comments and whitespace are likewise inferred from available
sources, as is most of the Makefile.
