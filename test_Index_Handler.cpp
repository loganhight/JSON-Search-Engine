#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "Index_Handler.h"

TEST_CASE("Check that Index_Handler works", "[Index_Handler]")
{
    Index_Handler handler;

    // make sure indices are empty
    CHECK(handler.getDocCount() == 0);
    CHECK(handler.getWordCount() == 0);
    CHECK(handler.getOrganizationCount() == 0);
    CHECK(handler.getPersonCount() == 0);
    
    // "./test_data" is in my build folder - contains 292 documents from total dataset
    // tests parseFolder(), readJSONFile(), handleText(), handleOrganizations(), handlePersons(), updateDocIndex(), updatedWordIndex(), updateOrganizationIndex(), updatePersonIndex()
    handler.parseFolder("./test_data");

    // make sure indices are not empty
    CHECK(handler.getDocCount() != 0);
    CHECK(handler.getWordCount() != 0);
    CHECK(handler.getOrganizationCount() != 0);
    CHECK(handler.getPersonCount() != 0);

    // tests all persistence methods - do files match? Yes!
    handler.makeIndicesPersistent("persistence_testing/testWordFile.txt", "persistence_testing/testOrgFile.txt", "persistence_testing/testPersonFile.txt", "persistence_testing/testDocFile.txt");
    handler.readIndices("persistence_testing/testWordFile.txt", "persistence_testing/testOrgFile.txt", "persistence_testing/testPersonFile.txt", "persistence_testing/testDocFile.txt");
    handler.makeIndicesPersistent("persistence_testing/testWordFile1.txt", "persistence_testing/testOrgFile1.txt", "persistence_testing/testPersonFile1.txt", "persistence_testing/testDocFile1.txt");

    // test clearIndices()
    handler.clearIndices();

    // make sure indices are empty
    CHECK(handler.getDocCount() == 0);
    CHECK(handler.getWordCount() == 0);
    CHECK(handler.getOrganizationCount() == 0);
    CHECK(handler.getPersonCount() == 0);

    // read documents
    handler.parseFolder("./sample_data");

    CHECK(handler.getDocCount() == 6);
    CHECK(handler.getWordCount() != 0);
    CHECK(handler.getOrganizationCount() != 0);
    CHECK(handler.getPersonCount() != 0);

    // test findWordDocs() - both cases
    auto wordTest1 = handler.findWordDocs("german");
    auto wordTest2 = handler.findWordDocs("aaaaaaaaaaaaaaaaa");

    CHECK(wordTest1.is_empty() == 0);
    CHECK(wordTest2.is_empty() == 1);

    // test findOrgDocs() - both cases
    auto orgTest1 = handler.findOrgDocs("reuters");
    auto orgTest2 = handler.findOrgDocs("aaaaaaaaaaaaaaaaa");

    CHECK(orgTest1.is_empty() == 0);
    CHECK(orgTest2.is_empty() == 1);

    // test findPersonDocs() - both cases
    auto personTest1 = handler.findPersonDocs("schweitzer");
    auto personTest2 = handler.findPersonDocs("aaaaaaaaaaaaaaaaa");

    CHECK(personTest1.is_empty() == 0);
    CHECK(personTest2.is_empty() == 1);

    // test findDocInfo() - both cases
    auto docTest1 = handler.findDocInfo("e17252151a2d36344b7104d14805414a55042abe");
    auto docTest2 = handler.findDocInfo("aaaaaaaaaaaaaaaaa");

    CHECK(docTest1.size() != 0);
    CHECK(docTest2.size() == 0);

}
