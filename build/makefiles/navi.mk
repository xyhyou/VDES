# navi.mk
TARGET_LIB = TRUE

SOURCEDIR += ../src/navi/ \
../src/navi/UserData/ \
../src/navi/PlotData/ \
../src/navi/AISModule/ \
../src/navi/NMEA0183/ \
../src/navi/AISStaticData/ \
../src/navi/NMEA2000/ \
../src/navi/GNSSModule/ \
../src/navi/GNSSModule/X-MODEM 

INCDIR += ../inc/common/ \
		  ../inc/common/SQLiteCpp/ \
		  ../inc/common/eventpp/ \
		  ../inc/BAM/ \
		  ../inc/navi/ \
		  ../inc/navi/PlotData/ \
		  ../inc/navi/UserData/ \
		  ../inc/navi/AISModule/ \
		  ../inc/navi/NMEA0183 \
		  ../inc/navi/NMEA2000 \
		  ../inc/navi/AISStaticData \
		  ../inc/navi/GNSSModule \
		  ../inc/navi/GNSSModule/X-MODEM \
		  ../inc/ \
		  ../inc/skia/core \
		  ../inc/skia/config \
		  ../inc/skia/effects \
		  ../inc/map/ \
		  ../inc/s57 \
		  ../inc/s57/ooc \
		  ../inc/s57/ecdis-sdk-2 \
		  ../inc/s57/geos \
		  ../inc/s57/scl \
		  ../inc/s57/tile \
		  ../inc/s57/tile/GsTile \
		  ../inc/gl
OBJDIR = $(OBJROOT)navi
C_PREDEF += $(ADD_PREDEF) SK_BUILD_FOR_PORTABLE
CFLAGS += $(ADD_CFLAGS)

-include $(MAKEROOT)$(MAKECORE)

