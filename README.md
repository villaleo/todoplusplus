# todoplusplus

A command line application used to log events. Available actions include inserting, removing, and viewing events. In
addition, there are `save` and `open` commands to save the list to new or existing file or to open and load the contents
of an existing list. Closing the application will not automatically save the contents of the list.

![A preview of the application](images/demo-todopp.gif)

<h2>Installation</h2>
This application was made using CMake, therefore ensure the environment you are using supports running C++ applications
using CMake. <br>
<h3>macOS</h3>
<ul>
  <li>
  I built this application using <i>CLion</i> and Apple clang version 12.0.5 (clang-1205.0.22.9). In addition,
  this project was built using the C++ 17 standard. I could not find any reliable sources nor methods to open a
  CMake project on macOS without using CLion.
  </li>
</ul>
<h3>Windows</h3>
<ul>
  <li>
  Using <b>Visual Studio</b>, open the Visual Studio Installer. Click on <i>Modify</i> and on the right side of the
  pop-up window, scroll down and <br>
  expand the menu labeled <i>Desktop development with C++</i>. Expand the <i>optional</i> submenu and ensure that "C++
  CMake tools for Windows" is checked.
  If it was not checked, install the components by clicking <i>Modify</i> in the bottom right of the window. After
  installing the CMake tools for Windows,
  launch Visual Studio. <br>
  </li>
  <li>
  Click on the "Clone a repository" option on the right side of the window. Enter
  https://github.com/villaleo/todoplusplus.git as the repository location.
  Choose the path to an empty folder where you would like the project to be saved to. Next, click on <i>Clone</i> in
  the bottom right. <br>
  </li>
  <li>
  Wait until Visual Studio finishes opening the project. If the <i>Solution Explorer</i> submenu does not display the
  project files, click on
  <i>Folder View</i> to toggle on the project files. Open the <i>CMakeLists.txt</i> file and choose it as the startup
  item. Debug the file.
  </li>
</ul>
