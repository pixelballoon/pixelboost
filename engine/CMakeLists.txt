cmake_minimum_required (VERSION 2.8.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")

if (PIXELBOOST_BUILD_PLATFORM_ANDROID)
	set (PLATFORM_HEADERS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/android"
		)

	set (PLATFORM_SOURCES
		"src/platform/android/pixelboost/audio/android/audioManagerSimple.cpp"
		"src/platform/android/pixelboost/audio/android/audioManagerSimple.h"
		"src/platform/android/pixelboost/file/fileHelpers.cpp"
		"src/platform/android/pixelboost/misc/jni.cpp"
		"src/platform/android/pixelboost/misc/jni.h"
		"src/platform/android/pixelboost/network/gameCenter.cpp"
		"src/platform/android/pixelboost/network/gameCenter.h"
		"src/platform/android/pixelboost/network/networkHelpers.cpp"
		"src/platform/android/pixelboost/util/localisation/localisation.cpp"
		)
endif (PIXELBOOST_BUILD_PLATFORM_ANDROID)

if (PIXELBOOST_BUILD_PLATFORM_OSX)
	set (PLATFORM_HEADERS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/mac"
		"${CMAKE_CURRENT_SOURCE_DIR}/../libs/ddhidlib/lib"
		)

	set (PLATFORM_SOURCES
		"src/platform/mac/pixelboost/audio/osx/audioManagerSimple.h"
		"src/platform/mac/pixelboost/audio/osx/audioManagerSimple.mm"
		"src/platform/mac/pixelboost/file/fileHelpers.mm"
		"src/platform/mac/pixelboost/network/gameCenter.h"
		"src/platform/mac/pixelboost/network/gameCenter.mm"
		"src/platform/mac/pixelboost/network/networkHelpers.mm"
		"src/platform/mac/pixelboost/util/localisation/localisation.mm"
		)

	set (PLATFORM_SOURCES ${PLATFORM_SOURCES}
		"src/platform/mac/pixelboost/framework/AppDelegate.mm"
		"src/platform/mac/pixelboost/framework/main.m"
		"src/platform/mac/pixelboost/framework/MainView.mm"
		)
endif (PIXELBOOST_BUILD_PLATFORM_OSX)

if (PIXELBOOST_BUILD_PLATFORM_IOS)
	set (PLATFORM_HEADERS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/ios/external/objectAL"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/ios/external/objectAL/Actions"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/ios/external/objectAL/AudioTrack"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/ios/external/objectAL/OpenAL"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/ios/external/objectAL/Session"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/ios/external/objectAL/Support"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/ios"
		)

	set (PLATFORM_SOURCES
		"src/platform/ios/external/objectAL/Actions/OALAction.m"
		"src/platform/ios/external/objectAL/Actions/OALActionManager.m"
		"src/platform/ios/external/objectAL/Actions/OALAudioActions.m"
		"src/platform/ios/external/objectAL/Actions/OALFunction.m"
		"src/platform/ios/external/objectAL/Actions/OALUtilityActions.m"
		"src/platform/ios/external/objectAL/AudioTrack/OALAudioTrack.m"
		"src/platform/ios/external/objectAL/AudioTrack/OALAudioTrackNotifications.m"
		"src/platform/ios/external/objectAL/AudioTrack/OALAudioTracks.m"
		"src/platform/ios/external/objectAL/OpenAL/ALBuffer.m"
		"src/platform/ios/external/objectAL/OpenAL/ALCaptureDevice.m"
		"src/platform/ios/external/objectAL/OpenAL/ALChannelSource.m"
		"src/platform/ios/external/objectAL/OpenAL/ALContext.m"
		"src/platform/ios/external/objectAL/OpenAL/ALDevice.m"
		"src/platform/ios/external/objectAL/OpenAL/ALListener.m"
		"src/platform/ios/external/objectAL/OpenAL/ALSoundSourcePool.m"
		"src/platform/ios/external/objectAL/OpenAL/ALSource.m"
		"src/platform/ios/external/objectAL/OpenAL/ALWrapper.m"
		"src/platform/ios/external/objectAL/OpenAL/OpenALManager.m"
		"src/platform/ios/external/objectAL/Session/OALAudioSession.m"
		"src/platform/ios/external/objectAL/Session/OALSuspendHandler.m"
		"src/platform/ios/external/objectAL/Support/IOSVersion.m"
		"src/platform/ios/external/objectAL/Support/mach_timing.c"
		"src/platform/ios/external/objectAL/Support/NSMutableArray+WeakReferences.m"
		"src/platform/ios/external/objectAL/Support/OALAudioFile.m"
		"src/platform/ios/external/objectAL/Support/OALTools.m"
		"src/platform/ios/external/objectAL/OALSimpleAudio.m"
		"src/platform/ios/pixelboost/audio/ios/audioManagerSimple.h"
		"src/platform/ios/pixelboost/audio/ios/audioManagerSimple.mm"
		"src/platform/ios/pixelboost/file/fileHelpers.mm"
		"src/platform/ios/pixelboost/framework/EAGLView.m"
		"src/platform/ios/pixelboost/framework/main.m"
		"src/platform/ios/pixelboost/framework/pixelboostAppDelegate.mm"
		"src/platform/ios/pixelboost/framework/pixelboostViewController.mm"
		"src/platform/ios/pixelboost/network/gameCenter.h"
		"src/platform/ios/pixelboost/network/gameCenter.mm"
		"src/platform/ios/pixelboost/network/networkHelpers.mm"
		"src/platform/ios/pixelboost/util/localisation/localisation.mm"
		)
endif (PIXELBOOST_BUILD_PLATFORM_IOS)

if (PIXELBOOST_BUILD_PLATFORM_EMSCRIPTEN)
	set (PLATFORM_HEADERS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/emscripten"
		)

	set (PLATFORM_SOURCES
		"src/platform/emscripten/pixelboost/audio/emscripten/audioManagerSimple.h"
		"src/platform/emscripten/pixelboost/audio/emscripten/audioManagerSimple.cpp"
		"src/platform/emscripten/pixelboost/file/fileHelpers.cpp"
		"src/platform/emscripten/pixelboost/network/gameCenter.h"
		"src/platform/emscripten/pixelboost/network/gameCenter.cpp"
		"src/platform/emscripten/pixelboost/network/networkHelpers.cpp"
		"src/platform/emscripten/pixelboost/util/localisation/localisation.cpp"
		)
endif (PIXELBOOST_BUILD_PLATFORM_EMSCRIPTEN)

set (PIXELBOOST_HEADERS
	"${PIXELBOOST_HEADERS}"
	"${PIXELBOOST_LIBRARY_HEADERS}"
	"${PLATFORM_HEADERS}"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/common"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/external"
	"${CMAKE_CURRENT_SOURCE_DIR}/../libs/box2d"
	"${CMAKE_CURRENT_SOURCE_DIR}/../libs/enet/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../libs/freetype2/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../libs/luabridge/Source"
	"${CMAKE_CURRENT_SOURCE_DIR}/../libs/lua/src"
	"${CMAKE_CURRENT_SOURCE_DIR}/../libs/physfs/src"
	"${CMAKE_CURRENT_SOURCE_DIR}/../shared/src/external"
	"${CMAKE_CURRENT_SOURCE_DIR}/../shared/src/shared"
	CACHE INTERNAL "Complete list of pixelboost headers")

include_directories("${PIXELBOOST_HEADERS}")

set (AUDIO_SOURCES
	"src/common/pixelboost/audio/audioManagerSimple.cpp"
	"src/common/pixelboost/audio/audioManagerSimple.h"
	)

set (DATA_RESOURCES_SOURCES
	"src/common/pixelboost/data/resources/svgResource.cpp"
	"src/common/pixelboost/data/resources/svgResource.h"
	"src/common/pixelboost/data/resources/xmlResource.cpp"
	"src/common/pixelboost/data/resources/xmlResource.h"
	)

set (DATA_SOURCES
	"src/common/pixelboost/data/stringHelpers.h"
	)

set (DB_TYPES_SOURCES
	"src/common/pixelboost/db/types/common.cpp"
	"src/common/pixelboost/db/types/common.h"
	"src/common/pixelboost/db/types/components.cpp"
	"src/common/pixelboost/db/types/components.h"
	"src/common/pixelboost/db/types/register.cpp"
	)

set (DB_SOURCES
	"src/common/pixelboost/db/database.cpp"
	"src/common/pixelboost/db/database.h"
	"src/common/pixelboost/db/entity.cpp"
	"src/common/pixelboost/db/entity.h"
	"src/common/pixelboost/db/record.cpp"
	"src/common/pixelboost/db/record.h"
	"src/common/pixelboost/db/register.h"
	"src/common/pixelboost/db/struct.cpp"
	"src/common/pixelboost/db/struct.h"
	"src/common/pixelboost/db/struct.inl"
	)

set (DEBUG_SOURCES
	"src/common/pixelboost/debug/assert.cpp"
	"src/common/pixelboost/debug/assert.h"
	"src/common/pixelboost/debug/debugDatabaseHandler.cpp"
	"src/common/pixelboost/debug/debugDatabaseHandler.h"
	"src/common/pixelboost/debug/debugVariable.cpp"
	"src/common/pixelboost/debug/debugVariable.h"
	"src/common/pixelboost/debug/debugVariableManager.cpp"
	"src/common/pixelboost/debug/debugVariableManager.h"
	"src/common/pixelboost/debug/log.cpp"
	"src/common/pixelboost/debug/log.h"
	)

set (FILE_SOURCES
	"src/common/pixelboost/file/fileHelpers.h"
	"src/common/pixelboost/file/fileSystem.cpp"
	"src/common/pixelboost/file/fileSystem.h"
	)

set (FRAMEWORK_SOURCES
	"src/common/pixelboost/framework/definitions.h"
	"src/common/pixelboost/framework/engine.cpp"
	"src/common/pixelboost/framework/engine.h"
	"src/common/pixelboost/framework/screen.cpp"
	"src/common/pixelboost/framework/screen.h"
	)

set (GRAPHICS_CAMERA_SOURCES
	"src/common/pixelboost/graphics/camera/camera.cpp"
	"src/common/pixelboost/graphics/camera/camera.h"
	"src/common/pixelboost/graphics/camera/viewport.cpp"
	"src/common/pixelboost/graphics/camera/viewport.h"
	)

set (GRAPHICS_DEVICE_SOURCES
	"src/common/pixelboost/graphics/device/gl/device.cpp"
	"src/common/pixelboost/graphics/device/gl/device.h"
	"src/common/pixelboost/graphics/device/gl/program.cpp"
	"src/common/pixelboost/graphics/device/gl/program.h"
	"src/common/pixelboost/graphics/device/gl/texture.cpp"
	"src/common/pixelboost/graphics/device/gl/texture.h"
	"src/common/pixelboost/graphics/device/gles1/device.cpp"
	"src/common/pixelboost/graphics/device/gles1/device.h"
	"src/common/pixelboost/graphics/device/gles1/texture.cpp"
	"src/common/pixelboost/graphics/device/gles1/texture.h"
	"src/common/pixelboost/graphics/device/device.cpp"
	"src/common/pixelboost/graphics/device/device.h"
	"src/common/pixelboost/graphics/device/indexBuffer.cpp"
	"src/common/pixelboost/graphics/device/indexBuffer.h"
	"src/common/pixelboost/graphics/device/program.cpp"
	"src/common/pixelboost/graphics/device/program.h"
	"src/common/pixelboost/graphics/device/texture.cpp"
	"src/common/pixelboost/graphics/device/texture.h"
	"src/common/pixelboost/graphics/device/vertexBuffer.cpp"
	"src/common/pixelboost/graphics/device/vertexBuffer.h"
	)

set (GRAPHICS_MATERIAL_SOURCES
	"src/common/pixelboost/graphics/material/material.cpp"
	"src/common/pixelboost/graphics/material/material.h"
	)

set (GRAPHICS_PARTICLE_SOURCES
	"src/common/pixelboost/graphics/particle/particleSystem.cpp"
	"src/common/pixelboost/graphics/particle/particleSystem.h"
	)

set (GRAPHICS_RENDERER_SOURCES
	"src/common/pixelboost/graphics/renderer/box2d/box2dRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/box2d/box2dRenderer.h"
	"src/common/pixelboost/graphics/renderer/buffer/bufferRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/buffer/bufferRenderer.h"
	"src/common/pixelboost/graphics/renderer/common/irenderer.cpp"
	"src/common/pixelboost/graphics/renderer/common/irenderer.h"
	"src/common/pixelboost/graphics/renderer/common/renderable.cpp"
	"src/common/pixelboost/graphics/renderer/common/renderable.h"
	"src/common/pixelboost/graphics/renderer/common/renderer.cpp"
	"src/common/pixelboost/graphics/renderer/common/renderer.h"
	"src/common/pixelboost/graphics/renderer/font/fontRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/font/fontRenderer.h"
	"src/common/pixelboost/graphics/renderer/gui/controls.cpp"
	"src/common/pixelboost/graphics/renderer/gui/controls.h"	
	"src/common/pixelboost/graphics/renderer/gui/guiRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/gui/guiRenderer.h"	
	"src/common/pixelboost/graphics/renderer/model/modelRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/model/modelRenderer.h"
	"src/common/pixelboost/graphics/renderer/particle/particleRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/particle/particleRenderer.h"
	"src/common/pixelboost/graphics/renderer/primitive/primitiveRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
	"src/common/pixelboost/graphics/renderer/sprite/sprite.cpp"
	"src/common/pixelboost/graphics/renderer/sprite/sprite.h"
	"src/common/pixelboost/graphics/renderer/sprite/spriteRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/sprite/spriteRenderer.h"
	)

set (GRAPHICS_RESOURCES_SOURCES
	"src/common/pixelboost/graphics/resources/materialResource.cpp"
	"src/common/pixelboost/graphics/resources/materialResource.h"
	"src/common/pixelboost/graphics/resources/modelResource.cpp"
	"src/common/pixelboost/graphics/resources/modelResource.h"
	"src/common/pixelboost/graphics/resources/shaderResource.cpp"
	"src/common/pixelboost/graphics/resources/shaderResource.h"
	"src/common/pixelboost/graphics/resources/spriteSheetResource.cpp"
	"src/common/pixelboost/graphics/resources/spriteSheetResource.h"
	"src/common/pixelboost/graphics/resources/textureResource.cpp"
	"src/common/pixelboost/graphics/resources/textureResource.h"
	)

set (GRAPHICS_SHADER_SOURCES
	"src/common/pixelboost/graphics/shader/shader.cpp"
	"src/common/pixelboost/graphics/shader/shader.h"
	)

set (GRAPHICS_SOURCES
	"src/common/pixelboost/graphics/definitions.h"
	)

set (INPUT_SOURCES
	"src/common/pixelboost/input/acceleratometerManager.cpp"
	"src/common/pixelboost/input/acceleratometerManager.h"
	"src/common/pixelboost/input/inputManager.cpp"
	"src/common/pixelboost/input/inputManager.h"
	"src/common/pixelboost/input/joystickManager.cpp"
	"src/common/pixelboost/input/joystickManager.h"
	"src/common/pixelboost/input/keyboardManager.cpp"
	"src/common/pixelboost/input/keyboardManager.h"
	"src/common/pixelboost/input/mouseManager.cpp"
	"src/common/pixelboost/input/mouseManager.h"
	"src/common/pixelboost/input/touchManager.cpp"
	"src/common/pixelboost/input/touchManager.h"
	)

set (LOGIC_COMPONENT_GRAPHICS_SOURCES
	"src/common/pixelboost/logic/component/graphics/buffer.cpp"
	"src/common/pixelboost/logic/component/graphics/buffer.h"
	"src/common/pixelboost/logic/component/graphics/camera.cpp"
	"src/common/pixelboost/logic/component/graphics/camera.h"
	"src/common/pixelboost/logic/component/graphics/ellipse.cpp"
	"src/common/pixelboost/logic/component/graphics/ellipse.h"
	"src/common/pixelboost/logic/component/graphics/font.cpp"
	"src/common/pixelboost/logic/component/graphics/font.h"
	"src/common/pixelboost/logic/component/graphics/gui.cpp"
	"src/common/pixelboost/logic/component/graphics/gui.h"
	"src/common/pixelboost/logic/component/graphics/line.cpp"
	"src/common/pixelboost/logic/component/graphics/line.h"
	"src/common/pixelboost/logic/component/graphics/model.cpp"
	"src/common/pixelboost/logic/component/graphics/model.h"
	"src/common/pixelboost/logic/component/graphics/rectangle.cpp"
	"src/common/pixelboost/logic/component/graphics/rectangle.h"
	"src/common/pixelboost/logic/component/graphics/renderable.h"
	"src/common/pixelboost/logic/component/graphics/renderable.inl"
	"src/common/pixelboost/logic/component/graphics/skinnedModel.cpp"
    "src/common/pixelboost/logic/component/graphics/skinnedModel.h"
	"src/common/pixelboost/logic/component/graphics/sprite.cpp"
	"src/common/pixelboost/logic/component/graphics/sprite.h"
	"src/common/pixelboost/logic/component/graphics/trail.cpp"
	"src/common/pixelboost/logic/component/graphics/trail.h"
	)

set (LOGIC_COMPONENT_INPUT_SOURCES
	"src/common/pixelboost/logic/component/input/rectTouch.cpp"
	"src/common/pixelboost/logic/component/input/rectTouch.h"
	)

set (LOGIC_COMPONENT_PHYSICS_2D_SOURCES
	"src/common/pixelboost/logic/component/physics/2d/physicsBody.cpp"
	"src/common/pixelboost/logic/component/physics/2d/physicsBody.h"
	"src/common/pixelboost/logic/component/physics/2d/physics.cpp"
	"src/common/pixelboost/logic/component/physics/2d/physics.h"
	"src/common/pixelboost/logic/component/physics/2d/userBody.cpp"
	"src/common/pixelboost/logic/component/physics/2d/userBody.h"
	)

set (LOGIC_COMPONENT_PHYSICS_SOURCES
	"src/common/pixelboost/logic/component/physics/physics.cpp"
	"src/common/pixelboost/logic/component/physics/physics.h"
	)

set (LOGIC_COMPONENT_SOURCES
	"src/common/pixelboost/logic/component/script.cpp"
	"src/common/pixelboost/logic/component/script.h"
	"src/common/pixelboost/logic/component/transform.cpp"
	"src/common/pixelboost/logic/component/transform.h"
	)

set (LOGIC_MESSAGE_SOURCES
	"src/common/pixelboost/logic/message/debug/render.cpp"
	"src/common/pixelboost/logic/message/debug/render.h"
	"src/common/pixelboost/logic/message/graphics/gui.cpp"
	"src/common/pixelboost/logic/message/graphics/gui.h"
	"src/common/pixelboost/logic/message/input/touch.cpp"
	"src/common/pixelboost/logic/message/input/touch.h"
	"src/common/pixelboost/logic/message/particle/finished.cpp"
	"src/common/pixelboost/logic/message/particle/finished.h"
	"src/common/pixelboost/logic/message/physics/collision.cpp"
	"src/common/pixelboost/logic/message/physics/collision.h"
	"src/common/pixelboost/logic/message/destroy.cpp"
	"src/common/pixelboost/logic/message/destroy.h"
	"src/common/pixelboost/logic/message/transform.cpp"
	"src/common/pixelboost/logic/message/transform.h"
	"src/common/pixelboost/logic/message/update.cpp"
	"src/common/pixelboost/logic/message/update.h"
	)

set (LOGIC_SYSTEM_DEBUG_SOURCES
	"src/common/pixelboost/logic/system/debug/render.cpp"
	"src/common/pixelboost/logic/system/debug/render.h"
	)

set (LOGIC_SYSTEM_GRAPHICS_GUI_SOURCES
	"src/common/pixelboost/logic/system/graphics/gui/gui.cpp"
	"src/common/pixelboost/logic/system/graphics/gui/gui.h"
	)

set (LOGIC_SYSTEM_GRAPHICS_RENDER_SOURCES
	"src/common/pixelboost/logic/system/graphics/render/basic.cpp"
	"src/common/pixelboost/logic/system/graphics/render/basic.h"
	"src/common/pixelboost/logic/system/graphics/render/bounds.cpp"
	"src/common/pixelboost/logic/system/graphics/render/bounds.h"
	"src/common/pixelboost/logic/system/graphics/render/render.cpp"
	"src/common/pixelboost/logic/system/graphics/render/render.h"
	)

set (LOGIC_SYSTEM_PHYSICS_SOURCES
	"src/common/pixelboost/logic/system/physics/2d/physics.cpp"
	"src/common/pixelboost/logic/system/physics/2d/physics.h"
	)

set (LOGIC_SOURCES
	"src/common/pixelboost/logic/component.cpp"
	"src/common/pixelboost/logic/component.h"
	"src/common/pixelboost/logic/definitions.h"
	"src/common/pixelboost/logic/entity.cpp"
	"src/common/pixelboost/logic/entity.h"
	"src/common/pixelboost/logic/entity.inl"
	"src/common/pixelboost/logic/message.cpp"
	"src/common/pixelboost/logic/message.h"
	"src/common/pixelboost/logic/message.inl"
	"src/common/pixelboost/logic/scene.cpp"
	"src/common/pixelboost/logic/scene.h"
	"src/common/pixelboost/logic/scene.inl"
	"src/common/pixelboost/logic/system.cpp"
	"src/common/pixelboost/logic/system.h"
	)

set (MATHS_SOURCES
	"src/common/pixelboost/maths/hermiteCurve.cpp"
	"src/common/pixelboost/maths/hermiteCurve.h"
	"src/common/pixelboost/maths/plane.cpp"
	"src/common/pixelboost/maths/plane.h"
	"src/common/pixelboost/maths/ray.cpp"
	"src/common/pixelboost/maths/ray.h"
	)

set (NETWORK_SOURCES
	"src/common/pixelboost/network/http/httpServer.cpp"
	"src/common/pixelboost/network/http/httpServer.h"
	"src/common/pixelboost/network/networkMessage.cpp"
	"src/common/pixelboost/network/networkMessage.h"
	"src/common/pixelboost/network/networkServer.cpp"
	"src/common/pixelboost/network/networkServer.h"
	)

set (PHYSICS_2D_SOURCES
	"src/common/pixelboost/physics/2d/helpers.cpp"
	"src/common/pixelboost/physics/2d/helpers.h"
	)

set (RESOURCE_SOURCES
	"src/common/pixelboost/resource/definitions.h"
	"src/common/pixelboost/resource/resourceManager.cpp"
	"src/common/pixelboost/resource/resourceManager.h"
	"src/common/pixelboost/resource/resourceManager.inl"
	"src/common/pixelboost/resource/resource.cpp"
	"src/common/pixelboost/resource/resource.h"
	)

set (SCRIPTING_SOURCES
	"src/common/pixelboost/scripting/lua.cpp"
	"src/common/pixelboost/scripting/lua.h"
	"src/common/pixelboost/scripting/register.cpp"
	)

set (UTIL_LOCALISATION_SOURCES
	"src/common/pixelboost/util/localisation/localisation.cpp"
	"src/common/pixelboost/util/localisation/localisation.h"
	"src/common/pixelboost/util/localisation/string.cpp"
	"src/common/pixelboost/util/localisation/string.h"
	)

set (PIXELBOOST_SOURCES
	${ASSET_SOURCES}
	${AUDIO_SOURCES}
	${DATA_RESOURCES_SOURCES}
	${DATA_SOURCES}
	${DB_TYPES_SOURCES}
	${DB_SOURCES}
	${DEBUG_SOURCES}
	${FILE_SOURCES}
	${FRAMEWORK_SOURCES}
	${GRAPHICS_CAMERA_SOURCES}
	${GRAPHICS_DEVICE_SOURCES}
	${GRAPHICS_MATERIAL_SOURCES}
	${GRAPHICS_PARTICLE_SOURCES}
	${GRAPHICS_RENDERER_SOURCES}
	${GRAPHICS_RESOURCES_SOURCES}
	${GRAPHICS_SHADER_SOURCES}
	${GRAPHICS_UI_SOURCES}
	${GRAPHICS_SOURCES}
	${INPUT_SOURCES}
	${LOGIC_COMPONENT_GRAPHICS_SOURCES}
	${LOGIC_COMPONENT_INPUT_SOURCES}
	${LOGIC_COMPONENT_PHYSICS_2D_SOURCES}
	${LOGIC_COMPONENT_PHYSICS_SOURCES}
	${LOGIC_COMPONENT_SOURCES}
	${LOGIC_MESSAGE_SOURCES}
	${LOGIC_SYSTEM_DEBUG_SOURCES}
	${LOGIC_SYSTEM_GRAPHICS_GUI_SOURCES}
	${LOGIC_SYSTEM_GRAPHICS_RENDER_SOURCES}
	${LOGIC_SYSTEM_PHYSICS_SOURCES}
	${LOGIC_SOURCES}
	${MATHS_SOURCES}
	${NETWORK_SOURCES}
	${PHYSICS_2D_SOURCES}
	${PLATFORM_SOURCES}
	${RESOURCE_SOURCES}
	${SCRIPTING_SOURCES}
	${UTIL_LOCALISATION_SOURCES}
	)

add_library (pixelboost_engine ${PIXELBOOST_LIBRARY_TYPE} ${PIXELBOOST_SOURCES})
target_link_libraries (pixelboost_engine)

if (PIXELBOOST_BUILD_PLATFORM_OSX)
	target_link_libraries (pixelboost_engine
		ddhidlib
		"-framework ApplicationServices"
		"-framework CoreFoundation"
		"-framework CoreGraphics"
		"-framework CoreServices"
		"-framework Cocoa"
		"-framework Carbon"
		"-framework Foundation"
		"-framework OpenGL"
		"-framework AppKit"
		"-framework IOKit"
		)
	pixelboost_setup_library(ddhidlib)
endif (PIXELBOOST_BUILD_PLATFORM_OSX)

if (PIXELBOOST_BUILD_PLATFORM_IOS)
	target_link_libraries (pixelboost_engine
		"-framework CoreGraphics"
		"-framework QuartzCore"
		"-framework AVFoundation"
		"-framework AudioToolbox"
		"-framework OpenAL"
		"-framework GameKit"
		"-framework Foundation"
		"-framework UIKit"
		"-framework OpenGLES")
endif (PIXELBOOST_BUILD_PLATFORM_IOS)

if (PIXELBOOST_BUILD_PLATFORM_ANDROID)
	target_link_libraries (pixelboost_engine GLESv2 log)
endif (PIXELBOOST_BUILD_PLATFORM_ANDROID)

source_group (asset FILES ${ASSET_SOURCES})
source_group (audio FILES ${AUDIO_SOURCES})
source_group (data\\resources FILES ${DATA_RESOURCES_SOURCES})
source_group (db\\types FILES ${DB_TYPES_SOURCES})
source_group (db FILES ${DB_SOURCES})
source_group (debug FILES ${DEBUG_SOURCES})
source_group (file FILES ${FILE_SOURCES})
source_group (framework FILES ${FRAMEWORK_SOURCES})
source_group (graphics\\camera FILES ${GRAPHICS_CAMERA_SOURCES})
source_group (graphics\\device FILES ${GRAPHICS_DEVICE_SOURCES})
source_group (graphics\\material FILES ${GRAPHICS_MATERIAL_SOURCES})
source_group (graphics\\particle FILES ${GRAPHICS_PARTICLE_SOURCES})
source_group (graphics\\renderer FILES ${GRAPHICS_RENDERER_SOURCES})
source_group (graphics\\resources FILES ${GRAPHICS_RESOURCES_SOURCES})
source_group (graphics\\shader FILES ${GRAPHICS_SHADER_SOURCES})
source_group (graphics FILES ${GRAPHICS_SOURCES})
source_group (input FILES ${INPUT_SOURCES})
source_group (logic\\component\\graphics FILES ${LOGIC_COMPONENT_GRAPHICS_SOURCES})
source_group (logic\\component\\input FILES ${LOGIC_COMPONENT_INPUT_SOURCES})
source_group (logic\\component\\physics\\2d FILES ${LOGIC_COMPONENT_PHYSICS_2D_SOURCES})
source_group (logic\\component\\physics FILES ${LOGIC_COMPONENT_PHYSICS_SOURCES})
source_group (logic\\component\\transform FILES ${LOGIC_COMPONENT_TRANSFORM_SOURCES})
source_group (logic\\component FILES ${LOGIC_COMPONENT_SOURCES})
source_group (logic\\message FILES ${LOGIC_MESSAGE_SOURCES})
source_group (logic\\system\\debug FILES ${LOGIC_SYSTEM_DEBUG_SOURCES})
source_group (logic\\system\\graphics\\gui FILES ${LOGIC_SYSTEM_GRAPHICS_GUI_SOURCES})
source_group (logic\\system\\graphics\\render FILES ${LOGIC_SYSTEM_GRAPHICS_RENDER_SOURCES})	
source_group (logic\\system\\physics FILES ${LOGIC_SYSTEM_PHYSICS_SOURCES})
source_group (logic FILES ${LOGIC_SOURCES})
source_group (maths FILES ${MATHS_SOURCES})
source_group (network FILES ${NETWORK_SOURCES})
source_group (physics\\2d FILES ${PHYSICS_2D_SOURCES})
source_group (platform FILES ${PLATFORM_SOURCES})
source_group (resource FILES ${RESOURCE_SOURCES})
source_group (scripting FILES ${SCRIPTING_SOURCES})
source_group (util\\localisation FILES ${UTIL_LOCALISATION_SOURCES})
