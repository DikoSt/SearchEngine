//
// Created by Denis on 18.04.2022.
//
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "config.h"
using json = nlohmann::json;
/** создение config.json */
#define CREATE_CONFIG_JSON

/** создение request.json */
#define CREATE_REQUESTS_JSON

/** создение answers.json */
int main(){
    std::cout << "Creator JSON files" << std::endl;

    std::ofstream outputFile;

#ifdef CREATE_CONFIG_JSON

    outputFile.open(defaultConfigFileName);

    nlohmann::json config;
    std::string version = "0.1";
    config["config"] = {
            {"name", "SearchEngine"},
            {"version", version},
            {"max_responses", 5},
            {"method_search", 1}
    };

    nlohmann::json configureFiles = nlohmann::json::array();

    std::string pathToFile;
 //  14280
    for (int i=0;i<=185;++i) {
        pathToFile = "resource\\file" +
                std::string(5-std::to_string(i).size(), '0') + std::to_string(i) + ".txt";
        //pathToFile = "text/file" + std::to_string(i) + ".txt";
        std::cout << pathToFile << std::endl;
        configureFiles.push_back(pathToFile);
    }
    config["files"] = configureFiles;

    std::cout << "config.json created. Saving...." << std::endl;
    outputFile << config.dump(4);
    outputFile.close();
#endif


#ifdef CREATE_REQUESTS_JSON
    /** созданим файл requests.json */
    nlohmann::json requests;

    outputFile.open(defaultRequestsFileName);
    if (outputFile.is_open()) {
        nlohmann::json listRequests = nlohmann::json::array();
listRequests = {" willing  your voice rumbles  this universe never",
                "button for a head",
                "lecture i read you loud and",
                "had saved swore before a notary that it was all mine nothing",
                " courses i teach the chancellor and i unders",
                " about a space suit built for two anyhow lets go to barsoom jacob you did tell me we could go anywhere in zip nothing flat quite true so lets go to barsoom i decided to flank her hilda we cant go to barsoom mors kajak and john carter dont have their swords want to bet deety said sweetly huh sir",
                "once this caused it to fit lines held",
                "stopped",
                "with black hat vermin its new york city you sit",
                "swung around and looked at her husband jake i dont think i have a vote my blood brother answered easily ill automatically be your ",
                "your father yes i know",
                "unless told gay room girls uncle tobias",
                " minutes before husband oscar gordon cliff the gordfather",
                "sdf",
                "sfd",
                "gssggsgasg jh askdfjb sadjkbh klasdbkhlasdb klafhsfasd klhfahk fbhasfd",
                "skdbjs lkjsdb lkjafb lkjasbflkjb asdkljb asfdlkjb asflkbj askljfajk afjsbk fsdbj kl",
                "fbwhfb asdfkb asdflhb f lk fk af asbdfh basdfhkb hsadfkb hb has hl bfasdlhfsdh lf",
                "lkasjdbhlkjsafb lkjf lkjb fasdlkjb asdlkjb asfdlkbjfas asl  lasbl asdlf slfasasf",
                "klwqh lk asflkb asdfklbj asfdbljk afsdbjklafl l asffl asasfl  bl afablasaksa"
};
std::string  req;

//        for (int i = 0; i < 10; ++i) {
///** создадимка набор случайных слов случайно длинны */
//            req = "";
//            for (int j=0; j<rand()%10;j++) {
//                for (int w = 0; w < rand() % 100; w++) {
//                    req += 'a' + rand()%(static_cast<int>('z')- static_cast<int>('a'));
//                }
//                req += " ";
//            }
///** **/
//            //listRequests.push_back("word" + std::to_string(i));
//            listRequests.push_back(req);
//        }
//
        requests["requests"] = listRequests;

        std::cerr << std::endl << "File requests.json created. Saving...." << std::endl;
        std::cout << requests;
        outputFile << requests.dump(4);
        outputFile.close();
    }
    else {
        std::cerr << "ERROR : File not created" << std:: endl;
    }
#endif
/***********************************************************/

    return 0;
}
