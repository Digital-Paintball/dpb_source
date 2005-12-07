SET( TIER1_SRCS
	bitbuf.cpp
	characterset.cpp
	checksum_crc.cpp
	checksum_md5.cpp
	convar.cpp
	interface.cpp
	KeyValues.cpp
	mempool.cpp
	rangecheckedvar.cpp
	resourcemanager.cpp
	stringpool.cpp
	utlbuffer.cpp
	utlsymbol.cpp
)

INCLUDE_DIRECTORIES(
	${DPB_SOURCE_DIR}/src/public/tier1
	${DPB_SOURCE_DIR}/src/public
	${DPB_SOURCE_DIR}/src/utils/common
)

ADD_LIBRARY(tier1 STATIC ${TIER1_SRCS})