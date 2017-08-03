# Makefile fragment for miscellaneous documentation files
#
# This 'feature' doesn't add any source files, it just provides
# a place to put documentation that has nowhere better to live
#
# $Id: dox.mk 2861 2012-07-16 14:48:59Z gvs $
FEAT_NAME=dox

# .dox files without a path
RAW_DOX_FILES=doxygen.dox testing.dox

# Add the dox files to DOTDOXFILES with the path to this
# directory prepended
# note, can't use += because need current value of FEAT_NAME
DOTDOXFILES:=$(DOTDOXFILES) $(addprefix $(PROTO_ROOT)/$(FEAT_NAME)/, \
    					$(RAW_DOX_FILES))
