#include "../../include/utilities/toml.hpp"

#include <filesystem>

namespace Mosaic::Utilities
{
    TOMLFile::TOMLFile(const std::string& path)
    {
        mFilename = path;
    }

    void TOMLFile::Open()
    {
        if (not std::filesystem::exists(mFilename))
        {
            Utilities::Throw("File {} does not exist", mFilename);
        }

        mData = toml::parse_file(mFilename);
    }

    void TOMLFile::Save() const
    {
        std::ofstream out(mFilename);

        out << mData;
    }

    std::vector<std::string> TOMLFile::SplitKey(const std::string& key)
    {
        std::vector<std::string> result;

        std::uint64_t start = 0;
        std::uint64_t end = 0;

        while ((end = key.find('.', start)) != std::string::npos)
        {
            result.push_back(key.substr(start, end - start));

            start = end + 1;
        }

        result.push_back(key.substr(start));

        return result;
    }
}