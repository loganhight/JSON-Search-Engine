#ifndef INDEX_HANDLER_H
#define INDEX_HANDLER_H

#include "AVL_Tree.h"
#include "Document_Parser.h"

#include <vector>
#include <filesystem>

using namespace std;

class Index_Handler {

private:
    Document_Parser parser;

    // total number of articles in dataset
    int articleCount = 0;

    AVL_Tree<string, AVL_Tree<string, int>> wordIndex;
    AVL_Tree<string, AVL_Tree<string, string>> organizationIndex;
    AVL_Tree<string, AVL_Tree<string, string>> personIndex;
    AVL_Tree<string, vector<string>> docIndex;

public:
    
    // retrieves desired information from parameter fileName - must be a JSON formatted document
    void readJSONFile(const string &fileName) {

        articleCount++;

        // TEMP: display count
        if (articleCount % 25000 == 0) { cout << "Parsing " << articleCount << "th document" << endl; }

        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Error: Cannot open file: " << fileName << endl;
            return;
        }

        // create wrapper - RapidJSON
        IStreamWrapper wrapper(file);
        Document currDoc;
        currDoc.ParseStream(wrapper);

        // get all needed information from document
        string docID = parser.getUUID(currDoc);
        string title = parser.getTitle(currDoc);
        string publication = parser.getPublication(currDoc);
        string date = parser.getDate(currDoc);
        string fullText = parser.getFullText(currDoc);
        vector<string> text = parser.getText(currDoc);
        vector<string> organizations = parser.getOrganizations(currDoc);
        vector<string> persons = parser.getPersons(currDoc);

        file.close();

        // creates vector of required information for document
        vector<string> info = {title, publication, date, fullText}; 

        // update document index
        updateDocIndex(docID, info);

        // handle text vector - updates word index
        handleText(text, docID);

        // handle organizations vector - updates organization index
        handleOrganizations(organizations, docID);

        // handle persons vector - updates person index
        handlePersons(persons, docID);
    }

    // updates word index for each word in vector containing all words from document text
    void handleText(vector<string> &text, const string &docID) {
        for (size_t i = 0; i < text.size(); i++) {
            this->updateWordIndex(text.at(i), docID);
        }
    }

    // updates organization index for each organization in vector containing all organizations from document
    void handleOrganizations(vector<string> &orgs, const string &docID) {
        for (size_t i = 0; i < orgs.size(); i++) {
            this->updateOrganizationIndex(orgs.at(i), docID);
        }
    }

    // updates person index for each organization in vector contanining all persons from document
    void handlePersons(vector<string> &persons, const string &docID) {
        for (size_t i = 0; i < persons.size(); i++) {
            this->updatePersonIndex(persons.at(i), docID);
        }
    }

    // read each document in parameter file path
    void parseFolder(const string &path) {

        // parsing a new folder deletes all current indices
        clearIndices();

        auto it = filesystem::recursive_directory_iterator(path);

        // read each document in file path - must be a JSON document
        for (const auto &file : it) {
            if (file.is_regular_file() && file.path().extension().string() == ".json") {
                this->readJSONFile(file.path().string());
            }
        }
    }

    // updates word index
    void updateWordIndex(const string &word, const string &docID) {

        // if docID is already found for current word - increases count (times word appears in document)
        if (wordIndex[word].find(docID) != nullptr) {
            wordIndex[word].increaseCount(docID);
        }
        // otheriwise - insert and count is 1
        else {
            wordIndex[word][docID] = 1;
        }
    }

    // returns word index
    AVL_Tree<string, AVL_Tree<string, int>> getWordIndex() {
        return wordIndex;
    }

    // updates organization index
    void updateOrganizationIndex(const string &org, const string &docID) {
        organizationIndex[org][docID] = "";
    }

    // updates person index
    void updatePersonIndex(const string &person, const string &docID) {
        personIndex[person][docID] = "";
    }

    // updates document index
    void updateDocIndex(const string &docID, const vector<string> &info) {
        docIndex[docID] = info;
    }

    // returns value tree of parameter word
    AVL_Tree<string, int> findWordDocs(const string &toFind) {

        AVL_Tree<string, int> results;

        // if word is not in index - returns empty tree
        if (wordIndex.find(toFind) == nullptr) { return results; }

        results = wordIndex.at(toFind);
        return results;
    }

    // returns value tree of parameter organization
    AVL_Tree<string, string> findOrgDocs(const string &toFind) {

        AVL_Tree<string, string> results;

        // if organization is not in index - returns empty tree
        if (organizationIndex.find(toFind) == nullptr) { return results; }

        results = organizationIndex.at(toFind);
        return results;
    }

    // returns value tree of parameter person
    AVL_Tree<string, string> findPersonDocs(const string &toFind) {

        AVL_Tree<string, string> results;

        // if person is not in index - returns empty tree
        if (personIndex.find(toFind) == nullptr) { return results; }

        results = personIndex.at(toFind);
        return results;
    }

    // returns value vector of parameter document
    vector<string> findDocInfo(const string &toFind) { 

        vector<string> results;

        // if docID is not in index - returns empty vector
        if (docIndex.find(toFind) == nullptr) { return results; }
        
        results = docIndex.at(toFind);
        return results;
    }

    // used to stem query arguments to match content of word index
    string stem(string &word) {
        return parser.stem(word);
    }

    // write each index to its parameter file
    void makeIndicesPersistent(const string &wordFile, const string &organizationFile, const string &personFile, const string &docFile) {
        wordIndex.writeWordIndexToFile(wordFile);
        organizationIndex.writeIndexToFile(organizationFile);
        personIndex.writeIndexToFile(personFile);
        docIndex.writeDocIndexToFile(docFile);
    }

    // read each index from its parameter file
    void readIndices(const string &wordFile, const string &organizationFile, const string &personFile, const string &docFile) {
        readWordIndex(wordIndex, wordFile);
        readIndex(organizationIndex, organizationFile);
        readIndex(personIndex, personFile);
        readDocIndex(docIndex, docFile);
    }

    // reads word index from parameter file according to structured format - should only be used for word index
    void readWordIndex(AVL_Tree<string, AVL_Tree<string, int>> &currIndex, const string &file) {

        // deletes current word index
        currIndex.clear();

        ifstream fileStream(file);

        if (!fileStream.is_open()) { cerr << "Error! Cannot open file: " << file << endl; }

        string fullLine, mainKey, uuid, currString;
        int docCount;
        AVL_Tree<string, int> currValueTree;
        vector<string> lineStrings;
        int count = 0;

        // reads entire file
        while (!fileStream.eof()) {
            
            // gets current line, clears line vector and current value tree
            getline(fileStream, fullLine);
            lineStrings.clear();
            currValueTree.clear();

            // adds each element of index to line vector - full line is be a single node and its associated value tree
            for (size_t i = 0; i < fullLine.length(); i++) {
                if (fullLine[i] == '/' ||  fullLine[i] == '|' || fullLine[i] == '}' ) {
                    lineStrings.push_back(currString);
                    currString = "";
                } 
                else {
                    currString += fullLine[i];
                }
            }

            count = 0;

            // not a full entry
            if (lineStrings.size() < 3) { continue; }

            for (size_t i = 0; i < lineStrings.size(); i++) {
                if (count == 0) {

                    // first element is word
                    mainKey = lineStrings.at(i);

                    // next element will be docID
                    count++;
                }
                else if (count == 1) {

                    // current element of vector is docID
                    uuid = lineStrings.at(i);

                    // adds docID to value tree
                    currValueTree[uuid];

                    // next element of vector will be number of times word appears in document
                    count = 2;
                }
                else if (count == 2) {
                    
                    // current element of vector is number of times word appears in document, but a string 
                    string strDocCount = lineStrings[i];

                    // convert to integer
                    docCount = stoi(strDocCount);

                    // maps number of times word appears in document to document
                    currValueTree[uuid] = docCount;
                    
                    // next element of vector will be docID
                    count = 1;
                }  
            }

            // adds word and its value tree to index
            currIndex[mainKey] = currValueTree;
        }
    }

    // reads index from parameter file according to structured format - should only be used for organization index and person index
    void readIndex(AVL_Tree<string, AVL_Tree<string, string>> &currIndex, const string &file) {

        // deletes current index
        currIndex.clear();

        ifstream fileStream(file);

        if (!fileStream.is_open()) { cerr << "Error! Cannot open file: " << file << endl; }

        string fullLine, mainKey, uuid, currString;
        AVL_Tree<string, string> currValueTree;
        vector<string> lineStrings;
        int count = 0;

        // reads entire file
        while (!fileStream.eof()) {

            // gets current line, clears line vector and current value tree
            getline(fileStream, fullLine);
            lineStrings.clear();
            currValueTree.clear();

            // adds each element of index to line vector - full line is a single node and its associated value tree
            for (size_t i = 0; i < fullLine.length(); i++) {
                if (fullLine[i] == '|' ) {
                    lineStrings.push_back(currString);
                    currString = "";
                } 
                else {
                    currString += fullLine[i];
                }
            }

            count = 0;

            // not a full entry
            if (lineStrings.size() < 2) { continue; }

            for (size_t i = 0; i < lineStrings.size(); i++) {
                if (count == 0) {

                    // first element is organization/person
                    mainKey = lineStrings.at(i);

                    // next element will be docID
                    count++;
                }
                else if (count == 1) {

                    // current element is docID
                    uuid = lineStrings.at(i);

                    // adds docID to value tree
                    currValueTree[uuid];

                    // next element will also be docID
                    count = 1;
                }  
            }

            // adds organization/person and their value tree to index
            currIndex[mainKey] = currValueTree;
        }
    }

    // reads document index from parameter file according to structured format - should only be used for document index
    void readDocIndex(AVL_Tree<string, vector<string>> &documentIndex, const string &file) {

        // deletes current doc index
        documentIndex.clear();

        // reset articleCount
        articleCount = 0;

        ifstream fileStream(file);

        if (!fileStream.is_open()) { cerr << "Error! Cannot open file: " << file << endl; }

        string fullLine, uuid, title, publication, date, fullText, currString;
        vector<string> currVect, lineStrings;

        // reads entre file
        while (!fileStream.eof()) {

            // gets current line, clears line vector
            getline(fileStream, fullLine);
            lineStrings.clear();

            // adds each element of index to line vector - full line will be a single node and its associated value vector
            for (size_t i = 0; i < fullLine.length(); i++) {
                if (fullLine[i] == '|' || fullLine[i] == '^' ) {
                    lineStrings.push_back(currString);
                    currString = "";
                } 
                else {
                    currString += fullLine[i];
                }
            }

            // not a full entry
            if (lineStrings.size() < 5) { continue; }

            // first element is docID
            uuid = lineStrings.at(0);

            // next element is title
            title = lineStrings.at(1);

            // next element is publication
            publication = lineStrings.at(2);

            // next element is date & time of publication
            date = lineStrings.at(3);

            // final element is full text of document
            fullText = lineStrings.at(4);

            // creates value vector
            currVect = {title, publication, date, fullText};

            // adds docID and its value vector to index
            documentIndex[uuid] = currVect;

            //updates articleCount
            articleCount++;
        }
    }

    // returns total number of articles in dataset
    int getDocCount() {
        return docIndex.size();
    }

    // returns total number of words in the index
    int getWordCount() {
        return wordIndex.size();
    }

    // returns total number of organizations in the index
    int getOrganizationCount() {
        return organizationIndex.size();
    }

    // returns total number of persons in the index
    int getPersonCount() {
        return personIndex.size();
    }

    // clears all indices
    void clearIndices() {
        docIndex.clear();
        wordIndex.clear();
        organizationIndex.clear();
        personIndex.clear();

        // reset articleCount
        articleCount = 0;
    }  
};

#endif
