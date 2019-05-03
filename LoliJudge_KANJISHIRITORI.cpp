#include <cstdio>
#include <iostream>
#include <fstream>
#include <thread>
#include <windows.h>

int mark;
char od[1000];

inline int Judge(const char *Input, const char *Output, const char *Stdans) {

    /*std::ifstream input, output, stdans;
    input.open(Input);
    output.open(Output);
    stdans.open(Stdans);*/

    FILE *input = fopen(Input, "r"), *output = fopen(Output, "r"), *stdans = fopen(Stdans, "r");

    char a[1000], b[1000];
    memset(a, 0, sizeof(a));
    memset(b, 0, sizeof(b));

    while(fscanf(stdans, "%s", a) != EOF) {
        if(fscanf(output, "%s", b) == EOF) return 0;
        int len = strlen(a);
        if(len != strlen(b)) return 0;

        for(int i = 0; i < len; i++) {
            if(a[i] != b[i]) return 0;
        }

        memset(a, 0, len * sizeof(char));
        memset(b, 0, len * sizeof(char));
    }
    if(fscanf(output, "%s", b) != EOF) {
        return 0;
    }
    return 10;
}

inline void Run(const char *commed) {
    std::thread run(system, commed);
    run.detach();
    Sleep(1000);
    return;
}

int main(int argc, char **argv) {

    if(argc < 4) {
        printf("please input the name of problem and code! \n");
        return 0;
    }

    sprintf(od, "rmdir temp /s /q"); /// delete [temp]
    system(od);

    sprintf(od, "rmdir result /s /q"); /// delete [result]
    system(od);

    const char *name = argv[1]; /// get problem name
    const char *name1 = argv[2], *name2 = argv[3];

    sprintf(od, "mkdir temp"); /// make [temp]
    system(od);

    sprintf(od, "mkdir result"); /// make [result]
    system(od);

    sprintf(od, "dir source /b /ad > user.txt"); /// get all user
    system(od);

    std::string nowUser;
    std::ifstream allUser;
    allUser.open("user.txt");

    sprintf(od, "copy data\\%s\\Anna.h temp\\Anna.h", name); /// copy Brunolib.h
    system(od);
    sprintf(od, "copy data\\%s\\Annalib.h temp\\Annalib.h", name); /// copy Brunolib.h
    system(od);
    sprintf(od, "copy data\\%s\\Bruno.h temp\\Bruno.h", name); /// copy Brunolib.h
    system(od);
    sprintf(od, "copy data\\%s\\Brunolib.h temp\\Brunolib.h", name); /// copy Brunolib.h
    system(od);
    sprintf(od, "copy data\\%s\\spj.exe temp\\spj.exe", name); /// copy Brunolib.h
    system(od);

    while(getline(allUser, nowUser)) {

        const char *user = nowUser.c_str(); /// get now user

        sprintf(od, "copy source\\%s\\%s.cpp temp\\%s.cpp", user, name1, name1); /// copy code 1
        system(od);

        sprintf(od, "copy source\\%s\\%s.cpp temp\\%s.cpp", user, name2, name2); /// copy code 2
        system(od);

        sprintf(od, "g++ temp\\%s.cpp data\\%s\\graderAnna.cpp -o temp\\%s -O2", name1, name, name1); /// get exe
        system(od);
        sprintf(od, "g++ temp\\%s.cpp data\\%s\\graderBruno.cpp -o temp\\%s -O2", name2, name, name2); /// get exe
        system(od);

        sprintf(od, "mkdir result\\%s", user); /// make [result\user]
        system(od);

        static char resultData[1000];
        sprintf(resultData, "result\\%s\\%s.txt", user, name);

        std::ofstream userScore;
        userScore.open(resultData);

        double totScore = 0;

        for(int i = 1; i <= 37; i++) {
            sprintf(od, "copy data\\%s\\%d.in temp\\%s.in", name, i, name); /// copy input
            system(od);

            sprintf(od, "cd temp&%s", name1);
            Run(od); /// run
            sprintf(od, "cd temp&%s", name2);
            Run(od);

            char *input, *output, *stdans;
            input = new char[100];
            output = new char[100];
            stdans = new char[100];
            sprintf(input, "temp\\%s.in", name);
            sprintf(output, "temp\\%s.out", name);
            sprintf(stdans, "data\\%s\\%d.out", name, i);
            delete input;
            delete output;
            delete stdans;

//            int score = Judge(input, output, stdans); /// judge

            sprintf(od, "cd temp && spj %s.out tempScore.orz", name);
            system(od);
            std::ifstream Sora;
            double score;
            Sora.open("temp\\tempScore.orz");
            Sora >> score;
            Sora.close();
            system("cd temp&del tempScore.orz");
            userScore << "#" << i << " " << (score=score*100/37) << "\n";
            userScore.flush();
            totScore += score;
        }

        userScore << "Your Score : " << totScore << "\n";
        userScore.close();

        sprintf(od, "del temp\\%s.cpp", name1);
        system(od);

        sprintf(od, "del temp\\%s.cpp", name2);
        system(od);

        sprintf(od, "del temp\\%s", name);
        system(od);
    }

    allUser.close();
    sprintf(od, "del user.txt"); /// delete all user
    system(od);

    sprintf(od, "rmdir temp /s /q"); /// delete [temp]
    system(od);
    return 0;
}
