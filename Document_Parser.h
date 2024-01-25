#ifndef DOCUMENT_PARSER_H
#define DOCUMENT_PARSER_H

#include "AVL_Tree.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "porter2/porter2_stemmer.cpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <iomanip>

using namespace rapidjson;
using namespace std;

class Document_Parser {

private:

    // map of stop words from https://www.webconfs.com/stop-words.php
    AVL_Tree<string, int> stopAVLTreeMap = this->buildStopAVLTreeMap();

public:

    // returns docID of current document
    string getUUID(rapidjson::Document &currDoc) {
        auto UUID = currDoc["uuid"].GetString();
        string docID = UUID;
        return docID;
    }

    // returns text of current document as a vector of strings - removes stop words and stems each word
    vector<string> getText(rapidjson::Document &currDoc) {
        vector<string> textWords;

        // gets full text from current document
        auto words = currDoc["text"].GetString();    

        // conversion from const char * to char * to use strtok()
        char *text = (char*)words;

        int i = 0;

        // makes every character lowercase
        while (text[i] != '\0') {
            text[i] = tolower(text[i]);
            i++;
        }

        string currString;

        // gets first word
        char *currWord = strtok(text, " !@#$%^&*()-_=+`~[{]};:'"",<.>/|?\"");

        // removes non-alphanumeric characters from each word, removes stop words, stems each word
        while (currWord != NULL) {

            // conversion from char * to string
            currString = currWord;

            // removes non-alphanumeric characters from each word
            currString.erase(remove_if(currString.begin(), currString.end(), is_notalnum), currString.end());

            // removes stop words and stems each word
            currString = this->stem(currString);

            // if word is not empty string - adds to return vector
            if (currString != "") { textWords.push_back(currString); }

            // gets next word
            currWord = strtok(NULL, " ''!@#$%^&*()-_=+`~[{]};:'"",<.>/|?\"");
        }

        return textWords;
    }

    // helper function to remove non-alphanumeric characters from text
    static bool is_notalnum(const char &curr) { return isalnum(curr) == 0; }

    string stem(string &word) {
        // returns empty string if word is a stop word
        if (this->is_stop(word)) { return ""; }

        //Porter 2 Stemming Algorithim - trims word
        Porter2Stemmer::trim(word);

        //Porter 2 Stemming Algorithim - stems word
        Porter2Stemmer::stem(word);

        return word;
    }

    // returns organizations of current document as a vector of strings
    vector<string> getOrganizations(rapidjson::Document &currDoc) {
        vector<string> orgs;

        auto organizations = currDoc["entities"]["organizations"].GetArray();
        for (auto &i : organizations) {
            string currOrg = i["name"].GetString();

            // remove persistence characters
            currOrg.erase(std::remove(currOrg.begin(), currOrg.end(), '|'), currOrg.cend());

            orgs.push_back(currOrg); 
        }

        return orgs;
    }

    // returns persons of current document as a vector of strings
    vector<string> getPersons(rapidjson::Document &currDoc) {
        vector<string> persons;

        auto pers = currDoc["entities"]["persons"].GetArray();
        for (auto &i : pers) { 
            string currPerson = i["name"].GetString();

            // remove persistence characters
            currPerson.erase(std::remove(currPerson.begin(), currPerson.end(), '|'), currPerson.cend());
            
            persons.push_back(currPerson);
        }

        return persons;
    }

    // returns title of current document
    string getTitle(rapidjson::Document &currDoc) {
        auto docTitle = currDoc["thread"]["title"].GetString();
        string title = docTitle;

        // remove persistence characters
        title.erase(std::remove(title.begin(), title.end(), '|'), title.cend());

        return title;
    }

    // returns full text of current document
    string getFullText(rapidjson::Document &currDoc) {
        auto text = currDoc["text"].GetString();
        string fullText = text;

        // remove newline characters and specific persistence characters
        fullText.erase(std::remove(fullText.begin(), fullText.end(), '\n'), fullText.cend());
        fullText.erase(std::remove(fullText.begin(), fullText.end(), '^'), fullText.cend());
        fullText.erase(std::remove(fullText.begin(), fullText.end(), '|'), fullText.cend());

        return fullText;
    }

    // returns publication of current document
    string getPublication(rapidjson::Document &currDoc) {
        auto pub = currDoc["thread"]["site"].GetString();
        string publication = pub;

        // remove persistence characters
        publication.erase(std::remove(publication.begin(), publication.end(), '|'), publication.cend());

        return publication;
    }

    // returns publication date & time of current document
    string getDate(rapidjson::Document &currDoc) {
        auto published = currDoc["thread"]["published"].GetString();
        string date = published;

        // remove persistence characters
        date.erase(std::remove(date.begin(), date.end(), '|'), date.cend());

        return date;
    }

    // builds the map - really just a tree - of stop words. stop words from: https://www.webconfs.com/stop-words.php
    AVL_Tree<string, int> buildStopAVLTreeMap() {
        string stopWords = "able about above abroad according accordingly across actually adj after afterwards again against ago ahead ain't all allow allows almost alone along alongside already also although always am amid amidst among amongst an and another any anybody anyhow anyone anything anyway anyways anywhere apart appear appreciate appropriate are aren't around as a's aside ask asking associated at available away awfully back backward backwards be became because become becomes becoming been before beforehand begin behind being believe below beside besides best better between beyond both brief but by came can cannot cant can't caption cause causes certain certainly changes clearly c'mon co co. com come comes concerning consequently consider considering contain containing contains corresponding could couldn't course c's currently dare daren't definitely described despite did didn't different directly do does doesn't doing done don't down downwards during each edu eg eight eighty either else elsewhere end ending enough entirely especially et etc even ever evermore every everybody everyone everything everywhere ex exactly example except fairly far farther few fewer fifth first five followed following follows for forever former formerly forth forward found four from further furthermore get gets getting given gives go goes going gone got gotten greetings had hadn't half happens hardly has hasn't have haven't having he he'd he'll hello help hence her here hereafter hereby herein here'shereupon hers herself he's hi him himself his hither hopefully how howbeit however hundred i'd ie if ignored i'll i'm immediate in inasmuchinc inc. indeed indicate indicated indicates inner inside insofar instead into inward is isn't it it'd it'll its it's itself i've just k keep keeps kept know known knows last lately later latter latterly least less lest let let's like liked likely likewise little look looking looks low lower ltd made mainly make makes many may maybe mayn't me mean meantime meanwhile merely might mightn't mine minus miss more moreover most mostly mr mrs much must mustn't my myself name namely nd near nearly necessary need needn't needs neither never neverf neverless nevertheless new next nine ninety no nobody non none nonetheless noone no-one nor normally not nothing notwithstanding novel now nowhere obviously of off often oh ok okay old on once one ones one's only onto opposite or other others otherwise ought oughtn't our ours ourselves out outside over overall own particular particularly past per perhaps placed please plus possible presumably probably provide provides que quite qv rather rd re really reasonably recent recently regarding regardless regards relatively respectively right round said same saw say saying says second secondly see seeing seem seemed seeming seems seen self selves sensible sent serious seriously seven several shall shan't she she'd she'll she's should shouldn't since six so some somebody someday somehow someone something sometime sometimes somewhat somewhere soon sorry specified specify specifying still sub such sup sure take taken taking tell tends th than thank thanks thanx that that'll thats that's that've the their theirs them themselves then thence there thereafter thereby there'd therefore therein there'll there're theres there's thereupon there've these they they'd they'll they're they've thing things think third thirty this thorough thoroughly those though three through throughout thru thus till to together too took toward towards tried tries truly try trying t's twice two un under underneath undoing unfortunately unless unlike unlikely until unto up upon upwards us use used useful uses using usually v value various versus very via viz vs want wants was wasn't way we we'd welcome well we'll went were we're weren't we've what whatever what'll what's what've when whence whenever where whereafter whereas whereby wherein where's whereupon wherever whether which whichever while whilst whither who who'd whoever whole who'll whom whomever who's whose why will willing wish with within without wonder won't would wouldn't yes yet you you'd you'll your you're yours yourself yourselves you've zero";
        istringstream stopWordsStream(stopWords);

        string currStopWord;

        AVL_Tree<string, int> stopAVLTreeMap;

        while(stopWordsStream >> currStopWord) { stopAVLTreeMap.insert(currStopWord); }

        return stopAVLTreeMap;
    }

    // checks if paramter string is in the map of stop words
    bool is_stop(const string &word) { return(stopAVLTreeMap.contains(word) == 1); }

};

#endif
