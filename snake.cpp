#include <iostream>
#include <ctime>
#include <vector>
#include "console.h"
using namespace std;

// Định nghĩa các hằng số
#define MAX 100 // Số đốt tối đa của rắn
#define Len 1 // Hướng đi lên
#define Xuong 2 // Hướng đi xuống
#define Trai 3 // Hướng đi trái
#define Phai 4 // Hướng đi phải
#define TuongTren 1 // Tọa độ y của tường trên
#define TuongDuoi 28 // Tọa độ y của tường dưới
#define TuongTrai 20 // Tọa độ x của tường trái
#define TuongPhai 100 // Tọa độ x của tường phải
#define MacDinhX TuongTrai+3 // Tọa độ x mặc định của đầu rắn
#define MacDinhY TuongTren+3 // Tọa độ y mặc định của đầu rắn
int speed = 200; // Tốc độ di chuyển ban đầu của rắn (ms), càng nhỏ càng nhanh

// Lưu vị trí
class ToaDo {
private:
    int x, y; // Tọa độ x, y trên màn hình console
public:
    void setX(int x); //Gán giá trị x
    void setY(int y); //Gán giá trị y
    int getX(); //Lấy giá trị x
    int getY(); //Lấy giá trị y
};

// Lớp trừu tượng
class GameSnake {
public:
    virtual void Draw() = 0; // Phương thức thuần ảo để vẽ đối tượng lên màn hình
    virtual ~GameSnake() {} // Hàm hủy ảo để đảm bảo tính đa hình
};

//Giao diện
class GiaoDien: public GameSnake{
    public:
        void Draw() override; // Hàm vẽ giao diện bắt đầu game
};

// Lớp Food
class Food : public GameSnake {
protected:
    ToaDo Moi; // Tọa độ của mồi
public:
    Food(); //Hàm khởi tạo
    void Draw() override; // Hàm vẽ mồi lên màn hình
    ToaDo getMoi(); // Lấy tọa độ mồi
    void taoMoi(); // Tạo mồi mới ở vị trí ngẫu nhiên
};

// Lớp Snake
class Snake : public GameSnake {
protected:
    ToaDo ran[MAX]; // Mảng lưu tọa độ các đốt của rắn
    int sodot; // Số đốt hiện tại của rắn
public:
    Snake(); // Hàm khởi tạo
    void Draw() override; // Hàm vẽ rắn
    ToaDo DiChuyen(int &huong); // Hàm di chuyển rắn theo hướng
    bool ktraChamBien(); // Kiểm tra rắn chạm tường
    bool ktraChamDuoi(); // Kiểm tra rắn chạm vào đuôi
    void AnMoi(int &speed, Food &food); // Xử lý khi rắn ăn mồi
    bool ktraMoi(Food &food); // Kiểm tra rắn có ăn mồi hay không
    void rsSodot(); // Reset số đốt về 1
    ToaDo getRan(int x); // Lấy tọa độ đốt rắn tại vị trí x
};

// Lớp Wall
class Wall : public GameSnake {
public:
    void Draw() override; // Hàm vẽ tường
};

void InputKey(int &huong); // Lấy thông tin từ bàn phím

// Hàm chính của game
void snakegame() {
    srand(time(NULL)); // Khởi tạo bộ tạo số ngẫu nhiên
    noCursorType(); // Ẩn con trỏ chuột
    int huong = Phai; // Hướng mặc định của rắn là đi phải
    vector<GameSnake*> game; // Vector lưu các đối tượng game (tường, rắn, mồi, giao diện)
    Wall wall; // Khởi tạo tường
    Snake snake; // Khởi tạo rắn
    Food food; // Khởi tạo mồi
    GiaoDien giaodien; // Khởi tạo giao diện
    game.push_back(&wall); // Thêm tường vào vector
    game.push_back(&snake); // Thêm rắn
    game.push_back(&food); // Thêm mồi
    game.push_back(&giaodien); // Thêm giao diện
    clrscr(); // Xóa màn hình
    game[3]->Draw(); // Vẽ giao diện bắt đầu
    Sleep(50); // Đợi 50ms
    game[0]->Draw(); // Vẽ tường
    game[2]->Draw(); // Vẽ mồi
    int score = 0; // Điểm ban đầu
    gotoXY(0, 0);
    cout << "Diem: " << score; // Hiển thị điểm
    while (1) { // Vòng lặp game chính
        game[1]->Draw(); // Vẽ rắn
        if (kbhit()) { // Kiểm tra xem có phím được nhấn
            InputKey(huong); // Cập nhật hướng
        }
        if (snake.ktraMoi(food)) { // Nếu rắn ăn mồi
            ++score; // Tăng điểm
            gotoXY(0, 0);
            cout << "Diem: " << score; // Cập nhật điểm trên màn hình
            snake.AnMoi(speed, food); // Xử lý ăn mồi
            gotoXY(food.getMoi().getX(), food.getMoi().getY());
            cout << " "; // Xóa mồi cũ
            game[2]->Draw(); // Vẽ mồi mới
        }
        ToaDo dotCuoi = snake.DiChuyen(huong); // Di chuyển rắn
        gotoXY(dotCuoi.getX(), dotCuoi.getY());
        cout << " "; // Xóa đốt cuối
        Sleep(speed); // Đợi theo tốc độ game
        if (snake.ktraChamBien() || snake.ktraChamDuoi()) { // Nếu rắn chạm tường hoặc đuôi
            clrscr(); // Xóa màn hình
            gotoXY(50, 10);
            cout << "GameOver"; // Hiển thị thông báo thua
            gotoXY(50, 11);
            cout << "Try Again?";
            gotoXY(50, 12);
            cout << "y yes"; // Lựa chọn chơi lại
            gotoXY(50, 13);
            cout << "n no"; // Lựa chọn quay lại giao diện
            while (!kbhit()) { // Hiệu ứng nhấp nháy Game Over
                Sleep(500);
                setTextColor(1 + rand() % 14); // Màu ngẫu nhiên
                gotoXY(50, 10);
                cout << "Game Over";
            }
            setTextColor(15); // Khôi phục màu trắng
            gotoXY(50, 14);
            char c;
            cin >> c; // Đọc lựa chọn người chơi
            if (c == 'y') { // Chơi lại
                score = 0; // Reset điểm
                snake.rsSodot(); // Reset số đốt
                speed = 200; // Reset tốc độ
                huong = Phai; // Reset hướng
                snake = Snake(); // Tạo rắn mới
                food = Food(); // Tạo mồi mới
                game[1] = &snake; // Cập nhật vector
                game[2] = &food;
                clrscr(); // Xóa màn hình
                game[0]->Draw(); // Vẽ lại tường
                game[2]->Draw(); // Vẽ lại mồi
                gotoXY(0, 0);
                cout << "Diem: " << score; // Hiển thị điểm
            }
            else if (c == 'n') { // Quay lại giao diện
                score = 0;
                game[3]->Draw(); // Vẽ giao diện
                snake.rsSodot();
                speed = 200;
                huong = Phai;
                snake = Snake();
                food = Food();
                game[1] = &snake;
                game[2] = &food;
                clrscr();
                game[0]->Draw();
                game[2]->Draw();
                gotoXY(0, 0);
                cout << "Diem: " << score;
            }
            else { // Thoát game nếu nhập phím không hợp lệ
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
    this->x = x; // Gán giá trị x cho đối tượng
}
void ToaDo::setY(int y) {
    this->y = y; // Gán giá trị y cho đối tượng
}
int ToaDo::getX() {
    return this->x; // Trả về giá trị x
}
int ToaDo::getY() {
    return this->y; // Trả về giá trị y
}

//Tạo giao diện bắt đầu game
void GiaoDien::Draw(){
    clrscr(); // Xóa toàn bộ màn hình console
    setTextColor(10); // Màu xanh lá cho tiêu đề
    gotoXY(45, 10); // Di chuyển con trỏ tới vị trí (45, 10)
    cout << "===== SNAKE GAME ====="; //In tiêu đề game
    setTextColor(14); // Màu vàng cho hướng dẫn
    gotoXY(40, 12); 
    cout << "Press any key to start the game!"; // Hướng dẫn bắt đầu
    gotoXY(40, 14);
    cout << "Controls:";// Hiển thị điều khiển
    gotoXY(40, 15);
    cout << "W or Up Arrow: Move Up"; // Điều khiển đi lên
    gotoXY(40, 16);
    cout << "S or Down Arrow: Move Down"; // Điều khiển đi xuống
    gotoXY(40, 17);
    cout << "A or Left Arrow: Move Left"; // Điều khiển sang trái
    gotoXY(40, 18);
    cout << "D or Right Arrow: Move Right"; // Điều khiển sang phải
    setTextColor(15); // Màu trắng mặc định
    while (!kbhit()) { // Chờ người chơi nhấn phím
        Sleep(500); // Đợi 500ms để tạo hiệu ứng nhấp nháy
        setTextColor(rand() % 15 + 1); // Đổi màu ngẫu nhiên cho tiêu đề
        gotoXY(45, 10); 
        cout << "===== SNAKE GAME =====";
        setTextColor(15); // Khôi phục màu trắng
    }
    clrscr(); // Xóa màn hình để bắt đầu game
}

// Định nghĩa các hàm thành viên của Food
Food::Food() {
    Moi.setX(TuongTrai + 2 + rand() % (TuongPhai - TuongTrai - 2)); // Tọa độ x ngẫu nhiên trong giới hạn tường
    Moi.setY(TuongTren + 2 + rand() % (TuongDuoi - TuongTren - 2)); // Tọa độ y ngẫu nhiên trong giới hạn tường
}
void Food::Draw() {
    setTextColor(rand() %15); // Màu ngẫu nhiên cho mồi
    gotoXY(Moi.getX(), Moi.getY()); // Di chuyển con trỏ tới vị trí mồi
    cout << (char)42; // Vẽ mồi bằng ký tự '*'
    setTextColor(15); // Khôi phục màu trắng
}
ToaDo Food::getMoi() {
    return Moi; // Trả về tọa độ mồi
}
void Food::taoMoi() {
    Moi.setX(TuongTrai + 2 + rand() % (TuongPhai - TuongTrai - 2)); // Tạo tọa độ x ngẫu nhiên
    Moi.setY(TuongTren + 2 + rand() % (TuongDuoi - TuongTren - 2)); // Tạo tọa độ y ngẫu nhiên
}

// Định nghĩa các hàm thành viên của Snake
Snake::Snake() {
    sodot = 1; // Rắn bắt đầu với 1 đốt
    for (int i = 0; i < MAX; i++) { // Khởi tạo tất cả đốt
        ran[i].setX(MacDinhX - i); // Tọa độ x mặc định, rắn nằm ngang
        ran[i].setY(MacDinhY); // Tọa độ y mặc định
    }
}
void Snake::Draw() {
    gotoXY(ran[0].getX(), ran[0].getY());
    cout << "0"; // Vẽ đầu rắn bằng ký tự '0'
    for (int i = 1; i < sodot; i++) {
        gotoXY(ran[i].getX(), ran[i].getY());
        cout << "o"; // Vẽ thân rắn bằng ký tự 'o'
    }
}
ToaDo Snake::DiChuyen(int &huong) {
    ToaDo dotCuoi = ran[sodot - 1]; // Lưu đốt cuối để xóa sau khi di chuyển
    for (int i = sodot - 1; i >= 1; i--) {
        ran[i] = ran[i - 1]; // Di chuyển các đốt thân theo đốt trước
    }
    switch (huong) { // Cập nhật tọa độ đầu rắn theo hướng
    case Len:
        ran[0].setY(ran[0].getY() - 1); // Di chuyển lên
        break;
    case Xuong:
        ran[0].setY(ran[0].getY() + 1); // Di chuyển xuống
        break;
    case Trai:
        ran[0].setX(ran[0].getX() - 1); // Di chuyển trái
        break;
    case Phai:
        ran[0].setX(ran[0].getX() + 1); // Di chuyển phải
        break;
    }
    this->Draw(); // Vẽ lại rắn sau khi di chuyển
    return dotCuoi; // Trả về tọa độ đốt cuối để xóa
}
bool Snake::ktraChamBien() {
    return ran[0].getY() == TuongTren || ran[0].getY() == TuongDuoi ||
           ran[0].getX() == TuongTrai || ran[0].getX() == TuongPhai; // Kiểm tra đầu rắn chạm tường
}
bool Snake::ktraChamDuoi() {
    for (int i = 1; i < sodot; i++) {
        if (ran[0].getX() == ran[i].getX() && ran[0].getY() == ran[i].getY())
            return true; // Kiểm tra đầu rắn chạm vào thân
    }
    return false;
}
void Snake::AnMoi(int &speed, Food &food) {
    if (speed > 50) speed -= 50; // Tăng tốc độ khi ăn mồi (giảm thời gian chờ)
    sodot++; // Tăng số đốt
    ran[sodot - 1] = ran[sodot - 2]; // Thêm đốt mới giống đốt trước
    food.taoMoi(); // Tạo mồi mới
}
bool Snake::ktraMoi(Food &food) {
    return ran[0].getX() == food.getMoi().getX() && ran[0].getY() == food.getMoi().getY(); // Kiểm tra đầu rắn trùng tọa độ mồi
}
void Snake::rsSodot() {
    sodot = 1; // Reset số đốt về 1 khi game over
}
ToaDo Snake::getRan(int x) {
    return ran[x]; // Trả về tọa độ đốt rắn tại vị trí x
}

// Định nghĩa hàm vẽ tường
void Wall::Draw() {
    setTextColor(9); // Màu xanh dương cho tường
    for (int i = TuongTrai; i <= TuongPhai; i++) {
        gotoXY(i, TuongTren);
        cout << "+"; // Vẽ tường trên
    }
    for (int i = TuongTren; i <= TuongDuoi; i++) {
        gotoXY(TuongTrai, i);
        cout << "+"; // Vẽ tường trái
    }
    for (int i = TuongTrai; i <= TuongPhai; i++) {
        gotoXY(i, TuongDuoi);
        cout << "+"; // Vẽ tường dưới
    }
    for (int i = TuongTren; i <= TuongDuoi; i++) {
        gotoXY(TuongPhai, i);
        cout << "+"; // Vẽ tường phải
    }
    setTextColor(15); // Khôi phục màu trắng
}

// Hàm điều khiển bằng phím W, A, S, D
void InputKey(int &huong) {
    int key = inputKey(); // Lấy mã phím từ bàn phím
    if ((key == 'w' || key == 'W') && huong != Xuong) huong = Len; // Nhấn W để đi lên, trừ khi đang đi xuống
    else if ((key == 's' || key == 'S') && huong != Len) huong = Xuong; // Nhấn S để đi xuống, trừ khi đang đi lên
    else if ((key == 'a' || key == 'A') && huong != Phai) huong = Trai; // Nhấn A để đi trái, trừ khi đang đi phải
    else if ((key == 'd' || key == 'D') && huong != Trai) huong = Phai; // Nhấn D để đi phải, trừ khi đang đi trái
}

// Hàm điều khiển bằng phím mũi tên (chưa sử dụng trong game)
void NumKey(int &huong) {
    int key = inputKey(); // Lấy mã phím
    if ((key == KEY_UP) && huong != Xuong) huong = Len; // Phím lên
    else if ((key == KEY_DOWN) && huong != Len) huong = Xuong; // Phím xuống
    else if ((key == KEY_LEFT) && huong != Phai) huong = Trai; // Phím trái
    else if ((key == KEY_RIGHT) && huong != Trai) huong = Phai; // Phím phải
}