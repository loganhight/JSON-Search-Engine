#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include "Index_Handler.h"

using namespace std;

class Query_Processor {

private:
    Index_Handler handler;
    std::chrono::microseconds indexTime;
    std::chrono::microseconds saveIndexTime;
    std::chrono::microseconds loadIndexTime;
    std::chrono::microseconds queryTime;
    std::chrono::microseconds fastestQueryTime;
    int totalDocs, totalWords, totalOrgs, totalPersons;
    bool is_index = false;
    bool is_query = false;

public:

    // begins search engine functionality for initial command line arguments
    void runQueryProcessor(int argc, char** argv) {

        if ((string)argv[0] == "time") {

            // ensures that initial command line arguments are properly formatted
            if (argc == 3 && (string)argv[2] != "ui") { 
                cerr << "Incorrect command line arguments entered" << endl;
                cerr << "Please exit and re-enter any of the following initial command line arguments:" << endl;
                cerr << "./supersearch index 'directory'" << endl;
                cerr << "./supersearch query 'query contents'" << endl;
                cerr << "./supersearch ui" << endl; 
            }

            // runs appropriate functionality based initial command line arguments
            else {
                if ((string)argv[2] == "index") { createInitialIndex(argv[3]); }
                else if ((string)argv[2] == "ui") { runUI(); }
                else if ((string)argv[2] == "query") {
                    vector<string> queryArguments;
                    for (int i = 3 ; i < argc; i++) { queryArguments.push_back((string)argv[i]); }

                    //TODO - move to runQuery()
                    for (size_t i = 0; i < queryArguments.size(); i++) {
                        string curr = queryArguments.at(i);
                        transform(curr.begin(), curr.end(), curr.begin(), ::tolower);
                    }


                    // load existing index from default files
                    loadIndex("../WordIndex.txt", "../OrganizationIndex.txt", "../PersonIndex.txt", "../DocumentIndex.txt");

                    runQuery(queryArguments);
                }
            }

            return;
        }

        // ensures that initial command line arguments are properly formatted
        if (argc == 2 && (string)argv[1] != "ui") { 
            cerr << "Incorrect command line arguments entered" << endl;
            cerr << "Please exit and re-enter any of the following initial command line arguments:" << endl;
            cerr << "./supersearch index 'directory'" << endl;
            cerr << "./supersearch query 'query contents'" << endl;
            cerr << "./supersearch ui" << endl; 
        }

        // runs appropriate functionality based initial command line arguments
        else {
            if ((string)argv[1] == "index") { createInitialIndex(argv[2]); }
            else if ((string)argv[1] == "ui") { runUI(); }
            else if ((string)argv[1] == "query") {
                vector<string> queryArguments;
                for (int i = 2 ; i < argc; i++) { queryArguments.push_back((string)argv[i]); }

                // load existing index from default files
                loadIndex("../WordIndex.txt", "../OrganizationIndex.txt", "../PersonIndex.txt", "../DocumentIndex.txt");

                runQuery(queryArguments);
            }
        }

    }

    // creates index from parameter file path - only from initial command line arguments
    void createInitialIndex(char *argv) {
        string path = (string)argv;

        cout << "Creating index..." << endl;

        // start index timer
        auto start = std::chrono::high_resolution_clock::now();

        handler.parseFolder(path);

        // stop index timer
        auto stop = std::chrono::high_resolution_clock::now();
        indexTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        // display index time
        cout << "It took " << indexTime.count() << " microseconds to create the index." << endl << endl; 

        // update data members
        is_index = true;
        totalDocs = handler.getDocCount();
        totalWords = handler.getWordCount();
        totalOrgs = handler.getOrganizationCount();
        totalPersons = handler.getPersonCount();

        // save index to default files
        saveIndex("../WordIndex.txt", "../OrganizationIndex.txt", "../PersonIndex.txt", "../DocumentIndex.txt");
    }

    // displays user interface and continues running search engine functionality until user enters "exit" command
    void runUI() {

        // displays initial menu
        displayUI();
        string command;

        getline(cin, command);
        cout << endl;

        // executes appropriate functionality for all commands until user enters "exit" command
        while (command != "exit") {

            if (command == "index") { createIndex(); }
            else if (command == "save") { saveIndex(); }
            else if (command == "load") { loadIndex(); }
            else if (command == "query") { runSubsequentQuery(); }
            else if (command == "analytics") { displayAnalytics(); }

            // not valid command
            else { cerr << "Error - invalid command entered. Please enter a valid command (index, save, load, query, analytics, or exit) and try again." << endl; }
            cout << "Please input your next desired function (index, save, load, query, analytics, or exit):" << endl;
            getline(cin, command);
            cout << endl;
        }
        return;
    }

    // initial menu
    void displayUI() {
        cout << "Welcome to DS Search Engine!" << endl;
        cout << "Please select an option from the following menu:" << endl;
        cout << "Enter 'index' to create a new index" << endl;
        cout << "Enter 'save' to save your current index" << endl;
        cout << "Enter 'load' to load an index" << endl;
        cout << "Enter 'query' to perform a query" << endl;
        cout << "Enter 'analytics' to perform a query" << endl;
        cout << "Enter 'exit' to exit the search engine" << endl;
    }

    // creates index from file path given by user - used for creating all indices that do not originate from the initial command line arguments
    void createIndex() {

        string path;
        cout << "Please enter the file path for the corpus of the index you would like to create: " << endl;
        getline(cin, path);
        cout << endl;

        cout << "Creating index..." << endl;

        // start index timer
        auto start = std::chrono::high_resolution_clock::now();

        handler.parseFolder(path);

        // stop index timer
        auto stop = std::chrono::high_resolution_clock::now();
        indexTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        // display index time
        cout << "It took " << indexTime.count() << " microseconds to create the index." << endl << endl; 

        // update data members
        is_index = true;
        totalDocs = handler.getDocCount();
        totalWords = handler.getWordCount();
        totalOrgs = handler.getOrganizationCount();
        totalPersons = handler.getPersonCount();

    }

    // writes all indices to each file given by user - files for each index must be unique. precision is necessary
    void saveIndex() {

        // ensures that an index has been created
        if (!is_index) { 
            cout << "Error: Unable to save the index until an index has been created. Please create an index." << endl;
            return;
        }

        string choice, wordIndexFileName, organizationIndexFileName, personIndexFileName, docIndexFileName;

        cout << "You must enter distinct file names for the word index, organization index, person index, and document index to be saved to." << endl;
        cout << "Recommended file names are '../WordIndex.txt', '../OrganizationIndex.txt', '../PersonIndex.txt', '../DocumentIndex.txt'" << endl;

        cout << "If you would like to use the recommended file names, enter 'yes'. If not, enter anything else (no):" << endl;
        getline(cin, choice);
        cout << endl;

        if (choice == "yes") {
            saveIndex("../WordIndex.txt", "../OrganizationIndex.txt", "../PersonIndex.txt", "../DocumentIndex.txt");
            return;
        }
    
        cout << "You have elected to manually enter the file names." << endl;
        cout << "Please be precise!!" << endl << endl;

        cout << "Enter the file name to save the word index to:" << endl;
        getline(cin, wordIndexFileName);
        cout << endl;

        cout << "Enter the file name to save the organization index to:" << endl;
        getline(cin, organizationIndexFileName);
        cout << endl;

        cout << "Enter the file name to save the person index to:" << endl;
        getline(cin, personIndexFileName);
        cout << endl;

        cout << "Enter the file name to save the document index to:" << endl;
        getline(cin, docIndexFileName);
        cout << endl;
    
        cout << "Saving indices..." << endl;

        // start save index timer
        auto start = std::chrono::high_resolution_clock::now();

        handler.makeIndicesPersistent(wordIndexFileName, organizationIndexFileName, personIndexFileName, docIndexFileName);

        // stop save index timer
        auto stop = std::chrono::high_resolution_clock::now();
        saveIndexTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        // display index time
        cout << "It took " << saveIndexTime.count() << " microseconds to save the index." << endl << endl; 

        is_index = true;
    }

    // writes all indices to each parameter file. Saves indices to default files for "./supersearch index" command line argument
    void saveIndex(const string &wordIndexFileName, const string &organizationIndexFileName, const string &personIndexFileName, const string &docIndexFileName) {

        // ensures that an index has been created
        if (!is_index) { 
            cout << "Error: Unable to save the index until an index has been created. Please create an index." << endl;
            return;
        }
    
        cout << "Saving indices..." << endl;

        // start save index timer
        auto start = std::chrono::high_resolution_clock::now();

        handler.makeIndicesPersistent(wordIndexFileName, organizationIndexFileName, personIndexFileName, docIndexFileName);

        // stop save index timer
        auto stop = std::chrono::high_resolution_clock::now();
        saveIndexTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        // display index time
        cout << "It took " << saveIndexTime.count() << " microseconds to save the index." << endl << endl; 

        is_index = true;
    }

    // retrieves all indices from each file given by user - files for each index must be unique. precision is necessary
    void loadIndex() {
        string choice, wordIndexFileName, organizationIndexFileName, personIndexFileName, docIndexFileName;

        cout << "You must enter distinct file names for the word index, organization index, person index, and document index to be loaded from." << endl;
        cout << "Recommended file names are '../WordIndex.txt', '../OrganizationIndex.txt', '../PersonIndex.txt', '../DocumentIndex.txt'" << endl;

        cout << "If you would like to use the recommended file names, enter 'yes'. If not, enter anything else (no):" << endl;
        getline(cin, choice);
        cout << endl;

        if (choice == "yes") {
            loadIndex("../WordIndex.txt", "../OrganizationIndex.txt", "../PersonIndex.txt", "../DocumentIndex.txt");
            return;
        }

        cout << "You have elected to manually enter the file names." << endl;
        cout << "Please be precise!!" << endl << endl;

        cout << "Enter the file name to load the word index from:" << endl;
        getline(cin, wordIndexFileName);
        cout << endl;

        cout << "Enter the file name to load the organization index from:" << endl;
        getline(cin, organizationIndexFileName);
        cout << endl;

        cout << "Enter the file name to load the person index from:" << endl;
        getline(cin, personIndexFileName);
        cout << endl;

        cout << "Enter the file name to load the document index from:" << endl;
        getline(cin, docIndexFileName);
        cout << endl;

        // start index load time
        auto start = std::chrono::high_resolution_clock::now();

        cout << "Loading indices..." << endl;

        handler.readIndices(wordIndexFileName, organizationIndexFileName, personIndexFileName, docIndexFileName);

        // stop index load time
        auto stop = std::chrono::high_resolution_clock::now();
        loadIndexTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        // display index load time
        cout << "It took " << loadIndexTime.count() << " microseconds to load the indices." << endl << endl;

        // update data members
        is_index = true;
        indexTime = loadIndexTime;
        totalDocs = handler.getDocCount();
        totalWords = handler.getWordCount();
        totalOrgs = handler.getOrganizationCount();
        totalPersons = handler.getPersonCount();

    }

    // retrieves all indices from each parameter file. Creates indices from default files for "./supersearch query" command line argument
    void loadIndex(const string &wordIndexFileName, const string &organizationIndexFileName, const string &personIndexFileName, const string &docIndexFileName) {

        // start index load time
        auto start = std::chrono::high_resolution_clock::now();

        cout << "Loading indices..." << endl;

        handler.readIndices(wordIndexFileName, organizationIndexFileName, personIndexFileName, docIndexFileName);

        // stop index load time
        auto stop = std::chrono::high_resolution_clock::now();
        loadIndexTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        // display index load time
        cout << "It took " << loadIndexTime.count() << " microseconds to load the indices." << endl << endl;

        // update data members
        is_index = true;
        totalDocs = handler.getDocCount();
        totalWords = handler.getWordCount();
        totalOrgs = handler.getOrganizationCount();
        totalPersons = handler.getPersonCount();
    }

    // used to execute all user queries
    void runQuery(const vector<string> &queryArguments) {

        // ensures that an index has been created
        if (!is_index) { 
            cout << "Error: Unable to execute a query until an index has been created. Please create an index." << endl;
            return;
        }

        string person, org, notWord, word;
        vector<string> personDocs, orgDocs, notWordDocs, wordDocs;
        vector<string> allDocs;
        vector<string> allNotDocs;

        // start query timer
        auto start = std::chrono::high_resolution_clock::now();

        // iterates through all query arguments
        for (size_t i = 0; i < queryArguments.size(); i++) {

            // person
            if (queryArguments.at(i).size() > 7 && queryArguments.at(i).substr(0,7) == "PERSON:") {

                // gets just person
                person = queryArguments.at(i).substr(7, queryArguments.at(i).size() - 7);

                // make person lowercase
                transform(person.begin(), person.end(), person.begin(), ::tolower);

                // retrieves all documents associated with specified person
                handler.findPersonDocs(person).treeToVect(personDocs);

                // if first query argument - allDocs are all documents associated with specified person
                if (i == 0) { allDocs = personDocs; }

                // if not first query argument, allDocs is now the intersection between current allDocs and documents associated with specified person
                else { allDocs = findIntersection(allDocs, personDocs); }
            }

            // organization
            else if (queryArguments.at(i).size() > 4 && queryArguments.at(i).substr(0,4) == "ORG:") {

                // gets just organization
                org = queryArguments.at(i).substr(4, queryArguments.at(i).size() - 4);

                // make org lowercase
                transform(org.begin(), org.end(), org.begin(), ::tolower);

                // retrieves all documents associated with specified organization
                handler.findOrgDocs(org).treeToVect(orgDocs);

                // if first query argument - allDocs are all documents associated with specified organization
                if (i == 0) { allDocs = orgDocs; }

                // if not first query argument, allDocs is now the intersection between current allDocs and documents associated with specified organization
                else { allDocs = findIntersection(allDocs, orgDocs); }
            }

            // not word
            else if (queryArguments.at(i)[0] == '-') {

                // get just not word
                notWord = queryArguments.at(i).substr(1, queryArguments.at(i).size() - 1);

                // make notWord lowercase
                transform(notWord.begin(), notWord.end(), notWord.begin(), ::tolower);

                // stem not word to match words in wordIndex
                notWord = handler.stem(notWord);

                // retrieves all documents associated with specified not word
                handler.findWordDocs(notWord).treeToVect(notWordDocs);
                
                // allDocs is now the complement of current allDocs and documents associated with specified not word
                allDocs = findComplement(allDocs, notWordDocs);
            }

            // word
            else {
                word = queryArguments.at(i);

                // make word lowercase
                transform(word.begin(), word.end(), word.begin(), ::tolower);

                // stem word to match words in wordIndex
                word = handler.stem(word);

                // retrieves all documents associated with specified word
                handler.findWordDocs(word).treeToVect(wordDocs);

                // if first query argument - allDocs are all documents associated with specified word
                if (i == 0) { allDocs = wordDocs; }
                
                // if not first query argument, allDocs is now the intersection between current allDocs and documents associated with specified word
                else { allDocs = findIntersection(allDocs, wordDocs); }
            }
        }

        // stop query timer
        auto stop = std::chrono::high_resolution_clock::now();
        queryTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        // display current query time
        cout << "It took " << queryTime.count() << " microseconds to execute your query." << endl;

        // if query time is less than fastest query time, fastestQueryTime = queryTime
        if (queryTime.count() < fastestQueryTime.count()) { 
            fastestQueryTime = queryTime; 
            cout << "New fastest query time!" << endl;
        }
        cout << endl;

        // adds required information for each document in the query results
        vector<pair<string, vector<string>>> displayDocs = addDocInfo(allDocs);

        // display query results in order of relevancy
        displayQueryResults(displayDocs);

        is_query = true;
    }

    // gets query input as a vector of strings from user
    vector<string> processQueryInput() {

        vector<string> queryArguments;

        cout << "Please enter your query: " << endl;
    
        string allArgs;
        string currArg;
        getline(cin, allArgs);
        istringstream argStream(allArgs);

        // adds each string to return vector
        while (!argStream.eof()) {
            argStream >> currArg;
            queryArguments.push_back(currArg); 
        }

        return queryArguments;
    }

    // runs additonal queries (all queries that do not originate from the initial command line arguments)
    void runSubsequentQuery() {

        // ensures that an index has been created
        if (!is_index) { 
            cout << "Error: Unable to execute a query until an index has been created. Please create an index." << endl;
            return;
        }

        vector<string> queryArguments = processQueryInput();
        runQuery(queryArguments);
    }

    // finds the intersection of the two parameter vectors - elements of allDocs tha are also in checkDocs will be returned
    vector<string> findIntersection(const vector<string> &allDocs, const vector<string> &checkDocs) {
        vector<string> returnDocs;

        for (size_t i = 0; i < allDocs.size(); i++) {
            string currDocID = allDocs.at(i);

            // if match - added to return vector
            if (std::find(checkDocs.begin(), checkDocs.end(), currDocID) != checkDocs.end()) {
                returnDocs.push_back(currDocID);
            }
        }

        return returnDocs;
    }

    // finds the complement of the two paramter vectors - elements of allDocs that are not in notWordDocs will be returned
    vector<string> findComplement(const vector<string> &allDocs, const vector<string> &notWordDocs) {
        vector<string> returnDocs;

        for (size_t i = 0; i < allDocs.size(); i++) {
            string currDocID = allDocs.at(i);

            // if no match - added to return vector
            if (std::find(notWordDocs.begin(), notWordDocs.end(), currDocID) == notWordDocs.end()) {
                returnDocs.push_back(currDocID);
            }
        }
    
        return returnDocs;
    }

    // adds required information for each document in the query results
    vector<pair<string, vector<string>>> addDocInfo(const vector<string> &allDocs) {
        vector<pair<string, vector<string>>> resultDocs;

        for (size_t i = 0; i < allDocs.size(); i++) {
            pair<string, vector<string>> currPair;
            currPair.first = allDocs.at(i);
            currPair.second = handler.findDocInfo(currPair.first);
            resultDocs.push_back(currPair);
        }

        return resultDocs;
    }

    // displays results of user's query
    void displayQueryResults(vector<pair<string, vector<string>>> &allDocs) {
        cout << "Your query returned " << allDocs.size() << " results." << endl;

        // return if no results
        if (allDocs.size() == 0) { return; }

        // this code block handles errors that can occur when documents are not properly formatted - mainly occurs when using command line query instead of ui query - not sure why
        // still outputs correct results - just not as much info as with ui query
        // UI query is recommend useage
        bool formattingError = false;
        for (size_t x = 0; x < allDocs.size(); x++) { 
            if (allDocs.at(x).second.size() != 4) {
                cout << "Please use our user interface for more detailed results!" << endl << endl;
                formattingError = true;
                break;
            }
        }

        cout << "Query Results:" << endl;

        if (formattingError) {
            if (allDocs.size() > 15) {
                for (size_t i = 0; i < 15; i++) {
                    cout << i + 1 << ". ";
                    cout << "UUID: " << allDocs.at(i).first << endl;
                }

                cout << endl;
                return;
            }

            // prints all results - in order of relevancy if query returned 15 results or less
            for (size_t i = 0; i < allDocs.size(); i++) {
                cout << i + 1 << ". ";
                cout << allDocs.at(i).first << endl;
            }
            cout << endl;
            return;
        }

        // normal printing if no formatting error is detected

        // swaps docID with date in order to sort results by most recent date
        for (size_t i = 0; i < allDocs.size(); i++) {
            swap(allDocs.at(i).first, allDocs.at(i).second.at(2));
        }

        // sorts results by most recent data
        sort(allDocs.rbegin(), allDocs.rend());

        // swaps back so that structured printing is formatted correctly
        for (size_t i = 0; i < allDocs.size(); i++) {
            swap(allDocs.at(i).first, allDocs.at(i).second.at(2));
        }

        // prints top 15 results in order of relevancy if query returned more than 15 results
        if (allDocs.size() > 15) {
            for (size_t i = 0; i < 15; i++) {
                cout << i + 1 << ". ";
                printPair(allDocs.at(i));
            }

            cout << endl;
            printFullText(allDocs);
            return;
        }

        // prints all results - in order of relevancy if query returned 15 results or less
        for (size_t i = 0; i < allDocs.size(); i++) {
            cout << i + 1 << ". ";
            printPair(allDocs.at(i));
        }
        cout << endl;

        printFullText(allDocs);
    }

    // prints paramater std::pair in structured format - used for displaying query results
    void printPair(const pair<string, vector<string>> &currPair) {
        cout << "UUID: " << currPair.first << " -- Title: " << currPair.second.at(0) << " -- Publication: " << currPair.second.at(1) << " -- Date: " << currPair.second.at(2) << endl;
    }

    // gives user the option to print the full text from one of the results of their query
    void printFullText(vector<pair<string,vector<string>>> allDocs) {
        string command = "";
        string strChoice;
        size_t choice;

        cout << "If you would like to print the full text from one of your query results, type 'yes'. If not, type 'no'." << endl;
        getline(cin, command);
        cout << endl;

        while (command != "no") {

            if (command == "yes") {
                cout << "Which number result would you like to print?" << endl;
                getline(cin, strChoice);
                choice = stoi(strChoice);
                cout << endl;
                if (choice > 0 && choice <= allDocs.size()) {
                    cout << "Printing full text for document titled: " << allDocs.at(choice - 1).second.at(0) << endl << endl;
                    cout << allDocs.at(choice - 1).second.at(3) << endl;
                }
                else {
                    cout << "Error! Invalid number selected - no text will be printed" << endl;
                }

                cout << endl;
                return;
            }
            else {
                cout << "Invalid option! Please type 'yes' if you would like to print the full text of one of your query results or 'no' if not." << endl;
                getline(cin, command);
                cout << endl;
            }
        }
    }

    // display search engine analytics - bonus
    void displayAnalytics() {

        // ensures that an index has been created
        if (!is_index) { 
            cerr << "Error: Unable to display analytics until an index has been created. Please create an index." << endl;
            return;
        }

        // ensures that a query has been executed
        if (!is_query) {
            cerr << "Error: Unable to display analytics until a query has been executed. Please executed a query." << endl;
            return;
        }

        // current index time/speed
        cout << "Index Timing Analytics:" << endl;
        cout << "It took " << indexTime.count() << " microseconds to create the current index." << endl;
        cout << "The average speed for creating the current index was " << 1000000 *((double)totalDocs / indexTime.count()) << " documents per second." << endl << endl;

        // index sizes
        cout << "Index Size Analytics:" << endl;
        cout << "There are " << totalDocs << " documents in the current index." << endl;
        cout << "There are " << totalWords << " words in the current index." << endl;
        cout << "There are " << totalOrgs << " organizations in the current index." << endl;
        cout << "There are " << totalPersons << " persons in the current index." << endl << endl;

        // query analytics
        cout << "Query Analytics:" << endl;
        cout << "It took " << queryTime.count() << " microseconds to perform the latest query." << endl;
        cout << "It took " << fastestQueryTime.count() << " microseconds to perform the fastest query." << endl << endl; 

    }

};

#endif
