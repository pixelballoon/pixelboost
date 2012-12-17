cmake_minimum_required (VERSION 2.6)

if (PIXELBOOST_BUILD_PLATFORM_ANDROID)
	set (PLATFORM_HEADERS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/android"
		)

	set (PLATFORM_SOURCES
		"src/platform/android/pixelboost/audio/soundManager.h"
		"src/platform/android/pixelboost/audio/soundManager.cpp"
		"src/platform/android/pixelboost/file/fileHelpers.cpp"
		"src/platform/android/pixelboost/file/fileSystem.cpp"
		"src/platform/android/pixelboost/misc/jni.cpp"
		"src/platform/android/pixelboost/network/gameCenter.h"
		"src/platform/android/pixelboost/network/gameCenter.cpp"
		"src/platform/android/pixelboost/network/networkHelpers.cpp"
		"src/platform/android/pixelboost/util/localisation/localisation.cpp"
		)
endif (PIXELBOOST_BUILD_PLATFORM_ANDROID)

if (PIXELBOOST_BUILD_PLATFORM_OSX)
	set (PLATFORM_HEADERS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/mac"
		)

	set (PLATFORM_SOURCES
		"src/platform/mac/pixelboost/audio/soundManager.h"
		"src/platform/mac/pixelboost/audio/soundManager.mm"
		"src/platform/mac/pixelboost/file/fileHelpers.mm"
		"src/platform/mac/pixelboost/framework/AppDelegate.mm"
		"src/platform/mac/pixelboost/framework/main.m"
		"src/platform/mac/pixelboost/framework/MainView.mm"
		"src/platform/mac/pixelboost/network/gameCenter.h"
		"src/platform/mac/pixelboost/network/gameCenter.mm"
		"src/platform/mac/pixelboost/network/networkHelpers.mm"
		"src/platform/mac/pixelboost/util/localisation/localisation.mm"
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
		"src/platform/ios/pixelboost/audio/soundManager.h"
		"src/platform/ios/pixelboost/audio/soundManager.mm"
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

set (PIXELBOOST_HEADERS
	"${PIXELBOOST_HEADERS}"
	"${PLATFORM_HEADERS}"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/common"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/external"
	CACHE INTERNAL "Complete list of pixelboost headers")

include_directories("${PIXELBOOST_HEADERS}")

set (DATA_SOURCES
	"src/common/pixelboost/data/asset/assetLoader.cpp"
	"src/common/pixelboost/data/xml/xml.cpp"
	)

set (DB_SOURCES
	"src/common/pixelboost/db/database.cpp"
	"src/common/pixelboost/db/definitions.cpp"
	"src/common/pixelboost/db/entity.cpp"
	"src/common/pixelboost/db/record.cpp"
	"src/common/pixelboost/db/struct.cpp"
	)

set (DEBUG_SOURCES
	"src/common/pixelboost/debug/assert.cpp"
	"src/common/pixelboost/debug/debugDatabaseHandler.cpp"
	"src/common/pixelboost/debug/debugVariable.cpp"
	"src/common/pixelboost/debug/debugVariableManager.cpp"
	)

set (FILE_SOURCES
	"src/common/pixelboost/file/fileHelpers.cpp"
	"src/common/pixelboost/file/fileSystem.cpp"
	)

set (FRAMEWORK_SOURCES
	"src/common/pixelboost/framework/engine.cpp"
	"src/common/pixelboost/framework/screen.cpp"
	)

set (GRAPHICS_CAMERA_SOURCES
	"src/common/pixelboost/graphics/camera/camera.cpp"
	"src/common/pixelboost/graphics/camera/viewport.cpp"
	)

set (GRAPHICS_DEVICE_SOURCES
	"src/common/pixelboost/graphics/device/gl/device.cpp"
	"src/common/pixelboost/graphics/device/gl/program.cpp"
	"src/common/pixelboost/graphics/device/gl/texture.cpp"
	"src/common/pixelboost/graphics/device/gles1/device.cpp"
	"src/common/pixelboost/graphics/device/gles1/texture.cpp"
	"src/common/pixelboost/graphics/device/device.cpp"
	"src/common/pixelboost/graphics/device/indexBuffer.cpp"
	"src/common/pixelboost/graphics/device/program.cpp"
	"src/common/pixelboost/graphics/device/texture.cpp"
	"src/common/pixelboost/graphics/device/vertexBuffer.cpp"
	)

set (GRAPHICS_HELPER_SOURCES
	"src/common/pixelboost/graphics/helper/screenHelpers.cpp"
	)

set (GRAPHICS_RENDERER_SOURCES
	"src/common/pixelboost/graphics/renderer/box2d/box2dRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/buffer/bufferRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/common/irenderer.cpp"
	"src/common/pixelboost/graphics/renderer/common/renderable.cpp"
	"src/common/pixelboost/graphics/renderer/common/renderer.cpp"
	"src/common/pixelboost/graphics/renderer/font/fontRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/gwen/gwenRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/model/model.cpp"
	"src/common/pixelboost/graphics/renderer/model/modelRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/particle/particleRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/primitive/primitiveRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/rocket/rocketRenderer.cpp"
	"src/common/pixelboost/graphics/renderer/sprite/sprite.cpp"
	"src/common/pixelboost/graphics/renderer/sprite/spriteRenderer.cpp"
	)

set (GRAPHICS_SHADER_SOURCES
	"src/common/pixelboost/graphics/shader/manager.cpp"
	"src/common/pixelboost/graphics/shader/shader.cpp"
	)

set (GRAPHICS_UI_SOURCES
	"src/common/pixelboost/graphics/ui/rocket.cpp"
	)

set (INPUT_SOURCES
	"src/common/pixelboost/input/acceleratometerManager.cpp"
	"src/common/pixelboost/input/inputManager.cpp"
	"src/common/pixelboost/input/keyboardManager.cpp"
	"src/common/pixelboost/input/mouseManager.cpp"
	"src/common/pixelboost/input/touchManager.cpp"
	)

set (LOGIC_COMPONENT_GRAPHICS_SOURCES
	"src/common/pixelboost/logic/component/graphics/buffer.cpp"
	"src/common/pixelboost/logic/component/graphics/camera.cpp"
	"src/common/pixelboost/logic/component/graphics/ellipse.cpp"
	"src/common/pixelboost/logic/component/graphics/font.cpp"
	"src/common/pixelboost/logic/component/graphics/line.cpp"
	"src/common/pixelboost/logic/component/graphics/model.cpp"
	"src/common/pixelboost/logic/component/graphics/particleEmitter.cpp"
	"src/common/pixelboost/logic/component/graphics/rectangle.cpp"
	"src/common/pixelboost/logic/component/graphics/rocket.cpp"
	"src/common/pixelboost/logic/component/graphics/sprite.cpp"
	)

set (LOGIC_COMPONENT_INPUT_SOURCES
	"src/common/pixelboost/logic/component/input/rectTouch.cpp"
	)

set (LOGIC_COMPONENT_PHYSICS_2D_SOURCES
	"src/common/pixelboost/logic/component/physics/2d/physicsBody.cpp"
	"src/common/pixelboost/logic/component/physics/2d/userBody.cpp"
	)

set (LOGIC_COMPONENT_PHYSICS_SOURCES
	"src/common/pixelboost/logic/component/physics/physics.cpp"
	)

set (LOGIC_COMPONENT_TRANSFORM_SOURCES
	"src/common/pixelboost/logic/component/transform/basic.cpp"
	"src/common/pixelboost/logic/component/transform/parallax.cpp"
	)

set (LOGIC_COMPONENT_SOURCES
	"src/common/pixelboost/logic/component/transform.cpp"
	)

set (LOGIC_MESSAGE_SOURCES
	"src/common/pixelboost/logic/message/debug/render.cpp"
	"src/common/pixelboost/logic/message/input/touch.cpp"
	"src/common/pixelboost/logic/message/particle/finished.cpp"
	"src/common/pixelboost/logic/message/physics/collision.cpp"
	"src/common/pixelboost/logic/message/destroy.cpp"
	"src/common/pixelboost/logic/message/transform.cpp"
	"src/common/pixelboost/logic/message/update.cpp"
	)

set (LOGIC_SYSTEM_SOURCES
	"src/common/pixelboost/logic/system/debug/render.cpp"
	"src/common/pixelboost/logic/system/graphics/render/basic.cpp"
	"src/common/pixelboost/logic/system/graphics/render/bounds.cpp"
	"src/common/pixelboost/logic/system/graphics/render/render.cpp"
	"src/common/pixelboost/logic/system/physics/2d/physics.cpp"
	)

set (LOGIC_SOURCES
	"src/common/pixelboost/logic/component.cpp"
	"src/common/pixelboost/logic/entity.cpp"
	"src/common/pixelboost/logic/message.cpp"
	"src/common/pixelboost/logic/scene.cpp"
	"src/common/pixelboost/logic/system.cpp"
	)

set (MATHS_SOURCES
	"src/common/pixelboost/maths/boundingBox.cpp"
	"src/common/pixelboost/maths/boundingFrustum.cpp"
	"src/common/pixelboost/maths/boundingSphere.cpp"
	)

set (MISC_SOURCES
	"src/common/pixelboost/misc/gwen/inputHandler.cpp"
	)

set (NETWORK_SOURCES
	"src/common/pixelboost/network/http/httpServer.cpp"
	"src/common/pixelboost/network/networkMessage.cpp"
	"src/common/pixelboost/network/networkServer.cpp"
	)

set (PHYSICS_2D_SOURCES
	"src/common/pixelboost/physics/2d/helpers.cpp"
	)

set (UTIL_LOCALISATION_SOURCES
	"src/common/pixelboost/util/localisation/localisation.cpp"
	"src/common/pixelboost/util/localisation/string.cpp"
	)

set (EXTERNAL_SOURCES
	"src/external/GL/glew.c"
	"src/external/jpegcompressor/jpgd.cpp"
	"src/external/jpegcompressor/jpge.cpp"
	"src/external/lodepng/lodepng.cpp"
	"src/external/regex/trex.c"
	"src/external/stbimage/stb_image.c"
	"src/external/stbimage/stb_image_write.c"
	)

set (PIXELBOOST_SOURCES
	${DATA_SOURCES}
	${DB_SOURCES}
	${DEBUG_SOURCES}
	${FILE_SOURCES}
	${FRAMEWORK_SOURCES}
	${GRAPHICS_CAMERA_SOURCES}
	${GRAPHICS_DEVICE_SOURCES}
	${GRAPHICS_HELPER_SOURCES}
	${GRAPHICS_RENDERER_SOURCES}
	${GRAPHICS_SHADER_SOURCES}
	${GRAPHICS_UI_SOURCES}
	${INPUT_SOURCES}
	${LOGIC_COMPONENT_GRAPHICS_SOURCES}
	${LOGIC_COMPONENT_INPUT_SOURCES}
	${LOGIC_COMPONENT_PHYSICS_2D_SOURCES}
	${LOGIC_COMPONENT_PHYSICS_SOURCES}
	${LOGIC_COMPONENT_TRANSFORM_SOURCES}
	${LOGIC_COMPONENT_SOURCES}
	${LOGIC_MESSAGE_SOURCES}
	${LOGIC_SYSTEM_SOURCES}
	${LOGIC_SOURCES}
	${MATHS_SOURCES}
	${MISC_SOURCES}
	${NETWORK_SOURCES}
	${PHYSICS_2D_SOURCES}
	${PLATFORM_SOURCES}
	${UTIL_LOCALISATION_SOURCES}
	${EXTERNAL_SOURCES}
	)

add_library (pixelboost_engine STATIC ${PIXELBOOST_SOURCES})
target_link_libraries (pixelboost_engine Box2D freetype2 librocket lua)

if (PIXELBOOST_BUILD_PLATFORM_OSX)
	target_link_libraries (pixelboost_engine
		"-framework CoreGraphics"
		"-framework Cocoa"
		"-framework Foundation"
		"-framework OpenGL"
		"-framework AppKit")
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

source_group (data FILES ${DATA_SOURCES})
source_group (db FILES ${DB_SOURCES})
source_group (debug FILES ${DEBUG_SOURCES})
source_group (file FILES ${FILE_SOURCES})
source_group (framework FILES ${FRAMEWORK_SOURCES})
source_group (graphics\\camera FILES ${GRAPHICS_CAMERA_SOURCES})
source_group (graphics\\device FILES ${GRAPHICS_DEVICE_SOURCES})
source_group (graphics\\helper FILES ${GRAPHICS_HELPER_SOURCES})
source_group (graphics\\renderer FILES ${GRAPHICS_RENDERER_SOURCES})
source_group (graphics\\shader FILES ${GRAPHICS_SHADER_SOURCES})
source_group (graphics\\ui FILES ${GRAPHICS_UI_SOURCES})
source_group (input FILES ${INPUT_SOURCES})
source_group (logic\\component\\graphics FILES ${LOGIC_COMPONENT_GRAPHICS_SOURCES})
source_group (logic\\component\\input FILES ${LOGIC_COMPONENT_INPUT_SOURCES})
source_group (logic\\component\\physics\\2d FILES ${LOGIC_COMPONENT_PHYSICS_2D_SOURCES})
source_group (logic\\component\\physics FILES ${LOGIC_COMPONENT_PHYSICS_SOURCES})
source_group (logic\\component\\transform FILES ${LOGIC_COMPONENT_TRANSFORM_SOURCES})
source_group (logic\\component FILES ${LOGIC_COMPONENT_SOURCES})
source_group (logic\\message FILES ${LOGIC_MESSAGE_SOURCES})
source_group (logic\\system FILES ${LOGIC_SYSTEM_SOURCES})
source_group (logic FILES ${LOGIC_SOURCES})
source_group (maths FILES ${MATHS_SOURCES})
source_group (misc FILES ${MISC_SOURCES})
source_group (network FILES ${NETWORK_SOURCES})
source_group (physics\\2d FILES ${PHYSICS_2D_SOURCES})
source_group (platform FILES ${PLATFORM_SOURCES})
source_group (util\\localisation FILES ${UTIL_LOCALISATION_SOURCES})
