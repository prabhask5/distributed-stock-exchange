#include <DefaultDomainParticipantConstants.hpp>
#include <atomic>
#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <iostream>

std::atomic<bool> is_running;

int main(int argc, char *argv[]) {
  LOG4CXX_INFO(logger, "Data service starting up");

  try {
    // Set up program options to select config file to use for matching engine
    // preferences.
    std::string config_file = "";
    boost::program_options::options_description options{"Options"};

    options.add_options()("help,h", "Help screen")(
        "config,c",
        boost::program_options::value<std::string>()->default_value(""),
        "QuickFIX Config file");

    boost::program_options::variables_map vm;
    boost::program_options::store(parse_command_line(argc, argv, options), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
      std::cout << options << '\n';
    else if (vm.count("config"))
      config_file = vm["config"].as<std::string>();

    if (config_file.empty())
      throw std::runtime_error("Error: Config file name is not specified.");

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(config_file, pt);
  } catch (std::exception &e) {
    LOG4CXX_ERROR(logger, "Exception during the initialization of Data Service"
                              << e.what());
    return 1;
  }
}