cmake_minimum_required (VERSION 2.8.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")

set (SHARED_HEADERS
	"${SHARED_HEADERS}"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/external"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/shared"
	CACHE INTERNAL "Complete list of pipeline headers")

include_directories("${SHARED_HEADERS}")

set (ASSET_SOURCES
	"src/shared/pixelboost/asset/model.cpp"
	"src/shared/pixelboost/asset/model.h"
	)

set (EXTERNAL_SOURCES
	"src/external/GL/glew.c"
	"src/external/jpegcompressor/jpgd.cpp"
	"src/external/jpegcompressor/jpge.cpp"
	"src/external/json/elements.h"
	"src/external/json/elements.inl"
	"src/external/json/reader.h"
	"src/external/json/reader.inl"
	"src/external/json/visitor.h"
	"src/external/json/writer.h"
	"src/external/json/writer.inl"
	"src/external/lodepng/lodepng.cpp"
	"src/external/mongoose/mongoose.c"
	"src/external/optionparser/optionparser.h"
	"src/external/pugixml/pugiconfig.hpp"
	"src/external/pugixml/pugixml.cpp"
	"src/external/pugixml/pugixml.hpp"
	"src/external/regex/trex.c"
	"src/external/stbimage/stb_image.c"
	"src/external/stbimage/stb_image_write.c"
	)

set (MATHS_SOURCES
	"src/shared/pixelboost/maths/boundingBox.cpp"
	"src/shared/pixelboost/maths/boundingBox.h"
	"src/shared/pixelboost/maths/boundingFrustum.cpp"
	"src/shared/pixelboost/maths/boundingFrustum.h"
	"src/shared/pixelboost/maths/boundingSphere.cpp"
	"src/shared/pixelboost/maths/boundingSphere.h"
	"src/shared/pixelboost/maths/matrixHelpers.h"
	)

set (SHARED_SOURCES
	${ASSET_SOURCES}
	${EXTERNAL_SOURCES}
	${MATHS_SOURCES}
	)

add_library (pixelboost_shared STATIC ${SHARED_SOURCES})

source_group (asset FILES ${ASSET_SOURCES})
source_group (external FILES ${EXTERNAL_SOURCES})
source_group (maths FILES ${MATHS_SOURCES})
