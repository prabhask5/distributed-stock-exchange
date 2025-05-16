#include <DefaultDomainParticipant.hpp>
#include <DefaultDomainParticipantConstants.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <quickfix/SessionSettings.h>

std::atomic<bool> is_running;

int main(int argc, char *argv[]) {
  LOG4CXX_INFO(logger, "FIX gateway starting up");

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
      std::runtime_error("Error: Config file name is not specified.");

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(config_file, pt);

    // Parse settings/preferences from config file.
    auto settings = std::make_shared<FIX::SessionSettings>(config_file);
    std::string fix_gateway_name = settings->get().getString("SenderCompID");

    // Initialize this machines default session from config file.
    FIX::SessionID default_session_id("FIX.4.4", "DEFAULT", "DEFAULT");
    auto session_settings = settings->get(default_session_id);

    // Parse env variables.
    auto home_env = std::getenv("DSE_HOME");
    auto log_env = std::getenv("EXCHANGE_LOGDIR");
    if (home_env == NULL || log_env == NULL)
      throw std::runtime_error("DSE_HOME or/and EXCHANGE_LOGDIR is not set");

    // Create quickfix data dictionary.
    auto default_dictionary = std::make_shared<FIX::Dictionary>(
        FIX::Dictionary(settings->get(default_session_id)));
    default_dictionary->setString(
        "DATADICTIONARY", std::string(home_env) + "/spec/" +
                              default_dictionary->getString("DATADICTIONARY"));

    // Parse data service name from data dictionary.
    std::string data_service_name =
        default_dictionary->getString("DataService");

    LOG4CXX_INFO(logger, "SenderCompID| " << fix_gateway_name
                                          << " |Data Service Name| "
                                          << data_service_name);

    // Make DDS participant representing matching engine, and set up
    // publisher/subscriber.
    auto participant_ptr =
        std::make_shared<DefaultDomainParticipant>(0, fix_gateway_name);

    participant_ptr->create_publisher();
    participant_ptr->create_subscriber();
  } catch (std::exception &e) {
    LOG4CXX_ERROR(logger, "Exception during the initialization of FIX Gateway: "
                              << e.what());

    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}