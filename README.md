# wxwidgets-cmake
Some of the wxWidgets samples with CMake build files for vcpkg
## Build
### On Windows
#### With Visual Studio
- Install vcpkg by following the instructions [here](https://github.com/microsoft/vcpkg)
- Install [wxwidgets](https://www.wxwidgets.org) with command ```vcpkg install wxwidgets:x64-windows``` 
- Clone this ```wxwidgets-cmake``` repo
- Open Visual Studio, select Open Folder and browse to the ```wxwidgets-cmake``` directory. CMake will kick in and generate build files.
- Select ```Build all```
- Select a sample application in startup target pull down menu
- Select ```Debug->Start without debugging```
##### Create release build
- Create an ```x64-Release``` configuration in ```Project->CMake settings->Configurations``` and save with ```Ctrl+S```
- Select the ```x64-Release``` configuration and select ```Build all```
#### Without Visual Studio
In order to use vcpkg with CMake outside of an IDE, you can use the toolchain file:
```
> cmake -B [build directory] -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
> cmake --build [build directory]```
