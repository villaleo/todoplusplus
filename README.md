# todoplusplus
A command line application used to log tasks and save to a file. Available actions include inserting, removing, and viewing tasks.
The user also the ability to save list contents to a new `.txt` file and to open an existing file created by the application to continue
from where it last left off. Closing the application will not save the contents of the list unless explicity asked to using the `sv` command. <br>

<h2>Installation</h2>
This application was made using CMake, therefore ensure the environment you are using supports running C++ applications using CMake. <br>
<h3>macOS</h3>
<ul>
  <li>Install <i>Xcode</i> from the App Store and ensure that you have C++ installed on your Mac.</li>
  <li>Clone the repository and open the project using CLion. This application uses the C++ 17 standard.</li>
  <li>Build and run the application directly from within CLion.</li>
</ul>
<h3>Windows</h3>
<ul>
  <li>
  Using <b>Visual Studio</b>, open the Visual Studio Installer. Click on <i>Modify</i> and on the right side of the pop-up window, scroll down and <br>
  expand the menu labeled <i>Desktop development with C++</i>. Expand the <i>optional</i> submenu and ensure that "C++ CMake tools for Windows" is checked.
  If it was not checked, install the components by clicking <i>Modify</i> in the bottom right of the window. After installing the CMake tools for Windows,
  launch Visual Studio. <br>
  </li>
  <li>
  Click on the "Clone a repository" option on the right side of the window. Enter https://github.com/villaleo/todoplusplus.git as the repository location.
  Choose the path to an empty folder where you would like the project to be saved to. Next, click on <i>Clone</i> in the bottom right. <br>
  </li>
  <li>
  Wait until Visual Studio finishes opening the project. If the <i>Solution Explorer</i> submenu does not display the project files, click on
  <i>Folder View</i> to toggle on the project files. Open the <i>CMakeLists.txt</i> file and choose it as the startup item. Debug the file.
  </li>
</ul>
