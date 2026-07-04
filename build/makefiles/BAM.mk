# alert.mk
TARGET_LIB = TRUE

SOURCEDIR += ../src/BAM/
INCDIR += ../inc/common/ ../inc/BAM/
OBJDIR = $(OBJROOT)BAM
C_PREDEF += $(ADD_PREDEF)
CFLAGS += $(ADD_CFLAGS)

-include $(MAKEROOT)$(MAKECORE)

