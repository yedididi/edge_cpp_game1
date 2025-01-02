#ifndef CHERRY_H
#define CHERRY_H

// class cherry
// {
//     public:
//         int x[2][3] =  {
//             {160, 20, 60},
//             {260, 110, 60}
//         };
//         int y[2][3] = { 
//             {110, 20, 180},
//             {170, 80, 110}
//         };
//         bool cherryExistance[3] = {true, true, true};

//         int cherryArr[10][10] = {
//             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//             {0, 0, 0, 0, 3, 0, 0, 0, 0, 0},
//             {0, 0, 0, 3, 0, 3, 0, 0, 0, 0},
//             {0, 0, 3, 0, 0, 0, 3, 0, 0, 0},
//             {0, 0, 3, 0, 0, 0, 0, 3, 0, 0},
//             {0, 2, 2, 2, 0, 0, 2, 2, 2, 0},
//             {0, 2, 2, 2, 0, 0, 2, 2, 2, 0},
//             {0, 2, 2, 2, 0, 0, 2, 2, 2, 0},
//             {0, 2, 2, 2, 0, 0, 2, 2, 2, 0},
//             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
//         };

//         cherry();
//         void drawCherry(int x, int y);
//         void eraseCherry(int cherryNum, int mapNum);
// };

class cherry
{
    public:
        int x[2][3];
        int y[2][3];
        bool cherryExistance[3];
        int cherryArr[10][10];

        cherry(); // 생성자 선언

        void drawCherry(int num, int mapNum);
        void eraseCherry(int cherryNum, int mapNum);
};

#endif