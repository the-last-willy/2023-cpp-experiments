#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

int main()
{
    auto input = std::ifstream("/home/willy/Downloads/romeo.txt", std::ios::in);
    if(!input.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
    }

    std::stringstream buffer;
    buffer << input.rdbuf();

    auto text = buffer.str().substr(0, 10000);

    auto last = std::remove(std::begin(text), std::end(text), '\r');
    text.erase(last, text.end());

    auto func = [](char c) -> bool { return std::ispunct(c); };
    std::replace_if(std::begin(text), std::end(text), func, ' ');

    for(auto& ch : text)
        ch = std::tolower(ch);

    std::stringstream sstr;
    sstr << text;

    auto words = std::vector<std::string>();

    std::string word;
    while(sstr >> word)
        words.push_back(word);

    std::cout << text << "..." << std::endl;

    for(auto w : words)
        std::cout << w << ", ";

    auto wordCounts = std::map<std::string, int>();
    for(auto w : words)
        wordCounts[w] += 1;

    auto wordCountList = std::vector<std::tuple<std::string, int>>();

    for(auto [word, count] : wordCounts)
    {
        wordCountList.push_back({word, count});
    }

    std::sort(begin(wordCountList), end(wordCountList),
        [](auto& l, auto& r) { return std::get<1>(l) < std::get<1>(r); });

    for(auto [w, c] : wordCountList)
        std::cout << w << " " << c << std::endl;

    auto output = std::ofstream("/home/willy/Downloads/romeo2.txt");
    output << text;
}
