cmake_minimum_required (VERSION 2.8.10)
project (editor)

include (../../build.cmake)

pixelboost_setup_pre (../../ editor "com.pixelballoon.editor" "Editor")

add_subdirectory (../../pipeline ${CMAKE_BINARY_DIR}/pipeline)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")

include_directories ("source")
include_directories("${PIPELINE_HEADERS}")

set (COMMAND_STANDARD_SOURCES
	"source/command/standard/checkDepth.cpp"
	"source/command/standard/checkDepth.h"
	"source/command/standard/close.cpp"
	"source/command/standard/copy.cpp"
	"source/command/standard/copy.h"
	"source/command/standard/createEntity.cpp"
	"source/command/standard/createEntity.h"
	"source/command/standard/deleteCommand.cpp"
	"source/command/standard/deleteCommand.h"
	"source/command/standard/export.cpp"
	"source/command/standard/file.h"
	"source/command/standard/modifyDepth.cpp"
	"source/command/standard/modifyDepth.h"	
	"source/command/standard/open.cpp"
	"source/command/standard/paste.cpp"
	"source/command/standard/paste.h"
	"source/command/standard/save.cpp"
	"source/command/standard/selectCommand.cpp"
	"source/command/standard/selectCommand.h"	
	"source/command/standard/transformCommand.cpp"
	"source/command/standard/transformCommand.h"
	)

set (COMMAND_SOURCES
	"source/command/base.cpp"
	"source/command/base.h"
	"source/command/manager.cpp"
	"source/command/manager.h"
	"source/command/register.cpp"
	)

set (CORE_SOURCES
	"source/core/selection.cpp"
	"source/core/selection.h"
	"source/core/uidHelpers.cpp"
	"source/core/uidHelpers.h"	
	)

set (PROJECT_SOURCES
	"source/project/entity.cpp"
	"source/project/entity.h"
	"source/project/project.cpp"
	"source/project/project.h"
	"source/project/property.cpp"
	"source/project/property.h"
	"source/project/record.cpp"
	"source/project/record.h"
	"source/project/schema.cpp"
	"source/project/schema.h"
	"source/project/schemaParser.cpp"
	"source/project/schemaParser.h"
	"source/project/struct.cpp"
	"source/project/struct.h"
	)

set (VIEW_ENTITY_PROPERTY_SOURCES
	"source/view/entity/property/model.cpp"
	"source/view/entity/property/model.h"
	"source/view/entity/property/property.cpp"
	"source/view/entity/property/property.h"
	"source/view/entity/property/rectangle.cpp"
	"source/view/entity/property/rectangle.h"
	"source/view/entity/property/sprite.cpp"
	"source/view/entity/property/sprite.h"
	)

set (VIEW_ENTITY_SOURCES
	"source/view/entity/entity.cpp"
	"source/view/entity/entity.h"
	)

set (VIEW_MANIPULATOR_SOURCES
	"source/view/manipulator/create.cpp"
	"source/view/manipulator/create.h"
	"source/view/manipulator/manipulator.cpp"
	"source/view/manipulator/manipulator.h"
	"source/view/manipulator/move.cpp"
	"source/view/manipulator/move.h"
	"source/view/manipulator/rotate.cpp"
	"source/view/manipulator/rotate.h"
	"source/view/manipulator/scale.cpp"
	"source/view/manipulator/scale.h"
	"source/view/manipulator/select.cpp"
	"source/view/manipulator/select.h"
	)

set (VIEW_UI_CREATE_SOURCES
	"source/view/ui/create/createPanel.cpp"
	"source/view/ui/create/createPanel.h"
	)

set (VIEW_UI_SETTINGS_SOURCES
	"source/view/ui/settings/networkWindow.cpp"
	"source/view/ui/settings/networkWindow.h"
	)

set (VIEW_UI_SOURCES
	"source/view/ui/editor.cpp"
	"source/view/ui/editor.h"
	"source/view/ui/property.cpp"
	"source/view/ui/property.h"
	)

set (VIEW_SOURCES
	"source/view/level.cpp"
	"source/view/level.h"
	)

set (EDITOR_SOURCES
	${COMMAND_STANDARD_SOURCES}
	${COMMAND_SOURCES}
	${CORE_SOURCES}
	${HTTP_SOURCES}
	${PROJECT_SOURCES}
	${VIEW_ENTITY_PROPERTY_SOURCES}
	${VIEW_ENTITY_SOURCES}
	${VIEW_MANIPULATOR_SOURCES}
	${VIEW_UI_CREATE_SOURCES}
	${VIEW_UI_SETTINGS_SOURCES}
	${VIEW_UI_SOURCES}
	${VIEW_SOURCES}
	"source/core.cpp"
	"source/core.h"
	"source/view.cpp"
	"source/view.h"
	)

source_group (command\\standard FILES ${COMMAND_STANDARD_SOURCES})
source_group (command FILES ${COMMAND_SOURCES})
source_group (core FILES ${CORE_SOURCES})
source_group (project FILES ${PROJECT_SOURCES})
source_group (view\\entity\\property FILES ${VIEW_ENTITY_PROPERTY_SOURCES})
source_group (view\\entity FILES ${VIEW_ENTITY_SOURCES})
source_group (view\\manipulator FILES ${VIEW_MANIPULATOR_SOURCES})
source_group (view\\ui\\create FILES ${VIEW_UI_CREATE_SOURCES})
source_group (view\\ui\\settings FILES ${VIEW_UI_SETTINGS_SOURCES})
source_group (view\\ui FILES ${VIEW_UI_SOURCES})
source_group (view FILES ${VIEW_SOURCES})

pixelboost_setup_post ("${EDITOR_SOURCES}")
pixelboost_setup_library (pixelboost_pipeline)
target_link_libraries (editor pixelboost_pipeline)
