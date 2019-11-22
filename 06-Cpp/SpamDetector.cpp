#include <iostream>
#include <fstream>
#include "HashMap.hpp"
#include <string>
#include <algorithm>

#define NUM_OF_ARGS 4
#define DATABASE_INDEX 1
#define MSG_INDEX 2
#define THRESHOLD_INDEX 3
#define DELIM ','

/**
 * parsing db stream content to map
 * @param db
 * @param map
 * @return if process was successful
 */
int parseDb(std::fstream& db, HashMap<std::string, int>& map)
{
    std::string line;
    while (std::getline(db, line))
    {
        //strip \r
        if (line[line.size() - 1] == '\r')
        {
            line.erase((line.size() - 1));
        }

        //validate num of columns
        size_t cur, prev = 0;
        cur = line.find(DELIM);
        if (cur == std::string::npos) //delim do not exist
        {
            return EXIT_FAILURE;
        }

        //parse bad str
        std::string badStr = line.substr(0, cur);
        if (badStr.empty())
        {
            return EXIT_FAILURE;
        }

        //parse points
        std::string pointsStr = line.substr(cur + 1, line.size() - 1);
        if (pointsStr.empty())
        {
            return EXIT_FAILURE;
        }

        int points;
        try
        {
            points = std::stoi(pointsStr);
            if (points < 0)
            {
                return EXIT_FAILURE;
            }
        }
        catch (std::invalid_argument& e)
        {
            return EXIT_FAILURE;
        }
        catch (std::out_of_range& e)
        {
            return EXIT_FAILURE;
        }

        //validate no more delims
        prev = cur + 1;
        cur = line.find(DELIM, prev);
        if (cur != std::string::npos)
        {
            return EXIT_FAILURE;
        }

        //add pair to map
        std::transform(badStr.begin(), badStr.end(), badStr.begin(), [](unsigned char c){ return std::tolower(c); });
        map.insert(badStr, points);
    }
    return EXIT_SUCCESS;
}

/**
 * checks if givem msg is spam, based on dictionary and threshold
 */
bool isSpam(std::fstream& msg_stream, int threshold, const HashMap<std::string, int>& map)
{
    int score = 0;
    std::string msg;
    msg.assign(std::istreambuf_iterator<char>(msg_stream), (std::istreambuf_iterator<char>()));
    std::transform(msg.begin(), msg.end(), msg.begin(), [](unsigned char c){ return std::tolower(c); });

    for (const auto & it : map)
    {
        if (msg.find(it.first, 0) != std::string::npos)
        {
            score += it.second;
        }
        if (score >= threshold)
        {
            return true;
        }
    }
    return false;
}


/**
 * gets 2 files as args- "database" of bad words (CSV pormat), and file with required
 * msg (plain txt). 3rd arg is the threshold for a spam msg (possitive num). the program analyzes the msg and
 * determine if it is spam.
 *
 * @param argc
 * @param argv
 * @return if msg is spam or not
 */
int main(int argc, char* argv[])
{
    //validate num of args
    if (argc != NUM_OF_ARGS)
    {
        std::cerr << "Usage: SpamDetector <database path> <message path> <threshold>\n";
        return EXIT_FAILURE;
    }

    //process database file
    std::fstream db_stream(argv[DATABASE_INDEX]);

    //validate files exist
    if (!db_stream.good())
    {
        std::cerr << "Invalid input\n";
        db_stream.close();
        return EXIT_FAILURE;
    }

    //process db_stream file
    HashMap<std::string, int> badWords;
    if (db_stream.peek() != db_stream.eof())
    {
        if (parseDb(db_stream, badWords) == EXIT_FAILURE)
        {
            std::cerr << "Invalid input\n";
            db_stream.close();
            return EXIT_FAILURE;
        }
    }
    db_stream.close();

    std::fstream msg_stream(argv[MSG_INDEX]);
    std::string threshold_str = argv[THRESHOLD_INDEX];

    //validate files exist
    if (!msg_stream.good())
    {
        std::cerr << "Invalid input\n";
        msg_stream.close();
        return EXIT_FAILURE;
    }

    //process msg_stream file
    if (msg_stream.peek() == msg_stream.eof())
    {
        msg_stream.close();
        return EXIT_SUCCESS;
    }

    //validate threshold_str
    int threshold;
    try
    {
        threshold = std::stoi(threshold_str);
        if (threshold <= 0)
        {
            std::cerr << "Invalid input\n";
            return EXIT_FAILURE;
        }
    }
    catch (std::invalid_argument& e)
    {
        std::cerr << "Invalid input\n";
        msg_stream.close();
        return EXIT_FAILURE;
    }
    catch (std::out_of_range& e)
    {
        std::cerr << "Invalid input\n";
        msg_stream.close();
        return EXIT_FAILURE;
    }

    //check if msg is spam
    if (isSpam(msg_stream, threshold, badWords))
    {
        msg_stream.close();
        std::cout << "SPAM\n";
    }
    else
    {
        msg_stream.close();
        std::cout << "NOT_SPAM\n";
    }
}
