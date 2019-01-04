# bwtzip Master Makefile
# Stephan T. Lavavej
# http://stl.caltech.edu

SOURCE_FILES = bcell_png.cc bcell.cc chunk.cc clock.cc ucell.cc bwtzip_zlib.cc bwtzip_common.cc bwtzip_file.cc

GPP = @g++
DELETER = @rm
UPX = @upx -qqq --best
GPP_WARNING = -Wall -W
GPP_OPTS = -O3 -march=native -mmmx -msse -mfpmath=sse -maccumulate-outgoing-args \
-minline-all-stringops -fomit-frame-pointer -ffast-math -funroll-loops -malign-double
INC = -I../include
GPP_FLAGS = $(GPP_WARNING) $(GPP_OPTS) $(INC) -s
ECHO = @echo
LBZ = -lbz2 # bzip2 library
LZ = -lz    # zlib library

.PHONY: usage clean reallyclean all bwtzips \
bwtzip bwtunzip display pngacc bwtsarray bwtcompare arithverify \
huffverify mtftest prngtest sha1test bzip2test unbzip2test zlibtest unzlibtest main \
xbwtzip xbwtunzip xdisplay xpngacc xbwtsarray xbwtcompare xarithverify \
xhuffverify xmtftest xprngtest xsha1test xbzip2test xunbzip2test xzlibtest \
xunzlibtest xmain fast_bwtzips bwtstlsort xbwtstlsort sha512test xsha512test bwtbs xbwtbs \
xsrle srle xunsrle unsrle linux_bwtzips

usage :
	$(ECHO) Bcell Master Makefile, by Stephan T. Lavavej - http://stl.caltech.edu
	$(ECHO) This Makefile will try to call upx. - http://upx.sourceforge.net
	$(ECHO) Change the UPX variable to dir or the like if this is undesirable.
	$(ECHO) Programs that can be built:
	$(ECHO) -------------------------------------------------------------------------------
	$(ECHO) bwtzip --------- bwtzip:   Suffix tree BWT compressor
	$(ECHO) bwtunzip ------- bwtunzip: Suffix tree BWT decompressor
	$(ECHO) main ----------- Dummy main file
	$(ECHO) display -------- Data dump program
	$(ECHO) pngacc --------- WORK IN PROGRESS
	$(ECHO) sha1test ------- Example SHA-1 hasher NOTE: Less efficient than standalone
	$(ECHO) sha512test ----- Example SHA-512 hasher NOTE: Less efficient than standalone
	$(ECHO) prngtest ------- Example PseudoRandom Number Generator
	$(ECHO) bwtcompare ----- Compare filesizes produced by various BWT algorithms
	$(ECHO) mtftest -------- Compare second-step algorithms
	$(ECHO) bwtsarray ------ Suffix array BWT compressor
	$(ECHO) bwtstlsort ----- STL sort BWT compressor
	$(ECHO) bwtbs ---------- Bentley-Sedgewick Tripartition Quicksort BWT compressor
	$(ECHO) bzip2test ------ Example bzip2 compressor NOT compatible with bzip2
	$(ECHO) unbzip2test ---- Example bzip2 decompressor NOT compatible with bzip2
	$(ECHO) zlibtest ------- Example zlib compressor NOT compatible with other programs
	$(ECHO) unzlibtest ----- Example zlib decompressor NOT compatible with other programs
	$(ECHO) arithverify ---- Verify correct operation of arithmetic compression
	$(ECHO) huffverify ----- Verify correct operation of Huffman compression
	$(ECHO) srle ----------- Perform Switched RLE
	$(ECHO) unsrle --------- Reverse Switched RLE
	$(ECHO) -------------------------------------------------------------------------------
	$(ECHO) all ------------ All programs, accelerated
	$(ECHO) bwtzips -------- bwtzip and bwtunzip, accelerated
	$(ECHO) fast_bwtzips --- bwtzips with -fprofile-arcs, -fbranch-probabilities
	$(ECHO) linux_bwtzips -- Works on Linux
	$(ECHO) clean ---------- Delete all object files
	$(ECHO) reallyclean ---- Delete all object files and executables

%.o : %.cc
	$(GPP) $(GPP_FLAGS) -c $+

clean :
	$(DELETER) *.o

reallyclean: clean
	$(DELETER) *.exe

all : xbwtzip xbwtunzip xdisplay xpngacc xbwtsarray xbwtcompare xarithverify xsha512test \
xhuffverify xmtftest xprngtest xsha1test xbzip2test xunbzip2test xzlibtest xunzlibtest \
xbwtstlsort xbwtbs xsrle xunsrle clean

bwtzips : xbwtzip xbwtunzip clean

bwtzip : xbwtzip clean
bwtunzip : xbwtunzip clean
display : xdisplay clean
pngacc : xpngacc clean
bwtsarray : xbwtsarray clean
bwtcompare : xbwtcompare clean
arithverify : xarithverify clean
huffverify : xhuffverify clean
mtftest : xmtftest clean
prngtest : xprngtest clean
sha1test : xsha1test clean
sha512test : xsha512test clean
bzip2test : xbzip2test clean
unbzip2test : xunbzip2test clean
zlibtest : xzlibtest clean
unzlibtest : xunzlibtest clean
main : xmain clean
bwtstlsort: xbwtstlsort clean
bwtbs: xbwtbs clean
srle : xsrle clean
unsrle : xunsrle clean

fast_bwtzips : bwtzip_common.cc bwtzip_suffixtree.cc bwtzip_mtf.cc bwtzip_zle.cc bwtzip_arith.cc \
bwtzip_file.cc clock.cc main_bwtzip.cc bwtunzip.cc main_bwtunzip.cc
	$(GPP) $(GPP_FLAGS) -fprofile-arcs bwtzip_common.cc bwtzip_suffixtree.cc bwtzip_mtf.cc bwtzip_zle.cc \
bwtzip_arith.cc bwtzip_file.cc clock.cc main_bwtzip.cc -o bwtzip.exe
	bwtzip jargon.html jargon.bwt
	$(DELETER) bwtzip.exe
	$(GPP) $(GPP_FLAGS) -fbranch-probabilities bwtzip_common.cc bwtzip_suffixtree.cc bwtzip_mtf.cc bwtzip_zle.cc \
bwtzip_arith.cc bwtzip_file.cc clock.cc main_bwtzip.cc -o bwtzip.exe
	$(DELETER) *.da
	$(GPP) $(GPP_FLAGS) -fprofile-arcs bwtzip_common.cc bwtzip_mtf.cc bwtzip_zle.cc \
bwtzip_arith.cc bwtzip_file.cc clock.cc bwtunzip.cc main_bwtunzip.cc -o bwtunzip.exe
	bwtunzip jargon.bwt jargon.out
	$(DELETER) bwtunzip.exe
	$(GPP) $(GPP_FLAGS) -fbranch-probabilities bwtzip_common.cc bwtzip_mtf.cc bwtzip_zle.cc \
bwtzip_arith.cc bwtzip_file.cc clock.cc bwtunzip.cc main_bwtunzip.cc -o bwtunzip.exe
	$(DELETER) *.da
	$(DELETER) jargon.bwt
	$(DELETER) jargon.out
	$(UPX) bwtzip.exe
	$(UPX) bwtunzip.exe

linux_bwtzips:
	g++ -Wall -W -O3 -DNO_RDTSC -fomit-frame-pointer -ffast-math -funroll-loops \
	bwtzip_common.cc bwtzip_suffixtree.cc bwtzip_mtf.cc bwtzip_zle.cc bwtzip_arith.cc \
	bwtzip_file.cc clock.cc main_bwtzip.cc -o bwtzip
	g++ -Wall -W -O3 -DNO_RDTSC -fomit-frame-pointer -ffast-math -funroll-loops \
	bwtzip_common.cc bwtzip_mtf.cc bwtzip_zle.cc bwtzip_arith.cc bwtzip_file.cc clock.cc \
	bwtunzip.cc main_bwtunzip.cc -o bwtunzip

# These rules do the grunt work and don't clean up their object files.

xbwtzip : bwtzip_common.o bwtzip_suffixtree.o bwtzip_mtf.o bwtzip_zle.o bwtzip_arith.o bwtzip_file.o clock.o
	$(GPP) $(GPP_FLAGS) $+ main_bwtzip.cc -o bwtzip.exe
	$(UPX) bwtzip.exe

xbwtunzip : bwtzip_common.o bwtzip_mtf.o bwtzip_zle.o bwtzip_arith.o bwtzip_file.o clock.o bwtunzip.o
	$(GPP) $(GPP_FLAGS) $+ main_bwtunzip.cc -o bwtunzip.exe
	$(UPX) bwtunzip.exe

xbwtsarray : bwtzip_common.o bwtzip_suffixarray.o bwtzip_mtf.o bwtzip_zle.o bwtzip_arith.o bwtzip_file.o clock.o bwtzip.o
	$(GPP) $(GPP_FLAGS) $+ main_bwtsarray.cc -o bwtsarray.exe
	$(UPX) bwtsarray.exe

xbwtstlsort : bwtzip_common.o bwtzip_suffixstlsort.o bwtzip_mtf.o bwtzip_zle.o bwtzip_arith.o bwtzip_file.o clock.o bwtzip.o
	$(GPP) $(GPP_FLAGS) $+ main_bwtstlsort.cc -o bwtstlsort.exe
	$(UPX) bwtstlsort.exe

xbwtbs : bwtzip_common.o bwtzip_suffixbentleysedgewick.o bwtzip_mtf.o bwtzip_zle.o bwtzip_arith.o bwtzip_file.o clock.o bwtzip.o
	$(GPP) $(GPP_FLAGS) $+ main_bwtbs.cc -o bwtbs.exe
	$(UPX) bwtbs.exe

xmtftest : bwtzip_common.o bwtzip_suffixtree.o bwtzip_mtf.o bwtzip_zle.o bwtzip_arith.o bwtzip_file.o clock.o
	$(GPP) $(GPP_FLAGS) $+ main_mtftest.cc -o mtftest.exe
	$(UPX) mtftest.exe

xmain : bwtzip_file.o
	$(GPP) $(GPP_FLAGS) $+ main.cc -o main.exe
	$(UPX) main.exe

xbzip2test : bwtzip_bzip2.o bwtzip_common.o bwtzip_file.o
	$(GPP) $(GPP_FLAGS) $+ main_bzip2test.cc -o bzip2test.exe $(LBZ)
	$(UPX) bzip2test.exe

xunbzip2test : bwtzip_bzip2.o bwtzip_common.o bwtzip_file.o
	$(GPP) $(GPP_FLAGS) $+ main_unbzip2test.cc -o unbzip2test.exe $(LBZ)
	$(UPX) unbzip2test.exe

xunzlibtest : bwtzip_zlib.o bwtzip_common.o bwtzip_file.o
	$(GPP) $(GPP_FLAGS) $+ main_unzlibtest.cc -o unzlibtest.exe $(LZ)
	$(UPX) unzlibtest.exe

xzlibtest : bwtzip_zlib.o bwtzip_common.o bwtzip_file.o
	$(GPP) $(GPP_FLAGS) $+ main_zlibtest.cc -o zlibtest.exe $(LZ)
	$(UPX) zlibtest.exe

xprngtest : bwtzip_common.o clock.o
	$(GPP) $(GPP_FLAGS) $+ main_prngtest.cc -o prngtest.exe
	$(UPX) prngtest.exe

xarithverify : bwtzip_arith.o bwtzip_file.o bwtzip_common.o
	$(GPP) $(GPP_FLAGS) $+ main_arithverify.cc -o arithverify.exe
	$(UPX) arithverify.exe

xhuffverify : bwtzip_huff.o bwtzip_file.o bwtzip_common.o clock.o
	$(GPP) $(GPP_FLAGS) $+ main_huffverify.cc -o huffverify.exe
	$(UPX) huffverify.exe

xbwtcompare : bwtzip_common.o bwtzip_suffixtree.o bwtzip_mtf.o bwtzip_zle.o \
			  bwtzip_arith.o bwtzip_huff.o bwtzip_file.o clock.o bwtunzip.o bwtzip_ahuff.o
	$(GPP) $(GPP_FLAGS) $+ main_bwtcompare.cc -o bwtcompare.exe
	$(UPX) bwtcompare.exe

xpngacc : $(SOURCE_FILES:.cc=.o)
	$(GPP) $(GPP_FLAGS) $+ main_pngacc.cc -o pngacc.exe $(LZ)
	$(UPX) pngacc.exe

xdisplay : $(SOURCE_FILES:.cc=.o)
	$(GPP) $(GPP_FLAGS) $+ main_display.cc -o display.exe $(LZ)
	$(UPX) display.exe

xsha1test : bwtzip_file.o bwtzip_common.o bwtzip_sha1.o
	$(GPP) $(GPP_FLAGS) $+ main_sha1test.cc -o sha1test.exe
	$(UPX) sha1test.exe

xsha512test : bwtzip_file.o bwtzip_common.o bwtzip_sha512.o
	$(GPP) $(GPP_FLAGS) $+ main_sha512test.cc -o sha512test.exe
	$(UPX) sha512test.exe

xsrle : bwtzip_file.o bwtzip_common.o bwtzip_srle.o
	$(GPP) $(GPP_FLAGS) $+ main_srle.cc -o srle.exe
	$(UPX) srle.exe

xunsrle : bwtzip_file.o bwtzip_common.o bwtzip_srle.o
	$(GPP) $(GPP_FLAGS) $+ main_unsrle.cc -o unsrle.exe
	$(UPX) unsrle.exe
