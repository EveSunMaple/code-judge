#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

// ANSI color codes
#define GREEN_TEXT "\033[1;32m"
#define RED_TEXT "\033[1;31m"
#define BLUE_TEXT "\033[1;34m"
#define PURPLE_TEXT "\033[1;35m"
#define ORANGE_TEXT "\033[1;33m"
#define RESET_TEXT "\033[0m"

using namespace std;
using namespace chrono;

string trim(const string& str)
{
    size_t endpos = str.find_last_not_of(" \t\n\r");
    return (endpos != string::npos) ? str.substr(0, endpos + 1) : "";
}

// 寻找两个字符串第一次不匹配的位置
size_t findFirstDiff(const string& str1, const string& str2)
{
    size_t minLen = min(str1.length(), str2.length());
    for (size_t i = 0; i < minLen; ++i)
    {
        if (str1[i] != str2[i])
        {
            return i;
        }
    }
    return minLen;
}

void saveInputToFile(const string& input, const string& filename)
{
    ofstream outputFile(filename);
    if (outputFile.is_open())
    {
        outputFile << input;
        outputFile.close();
        cout << PURPLE_TEXT << "Saved input data to: " << filename << RESET_TEXT << " ";
    }
    else
    {
        cerr << PURPLE_TEXT << "Error: Unable to open file for saving input data." << RESET_TEXT << endl;
    }
}

int main(int argc, char* argv[])
{
    srand(time(0));

    string rand_executable, std_executable, user_executable;

    // 获取 rand 可执行文件路径
    cout << "Drag and drop the 'rand' executable file, then press Enter:" << endl;
    getline(cin, rand_executable);

    // 获取 std 可执行文件路径
    cout << "Drag and drop the 'std' executable file, then press Enter:" << endl;
    getline(cin, std_executable);

    // 获取 user 可执行文件路径
    cout << "Drag and drop the 'user' executable file, then press Enter:" << endl;
    getline(cin, user_executable);

    const int numTests = 10;
    float totalPoints = 0;

    // 编译运行 rand.cpp
    auto compile_rand_start = high_resolution_clock::now();
    int compile_rand_result = system(("g++ -o rand " + rand_executable).c_str());
    auto compile_rand_stop = high_resolution_clock::now();
    auto compile_rand_duration = duration_cast<milliseconds>(compile_rand_stop - compile_rand_start);

    if (compile_rand_result != 0)
    {
        cout << RED_TEXT << "ERROR! " << RESET_TEXT << " rand code " << ORANGE_TEXT
            << "CE (Compile Error)" << RESET_TEXT << "! \n";
        system("pause");
        return 0;
    }
    else
    {
        cout << "Successfully compiled rand code " << GREEN_TEXT << "✅" << RESET_TEXT
            << " took " << compile_rand_duration.count() << "ms \n";
    }

    // 编译运行 std.cpp
    auto compile_std_start = high_resolution_clock::now();
    int compile_std_result = system(("g++ -o std " + std_executable).c_str());
    auto compile_std_stop = high_resolution_clock::now();
    auto compile_std_duration = duration_cast<milliseconds>(compile_std_stop - compile_std_start);

    if (compile_std_result != 0)
    {
        cout << RED_TEXT << "ERROR! " << RESET_TEXT << " std code " << ORANGE_TEXT
            << "CE (Compile Error)" << RESET_TEXT << "! \n";
        system("pause");
        return 0;
    }
    else
    {
        cout << "Successfully compiled std code " << GREEN_TEXT << "✅" << RESET_TEXT
            << " took " << compile_std_duration.count() << "ms \n";
    }

    // 编译运行 user.cpp
    auto compile_user_start = high_resolution_clock::now();
    int compile_user_result = system(("g++ -o user " + user_executable).c_str());
    auto compile_user_stop = high_resolution_clock::now();
    auto compile_user_duration = duration_cast<milliseconds>(compile_user_stop - compile_user_start);

    if (compile_user_result != 0)
    {
        cout << RED_TEXT << "ERROR! " << RESET_TEXT << " user code " << ORANGE_TEXT
            << "CE (Compile Error)" << RESET_TEXT << "! \n";
        system("pause");
        return 0;
    }
    else
    {
        cout << "Successfully compiled user code " << GREEN_TEXT << "✅" << RESET_TEXT
            << " took " << compile_user_duration.count() << "ms \n";
    }

    for (int i = 1; i <= numTests; ++i)
    {
        system("rand.exe > data/input.txt");

        auto start_std = high_resolution_clock::now();
        int run_std_result = system("std.exe < data/input.txt > data/std_output.txt");
        auto stop_std = high_resolution_clock::now();
        auto duration_std = duration_cast<milliseconds>(stop_std - start_std);

        auto start_user = high_resolution_clock::now();
        int run_user_result = system("user.exe < data/input.txt > data/user_output.txt");
        auto stop_user = high_resolution_clock::now();
        auto duration_user = duration_cast<milliseconds>(stop_user - start_user);

        ifstream std_output_file("data/std_output.txt");
        ifstream user_output_file("data/user_output.txt");

        stringstream std_ss, user_ss;
        std_ss << std_output_file.rdbuf();
        user_ss << user_output_file.rdbuf();

        string std_result = trim(std_ss.str());
        string user_result = trim(user_ss.str());

        cout << "Test " << i << ": ";
        // 判断运行结果
        if (run_std_result != 0)
        {
            cout << BLUE_TEXT << "RE" << RESET_TEXT << " ";
            // 保存生成的输入数据
            ifstream input_file("data/input.txt");
            stringstream input_ss;
            input_ss << input_file.rdbuf();
            saveInputToFile(trim(input_ss.str()), "data/error_input.txt");
            input_file.close();
        }
        else if (duration_user.count() > 1000)
        {
            cout << BLUE_TEXT << "TLE" << RESET_TEXT << " ";
            // 保存生成的输入数据
            ifstream input_file("data/input.txt");
            stringstream input_ss;
            input_ss << input_file.rdbuf();
            saveInputToFile(trim(input_ss.str()), "data/error_input.txt");
            input_file.close();
        }
        else
        {
            bool isCorrect = std_result == user_result;

            if (isCorrect)
            {
                cout << GREEN_TEXT << "AC" << RESET_TEXT << " ";
                totalPoints += 100 / (float)numTests; // 如果通过测试，加分
            }
            else
            {
                // 找到第一次不匹配的位置
                size_t firstDiffPos = findFirstDiff(std_result, user_result);
                // 输出第一次不匹配之前的部分，并用红色底色标记
                cout << RED_TEXT << "WA" << RESET_TEXT << " ";
                // 保存生成的输入数据
                ifstream input_file("data/input.txt");
                stringstream input_ss;
                input_ss << input_file.rdbuf();
                saveInputToFile(trim(input_ss.str()), "data/error_input.txt");
                input_file.close();

                // 输出第一次不匹配的位置
                if (firstDiffPos < std_result.length())
                {
                    cout << "First Difference Position: " << firstDiffPos << endl;
                    // 输出差异信息，std使用绿色底色标记，user使用红色底色标记
                    cout << "std: " << GREEN_TEXT << std_result.substr(0, firstDiffPos) << RESET_TEXT
                        << RED_TEXT << std_result.substr(firstDiffPos) << RESET_TEXT << endl;
                    cout << "user: " << GREEN_TEXT << user_result.substr(0, firstDiffPos) << RESET_TEXT
                        << RED_TEXT << user_result.substr(firstDiffPos) << RESET_TEXT << endl;
                }
                else
                {
                    // 如果超出字符串长度，则输出整个字符串
                    cout << "First Difference Position: (exceeds string length)" << endl;
                    cout << "std: " << GREEN_TEXT << std_result << RESET_TEXT << endl;
                    cout << "user: " << RED_TEXT << user_result << RESET_TEXT << endl;
                }
            }
        }
        cout << "Time (std, user): " << duration_std.count() << "ms, " << duration_user.count() << "ms" << endl;

        std_output_file.close();
        user_output_file.close();
    }

    cout << "Total Points: " << totalPoints << " / 100" << endl;
    system("pause");
    return 0;
}
