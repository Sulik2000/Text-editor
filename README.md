<h1>Code editor</h1>
<p>This is my implementation of code editor using C++ and Qt framework</p>
<h2>How use it?</h2>
<p>Program works only with MinGW compiler in Windows. Program in first launch requests you select *.exe file of compiler, then you can change path to compiler in <b>Code->Settings</b></p>
<p>To open folder select <b>File->Open folder</b>. Program automatically find all *.cpp files and compile them with MinGW compiler which you selected in settings. If you clicked <b>Code->Run</b> program compiles your code and launch compiled executable file with function <b>system()</b> of C++</p>
<p>To create file or folder in root path click right click on <b>".."</b></p>
<h2>How this works?</h2>
<p>Program saves path of compiler in JSON file <b>"settings.json"</b> which contains in root path of application. Program uses implemented QFileSystemModel to work with file system.</p>
<h3>This is my first full project for resume. Thanks for using</h3>