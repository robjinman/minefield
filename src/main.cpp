/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include <iostream>
#include <dodge/Exception.hpp>
#include "Application.hpp"


//===========================================
// programExit
//===========================================
void programExit() {
   exit(EXIT_SUCCESS);
}

//===========================================
// main
//===========================================
int main(int argc, char** argv) {
   Application app;

   try {
      app.onExit(Functor<void, TYPELIST_0()>(programExit));
      app.launch(argc, argv);
   }
   catch (Dodge::Exception& e) {
      std::cerr << "An error occured; " << e.what() << "\n" << std::flush;
      app.quit();
   }
   catch (...) {
      std::cerr << "An unknown error occured\n" << std::flush;
      app.quit();
   }

   return EXIT_FAILURE;
}
