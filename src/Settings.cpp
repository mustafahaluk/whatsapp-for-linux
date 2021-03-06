#include "Settings.hpp"
#include <fstream>
#include <iostream>


namespace
{
    auto const CONFIG_DIR              = std::string{getenv("HOME")} + "/.config/whatsapp-for-linux";
    auto const CONFIG_FILE_PATH        = CONFIG_DIR + "/settings.conf";
    constexpr auto const GROUP_NETWORK = "network";
}


Settings& Settings::instance()
{
    static auto instance = Settings{};
    return instance;
}

Settings::Settings()
{
    auto inputFile = std::ifstream{CONFIG_FILE_PATH};
    if (!inputFile.good())
    {
        auto const createDirCommand = "mkdir -p " + CONFIG_DIR;
        system(createDirCommand.c_str());

        auto outputFile = std::ofstream{CONFIG_FILE_PATH};
        outputFile << '[' << GROUP_NETWORK << ']';
    }

    keyFile.load_from_file(CONFIG_FILE_PATH);
}

Settings::~Settings()
{
    keyFile.save_to_file(CONFIG_FILE_PATH);
}

void Settings::setAllowPermissions(bool allow)
{
    keyFile.set_boolean(GROUP_NETWORK, "allow_permissions", allow);
}

bool Settings::allowPermissions() const
{
    auto allow = false;
    try
    {
        allow = keyFile.get_boolean(GROUP_NETWORK, "allow_permissions");
    }
    catch (Glib::KeyFileError const& error)
    {
        std::cerr << "Settings: " <<  error.what() << std::endl;
    }

    return allow;
}
