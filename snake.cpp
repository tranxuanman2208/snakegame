#include <iostream>
#include "console.h"
using namespace std;

// Định nghĩa
#define Len 1
#define Xuong 2
#define Trai 3
#define Phai 4
#define TuongTren 1
#define TuongDuoi 13
#define TuongTrai 3
#define TuongPhai 53
#define MacDinhX TuongTrai+sodot
#define MacDinhY TuongTren+sodot

// Lưu vị trí
class ToaDo {
private:
    int x, y;
public:
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    int getX() { return this->x; }
    int getY() { return this->y; }
};

// Các hoạt động
void Snake(); // Tạo rắn
void printSnake(); // Hiển thị rắn
void DiChuyen(int &huong); // Di chuyển rắn
void InputKey(int &huong); // Lấy thông tin từ bàn phím
void KeyTen(int &huong); // Lấy thông tin từ các nút mũi tên
void const taoTuong(); // Tạo khung
bool ktraThua(); //Kiểm tra thua
void xulyThua(); //Xử lý thuathua
ToaDo ran[100]; // Tạo mảng
int sodot = 3; // Số lượng phần tử
int huong= Phai; //Hướng đi mặc địnhđịnh
int main() {
    Snake();
    // Game loop
    while (1) {
        clrscr(); // Xóa màn hình trước mỗi lần vẽ
        taoTuong();
        DiChuyen(huong);
        printSnake();
        Sleep(200); // Điều chỉnh tốc độ di chuyển
        KeyTen(huong);
        if(ktraThua()) break;
    }
    xulyThua();
    return 0;
}
//Khởi tạo rắn mặc định
void Snake() {
    ran[0].setX(MacDinhX); ran[1].setX(MacDinhX-1); ran[2].setX(MacDinhX-2);
    ran[0].setY(MacDinhY); ran[1].setY(MacDinhY); ran[2].setY(MacDinhY);
}
//Hiển thị rắn
void printSnake() {
    for(int i=0;i<sodot;i++){
        gotoXY(ran[i].getX(),ran[i].getY());
        cout<<(char)254;
    }
}
//Di chuyển rắn
void DiChuyen(int &huong) {
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
}
//Điều hướng bằng chữ cái
void InputKey(int &huong) {
    int key = inputKey();
    if ((key == 'w' || key == 'W') && huong != Xuong) huong = Len;
    else if ((key == 's' || key == 'S') && huong != Len) huong = Xuong;
    else if ((key == 'a' || key == 'A') && huong != Phai) huong = Trai;
    else if ((key == 'd' || key == 'D') && huong != Trai) huong = Phai;
}
//Điều hướng bằng mũi tên
void KeyTen(int &huong) {
    int key = inputKey();
    if (key == KEY_UP && huong != Xuong) huong = Len;
    else if (key == KEY_DOWN && huong != Len) huong = Xuong;
    else if (key == KEY_LEFT && huong != Phai) huong = Trai;
    else if (key == KEY_RIGHT && huong != Trai) huong = Phai;
}
//Tạo tường
void const taoTuong() {
    //Tường trên
    for(int x=TuongTrai;x<=TuongPhai;x++){
        gotoXY(x,TuongTren);
        cout<<(char)220;
    }
    //Tường dưới
    for(int x=TuongTrai;x<=TuongPhai;x++){
        gotoXY(x,TuongDuoi+1);
        cout<<(char)223;
    }
    //Tường trái
    for(int y=TuongTren+1;y<=TuongDuoi;y++){
        gotoXY(TuongTrai,y);
        cout<<(char)221;
    }
    //Tường phải
    for(int y=TuongTren+1;y<=TuongDuoi;y++){
        gotoXY(TuongPhai,y);
        cout<<(char)222;
    }
}
//Kiểm tra thua
bool ktraThua(){
    return ran[0].getY()==TuongTren || ran[0].getY()==TuongDuoi || ran[0].getX()==TuongTrai || ran[0].getX()==TuongPhai;
}
//Xử lý thua
void xulyThua(){
    if(ktraThua()){
        Sleep(200);
        clrscr();
        cout<<"Game Over"<<endl;
        char c;
        cout<<"Chon y hoac n de dung lai hoac tiep tuc"<<endl;
        cin>>c;
        if(c=='y'){
            while(1){
                clrscr(); // Xóa màn hình trước mỗi lần vẽ
                taoTuong();
                DiChuyen(huong);
                printSnake();
                Sleep(200); // Điều chỉnh tốc độ di chuyển
                KeyTen(huong);
                if(ktraThua()) break;
            }
            xulyThua();
        }
        else if(c=='n'){
            clrscr();
        }
    }
}