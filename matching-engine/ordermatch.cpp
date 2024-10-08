#ifdef _MSC_VER
#pragma warning(disable : 4503 4355 4786)
#endif

#include "quickfix/config.h"

#include "quickfix/FileStore.h"
#include "quickfix/SocketAcceptor.h"
#include "quickfix/SessionSettings.h"
#include "Application.h"

#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cout << "usage: " << argv[0]
              << " FILE." << std::endl;
    return 0;
  }
  std::string file = argv[1];

  try
  {
    FIX::SessionSettings settings(file);

    Application application;
    FIX::FileStoreFactory storeFactory(settings);
    FIX::ScreenLogFactory logFactory(settings);
    FIX::SocketAcceptor acceptor(application, storeFactory, settings, logFactory);

    acceptor.start();
    while (true)
    {
      std::string value;
      std::cin >> value;

      if (value == "#symbols")
        application.orderMatcher().display();
      else if (value == "#quit")
        break;
      else
        application.orderMatcher().display(value);

      std::cout << std::endl;
    }
    acceptor.stop();
    return 0;
  }
  catch (std::exception &e)
  {
    std::cout << e.what() << std::endl;
    return 1;
  }
}