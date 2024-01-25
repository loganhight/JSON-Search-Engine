#include <iostream>
#include "Query_Processor.h"

using namespace std;

int main(int argc, char **argv)
{
    Query_Processor processor;
    processor.runQueryProcessor(argc, argv);

    return 0;
}