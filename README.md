ABOUT
===

Pixelboost is a portable cross-platform game engine that currently supports iOS, OSX, Android and Emscripten.

It's continuously under development, but over the years has been used on upwards of 10 commercial iOS and Android games.

There is no official support, and there likely never will be. I will answer questions and have a look at bugs if they're important to the projects I'm working on, but there's no promises. You're welcome to have a look at adding things yourself though!

LICENSE
===

See LICENSE.

There are a number of third-party libraries used within the engine, all licensed under MIT/ZLib or similar. See the individual licenses in libs/* and shared/src/external* for details.

FEATURES
===

* Basic audio support
* Threaded resource loading
* Custom schema driven database with automatic deserialisation
* File system (with pack support) via physfs
* Integrated Box2D for 2D physics
* 3D skinned animation
* Timeline/tweening animation system with easing
* Various common 3D features including multiple viewports, shader and material pipeline
* Custom JPG with PNG alpha file format for 2D images with transparency
* Network messaging system
* Entity/component based game system with various physics, input and graphics components
* Level and game data editor
* Real-time variable manipulation tools (WIP)
* Basic text localisation support via gettext files
* Various other minor features that you'd expect, with more added regularly...

PRE-REQUISITES
===

Base requirements
---

CMake 2.8.10
http://www.cmake.org/

FBX SDK 2013.3
http://usa.autodesk.com/adsk/servlet/pc/item?siteID=123112&id=10775847

XCode 4.6.1
https://developer.apple.com/xcode/

TexturePacker (+ Command Line Tools)
http://www.codeandweb.com/texturepacker

GlyphDesigner (+ Command Line Tools)
http://www.71squared.com/glyphdesigner


Android Requirements
---

Android SDK
Android NDK r8e

Set the relevant paths in your environment, e.g.
export PATH=~/Development/sdk/android-sdk-macosx/tools:~/Development/sdk/android-sdk-macosx/platform-tools:~/Development/sdk/android-ndk-r8e:/opt/local/bin:/opt/local/sbin:$PATH
export ANDROID_NDK=~/Development/sdk/android-ndk-r8e
export ANDROID_HOME=~/Development/sdk/android-ndk-r8e


Emscripten Requirements
---

Install the Emscripten SDK
https://github.com/kripken/emscripten/wiki/Getting-started-on-Mac-OS-X

Set the relevant paths in your environment, e.g.
export EMSCRIPTEN=/Users/aeonflame/Development/sdk/emscripten

SETTING UP
===

Clone the project repository, and initialise all submodules to make sure all Pixelboost dependencies are grabbed (git submodule update --init --recursive)

Once you have the code and assets downloaded follow the next few steps

Run 'launchtools.command', and note the options. You want to run in the following order:
*Make Projects* - This will generate all of the relevant projects files for iOS, OSX, Android, and Emscripten.
*Build Assets*  - This will build all of the assets and tools necessary for the project, including the level editor and game tools.
*Run Editor* - This will open the level editor, we'll go over specifics on using this later, for now just hit export.

Once these steps are complete, you're ready to run the project.

Open the project that you're interested in building within the build folder. In this case we'll use OSX as an example.

Open build/osx/<project_name>.xcodeproj.

In the scheme drop-down box, change the build from ALL_BUILD to <project_name>, and hit run.

If you are going to be modifying assets, then XCode don't always pick up asset changes. Pixelboost supports an asset override directory to get around this. During development add an argument to the command line with the path to the root of the project directory (the directory that contains launchtools.command), '--project="/path/to/project/"'. To set this, on the dropdown where you changed ALL_BUILD to <project_name>, press 'Edit Scheme...' and enter that argument into "Arguments Passed On Launch".
