#include <iostream>
#include <ctime>
#include <vector>
#include "console.h"
using namespace std;

// Định nghĩa
#define MAX 100
#define Len 1
#define Xuong 2
#define Trai 3
#define Phai 4
#define TuongTren 1
#define TuongDuoi 28
#define TuongTrai 20
#define TuongPhai 100
#define MacDinhX TuongTrai+3
#define MacDinhY TuongTren+3
int speed = 200; // Giảm speed để game mượt hơn

// Lưu vị trí
class ToaDo {
private:
    int x, y;
public:
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
};

// Lớp trừu tượng
class GameSnake {
public:
    virtual void Draw() = 0;
    virtual ~GameSnake() {}
};

//Giao diện
class GiaoDien: public GameSnake{
    public:
        void Draw();
};

// Lớp Food
class Food : public GameSnake {
protected:
    ToaDo Moi;
public:
    Food();
    void Draw() override;
    ToaDo getMoi();
    void taoMoi();
};

// Lớp Snake
class Snake : public GameSnake {
protected:
    ToaDo ran[MAX];
    int sodot;
public:
    Snake();
    void Draw() override;
    ToaDo DiChuyen(int &huong);
    bool ktraChamBien();
    bool ktraChamDuoi();
    void AnMoi(int &speed, Food &food);
    bool ktraMoi(Food &food);
    void rsSodot();
    ToaDo getRan(int x);
};

// Lớp Wall
class Wall : public GameSnake {
public:
    void Draw() override;
};

void InputKey(int &huong); // Lấy thông tin từ bàn phím
//Game
void snakegame(){
    srand(time(NULL));
    noCursorType();
    int huong = Phai; // Hướng mặc định
    vector<GameSnake*> game;
    Wall wall;
    Snake snake;
    Food food;
    GiaoDien giaodien;
    game.push_back(&wall);
    game.push_back(&snake);
    game.push_back(&food);
    game.push_back(&giaodien);
    clrscr(); // Xóa màn hình ban đầu
    game[3]->Draw(); // Giao diện chờ bắt đầu
    Sleep(50);
    game[0]->Draw(); // Vẽ tường
    game[2]->Draw(); // Tạo mồi
    int score=0; // Điểm
    gotoXY(0,0);
    cout<<"Diem: "<<score;
    while (1) {
        game[1]->Draw(); // Vẽ rắn mỗi khung hình
        if (kbhit()) {
            InputKey(huong);
        }
        if (snake.ktraMoi(food)) {
            ++score;
            gotoXY(0,0);
            cout<<"Diem: "<<score;
            snake.AnMoi(speed, food);
            gotoXY(food.getMoi().getX(), food.getMoi().getY());
            cout << " "; // Xóa mồi cũ
            game[2]->Draw(); // Vẽ mồi mới
        }
        ToaDo dotCuoi= snake.DiChuyen(huong);
        gotoXY(dotCuoi.getX(),dotCuoi.getY());
        cout<<" ";
        Sleep(speed);
        if (snake.ktraChamBien() || snake.ktraChamDuoi()) {
            clrscr();
            gotoXY(50, 10);
            cout << "GameOver";
            gotoXY(50,11);
            cout<<"Try Again?";
            gotoXY(50,12);
            cout<<"y yes";
            gotoXY(50,13);
            cout<<"n no";
            while(!kbhit()){
                Sleep(500);
                setTextColor(1+ rand()%14);
                gotoXY(50,10);
                cout<<"Game Over";
            }
            setTextColor(15);
            gotoXY(50,14);
            char c;
            cin >> c;
            if (c == 'y') {
                score=0;
                snake.rsSodot();
                speed = 200;
                huong = Phai;
                snake = Snake();
                food = Food();
                game[1] = &snake;
                game[2] = &food;
                clrscr(); // Xóa màn hình khi reset
                game[0]->Draw(); // Vẽ lại tường
                game[2]->Draw(); // Vẽ lại mồi
                gotoXY(0,0);
                cout<<"Diem: "<<score;
            }
            else if(c=='n'){
                score=0;
                game[3]->Draw();
                snake.rsSodot();
                speed = 200;
                huong = Phai;
                snake = Snake();
                food = Food();
                game[1] = &snake;
                game[2] = &food;
                clrscr(); // Xóa màn hình khi reset
                game[0]->Draw(); // Vẽ lại tường
                game[2]->Draw(); // Vẽ lại mồi
                gotoXY(0,0);
                cout<<"Diem: "<<score;
            }
            else{
                exit(1);
            }
        }

    }
}
// Hàm main
int main() {
    snakegame();
    return 0;
}

// Định nghĩa các hàm thành viên của ToaDo
void ToaDo::setX(int x) {
    this->x = x;
}
void ToaDo::setY(int y) {
    this->y = y;
}
int ToaDo::getX() {
    return this->x;
}
int ToaDo::getY() {
    return this->y;
}

//Tạo giao diện bắt đầu game
void GiaoDien::Draw(){
    clrscr();
    setTextColor(10);
    gotoXY(45, 10);
    cout << "===== SNAKE GAME =====";
    setTextColor(14);
    gotoXY(40, 12);
    cout << "Press any key to start the game!";
    gotoXY(40, 14);
    cout << "Controls:";
    gotoXY(40, 15);
    cout << "W or Up Arrow: Move Up";
    gotoXY(40, 16);
    cout << "S or Down Arrow: Move Down";
    gotoXY(40, 17);
    cout << "A or Left Arrow: Move Left";
    gotoXY(40, 18);
    cout << "D or Right Arrow: Move Right";
    setTextColor(15);
    while (!kbhit()) {
        Sleep(500);
        setTextColor(rand() % 15 + 1);
        gotoXY(45, 10);
        cout << "===== SNAKE GAME =====";
        setTextColor(15);
    }
    clrscr();
}

// Định nghĩa các hàm thành viên của Food
Food::Food() {
    Moi.setX(TuongTrai + 2 + rand() % (TuongPhai - TuongTrai - 2));
    Moi.setY(TuongTren + 2 + rand() % (TuongDuoi - TuongTren - 2));
}
void Food::Draw() {
    setTextColor(rand() %15);
    gotoXY(Moi.getX(), Moi.getY());
    cout << (char)42; // Hiển thị mồi (*)
    setTextColor(15);
}
ToaDo Food::getMoi() {
    return Moi;
}
void Food::taoMoi() {
    Moi.setX(TuongTrai + 2 + rand() % (TuongPhai - TuongTrai - 2));
    Moi.setY(TuongTren + 2 + rand() % (TuongDuoi - TuongTren - 2));
}

// Định nghĩa các hàm thành viên của Snake
Snake::Snake() {
    sodot = 1;
    for (int i = 0; i < MAX; i++) { // Khởi tạo tất cả đốt
        ran[i].setX(MacDinhX - i);
        ran[i].setY(MacDinhY);
    }
}
void Snake::Draw() {
    gotoXY(ran[0].getX(), ran[0].getY());
    cout << "0"; // Đầu rắn
    for (int i = 1; i < sodot; i++) {
        gotoXY(ran[i].getX(), ran[i].getY());
        cout << "o"; // Thân rắn
    }
}
ToaDo Snake::DiChuyen(int &huong) {
    ToaDo dotCuoi = ran[sodot - 1];
    for (int i = sodot - 1; i >= 1; i--) {
        ran[i] = ran[i - 1];
    }
    switch (huong) {
    case Len:
        ran[0].setY(ran[0].getY() - 1);
        break;
    case Xuong:
        ran[0].setY(ran[0].getY() + 1);
        break;
    case Trai:
        ran[0].setX(ran[0].getX() - 1);
        break;
    case Phai:
        ran[0].setX(ran[0].getX() + 1);
        break;
    }
    this->Draw();
    return dotCuoi;
}
bool Snake::ktraChamBien() {
    return ran[0].getY() == TuongTren || ran[0].getY() == TuongDuoi ||
           ran[0].getX() == TuongTrai || ran[0].getX() == TuongPhai;
}
bool Snake::ktraChamDuoi() {
    for (int i = 1; i < sodot; i++) {
        if (ran[0].getX() == ran[i].getX() && ran[0].getY() == ran[i].getY())
            return true;
    }
    return false;
}
void Snake::AnMoi(int &speed, Food &food) {
    if (speed > 50) speed -= 50; // Giảm speed nhẹ hơn
    sodot++;
    ran[sodot - 1] = ran[sodot - 2]; // Thêm đốt mới
    food.taoMoi();
}
bool Snake::ktraMoi(Food &food) {
    return ran[0].getX() == food.getMoi().getX() && ran[0].getY() == food.getMoi().getY();
}
void Snake::rsSodot() {
    sodot = 1; // Reset về 3 đốt
}
ToaDo Snake::getRan(int x) {
    return ran[x];
}

// Định nghĩa các hàm thành viên của Wall
void Wall::Draw() {
    setTextColor(9);
    for (int i = TuongTrai; i <= TuongPhai; i++) {
        gotoXY(i, TuongTren);
        cout << "+";
    }
    for (int i = TuongTren; i <= TuongDuoi; i++) {
        gotoXY(TuongTrai, i);
        cout << "+";
    }
    for (int i = TuongTrai; i <= TuongPhai; i++) {
        gotoXY(i, TuongDuoi);
        cout << "+";
    }
    for (int i = TuongTren; i <= TuongDuoi; i++) {
        gotoXY(TuongPhai, i);
        cout << "+";
    }
    setTextColor(15);
}

// Hàm điều khiển
void InputKey(int &huong) {
    int key = inputKey();
    if ((key == 'w' || key == 'W') && huong != Xuong) huong = Len;
    else if ((key == 's' || key == 'S') && huong != Len) huong = Xuong;
    else if ((key == 'a' || key == 'A') && huong != Phai) huong = Trai;
    else if ((key == 'd' || key == 'D') && huong != Trai) huong = Phai;
}