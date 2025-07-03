// #include <Mosaic/Debug/Console.hpp>
//
// #include <Mosaic/Macros/PID.hpp>
//
// #include <chrono>
// #include <iostream>
// #include <sstream>
//
// namespace Mosaic
// {
//     Console::Console()
//         : mNextAvailableID(0)
//     {
//     }
//
//     LogTargetID Console::CreateLogTarget(const std::string& filepath)
//     {
//         LogTargetID outputID = 0;
//
//         if (!mFilepathIDs.contains(filepath))
//         {
//             outputID = mNextAvailableID;
//
//             mNextAvailableID++;
//
//             std::ofstream stream(filepath, std::ios::app);
//
//             if (!stream)
//             {
//                 Halt(stream.is_open(), "Failed to create or open log file {}", filepath);
//             }
//
//             AddInitialLogstamp(stream, filepath);
//
//             mFileOutputs.emplace(outputID, std::move(stream));
//
//             mFilepathIDs[filepath] = outputID;
//         }
//         else
//         {
//             outputID = mFilepathIDs[filepath];
//         }
//
//         return outputID;
//     }
//
//     std::string Console::GetTerminalRedirect(LogTargetID outputID)
//     {
//         return GetTimestampForTerminalOutput() + std::format(" {}{}[REDIRECT] ->{} LogTargetID {} does not exist or is unavailable. Output will be redirected to the // terminal", ANSI_BRIGHT_CYAN, ANSI_BOLD, ANSI_RESET, outputID);
//     }
//
//     std::string Console::GetFullPrefixForFileHalt()
//     {
//         return GetTimestampForFileOutput() + " [HALT] -----> ";
//     }
//
//     std::string Console::GetFullPrefixForTerminalHalt()
//     {
//         return GetTimestampForTerminalOutput() + std::format(" {}{}[HALT] ----->{} ", ANSI_BRIGHT_RED, ANSI_BOLD, ANSI_RESET);
//     }
//
//     void Console::DispatchToTerminal(const std::string& message)
//     {
//         std::cout << message << '\n';
//     }
//
//     void Console::DispatchToFile(FileOutput& fileOutput, const std::string& message)
//     {
//         fileOutput.Stream << message << '\n';
//         fileOutput.Stream.flush();
//     }
//
//     std::string Console::GetTimestampForLogstamp()
//     {
//         auto nowTimePoint = std::chrono::system_clock::now();
//         auto nowTimeT = std::chrono::system_clock::to_time_t(nowTimePoint);
//         auto localTime = *std::localtime(&nowTimeT);
//
//         std::ostringstream buffer;
//
//         buffer << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
//
//         return buffer.str();
//     }
//
//     std::string Console::GetTimestampForFileOutput()
//     {
//         auto nowTimePoint = std::chrono::system_clock::now();
//         auto nowTimeT = std::chrono::system_clock::to_time_t(nowTimePoint);
//         auto localTime = *std::localtime(&nowTimeT);
//
//         std::ostringstream buffer;
//
//         buffer << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
//
//         return buffer.str();
//     }
//
//     std::string Console::GetTimestampForTerminalOutput()
//     {
//         auto nowTimePoint = std::chrono::system_clock::now();
//         auto nowTimeT = std::chrono::system_clock::to_time_t(nowTimePoint);
//         auto localTime = *std::localtime(&nowTimeT);
//
//         std::ostringstream buffer;
//
//         buffer << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
//
//         return ANSI_CYAN + buffer.str() + ANSI_RESET;
//     }
//
//     void Console::AddInitialLogstamp(std::ofstream& file, const std::string& filepath)
//     {
//         auto pid = MOSAIC_GET_PID();
//         auto timestamp = GetTimestampForLogstamp();
//
//         std::ifstream checkFile(filepath, std::ios::ate | std::ios::binary);
//
//         const bool isNonEmpty = checkFile.tellg() > 0;
//
//         auto sessionHeader = std::format(
//             "{}--- Mosaic Log Session Started: {} | PID: {} | Filepath: {} ---\n",
//             isNonEmpty ? "\n" : "",
//             timestamp,
//             pid,
//             filepath);
//
//         file << sessionHeader;
//         file.flush();
//     }
// }