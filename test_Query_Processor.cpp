#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "Query_Processor.h"

TEST_CASE("Check that Query_Processor works - small dataset", "[Query_Processor]")
{
    Query_Processor processor;

    // "./test_data" is in my build folder - contains 292 documents from total dataset
    // tests createInitialIndex()
    processor.createInitialIndex((char *)"./test_data");

    vector<string> queryArguments = {"stocks", "ORG:reuters"};

    // tests runQuery(), findIntersection(), addDocInfo(), displayQueryResults(), printPair(), printFullText() - must type "yes" or "no" while testing
    // should have 10 results
    processor.runQuery(queryArguments);

    queryArguments = {"stocks", "ORG:reuters", "-italy", "-argentina", "-deficit"};

    // tests runQuery(), findIntersection(), findComplement(), addDocInfo(), displayQueryResults(), printPair(), printFullText() - must type "yes" or "no" while testing
    // should have 7 results
    processor.runQuery(queryArguments);

    queryArguments = {"markets", "tax", "germany"};

    // tests runQuery(), findIntersection(), findComplement(), addDocInfo(), displayQueryResults(), printPair(), printFullText() - must type "yes" or "no" while testing
    // should have 103 results
    processor.runQuery(queryArguments);
}

TEST_CASE("Check that Query_Processor works - large dataset", "[Query_Processor]")
{
    Query_Processor processor;

    // "./total_dataset" is all ~300k documents; "coll_1" is a folder of ~57k - takes time to create
    // tests createInitialIndex()
    processor.createInitialIndex((char *)"./total_dataset/coll_1");

    vector<string> queryArguments = {"stocks", "ORG:reuters"};

    // tests runQuery(), findIntersection(), addDocInfo(), displayQueryResults(), printPair(), printFullText() - must type "yes" or "no" while testing
    // should have 3501 results
    processor.runQuery(queryArguments);

    queryArguments = {"stocks", "ORG:reuters", "PERSON:johnson", "-italy", "-argentina", "-deficit", "-doctor", "-foreign", "-options", "-traders"};

    // tests runQuery(), findIntersection(), findComplement(), addDocInfo(), displayQueryResults(), printPair(), printFullText() - must type "yes" or "no" while testing
    // should have 1 results
    processor.runQuery(queryArguments);
}
