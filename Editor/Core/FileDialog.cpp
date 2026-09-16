//
// Created by sunvy on 16/09/2026.
//

#include "FileDialog.h"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

namespace
{
#ifdef _WIN32
    std::string BuildWindowsFilter(std::string_view extensionFilter)
    {
        std::string pattern = "*.";
        pattern += extensionFilter;

        std::string filter = "World files (";
        filter += pattern;
        filter += ')';
        filter.push_back('\0');
        filter += pattern;
        filter.push_back('\0');
        filter += "All files";
        filter.push_back('\0');
        filter += "*.*";
        filter.push_back('\0');
        filter.push_back('\0');

        return filter;
    }
#else
    std::string ShellQuote(const std::string& value)
    {
        std::string result = "'";
        for (const char character : value)
        {
            if (character == '\'')
                result += "'\\''";
            else
                result += character;
        }
        result += "'";
        return result;
    }

    bool CommandExists(const char* command)
    {
        const std::string testCommand = "command -v " + std::string(command) + " >/dev/null 2>&1";
        return std::system(testCommand.c_str()) == 0;
    }

    std::optional<std::filesystem::path> RunFileDialogCommand(const std::string& command)
    {
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe)
            return std::nullopt;

        std::array<char, 1024> buffer{};
        std::string output;
        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe))
            output += buffer.data();

        const int result = pclose(pipe);
        if (result != 0 || output.empty())
            return std::nullopt;

        while (!output.empty() && (output.back() == '\n' || output.back() == '\r'))
            output.pop_back();

        if (output.empty())
            return std::nullopt;

        return std::filesystem::path(output);
    }
#endif
}

namespace Sunset
{
    std::optional<std::filesystem::path> FileDialog::OpenFile(
        std::string_view title,
        const std::filesystem::path& initialDirectory,
        std::string_view extensionFilter
    )
    {
#ifdef _WIN32
        char filepath[MAX_PATH] = {};
        std::string filter = BuildWindowsFilter(extensionFilter);
        const std::string initialDirectoryString = initialDirectory.string();
        const std::string titleString = std::string(title);

        OPENFILENAMEA openFileName = {};
        openFileName.lStructSize = sizeof(OPENFILENAMEA);
        openFileName.lpstrFile = filepath;
        openFileName.nMaxFile = MAX_PATH;
        openFileName.lpstrFilter = filter.c_str();
        openFileName.nFilterIndex = 1;
        openFileName.lpstrInitialDir = initialDirectoryString.c_str();
        openFileName.lpstrTitle = titleString.c_str();
        openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&openFileName) == TRUE)
            return std::filesystem::path(filepath);

        return std::nullopt;
#else
        const std::string titleArg = ShellQuote(std::string(title));
        const std::string initialDirectoryArg = ShellQuote(initialDirectory.string());
        const std::string extension = std::string(extensionFilter);

        if (CommandExists("zenity"))
        {
            const std::string command =
                "zenity --file-selection --title=" + titleArg +
                " --filename=" + ShellQuote((initialDirectory / "").string()) +
                " --file-filter=" + ShellQuote("World files (*." + extension + ") | *." + extension);

            return RunFileDialogCommand(command);
        }

        if (CommandExists("kdialog"))
        {
            const std::string command =
                "kdialog --title " + titleArg +
                " --getopenfilename " + initialDirectoryArg +
                " " + ShellQuote("*." + extension + "|World files (*." + extension + ")");

            return RunFileDialogCommand(command);
        }

        return std::nullopt;
#endif
    }
} // Sunset
