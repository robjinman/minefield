/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include <iostream>
#include <fstream>
#include <dodge/Exception.hpp>
#include "Application.hpp"

#ifdef WIN32

#include <comutil.h>
#include <shlobj.h>

#define MAIN int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

#else

#define MAIN int main(int argc, char** argv)

#endif


using namespace std;


//===========================================
// programExit
//===========================================
void programExit() {
   exit(EXIT_SUCCESS);
}

//===========================================
// main
//===========================================
MAIN {
   Dodge::projectSettings_t settings;

#ifdef LINUX
   string str(argv[0]);
   size_t i = str.find_last_of("/\\");

   if (i != string::npos) {
      settings.workingDir = str.substr(0, static_cast<unsigned int>(i));
   }
#endif
#ifdef WIN32
   DWORD len = 128;
   TCHAR buf[128];

   int bytes = GetModuleFileName(NULL, buf, len);

   if (bytes > 0) {
      string str(buf);
      size_t i = str.find_last_of("/\\");
      settings.workingDir = str.substr(0, static_cast<unsigned int>(i));
   }

   int argc = 0;
   char** argv = NULL;
#endif

   Dodge::gInitialise(settings);

   Application app;

   try {
      app.onExit(Functor<void, TYPELIST_0()>(programExit));
      app.launch(argc, argv);
   }
   catch (Dodge::Exception& e) {
      ofstream log(settings.workingDir + "/log.txt");
      log << "An error occured; " << e.what() << "\n" << flush;
      log.close();
      app.quit();
   }
   catch (...) {
      ofstream log(settings.workingDir + "/log.txt");
      log << "An unknown error occured\n" << flush;
      log.close();
      app.quit();
   }

   return EXIT_FAILURE;
}
