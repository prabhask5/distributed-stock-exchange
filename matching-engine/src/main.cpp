#include <DefaultDomainParticipant.hpp>

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

std::atomic<bool> is_running;

int main(int argc, char *argv[])
{
    try
    {
        std::string config_file = "";
        boost::program_options::options_description options{"Options"};

        options.add_options()("help,h", "Help screen")("config,c", boost::program_options::value<std::string>()->default_value(""), "Matching Engine Config File");

        boost::program_options::variables_map vm;
        boost::program_options::store(parse_command_line(argc, argv, options), vm);
        boost::program_options::notify(vm);

        if (vm.count("help"))
            std::cout << options << '\n';
        else if (vm.count("config"))
            config_file = vm["config"].as<std::string>();

        if (config_file.empty())
        {
            std::cerr << "Error: Config file name is not specified." << std::endl;
            return -1;
        }

        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(config_file, pt);

        std::string data_service = pt.get<std::string>("matchingengine.dataservice");
        std::string market = pt.get<std::string>("matchingengine.market");

        // this defines the interval on how often to publish market data to the data service
        int data_pub_interval = pt.get<int>("marketdata.data_pub_interval");

        LOG4CXX_INFO(logger, "Market Name|" << market << "|Data Service Name|" << data_service << "|Market Data Publication Interval|" << data_pub_interval);

        std::atomic_init(&is_running, true);

        auto participant_ptr = std::make_shared<DefaultDomainParticipant>(0, "MatchingEngine");

        participant_ptr->create_subscriber();
        participant_ptr->create_publisher();
    }
    catch (std::exception &e)
    {
        LOG4CXX_ERROR(logger, "Exception during the initialization of Matching Engine :" << e.what());

        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
