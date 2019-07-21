# Project Reality Mumble

This repository contains a fork of the open source Mumble project, designed to be used as
an integrated part of the Project Reality mod. The integration of Mumble into the game
allowed us to replace BF2's VOIP system with positional audio and higher quality codecs.

In addition to providing positional audio this fork also takes care of creating shortcuts
for in game communication, such as cross-squad whispers, local speech, commander whisper etc.

The client is meant to be executed soley by the PR Launcher on game launch since it's
incapable of connecting to regular mumble servers and PR murmur without being in an active
game session.

# WINDOWS PREREQUISITES:
Get all the prerequires listed here(Make sure you follow up on the "temporarily remove Microsoft Visual C++ 2010 * Redistributable for the installer to work"):
https://web.archive.org/web/20170609174712/http://wiki.mumble.info/wiki/BuildingWindows
Direct X SDK (June 2010) isn't mentioned there but you need it as well.

# DEPENDENCIES:

## Windows
Follow the guide at Mumble's wiki to install the dependencies from the mumble-releng repo:
https://web.archive.org/web/20170609174712/http://wiki.mumble.info/wiki/BuildingWindows
Run everything through the cygwin shell provided at C:\MumbleBuild\win32-static-1.3.x-2017-06-03-f0df4cd-863 and replace
the mumble-releng folder in C:\MumbleBuild\win32-static-1.3.x-2017-06-03-f0df4cd-863 with the one in this repo and work from there.

I had to reinstall the prerequisites twice and tweak a couple of mumble-releng scripts to 
get things to work. Hopefully the fixes will work for you as well but you might need to 
hack it a bit.

I couldn't get Ice-3.6.3 to fully compile so I downloaded the official version from
https://github.com/zeroc-ice/ice/releases
installed it and copied the missing files from C:\Program Files (x86)\ZeroC\Ice-3.6.3\* to C:\Program Files (x86)\ZeroC\Ice-3.6.3\*

## Linux
Simply run:

    apt-get install build-essential pkg-config qt5-default libqt5svg5-dev \
                libboost-dev libasound2-dev libssl-dev \
                libspeechd-dev libzeroc-ice-dev libpulse-dev \
                libcap-dev libprotobuf-dev protobuf-compiler \
                libogg-dev libavahi-compat-libdnssd-dev libsndfile1-dev \
                libg15daemon-client-dev libxi-dev 
                
# BUILDING 

## WINDOWS PRMUMBLE AND PRMURMUR:
Once you have all of the dependencies ready enter the mumble repo in this folder using the 
windows shell provided at `C:\MumbleBuild\win32-static-1.3.x-xxxx-xx-xx-tag-xxx` and 
configure the repo with:

    qmake -recursive main.pro CONFIG+="release static no-overlay no-bonjour no-update \
    no-asio no-g15 tests warnings-as-errors no-elevation no-crash-report" CONFIG-="sse2"

Then you will be able to run:

    jom clean
    jom

### Windows build with Linux
I found this way easier than building on windows since the dependencies just work without 
any effort.

1) Add MXE's repository to your system's sources

    echo "deb http://pkg.mxe.cc/repos/apt/debian jessie main" > /etc/apt/sources.list.d/mxeapt.list
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys D43A795B73B16ABE9643FE1AFD8FFF16DB45C6AB
    apt-get update

2) Install the required packages

    apt-get install \
	mxe-${ARCH}-w64-mingw32.static-qtbase \
	mxe-${ARCH}-w64-mingw32.static-qtsvg \
	mxe-${ARCH}-w64-mingw32.static-qttools \
	mxe-${ARCH}-w64-mingw32.static-qttranslations \
	mxe-${ARCH}-w64-mingw32.static-boost \
	mxe-${ARCH}-w64-mingw32.static-protobuf \
	mxe-${ARCH}-w64-mingw32.static-sqlite \
	mxe-${ARCH}-w64-mingw32.static-flac \
 	mxe-${ARCH}-w64-mingw32.static-ogg \
	mxe-${ARCH}-w64-mingw32.static-vorbis \
	mxe-${ARCH}-w64-mingw32.static-libsndfile

3) Export environment variable to tell QMake where MXE's protobuf compiler is

    export MUMBLE_PROTOC=/usr/lib/mxe/usr/x86_64-unknown-linux-gnu/bin/protoc

4) Add MXE's directory to PATH

    PATH=$PATH:/usr/lib/mxe/usr/bin

5) Run QMake to process the project(s) files

    ${ARCH}-w64-mingw32.static-qmake-qt5 -recursive CONFIG+="release g15-emulator no-overlay no-bonjour no-elevation no-ice"

6) Start the build

    make

NOTE: Overlay, Logitech G15 LCD, ZeroC Ice, Bonjour won't work when compiling with linux but we aren't using those atm anyways.

## LINUX PRMURMUR
NOTE: If you were building the windows client through linux open a new shell without the 
MUMBLE_PROTOC export and then run:

    qmake -recursive main.pro CONFIG+=no-client
    make

# UPGRADING MUMBLE:
Checkout to the master branch and run:

    git pull https://github.com/mumble-voip/mumble master

Once master's head is synced with upstream - checkout to the PRMumble branch and run 
git merge with the master branch.

Go over the changes carefully and take a look at the commit history of the PRMumble branch 
to understand which changes were required for it to work with PR's Launcher and audio dlls.
