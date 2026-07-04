# VDES.mk
TARGET_LIB = TRUE

SOURCEDIR += ../Sources/ ../Sources/SQLiteCpp
INCDIR += ../Headers/ ../Headers/eventpp ../Headers/spdlog/ ../Headers/SQLiteCpp/
OBJDIR = $(OBJROOT)VDES
C_PREDEF += $(ADD_PREDEF) SQLITE_ENABLE_RTREE SPDLOG_COMPILED_LIB
CFLAGS += $(ADD_CFLAGS)

-include $(MAKEROOT)$(MAKECORE)

