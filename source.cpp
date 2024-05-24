#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

enum class FileExistsOption {
    Overwrite,
    Append,
    Keep
};

bool file_exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

int main(int argc, char* argv[]) {
    std::string destination_file;
    std::string input_string;
    FileExistsOption file_exists_option = FileExistsOption::Keep;

    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if(arg == "--destination_file" && i + 1 < argc) {
            destination_file = argv[++i];
        } else if(arg == "--string" && i + 1 < argc) {
            input_string = argv[++i];
        } else if(arg == "--file_exists" && i + 1 < argc) {
            std::string option = argv[++i];
            if(option == "overwrite") {
                file_exists_option = FileExistsOption::Overwrite;
            } else if(option == "append") {
                file_exists_option = FileExistsOption::Append;
            } else if(option == "keep") {
                file_exists_option = FileExistsOption::Keep;
            } else {
                std::cerr << "Unknown file_exists option: " << option << "\n";
                return 1;
            }
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return 1;
        }
    }

    if(destination_file.empty() || input_string.empty()) {
        std::cerr << "Usage: " << argv[0] << " --destination_file <filename> --string <string> [--file_exists <overwrite|append|keep>]\n";
        return 1;
    }

    if(file_exists_option == FileExistsOption::Keep && file_exists(destination_file)) {
        return 1;
    }

    std::ofstream file;
    if(file_exists_option == FileExistsOption::Append) {
        file.open(destination_file, std::ios_base::app);
    } else {
        file.open(destination_file);
    }

    if(!file) {
        std::cerr << "Error opening file " << destination_file << "\n";
        return 1;
    }

    file << input_string;
    file.close();

    return 0;
}