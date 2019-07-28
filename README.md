# SDL Dev
Repository for C++ game engineering

## Setup (MacOS)
```
brew install sdl2
brew install sdl2_image
brew install sdl2_ttf
```
## Usage
```
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
./sdl-dev
```

## Troubleshooting
If your Mac is running a recent version of MacOS High Sierra, then there is a chance that the standard header files were moved from the assumed `/usr/include/` directory. In order to rectify this, run:
```
open /Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg
```