#include <iostream>
using std::ostream;
using std::istream;
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <string>
using std::string;

void output_function(istream& in, ostream& out) {
    string cmd;
    string symbol;
    int tag;
    
    out << "const char* tag_to_name(int tag) {" << endl
         << "  switch(tag) {" << endl;
    while(in) {
        in >> cmd;
        if (cmd != "#define") continue;
        in >> symbol;
        in >> tag;
        if (in) {
            out << "    case " << tag << ":" << endl
                << "      return \"" << symbol << "\";" << endl;
        }
    }

    out << "    default:" << endl
        << "      return \"UNKNOWN\";" << endl
        << "  }" << endl
        << "}" << endl;

}

int main(int argc, const char* argv[]) {

    const char* infile;
    const char* outfile = 0;

    if (argc == 1) {
        cerr << "usage: " << argv[0] << " input.h [-o output.c]" << endl;
        return 1;
    }

    for(int i=1; i < argc; ++i) {
        if (string(argv[i]) == "-o") {
            ++i;
            if (i == argc) {
                cerr << "No value give for -o option." << endl;
                return 1;
            }
            outfile = argv[i];
        } else {
            infile = argv[i];        
        }
    }

    ifstream in(infile);
    if (outfile) {
        ofstream out(outfile);
        output_function(in, out);
        out.close();
    } else {
        output_function(in, cout);
    }
    in.close();

}