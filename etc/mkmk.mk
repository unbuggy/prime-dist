# @file mkmk.mk Builds the `mkmk` makefile generator.

PREFIX = $(shell git rev-parse --show-toplevel)
CXX = clang++
CPPFLAGS =
CXXFLAGS = -std=c++1y -pedantic -Wall -stdlib=libc++
LDFLAGS = -stdlib=libc++
OUTDIR = $(PREFIX)/var/libexec

$(OUTDIR)/mkmk: $(PREFIX)/etc/mkmk.cpp $(OUTDIR)
	$(CXX) -o $@ $(CPPFLAGS) $(CXXFLAGS) $< $(LDFLAGS)

$(OUTDIR):
	mkdir -p $@
