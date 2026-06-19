#include <iostream>

namespace {

int run(const char* input_path, const char* output_path) {
    (void)input_path;
    (void)output_path;
    std::cerr << "RMST solver pipeline is not implemented yet.\n";
    return 2;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
        return 1;
    }

    return run(argv[1], argv[2]);
}
