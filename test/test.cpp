#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>

const char*  TEST_FILENAME = "test.txt";

struct Test
{
    int test_number;
    std::string input;
    std::string output;
    std::string ref_output;
    bool status;
};

std::vector<Test> read_test (const char* filename)
{
    std::ofstream input_file;
    input_file
}

int main()
{
    std::vector<Test> tests = read_test (TEST_FILENAME);
    for (int i = 0; i < tests.size(); i++)
    {

    }
}
