SUBDIRS(web)

SET(COL_STATUS_SUMMED		"votes_summed")
SET(COL_STATUS_PLAYERS		"total_players")
SET(COL_STATUS_CATEGORIES	"total_categories")

SET(COL_USERS_ID			"id")
SET(COL_USERS_USER			"user")
SET(COL_USERS_PASSWD		"passwd")

SET(COL_GAMEIDS_ID			"id")
SET(COL_GAMEIDS_NETWORK		"net")
SET(COL_GAMEIDS_VALUE		"val")

SET(COL_CATS_ID				"id")
SET(COL_CATS_NAME			"name")

SET(COL_VOTES_CATEGORY		"cat")
SET(COL_VOTES_VOTE			"vote")
SET(COL_VOTES_FROMPLAYER	"frmp")
SET(COL_VOTES_TOPLAYER		"plyr")

SET(COL_SUMS_CATEGORY		"cat")
SET(COL_SUMS_PLAYER			"player")
SET(COL_SUMS_VOTE			"vote")
SET(COL_SUMS_TOTAL			"total")

CONFIGURE_FILE(db_mysql_columns.h.in	${DPB_BINARY_DIR}/src/soulvision/sv/db_mysql_columns.h)
CONFIGURE_FILE(testdata.sql.in			${DPB_BINARY_DIR}/src/soulvision/sv/testdata.sql)
CONFIGURE_FILE(schema.sql.in			${DPB_BINARY_DIR}/src/soulvision/sv/schema.sql)
CONFIGURE_FILE(resetdb.bat				${DPB_BINARY_DIR}/src/soulvision/sv/resetdb.bat)

SET( SV_SRCS
	database.cpp
	db_mysql.cpp
	db_mysqldata.cpp
	main.cpp
	scheduler.cpp
	soap.cpp
	../sh/configfile.cpp
	../sh/logger.cpp
	../sh/mmgr.cpp
	../sh/sv_util.cpp
	../sh/stdsoap2.cpp
)

SET( SV_GENERATED_SRCS
	${DPB_BINARY_DIR}/src/soulvision/sh/soapC.cpp
	${DPB_BINARY_DIR}/src/soulvision/sh/soapServer.cpp
)

SET_SOURCE_FILES_PROPERTIES(
	${SV_GENERATED_SRCS}
	PROPERTIES
	GENERATED TRUE
)

SET( SV_ALL_SRCS
	${SV_SRCS}
	${SV_GENERATED_SRCS}
	${SV_PLATFORM_SRCS}
)

ADD_DEFINITIONS(
	-DUSE_MMGR
)

INCLUDE_DIRECTORIES(
	${DPB_SOURCE_DIR}/src/soulvision/sh
	${DPB_SOURCE_DIR}/src/soulvision/sv
	${DPB_BINARY_DIR}/src/soulvision/sh
	${DPB_BINARY_DIR}/src/soulvision/sv
	${DPB_BINARY_DIR}/src/game_shared
)

IF(WIN32)
	INCLUDE_DIRECTORIES("/Program Files/MySQL/MySQL Server 5.0/include")
	LINK_DIRECTORIES("/Program Files/MySQL/MySQL Server 5.0/lib/opt")
	INCLUDE_DIRECTORIES("/Program Files/Apache Group/APR/include")
	LINK_DIRECTORIES("/Program Files/Apache Group/APR/LibR")
ELSE(WIN32)
	INCLUDE_DIRECTORIES("/usr/include/mysql")
	INCLUDE_DIRECTORIES("/usr/include/apr-1" "/usr/local/apr/include/apr-1/")
	LINK_DIRECTORIES("/usr/local/apr/lib/")
ENDIF(WIN32)

ADD_EXECUTABLE(statsrv ${SV_ALL_SRCS})

# I hate this too.
IF(WIN32)
	TARGET_LINK_LIBRARIES(statsrv libmysql apr-1 wsock32)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(statsrv mysqlclient apr-1)
ENDIF(WIN32)

ADD_DEPENDENCIES(statsrv soap-targets)
