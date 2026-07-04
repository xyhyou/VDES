# common.mk
TARGET_LIB = TRUE

SOURCEDIR += ../src/common/  ../src/common/eventpp ../src/common/spdlog
INCDIR += ../inc/common/  ../inc/common/eventpp ../inc/common/msgpack/ ../inc/common/openssl/ 
OBJDIR = $(OBJROOT)common
C_PREDEF += $(ADD_PREDEF) SQLITE_ENABLE_RTREE  SQLITE_HAS_CODEC SPDLOG_COMPILED_LIB
CFLAGS += $(ADD_CFLAGS)

-include $(MAKEROOT)$(MAKECORE)

