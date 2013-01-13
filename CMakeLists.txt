cmake_minimum_required (VERSION 2.8.10)
project (editor)

include (../../build.cmake)

pixelboost_setup_pre(../../ editor "com.pixelballoon.editor" "Editor")

include_directories("source")

set (GWEN_SOURCES
	"../../engine/src/external/gwen/src/Controls/Dialog/FileOpen.cpp"
	"../../engine/src/external/gwen/src/Controls/Dialog/FileSave.cpp"
	"../../engine/src/external/gwen/src/Controls/Dialog/FolderOpen.cpp"
	"../../engine/src/external/gwen/src/Controls/Dialog/Query.cpp"
	"../../engine/src/external/gwen/src/Controls/Base.cpp"
	"../../engine/src/external/gwen/src/Controls/Button.cpp"
	"../../engine/src/external/gwen/src/Controls/Canvas.cpp"
	"../../engine/src/external/gwen/src/Controls/CheckBox.cpp"
	"../../engine/src/external/gwen/src/Controls/CollapsibleCategory.cpp"
	"../../engine/src/external/gwen/src/Controls/ColorControls.cpp"
	"../../engine/src/external/gwen/src/Controls/ColorPicker.cpp"
	"../../engine/src/external/gwen/src/Controls/ComboBox.cpp"
	"../../engine/src/external/gwen/src/Controls/CrossSplitter.cpp"
	"../../engine/src/external/gwen/src/Controls/DockBase.cpp"
	"../../engine/src/external/gwen/src/Controls/DockedTabControl.cpp"
	"../../engine/src/external/gwen/src/Controls/Dragger.cpp"
	"../../engine/src/external/gwen/src/Controls/GroupBox.cpp"
	"../../engine/src/external/gwen/src/Controls/HorizontalScrollBar.cpp"
	"../../engine/src/external/gwen/src/Controls/HorizontalSlider.cpp"
	"../../engine/src/external/gwen/src/Controls/HSVColorPicker.cpp"
	"../../engine/src/external/gwen/src/Controls/ImagePanel.cpp"
	"../../engine/src/external/gwen/src/Controls/Label.cpp"
	"../../engine/src/external/gwen/src/Controls/LabelClickable.cpp"
	"../../engine/src/external/gwen/src/Controls/ListBox.cpp"
	"../../engine/src/external/gwen/src/Controls/Menu.cpp"
	"../../engine/src/external/gwen/src/Controls/MenuItem.cpp"
	"../../engine/src/external/gwen/src/Controls/MenuStrip.cpp"
	"../../engine/src/external/gwen/src/Controls/NumericUpDown.cpp"
	"../../engine/src/external/gwen/src/Controls/PageControl.cpp"
	"../../engine/src/external/gwen/src/Controls/ProgressBar.cpp"
	"../../engine/src/external/gwen/src/Controls/Properties.cpp"
	"../../engine/src/external/gwen/src/Controls/PropertyTree.cpp"
	"../../engine/src/external/gwen/src/Controls/RadioButton.cpp"
	"../../engine/src/external/gwen/src/Controls/RadioButtonController.cpp"
	"../../engine/src/external/gwen/src/Controls/Rectangle.cpp"
	"../../engine/src/external/gwen/src/Controls/ResizableControl.cpp"
	"../../engine/src/external/gwen/src/Controls/Resizer.cpp"
	"../../engine/src/external/gwen/src/Controls/RichLabel.cpp"
	"../../engine/src/external/gwen/src/Controls/ScrollBar.cpp"
	"../../engine/src/external/gwen/src/Controls/ScrollBarBar.cpp"
	"../../engine/src/external/gwen/src/Controls/ScrollBarButton.cpp"
	"../../engine/src/external/gwen/src/Controls/ScrollControl.cpp"
	"../../engine/src/external/gwen/src/Controls/Slider.cpp"
	"../../engine/src/external/gwen/src/Controls/SplitterBar.cpp"
	"../../engine/src/external/gwen/src/Controls/TabButton.cpp"
	"../../engine/src/external/gwen/src/Controls/TabControl.cpp"
	"../../engine/src/external/gwen/src/Controls/TabStrip.cpp"
	"../../engine/src/external/gwen/src/Controls/Text.cpp"
	"../../engine/src/external/gwen/src/Controls/TextBox.cpp"
	"../../engine/src/external/gwen/src/Controls/TextBoxNumeric.cpp"
	"../../engine/src/external/gwen/src/Controls/TreeControl.cpp"
	"../../engine/src/external/gwen/src/Controls/TreeNode.cpp"
	"../../engine/src/external/gwen/src/Controls/VerticalScrollBar.cpp"
	"../../engine/src/external/gwen/src/Controls/VerticalSlider.cpp"
	"../../engine/src/external/gwen/src/Controls/WindowCanvas.cpp"
	"../../engine/src/external/gwen/src/Controls/WindowControl.cpp"
	"../../engine/src/external/gwen/src/Platforms/Null.cpp"
	"../../engine/src/external/gwen/src/Platforms/Windows.cpp"
	"../../engine/src/external/gwen/src/Anim.cpp"
	"../../engine/src/external/gwen/src/BaseRender.cpp"
	"../../engine/src/external/gwen/src/ControlList.cpp"
	"../../engine/src/external/gwen/src/DragAndDrop.cpp"
	"../../engine/src/external/gwen/src/events.cpp"
	"../../engine/src/external/gwen/src/Gwen.cpp"
	"../../engine/src/external/gwen/src/Hook.cpp"
	"../../engine/src/external/gwen/src/inputhandler.cpp"
	"../../engine/src/external/gwen/src/Skin.cpp"
	"../../engine/src/external/gwen/src/ToolTip.cpp"
	"../../engine/src/external/gwen/src/Utility.cpp"
	)

set (COMMAND_SOURCES
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

set (HTTP_SOURCES
	"source/http/commands/file.cpp"
	"source/http/commands/file.h"
	"source/http/commands/record.cpp"
	"source/http/commands/record.h"
	"source/http/commands/schema.cpp"
	"source/http/commands/schema.h"
	"source/http/httpInterface.cpp"
	"source/http/httpInterface.h"
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

set (VIEW_SOURCES
	"source/view/entity/property/model.cpp"
	"source/view/entity/property/model.h"
	"source/view/entity/property/property.cpp"
	"source/view/entity/property/property.h"
	"source/view/entity/property/sprite.cpp"
	"source/view/entity/property/sprite.h"
	"source/view/entity/entity.cpp"
	"source/view/entity/entity.h"
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
	"source/view/ui/create/createPanel.cpp"
	"source/view/ui/create/createPanel.h"
	"source/view/ui/create/createRecord.cpp"
	"source/view/ui/create/createRecord.h"
	"source/view/ui/property/propertyPanel.cpp"
	"source/view/ui/property/propertyPanel.h"
	"source/view/ui/settings/moveManipulator.cpp"
	"source/view/ui/settings/moveManipulator.h"
	"source/view/ui/settings/networkWindow.cpp"
	"source/view/ui/settings/networkWindow.h"
	"source/view/ui/settings/rotateManipulator.cpp"
	"source/view/ui/settings/rotateManipulator.h"
	"source/view/level.cpp"
	"source/view/level.h"
	)

set (GAME_SOURCES
	${COMMAND_SOURCES}
	${CORE_SOURCES}
	${GWEN_SOURCES}
	${HTTP_SOURCES}
	${PROJECT_SOURCES}
	${VIEW_SOURCES}
	"source/core.cpp"
	"source/core.h"
	"source/view.cpp"
	"source/view.h"
	)

source_group (command FILES ${COMMAND_SOURCES})
source_group (core FILES ${CORE_SOURCES})
source_group (gwen FILES ${GWEN_SOURCES})
source_group (http FILES ${HTTP_SOURCES})
source_group (project FILES ${PROJECT_SOURCES})
source_group (view FILES ${VIEW_SOURCES})

pixelboost_setup_post()
